#define _CRT_SECURE_NO_WARNINGS
#define VERSION_ID 2

typedef enum {false=0, true=1} bool;

#include <stdio.h>
#include <stdlib.h>

#include "userinput.h"
#include "linkedlist.h"
#include "mathexpression.h"
#include "formatting.h"

LINKED_LIST* expressionList = NULL;

void printMainOptions() {
	printFormattedTitle("MAIN OPTIONS");
	printFormattedOption("INPUT MATH EXPRESSION", 'A');
	printFormattedOption("EDIT EXPRESSION VARIABLES", 'B');
	printFormattedOption("VIEW EXPRESSION DETAILS", 'C');
	printFormattedOption("DELETE EXPRESSION", 'D');
	printFormattedOption("VIEW HISTORY", 'E');
	printFormattedOption("CLEAR HISTORY", 'F');
	printFormattedOption("EXIT", 'Q');
}

void promptExpressionInput() {
	printFormattedTitle("EXPRESSION INPUT PROMPT");

	EXPRESSION* expr = createEmptyExpression();
	getInputInDefaultBuffer("ENTER A MATH EXPRESSION: ");
	setExpressionStr(expr, defaultBuffer);
	parse(expr, true);

	printf("'%s' EVALUATES TO '%f'\n", expr->str, expr->result);
	appendToList(expressionList, expr, &deleteExpression);
}

void printExpressionList() {
	printAllValues(expressionList, "EXPRESSION", &printExpression);
}

int getSelectionIndex() {
	printFormattedTitle("EXPRESSION SELECTION PROMPT");
	printExpressionList();

	return getNumberInput("SELECT AN EXPRESSION: ", 1, expressionList->size) - 1;
}

void promptVariableEdit() {
	EXPRESSION* expr = getValueAt(expressionList, getSelectionIndex());

	printFormattedTitle("VARIABLE EDIT PROMPT");
	printf("SELECTED EXPRESSION: ");
	printExpression(expr);

	if (expr->varX != NULL) {
		getInputInDefaultBuffer("VARIABLE X: ");
		setExpressionStr(expr->varX, defaultBuffer);
		parse(expr->varX, false);
	}
	if (expr->varY != NULL) {
		getInputInDefaultBuffer("VARIABLE Y: ");
		setExpressionStr(expr->varY, defaultBuffer);
		parse(expr->varY, false);
	}
	if (expr->varZ != NULL) {
		getInputInDefaultBuffer("VARIABLE Z: ");
		setExpressionStr(expr->varZ, defaultBuffer);
		parse(expr->varZ, false);
	}

	if (expr->varX == NULL && expr->varY == NULL && expr->varZ == NULL)
		printf("THE SELECTED EXPRESSION DOES NOT CONTAIN ANY VARIABLES\n");
	else {
		parse(expr, true);
		printf("'%s' EVALUATES TO '%f'\n", expr->str, expr->result);
	}
	getCharacterInput(">>");
}

void promptExpressionDetails() {
	EXPRESSION* expr = getValueAt(expressionList, getSelectionIndex());

	printFormattedTitle("EXPRESSION DETAILS");
	printExpressionDetails(expr);
	getCharacterInput(">>");
}

void promptExpressionDeletion() {
	int deleteIndex = getSelectionIndex();

	if (removeNodeAt(expressionList, deleteIndex, &deleteExpression))
		printf("DELETED\n");
	else
		printf("DELETION FAILED\n");
	getCharacterInput(">>");
}

void printHistory() {
	printFormattedTitle("HISTORY");

	printExpressionList();
	getCharacterInput(">>");
}

void clearHistory() {
	clearList(expressionList, &deleteExpression);
	printf("SUCCESSFULLY CLEARED HISTORY\n");
	getCharacterInput(">>");
}

void promptMainMenu() {
	char optionSelection = 0;
	bool exitMainPrompt = false;

	while (!exitMainPrompt) {
		printMainOptions();
		optionSelection = getCharacterInput(">>");

		switch (optionSelection) {
		case 'A': promptExpressionInput(); break;
		case 'B': promptVariableEdit(); break;
		case 'C': promptExpressionDetails(); break;
		case 'D': promptExpressionDeletion(); break;
		case 'E': printHistory(); break;
		case 'F': clearHistory(); break;
		case 'Q': exit(0);
		default:
			printf("'%c' IS NOT AN IDENTIFIABLE COMMAND\n", optionSelection);
			break;
		}
	}
}

int main(void) {
	printf(">>MATH EXPRESSION PARSER: VERSION %d\n", VERSION_ID);

	expressionList = createEmptyList();
	promptMainMenu();

	return 0;
}
