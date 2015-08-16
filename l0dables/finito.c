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
  setExtFont(GLOBAL(nickfont));
  showTextCentered(GLOBAL(nickname));
  getInputWaitRelease();
}

void up(TestMachine *state) {
  showTextCentered("up");
}

void down(TestMachine *state) {
  showTextCentered("down");
}

void done(TestMachine *state) {
  setTextColor(0xFF,0x00); // reset to normal?
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


void
print_transition(FinitoMachine *fsm, FinitoStateId old_state, FinitoStateId new_state) {
  return; // uncomment for debug

  const size_t STR_MAX = 100;
  char str[STR_MAX+1];

  strncpy(str, finito_definition_statename(fsm->def, old_state), STR_MAX);
  strncat(str, " -> ", STR_MAX);
  strncat(str, finito_definition_statename(fsm->def, new_state), STR_MAX);

  showTextCentered(str);
}

#include "machine.fsm.c" // our FSM definition
void ram(void) {
  FinitoMachine machine;
  TestMachine state = { BTN_NONE };

  finito_machine_init(&machine, &TestMachine_def, &state);
  machine.on_state_change = print_transition;

  while (finito_machine_running(&machine)) {
    state.button_pressed = getInput();
    finito_machine_run(&machine);
  }

  return;
  
}
