#include "opendefs.h"
#include "openbridge.h"
#include "openserial.h"
#include "packetfunctions.h"
#include "iphc.h"
#include "idmanager.h"
#include "openqueue.h"

//=========================== variables =======================================

//=========================== prototypes ======================================
//=========================== public ==========================================

void openbridge_init() {
}

void openbridge_triggerData() {
   uint8_t           input_buffer[136];//worst case: 8B of next hop + 128B of data
   OpenQueueEntry_t* pkt;
   uint8_t           numDataBytes;
  
   numDataBytes = openserial_getInputBufferFilllevel();
  
   //poipoi xv
   //this is a temporal workaround as we are never supposed to get chunks of data
   //longer than input buffer size.. I assume that HDLC will solve that.
   // MAC header is 13B + 8 next hop so we cannot accept packets that are longer than 118B
   if (numDataBytes>(136 - 10/*21*/) || numDataBytes<8){
   //to prevent too short or too long serial frames to kill the stack  
       openserial_printError(COMPONENT_OPENBRIDGE,ERR_INPUTBUFFER_LENGTH,
                   (errorparameter_t)numDataBytes,
                   (errorparameter_t)0);
       return;
   }
  
   //copying the buffer once we know it is not too big
   openserial_getInputBuffer(&(input_buffer[0]),numDataBytes);
  
   if (idmanager_getIsDAGroot()==TRUE && numDataBytes>0) {
      pkt = openqueue_getFreePacketBuffer(COMPONENT_OPENBRIDGE);
      if (pkt==NULL) {
         openserial_printError(COMPONENT_OPENBRIDGE,ERR_NO_FREE_PACKET_BUFFER,
                               (errorparameter_t)0,
                               (errorparameter_t)0);
         return;
      }
      //admin
      pkt->creator  = COMPONENT_OPENBRIDGE;
      pkt->owner    = COMPONENT_OPENBRIDGE;
      //l2
      pkt->l2_nextORpreviousHop.type = ADDR_64B;
      memcpy(&(pkt->l2_nextORpreviousHop.addr_64b[0]),&(input_buffer[0]),8);
      //payload
      packetfunctions_reserveHeaderSize(pkt,numDataBytes-8);
      memcpy(pkt->payload,&(input_buffer[8]),numDataBytes-8);

      //for root
      if(idmanager_isMyAddress(&pkt->l2_nextORpreviousHop)){
        
        packetfunctions_tossHeader(pkt,56);//magic number(maybe is other layer's header)
        //openudp_receive(pkt); //this method will make other function not working.
        //temp solution.QQ
        schedule_resetAllDistributeCell();

        uint8_t entryCount = pkt->payload[1];
        for(int i=0; i<entryCount; i++){
          uint8_t baseOffset = i*11+2;
          openserial_printInfo(COMPONENT_CGREEN, ERR_MSG_UNKNOWN_TYPE, baseOffset, 0);
          open_addr_t     temp_neighbor;
          memset(&temp_neighbor,0,sizeof(temp_neighbor));
          temp_neighbor.type = ADDR_64B;
          uint8_t slotOffset = pkt->payload[baseOffset];
          uint8_t channelOffset = pkt->payload[baseOffset+1];
          uint8_t cellType = pkt->payload[baseOffset+2];
          for(int j=0; j<8; j++){
            temp_neighbor.addr_64b[j] = pkt->payload[baseOffset+3+j];
          }
            
          if(cellType==1){  //1 TX 0 RX
            cellType = CELLTYPE_TX;
          }else{
            cellType = CELLTYPE_RX;
          }
          schedule_addActiveSlot(
            slotOffset,                    // slot offset
            cellType,                     // type of slot
            FALSE,                                 // shared?
            channelOffset,                                     // channel offset
            &temp_neighbor                         // neighbor
          );
        }
        openqueue_freePacketBuffer(pkt);
        return;
      }

      //this is to catch the too short packet. remove it after fw-103 is solved.
      if (numDataBytes<16){
              openserial_printError(COMPONENT_OPENBRIDGE,ERR_INVALIDSERIALFRAME,
                            (errorparameter_t)0,
                            (errorparameter_t)0);
      }        
      //send
      if ((iphc_sendFromBridge(pkt))==E_FAIL) {
         openqueue_freePacketBuffer(pkt);
      }
   }
}

void openbridge_sendDone(OpenQueueEntry_t* msg, owerror_t error) {
   msg->owner = COMPONENT_OPENBRIDGE;
   if (msg->creator!=COMPONENT_OPENBRIDGE) {
      openserial_printError(COMPONENT_OPENBRIDGE,ERR_UNEXPECTED_SENDDONE,
                            (errorparameter_t)0,
                            (errorparameter_t)0);
   }
   openqueue_freePacketBuffer(msg);
}

/**
\brief Receive a frame at the openbridge, which sends it out over serial.
*/
void openbridge_receive(OpenQueueEntry_t* msg) {
   
   // prepend previous hop
   packetfunctions_reserveHeaderSize(msg,LENGTH_ADDR64b);
   memcpy(msg->payload,msg->l2_nextORpreviousHop.addr_64b,LENGTH_ADDR64b);
   
   // prepend next hop (me)
   packetfunctions_reserveHeaderSize(msg,LENGTH_ADDR64b);
   memcpy(msg->payload,idmanager_getMyID(ADDR_64B)->addr_64b,LENGTH_ADDR64b);
   
   // send packet over serial (will be memcopied into serial buffer)
   openserial_printData((uint8_t*)(msg->payload),msg->length);
   
   // free packet
   openqueue_freePacketBuffer(msg);
}

//=========================== private =========================================
