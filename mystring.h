#ifndef MYSTRING_H_
#define MYSTRING_H_

#include <string.h>

char* getEmptyString(int characters) {
	return calloc(characters, sizeof(char));
}

bool isEqualToNChars(const char* str1, const char* str2, int n) {
	if (strncmp(str1, str2, n) == 0)
		return true;
	else
		return false;
}

void printStringFromRange(const char* str, int start, int end) {
	int rangeSize = end - start;

	for (int i = 0; i < rangeSize; i++)
		printf("%c", str[start + i]);
}

bool isNumeric(char c) {
	if (c >= '0' && c <= '9' || c == '.')
		return true;
	return false;
}

bool isLowercaseAlphabet(char c) {
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}

bool removeNewline(char* str) {
	char* lnptr = strchr(str, '\n');
	if (lnptr == NULL)
		return false;
	else
		*lnptr = '\0';

	return true;
}

#endif