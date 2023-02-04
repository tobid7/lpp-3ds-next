/*
 *   This file is part of lpp-3ds-next
 *   based on https://github.com/Rinnegatamante/lpp-3ds/
 *   Copyright (C) 2021-2023 Tobi-D7
 */

#include "include/luaplayer.hpp"
#include <3ds.h>
#include <ErrorHelper.hpp>
#include <NDS.hpp>
#include <stdio.h>
#include <string.h>

bool f_quit = false;
bool catch_at = true;

extern void nsocExit();

void InitLppServ() {
  gfxInitDefault();
  aptInit();
  cfguInit();
  romfsInit();
  consoleInit(GFX_BOTTOM, NULL);
  // nds::Init();
}

void ExitLppServ() {
  if (catch_at) {
    while (aptMainLoop()) {
      hidScanInput();
      if (hidKeysDown() & KEY_START) {
        break;
      }
    }
  }
  f_quit = true;
  aptExit();
  cfguExit();
  romfsExit();
  // nsocExit();
  // nds::Exit();
  gfxExit();
}

bool ftp_state;

bool isTopLCDOn = true;
bool isBottomLCDOn = true;

bool GW_MODE;
bool CIA_MODE;
bool isCSND;
char cur_dir[256];

int main(int argc, char **argv) {
  init_fnc(InitLppServ, ExitLppServ);
  ErrorHelper::SetupDirectories();
  sprintf(cur_dir, "sdmc:/");
  Run("romfs:/index.lua");
  // return 0; // Just caused by all the fnc
  //           // not contain return value pain lol
}