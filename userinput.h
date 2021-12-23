#ifndef USERINPUT_H_
#define USERINPUT_H_

#include "mystring.h"

#define DEFAULT_BUFFER_LENGTH 1024
char defaultBuffer[DEFAULT_BUFFER_LENGTH];

void getAsciiInput(const char* promptMessage, char* str, int inputLimit) {
	printf(promptMessage);
	fgets(str, inputLimit, stdin);

	if (!removeNewline(str))
		while (getchar() != '\n');
}

void getInputInDefaultBuffer(const char* promptMessage) {
	getAsciiInput(promptMessage, defaultBuffer, DEFAULT_BUFFER_LENGTH);
}

char getCharacterInput(const char* promptMessage) {
	getInputInDefaultBuffer(promptMessage);
	return defaultBuffer[0];
}

double getNumberInput(const char* promptMessage) {
	getInputInDefaultBuffer(promptMessage);

	double number = atof(defaultBuffer);
	return number;
}

#endif