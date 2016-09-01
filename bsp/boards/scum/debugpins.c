/**
\brief SCuM-specific definition of the "debugpins" bsp module.

To be implemented after issue: SCUM-25

\author Tengfei Chang <tengfei.chang@inria.fr>, August 2016.
*/

#include "memory_map.h"
#include "debugpins.h"

//=========================== defines =========================================

#define DEBUGPINS_IN_USE

//=========================== variables =======================================

//=========================== prototypes ======================================

//=========================== public ==========================================

void debugpins_init() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
}

void debugpins_frame_toggle() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    ^=  0x01;
#endif
}

void debugpins_frame_clr() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    &= ~0x01;
#endif
}

void debugpins_frame_set() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    |=  0x01;
#endif
}

void debugpins_slot_toggle() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    ^=  0x02;
#endif
}

void debugpins_slot_clr() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    &= ~0x02;
#endif
}

void debugpins_slot_set() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    |=  0x02;
#endif
}

void debugpins_fsm_toggle() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    ^=  0x04;
#endif
}

void debugpins_fsm_clr() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    &= ~0x04;
#endif
}

void debugpins_fsm_set() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    |=  0x04;
#endif
}

void debugpins_task_toggle() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
}

void debugpins_task_clr() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
}

void debugpins_task_set() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
}

void debugpins_isr_toggle() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    ^=  0x08;
#endif
}

void debugpins_isr_clr() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    &= ~0x08;
#endif
}

void debugpins_isr_set() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
#ifdef DEBUGPINS_IN_USE
    GPIO_REG__OUTPUT    |=  0x08;
#endif
}

void debugpins_radio_toggle() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
}

void debugpins_radio_clr() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
}

void debugpins_radio_set() {
    // empty for now, see https://openwsn.atlassian.net/browse/SCUM-25
}