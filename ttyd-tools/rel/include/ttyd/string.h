#pragma once

namespace ttyd::string {

extern "C" {

// strstr
// strchr
// strncmp
int32_t strcmp(const char *string1, const char *string2);
char *strcat(char *destination, const char *sourceString);
char *strncpy(char *destination, const char *sourceString, uint32_t amountOfBytesToCopy);
char *strcpy(char *destination, const char *sourceString);
// strlen

}

}