#ifndef MATHEXPRESSION_H_
#define MATHEXPRESSION_H_

#define PI 3.14159265358979323846
#define E 2.71828182845904523536
#define PHI 1.61803398874989484820 //golden ratio

#include <math.h>
#include <string.h>
#include "mystring.h"
#include "userinput.h"

typedef struct expression {
	char* str;
	double result;
	struct expression* varX;
	struct expression* varY;
	struct expression* varZ;
} EXPRESSION;

EXPRESSION* createEmptyExpression() {
	EXPRESSION* expr = malloc(sizeof(EXPRESSION));
	expr->str = NULL;
	expr->result = 0;
	expr->varX = NULL, expr->varY = NULL, expr->varZ = NULL;

	return expr;
}

void setExpressionStr(EXPRESSION* expr, const char* str) {
	int length = strlen(str);
	expr->str = getEmptyString(length + 1);
	strcpy(expr->str, str);
}

typedef struct {
	EXPRESSION* expression;
	int parseIndex;
	char currentChar;
	bool allowVariables;
} PARSE_OBJECT;

void parse(EXPRESSION*, bool);
double parseExpression(PARSE_OBJECT* obj);
double parseTerm(PARSE_OBJECT* obj);
double parseFactor(PARSE_OBJECT* obj);
double parseNamedFactor(PARSE_OBJECT* obj);

double getVariable(EXPRESSION* expr, char varType) {
	EXPRESSION** variable = NULL;

	if (varType == 'x') variable = &expr->varX;
	else if (varType == 'y') variable = &expr->varY;
	else if (varType == 'z') variable = &expr->varZ;

	if (*variable == NULL) {
		*variable = createEmptyExpression();
		printf("ENTER A VALUE FOR VARIABLE '%c': ", varType);
		getInputInDefaultBuffer("");
		setExpressionStr(*variable, defaultBuffer);
		parse(*variable, false);
	}

	return (*variable)->result;
}

void updateCurrentChar(PARSE_OBJECT* obj) {
	obj->currentChar = (obj->expression->str)[obj->parseIndex];
}

void advanceIndex(PARSE_OBJECT* obj) {
	if (obj->currentChar == '\0') return;

	obj->parseIndex++;
	updateCurrentChar(obj);
}

void consumeWhiteSpace(PARSE_OBJECT* obj) {
	while (obj->currentChar == ' ')
		advanceIndex(obj);
}

bool consumeChar(PARSE_OBJECT* obj, char c) {
	consumeWhiteSpace(obj);

	if (obj->currentChar == c) {
		advanceIndex(obj);
		return true;
	}
	else
		return false;
}

char* getUnparsedPosition(PARSE_OBJECT* obj) {
	return obj->expression->str + obj->parseIndex;
}

PARSE_OBJECT* createParseObject(EXPRESSION* expr, bool allowVariables) {
	PARSE_OBJECT* obj = malloc(sizeof(PARSE_OBJECT));
	obj->expression = expr;
	obj->parseIndex = 0;
	updateCurrentChar(obj);
	obj->allowVariables = allowVariables;

	return obj;
}

void parse(EXPRESSION* expr, bool allowVariables) {
	PARSE_OBJECT* obj = createParseObject(expr, allowVariables);
	expr->result = parseExpression(obj);

	free(obj);
}

//An expression is a single term, or a sum of two or more terms.
double parseExpression(PARSE_OBJECT* obj) {
	double result = parseTerm(obj);

	while (true) {
		if (consumeChar(obj, '+')) result += parseTerm(obj);
		else if (consumeChar(obj, '-')) result -= parseTerm(obj);
		else return result;
	}
}

//A term is a single factor, or a product of two or more factors.
double parseTerm(PARSE_OBJECT* obj) {
	double result = parseFactor(obj);

	while (true) {
		if (consumeChar(obj, '*')) result *= parseFactor(obj);
		else if (consumeChar(obj, '/')) result /= parseFactor(obj);
		else return result;
	}
}

//A factor is one part of a product.
double parseFactor(PARSE_OBJECT* obj) {
	if (consumeChar(obj, '+')) return parseFactor(obj);
	else if (consumeChar(obj, '-')) return -parseFactor(obj);

	double result = 0;
	if (consumeChar(obj, '(')) {
		result = parseExpression(obj);

		if (!consumeChar(obj, ')'))
			printf("CLOSING PARENTHESIS IS NOT PRESENT: EVALUATION MAY BE INCORRECT\n");
	}
	else if (isNumeric(obj->currentChar)) {
		result = atof(getUnparsedPosition(obj));
		while (isNumeric(obj->currentChar)) advanceIndex(obj);
	}
	else if (isLowercaseAlphabet(obj->currentChar)) {
		result = parseNamedFactor(obj);
	}
	else {
		if (obj->currentChar == '\0')
			printf("REACHED END OF EXPRESSION WHILE PARSING\n");
		else
			printf("UNEXPECTED CHARACTER '%c' AT INDEX %d\n", obj->currentChar, obj->parseIndex);
		result = 0;
	}

	if (consumeChar(obj, '^'))
		result = pow(result, parseFactor(obj));

	return result;
}

//A named factor is a named variable, a special constant, or an output of an existing function. (ex: x, e, sqrt(pi), sin(2), tanh(sqrt2 - 4))
double parseNamedFactor(PARSE_OBJECT* obj) {
	int nameStartIndex = obj->parseIndex;
	const char* name = getUnparsedPosition(obj);
	int nameLength = 0;

	while (isLowercaseAlphabet(obj->currentChar)) {
		advanceIndex(obj);
		nameLength++;
	}

	//checking for constant names
	if (isEqualToNChars(name, "e", nameLength))
		return E;
	else if (isEqualToNChars(name, "pi", nameLength))
		return PI;
	else if (isEqualToNChars(name, "phi", nameLength))
		return PHI;

	//checking for variable names
	else if (obj->allowVariables && isEqualToNChars(name, "x", nameLength))
		return getVariable(obj->expression, 'x');
	else if (obj->allowVariables && isEqualToNChars(name, "y", nameLength))
		return getVariable(obj->expression, 'y');
	else if (obj->allowVariables && isEqualToNChars(name, "z", nameLength))
		return getVariable(obj->expression, 'z');

	//checking for function names
	else {
		double nextFactor = parseFactor(obj);

		if (isEqualToNChars(name, "sqrt", nameLength))
			return sqrt(nextFactor);

		else if (isEqualToNChars(name, "sin", nameLength))
			return sin(nextFactor);

		else if (isEqualToNChars(name, "cos", nameLength))
			return cos(nextFactor);

		else if (isEqualToNChars(name, "tan", nameLength))
			return tan(nextFactor);

		else if (isEqualToNChars(name, "ln", nameLength))
			return log(nextFactor);

		else {
			printf("UNDEFINED CONSTANT/FUNCTION NAME ");
			printf("'"); printStringFromRange(name, 0, nameLength); printf("'");
			printf(" AT INDEX %d\n", nameStartIndex);
			return 0;
		}
	}
}

void printAsMathExpression(void* expr) {
	EXPRESSION* myExpr = (EXPRESSION*)expr;
	printf("Expression: '%s', Result: '%f'\n", myExpr->str, myExpr->result);
}

#endif