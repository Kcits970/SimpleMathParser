#ifndef FORMATTING_H_
#define FORMATTING_H_

#define MAX_OPTION_LENGTH 32

void printNDashes(int n) {
	for (int i = 0; i < n; i++)
		printf("-");
}

void printFormattedOption(const char* option, char command) {
	int offset = MAX_OPTION_LENGTH - strlen(option);
	printf("%s ", option);
	printNDashes(offset);
	printf(" %c", command);
	puts("");
}

void printFormattedTitle(const char* title) {
	puts("");
	printf("%-4s<%s>", "", title);
	puts("");
}

void printNCharacters(const char* str, int n) {
	int length = strlen(str);

	if (length > n) {
		for (int i = 0; i < n - 3; i++)
			printf("%c", str[i]);
		printf("...");
	}
	else {
		int offset = n - length;
		printf(str);
		for (int i = 0; i < n - length; i++)
			printf(" ");
	}
}

void printNDigits(int number, int n) {
	sprintf(defaultBuffer, "%d", number);
	printNCharacters(defaultBuffer, n);
}

#endif