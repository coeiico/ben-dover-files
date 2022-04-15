#pragma once

#include "includes.h"

void Patch(BYTE* dst, BYTE* src, unsigned int size);
bool Hook(char* src, char* dst, int len);
char* TrampolineHook(char* src, char* dst, unsigned int len);