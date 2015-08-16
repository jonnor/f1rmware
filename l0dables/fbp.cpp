
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <r0ketlib/display.h>
#include <r0ketlib/fonts.h>
#include <r0ketlib/render.h>
#include <r0ketlib/fonts/smallfonts.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/itoa.h>
#include <r0ketlib/config.h>
#include <r0ketlib/render.h>

extern "C" {
#include <rad1olib/sbrk.c>
}

#define MICROFLO_EMBED_GRAPH
#include <microflo/microflo.h>
#include <microflo/io.hpp>

#include "usetable.h"

extern "C" {
    void ram(void);

    void _exit(void) {
        // do nothing
    }
    void _kill(void) {
        /// ff
    }
    void _getpid(void) {
        /// ff
    }
};

void *operator new(size_t n)
{
  void * const p = malloc(n);
  // TODO: handle p == 0 ?
  return p;
}
void operator delete(void * p)
{
  free(p);
}

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

// MicroFlo
// Components
#include "componentlib-ids.h"
#include "componentlib-ports.h"

/* microflo_component yaml
    name: ReadJoystick
    description: "Read rad1io joystick"
    inports:
      start:
        type: bang
      stop:
        type: bang
    outports:
      enter:
        type: bang
      up:
        type: bang
      down:
        type: bang
      left:
        type: bang
      right:
        type: bang

microflo_component */
struct ReadJoystick : public Component {

  ReadJoystick()
    : Component(outPorts, ReadJoystickPorts::OutPorts::right+1)
    , started(false)
  {
  }

  virtual void process(Packet in, MicroFlo::PortId port) {
    using namespace ReadJoystickPorts;

    if (port == InPorts::start) {
      started = true;
    } else if (port == InPorts::stop) {
      started = false;
    }

    if (started && in.isTick()) {
      checkJoystick();
    }
  }

private:
  void checkJoystick() {
    using namespace ReadJoystickPorts;

    char key = getInputWait(); // FIXME: don't wait
	  switch(key) {
	  case BTN_ENTER:
      send(Packet(true), OutPorts::enter);
	    break;
	  case BTN_RIGHT:
      send(Packet(true), OutPorts::right);
	    break; 
	  case BTN_DOWN:
      send(Packet(true), OutPorts::down);
	    break;
	  case BTN_LEFT:
      send(Packet(true), OutPorts::left);
      break;
	  case BTN_UP:
      send(Packet(true), OutPorts::up);
	    break;
	  }
  }

private:
  Connection outPorts[ReadJoystickPorts::OutPorts::right+1];
  bool started;
};


/* microflo_graph fbp
    'true' -> START joystick(ReadJoystick)
microflo_graph */

#include <microflo/microflo.hpp>
#include "componentlib-source.hpp"
#include "main.h" // generated graph definition

void ram(void) {
  NullIO io;
  NullHostTransport transport; // TODO; enable serial for live
  Network network(&io);
  HostCommunication controller;

  transport.setup(&io, &controller);
  controller.setup(&network, &transport);
  
  void *dynamic = malloc(100);
  if (dynamic) {
    showTextCentered("malloc works");
  } else {
    showTextCentered("malloc broken");
  }
  //MICROFLO_LOAD_STATIC_GRAPH((&controller), graph); // FIXME: malloc is broken...

  // TODO: allow to stop network
  while (1) {
    //transport.runTick();
    //network.runTick();
  }

  return; 
}


