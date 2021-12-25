#ifndef MATHEXPRESSION_H_
#define MATHEXPRESSION_H_

#define PI 3.14159265358979323846
#define E 2.71828182845904523536
#define PHI 1.61803398874989484820 //golden ratio

#define VAR_X "x"
#define VAR_Y "y"
#define VAR_Z "z"

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

void deleteExpression(void* voidExpr) {
	EXPRESSION* expr = (EXPRESSION*) voidExpr;
	free(expr->str);

	if (expr->varX != NULL) deleteExpression(expr->varX);
	if (expr->varY != NULL) deleteExpression(expr->varY);
	if (expr->varZ != NULL) deleteExpression(expr->varZ);
	free(expr);
}

void setExpressionStr(EXPRESSION* expr, const char* str) {
	free(expr->str);

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
double parseNumber(PARSE_OBJECT* obj);
double parseNamedFactor(PARSE_OBJECT* obj);

double getVariable(EXPRESSION* expr, const char* varName) {
	EXPRESSION** variablePtr = NULL;

	if (varName == VAR_X) variablePtr = &expr->varX;
	else if (varName == VAR_Y) variablePtr = &expr->varY;
	else if (varName == VAR_Z) variablePtr = &expr->varZ;

	if (*variablePtr == NULL) {
		*variablePtr = createEmptyExpression();
		printf("ENTER A VALUE FOR VARIABLE '%s': ", varName);
		getInputInDefaultBuffer("");
		setExpressionStr(*variablePtr, defaultBuffer);
		parse(*variablePtr, false);
	}

	return (*variablePtr)->result;
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

char* getUnparsedStr(PARSE_OBJECT* obj) {
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

	if (obj->currentChar != '\0')
		printf("UNABLE TO PARSE FULL EXPRESSION: UNREACHABLE '%s' AT INDEX %d\n", getUnparsedStr(obj), obj->parseIndex);

	free(obj);
}

//expression = term | expression '+' term | expression '-' term
double parseExpression(PARSE_OBJECT* obj) {
	double result = parseTerm(obj);

	while (true) {
		if (consumeChar(obj, '+')) result += parseTerm(obj);
		else if (consumeChar(obj, '-')) result -= parseTerm(obj);
		else {
			consumeWhiteSpace(obj);
			return result;
		}
	}
}

//term = factor | term '*' factor | term '/' factor
double parseTerm(PARSE_OBJECT* obj) {
	double result = parseFactor(obj);

	while (true) {
		if (consumeChar(obj, '*')) result *= parseFactor(obj);
		else if (consumeChar(obj, '/')) result /= parseFactor(obj);
		else return result;
	}
}

//factor = '+'factor | '-'factor | (expression) | number | namedFactor | factor '^' factor
double parseFactor(PARSE_OBJECT* obj) {
	if (consumeChar(obj, '+')) return parseFactor(obj);
	else if (consumeChar(obj, '-')) return -parseFactor(obj);

	double result = 0;

	if (consumeChar(obj, '(')) {
		result = parseExpression(obj);
		consumeChar(obj, ')');
	}

	else if (isNumeric(obj->currentChar))
		result = parseNumber(obj);

	else if (isLowercaseAlphabet(obj->currentChar))
		result = parseNamedFactor(obj);

	else {
		if (obj->currentChar == '\0')
			printf("REACHED END OF STRING WHILE PARSING\n");
		else
			printf("UNEXPECTED CHARACTER '%c' AT INDEX %d\n", obj->currentChar, obj->parseIndex);
	}

	if (consumeChar(obj, '^'))
		result = pow(result, parseFactor(obj));

	return result;
}

//number = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '.' | numbernumber
double parseNumber(PARSE_OBJECT* obj) {
	double result = atof(getUnparsedStr(obj));
	while (isNumeric(obj->currentChar)) advanceIndex(obj);

	return result;
}

/*
namedFactor = constant | variable | functionName factor
constant = "e" | "pi" | "phi"
variable = "x" | "y" | "z"
functionName = "sqrt" | "cbrt" | "sin" | "cos" | "tan" | "ln"
*/
double parseNamedFactor(PARSE_OBJECT* obj) {
	int nameStartIndex = obj->parseIndex;
	const char* name = getUnparsedStr(obj);
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

	//checking for variable names (only if variables are allowed)
	if (obj->allowVariables) {
		if (isEqualToNChars(name, VAR_X, nameLength))
			return getVariable(obj->expression, VAR_X);

		else if (isEqualToNChars(name, VAR_Y, nameLength))
			return getVariable(obj->expression, VAR_Y);

		else if (isEqualToNChars(name, VAR_Z, nameLength))
			return getVariable(obj->expression, VAR_Z);
	}

	//checking for function names
	double nextFactor = parseFactor(obj);

	if (isEqualToNChars(name, "sqrt", nameLength))
		return sqrt(nextFactor);

	else if (isEqualToNChars(name, "cbrt", nameLength))
		return cbrt(nextFactor);

	else if (isEqualToNChars(name, "sin", nameLength))
		return sin(nextFactor);

	else if (isEqualToNChars(name, "cos", nameLength))
		return cos(nextFactor);

	else if (isEqualToNChars(name, "tan", nameLength))
		return tan(nextFactor);

	else if (isEqualToNChars(name, "ln", nameLength))
		return log(nextFactor);

	else {
		printf("UNDEFINED CONSTANT/VARIABLE/FUNCTION NAME ");
		printf("'"); printStringFromRange(name, 0, nameLength); printf("'");
		printf(" AT INDEX %d\n", nameStartIndex);
		return 0;
	}
}

void printExpression(void* voidExpr) {
	EXPRESSION* expr = (EXPRESSION*) voidExpr;
	printf("'%s'\n", expr->str);
}

void printExpressionDetails(void* voidExpr) {
	if (voidExpr == NULL) return;

	EXPRESSION* expr = (EXPRESSION*) voidExpr;
	printf("FULL EXPRESSION: '%s'\n", expr->str);

	if (expr->varX != NULL) printf("VARIABLE X: '%s'\n", expr->varX->str);
	if (expr->varY != NULL) printf("VARIABLE Y: '%s'\n", expr->varY->str);
	if (expr->varZ != NULL) printf("VARIABLE Z: '%s'\n", expr->varZ->str);

	printf("RESULT: %f\n", expr->result);
}

#endif