/*
*   This file is part of lpp-3ds-next
*   based on https://github.com/Rinnegatamante/lpp-3ds/
*   Copyright (C) 2021-2023 Tobi-D7
*/

#include "include/luaplayer.hpp"
#include <3ds.h>
#include <stdio.h>
#include <string.h> 

bool ftp_state;

int main(int argc, char **argv) {
	aptInit();
    cfguInit();
    Run("romfs:/index.lua");
    cfguExit();
    aptExit();
    return 0;
}