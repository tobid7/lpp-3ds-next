#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>

// Tis is for the Npi-Debug Viewer Tech

namespace Npi
{
    /// @brief Initialisizes The Server
    void InitDbgServer();
    /// @brief For the Thread
    void DbgServerLoop();
    /// @brief Kill the Server
    void ExitDbgServer();
}