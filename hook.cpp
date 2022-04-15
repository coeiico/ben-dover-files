//#include "includes.h"
#include "hook.h"

void Patch(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD originalProtectState;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &originalProtectState);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, originalProtectState, &originalProtectState);
}

bool Hook(char* srcToHook, char* dstOurFunc, int lenOfInstruction) {
	if (lenOfInstruction < 5)
		return false;
	DWORD oProtectState;
	VirtualProtect(srcToHook, lenOfInstruction, PAGE_EXECUTE_READWRITE, &oProtectState);
	memset(srcToHook, 0x90, lenOfInstruction);
	uintptr_t relativeAddress = ((uintptr_t) dstOurFunc - (uintptr_t) srcToHook) - 5;
	*srcToHook = (char) 0xE9;
	*(uintptr_t*) (srcToHook + 1) = (uintptr_t) relativeAddress;
	VirtualProtect(srcToHook, lenOfInstruction, oProtectState, &oProtectState);
	return true;
}

char* TrampolineHook(char* src, char* dst, unsigned int len) {
	if (len < 5)
		return 0;
	char* gateway = (char*) VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	uintptr_t jumpAddress = (uintptr_t) (src - gateway - 5);
	*(gateway + len) = (char) 0xE9;
	*(uintptr_t*) (gateway + len + 1) = jumpAddress;
	if (Hook(src, dst, len))
		return gateway;
	return nullptr;
}