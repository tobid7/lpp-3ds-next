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
#include <NDS.hpp>
#include <future>
#include <memory.hpp>

#include "include/luaplayer.hpp"

bool f_quit = false;
bool catch_at = true;
bool csndAccess = false;

extern void create(ThreadFunc entrypoint);
extern void destroy(void);

std::string formatBytes(int bytes) {
  char out[32];

  if (bytes == 1)
    snprintf(out, sizeof(out), "%d b", bytes);

  else if (bytes < 1024)
    snprintf(out, sizeof(out), "%d b", bytes);

  else if (bytes < 1024 * 1024)
    snprintf(out, sizeof(out), "%.1f kb", (float)bytes / 1024);

  else if (bytes < 1024 * 1024 * 1024)
    snprintf(out, sizeof(out), "%.1f mb", (float)bytes / 1024 / 1024);

  else
    snprintf(out, sizeof(out), "%.1f gb", (float)bytes / 1024 / 1024 / 1024);

  return out;
}

static void MemThrd() {
  while (!f_quit) {
    std::cout << "C: " << formatBytes(nlc::Memory::GetCurrent())
              << " A: " << formatBytes(nlc::Memory::GetTotalAllocated())
              << " F: " << formatBytes(nlc::Memory::GetTotalFreed()) << "\r";
    std::cout.flush();
  }
}

extern void nsocExit();

void InitLppServ() {
  gfxInitDefault();
  aptInit();
  cfguInit();
  romfsInit();
  // consoleInit(GFX_BOTTOM, NULL);
  // create((ThreadFunc)MemThrd);
  // nds::Init();
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
  destroy();
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

void Prnt() { std::cout << "Assync lol" << std::endl; }

int main(int argc, char** argv) {
  init_fnc(InitLppServ, ExitLppServ);
  ErrorHelper::SetupDirectories();

  cur_dir = "sdmc:/";
  Run("romfs:/index.lua");
  exit(0);
  // return 0; // Just caused by all the fnc
  //           // not contain return value pain lol
}