#define _CRT_SECURE_NO_WARNINGS
#define VERSION_ID 2

typedef enum {false=0, true=1} bool;

#include <stdio.h>
#include <stdlib.h>

#include "userinput.h"
#include "linkedlist.h"
#include "mathexpression.h"
#include "formatting.h"

LINKED_LIST* historyList = NULL;

void printMainOptions() {
	printFormattedTitle("MAIN OPTIONS");
	printFormattedOption("INPUT MATH EXPRESSION", 'A');
	printFormattedOption("VIEW HISTORY", 'B');
	printFormattedOption("CLEAR HISTORY", 'C');
	printFormattedOption("EXIT", 'Q');
}

void promptExpressionInput() {
	printFormattedTitle("EXPRESSION INPUT PROMPT");

	EXPRESSION* expr = createEmptyExpression();
	getInputInDefaultBuffer("ENTER A MATH EXPRESSION: ");
	setExpressionStr(expr, defaultBuffer);
	parse(expr, true);

	printf("'%s' EVALUATES TO '%f'\n", expr->str, expr->result);
	appendToList(historyList, expr);
}

void printHistory() {
	printFormattedTitle("HISTORY");

	forEachValue(historyList, &printAsMathExpression);
	getCharacterInput(">>");
}

void clearHistory() {
	clearList(historyList);
	printf("SUCCESSFULLY CLEARED HISTORY\n");
}

void promptMainMenu() {
	char optionSelection = 0;
	bool exitMainPrompt = false;

	while (!exitMainPrompt) {
		printMainOptions();
		optionSelection = getCharacterInput(">>");

		switch (optionSelection) {
		case 'A': promptExpressionInput(); break;
		case 'B': printHistory(); break;
		case 'C': clearHistory(); break;
		case 'Q': exit(0);
		default:
			printf("'%c' IS NOT AN IDENTIFIABLE COMMAND\n", optionSelection);
			break;
		}
	}
}

int main(void) {
	printf(">>MATH EXPRESSION PARSER: VERSION %d\n", VERSION_ID);

	historyList = createEmptyList();
	promptMainMenu();

	return 0;
}