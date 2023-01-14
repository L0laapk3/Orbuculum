#pragma once

#include <Windows.h>
#include <string>

DWORD launchRocketLeague(HANDLE& hProcess);
void injectBM(HANDLE hProcess);

void tryHideRocketLeagueWindow(DWORD pid);