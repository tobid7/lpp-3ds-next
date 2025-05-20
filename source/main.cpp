/*
 *   This file is part of lpp-3ds-next
 *   based on https://github.com/Rinnegatamante/lpp-3ds/
 *   Copyright (C) 2021-2023 Tobi-D7
 */

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <stdio.h>
#include <string.h>

#include <ErrorHelper.hpp>
#include <luaplayer.hpp>

bool f_quit = false;
bool catch_at = true;
bool csndAccess = false;

extern void nsocExit();

void InitLppServ() {
  gfxInitDefault();
  aptInit();
  cfguInit();
  romfsInit();
  // consoleInit(GFX_BOTTOM, NULL);
}

void ExitLppServ() {
  // if (catch_at) {
  //  while (aptMainLoop()) {
  //    hidScanInput();
  //    if (hidKeysDown() & KEY_START) {
  //      break;
  //    }
  //  }
  //}
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
std::string cur_dir;

int main(int argc, char* argv[]) {
  init_fnc(InitLppServ, ExitLppServ);
  ErrorHelper::SetupDirectories();

  cur_dir = "sdmc:/";
  Run("romfs:/index.lua");
  exit(0);
  return 0;
}