/*
 *   This file is part of lpp-3ds-next
 *   based on https://github.com/Rinnegatamante/lpp-3ds/
 *   Copyright (C) 2021-2023 Tobi-D7
 */

#include "include/luaplayer.hpp"
#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <ErrorHelper.hpp>
#include <NDS.hpp>

bool f_quit = false;

void InitLppServ()
{
    gfxInitDefault();
    aptInit();
    cfguInit();
    romfsInit();
    //nds::Init();
}

void ExitLppServ()
{
    aptMainLoop();
    f_quit = true;
    aptExit();
    cfguExit();
    romfsExit();
    //nds::Exit();
    gfxExit();
}

bool ftp_state;

bool isTopLCDOn = true;
bool isBottomLCDOn = true;

bool GW_MODE;
bool CIA_MODE;
bool isCSND;
char cur_dir[256];

int main(int argc, char **argv)
{
    init_fnc(InitLppServ, ExitLppServ);
    ErrorHelper::SetupDirectories();
    consoleInit(GFX_BOTTOM, NULL);
    sprintf(cur_dir, "sdmc:/");
    Run("romfs:/index.lua");
    //return 0; // Just caused by all the fnc
    //          // not contain return value pain lol
}