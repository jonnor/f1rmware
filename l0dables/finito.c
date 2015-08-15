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

// State
typedef struct _TestMachine {
    //button_pressed;
} TestMachine;


// Transitions predicates
bool up_pressed(const TestMachine *state) {
    return false;
}
bool down_pressed(const TestMachine *state) {
    return false;
}

#include "machine.fsm.c"

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

void ram(void) {

  TestMachine state;
  FinitoMachine machine;

  setExtFont(GLOBAL(nickfont));

  showTextCentered(GLOBAL(nickname));

  getInputWaitRelease();
  

  while(1){
    switch(getInput()){
        case BTN_UP:
          showTextCentered("up");
          break;
        case BTN_DOWN:
          showTextCentered("down");
          break;
        case BTN_RIGHT:
          showTextCentered("right");
          break;
        case BTN_LEFT:
          return;
        case BTN_ENTER:
          return;
    };
  }

  //getInputWait();
  setTextColor(0xFF,0x00); // reset to normal

  // FIXME: run in the loop
  finito_machine_init(&machine, &TestMachine_def, &state);
  //m.on_state_change = finito_debug_print_transition;
  finito_machine_run(&machine);

  return;
  
}
