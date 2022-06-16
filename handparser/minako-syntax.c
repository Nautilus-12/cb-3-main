#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "minako.h"
#include <stdbool.h>

int currentToken;
int nextToken;

typedef void MetaSymbol(void);
MetaSymbol program, functiondefinition, statementlist, block, statement, statement2, ifstatement, 
    returnstatement, returnstatement2, print, type, assignment, assignment2, expr, expr2, 
    simpexpr, simpexpr2, term, term2, factor, factor2;

int isToken(const int chs) {
    if ((chs == nextToken)) {
        return true;
    }
    return false;
}

void eat() {
    currentToken = nextToken;
    nextToken = yylex();
    if (nextToken == -1)
        exit(0);
}

void isTokenAndEat(const int chs) {
    if (!(isToken(chs))) {
        printf("ERROR: Syntaxfehler in Zeile (%i)\n", yylineno);
        exit(-1);
    }
    eat();
}

// Umsetzung der Grammatik:
void program() {
    if (isToken(KW_BOOLEAN) || isToken(KW_FLOAT) || isToken(KW_INT) || isToken(KW_VOID))
        {functiondefinition(), program();}
    else 
        {isTokenAndEat(EOF);}
}

void functiondefinition() {
    type(), isTokenAndEat(ID), isTokenAndEat('('), isTokenAndEat(')'), isTokenAndEat('{'), statementlist(), isTokenAndEat('}'); 
}

void statementlist() {
    if (isToken('{') || isToken(KW_IF) || isToken(KW_RETURN) || isToken(KW_PRINTF) || isToken(ID))
        {block(), statementlist();}
}

void block() {
    if (isToken('{'))
        {isTokenAndEat('{'), statementlist(), isTokenAndEat('}');}
    else
        {statement();}
}

void statement() {
    if (isToken(KW_IF))
        {ifstatement();}
    else if (isToken(KW_RETURN))
        {returnstatement(), isTokenAndEat(';');}
    else if (isToken(KW_PRINTF))
        {print(), isTokenAndEat(';');}
    else if (isToken(ID))
        {isTokenAndEat(ID), statement2();}
}

void statement2() {
    if (isToken('='))
        {isTokenAndEat('='), assignment(); isTokenAndEat(';');}
    else if (isToken('('))
        {isTokenAndEat('('), isTokenAndEat(')'); isTokenAndEat(';');}
}

void ifstatement() {
    isTokenAndEat(KW_IF), isTokenAndEat('('), assignment(), isTokenAndEat(')'), block();
}

void returnstatement() {
    isTokenAndEat(KW_RETURN), returnstatement2();
}

void returnstatement2() {
    if (!isToken(';'))
        {assignment();}
}

void print() {
    isTokenAndEat(KW_PRINTF), isTokenAndEat('('), assignment(), isTokenAndEat(')');
}

void type() {
    if (isToken(KW_BOOLEAN))
        {isTokenAndEat(KW_BOOLEAN);}
    else if (isToken(KW_FLOAT))
        {isTokenAndEat(KW_FLOAT);}
    else if (isToken(KW_INT))
        {isTokenAndEat(KW_INT);}
    else if (isToken(KW_VOID))
        {isTokenAndEat(KW_VOID);}
}

void statassignment() {
    isTokenAndEat(ID), isTokenAndEat('='), assignment();
}

void assignment() {
    if (isToken('-'))
        {isTokenAndEat('-'), term(), simpexpr2(), expr2();}
    else if (isToken(CONST_INT))
        {isTokenAndEat(CONST_INT), term2(), simpexpr2(), expr2();}
    else if (isToken(CONST_FLOAT))
        {isTokenAndEat(CONST_FLOAT), term2(), simpexpr2(), expr2();}
    else if (isToken(CONST_BOOLEAN))
        {isTokenAndEat(CONST_BOOLEAN), term2(), simpexpr2(), expr2();}
    else if (isToken('('))
        {isTokenAndEat('('), assignment(), isTokenAndEat(')'), term2(), simpexpr2(), expr2();}
    else if (isToken(ID))
        {isTokenAndEat(ID), assignment2();}
}

void assignment2() {
    if (isToken('='))
        {isTokenAndEat('='), assignment();}
    else if (isToken('('))
        {isTokenAndEat('('), isTokenAndEat(')'), term2(), simpexpr2(), expr2();}
    else 
        {term2(), simpexpr2(), expr2();}
}

void expr() {
    simpexpr(), expr2();
}

void expr2() {
    if (isToken(EQ))
        {isTokenAndEat(EQ), simpexpr();}
    else if (isToken(NEQ))
        {isTokenAndEat(NEQ), simpexpr();}
    else if (isToken(LEQ))
        {isTokenAndEat(LEQ), simpexpr();}
    else if (isToken(GEQ))
        {isTokenAndEat(GEQ), simpexpr();}
    else if (isToken(LSS))
        {isTokenAndEat(LSS), simpexpr();}
    else if (isToken(GRT))
        {isTokenAndEat(GRT), simpexpr();}
}

void simpexpr() {
    if (isToken('-'))
        {isTokenAndEat('-'), term(), simpexpr2();}
    else
        {term(), simpexpr2();}
}

void simpexpr2() {
    if (isToken('+'))
        {isTokenAndEat('+'), term(), simpexpr2();}
    else if (isToken('-'))
        {isTokenAndEat('-'), term(), simpexpr2();}
    else if (isToken(OR))
        {isTokenAndEat(OR), term(), simpexpr2();}
}

void term() {
    factor(), term2();
}

void term2() {
    if (isToken('*'))
        {isTokenAndEat('*'), factor(), term2();}
    else if (isToken('/'))
        {isTokenAndEat('/'), factor(), term2();}
    else if (isToken(AND))
        {isTokenAndEat(AND), factor(), term2();}
}

void factor() {
    if (isToken(CONST_INT))
        {isTokenAndEat(CONST_INT);}
    else if (isToken(CONST_FLOAT))
        {isTokenAndEat(CONST_FLOAT);}
    else if (isToken(CONST_BOOLEAN))
        {isTokenAndEat(CONST_BOOLEAN);}
    else if (isToken('('))
        {isTokenAndEat('('), assignment(), isTokenAndEat(')');}
    else if (isToken(ID))
        {isTokenAndEat(ID), factor2();}
}

void factor2() {
    if (isToken('('))
        {isTokenAndEat('('), isTokenAndEat(')');}
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
		yyin = stdin;
	} else {
		yyin = fopen(argv[1], "r");
		if (yyin == 0) {
			fprintf(
				stderr,
				"Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n",
				argv[1]
			);
			exit(-1);
		}
	}

    nextToken = yylex();

    program();

    return 0;
}