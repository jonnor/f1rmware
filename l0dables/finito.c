#include <stdint.h>
#include <string.h>

#include <r0ketlib/display.h>
#include <r0ketlib/fonts.h>
#include <r0ketlib/render.h>
#include <r0ketlib/fonts/smallfonts.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/itoa.h>
#include <r0ketlib/config.h>
#include <r0ketlib/render.h>

#include "usetable.h"

//#include <finito.h>
#include <finito.c>

void
showTextCentered(const char *str) {
  int dx=0;
  int dy=0;

  dx=DoString(0,0,GLOBAL(nickname));
  dx=(RESX-dx)/2;
  if (dx<0) dx=0;
  dy=(RESY-getFontHeight())/2;

  lcdClear();
  lcdFill(GLOBAL(nickbg));
  setTextColor(GLOBAL(nickbg),GLOBAL(nickfg));
  lcdSetCrsr(dx,dy);
  lcdPrint(str);
  lcdDisplay();
}

// Finito app
typedef struct _TestMachine {
    int button_pressed;
} TestMachine;

// State functions
void init(TestMachine *state) {
  state->button_pressed = BTN_NONE;
}

void up(TestMachine *state) {
  showTextCentered("up");
}

void down(TestMachine *state) {
  showTextCentered("down");         
}


// Transitions predicates
// FIXME: allow generating these
bool up_pressed(const TestMachine *state) {
  return state->button_pressed == BTN_UP;
}
bool down_pressed(const TestMachine *state) {
  return state->button_pressed == BTN_DOWN;
}
bool enter_pressed(const TestMachine *state) {
  return state->button_pressed == BTN_ENTER;
}
bool left_pressed(const TestMachine *state) {
  return state->button_pressed == BTN_LEFT;
}
bool right_pressed(const TestMachine *state) {
  return state->button_pressed == BTN_RIGHT;
}

#include "machine.fsm.c" // our FSM definition

void
print_transition(FinitoMachine *fsm, FinitoStateId old, FinitoStateId new_state) {

  showTextCentered(finito_definition_statename(fsm->def, new_state));
  // "state changed: %s -> %s\n",
  //  finito_definition_statename(fsm->def, old)
}

void ram(void) {
  TestMachine state;
  FinitoMachine machine;

  // FIXME: move into init()
  setExtFont(GLOBAL(nickfont));
  showTextCentered(GLOBAL(nickname));
  getInputWaitRelease();

  finito_machine_init(&machine, &TestMachine_def, &state);
  machine.on_state_change = print_transition;

  // FIXME: run until machine stops/exists. Should have while(finito_machine_running(&machine))
  while (1) {
    state.button_pressed = getInput();
    finito_machine_run(&machine);
  }

  //getInputWait();
  setTextColor(0xFF,0x00); // reset to normal

  return;
  
}
