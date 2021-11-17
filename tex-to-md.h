#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define ELEMENT_PADDING "\n\n"

/* interface com o flex */

extern int yylineno; /* do flex */
extern FILE* yyin; /* do flex */

int yylex();
void yyerror(const char* s);
int yyparse();

char* outFileName;
FILE* outFilePtr;

int currChapter;
int currSection;
int currSubSection;

enum NodeType {
    NT_DOCUMENT = 0,
    NT_SETTINGS,
    NT_CLASS,
    NT_PACKAGE,
    NT_IDENTIFICATION,
    NT_MAIN,
    NT_BEGIN,
    NT_END,
    NT_BODYLIST,
    NT_CHAPTER,
    NT_SUBSECTION,
    NT_SECTION,
    NT_BODY,
    NT_TEXT,
    NT_TEXTSTYLE,
    NT_LIST,
    NT_NUMBEREDLIST,
    NT_ITEMLIST,
    NT_ITENS
};

enum TextStyle {
    TS_BOLD,
    TS_ITALIC,
    TS_UNDERLINE
};

typedef struct ast {
    enum NodeType nodeType;
    struct ast* n1;
    struct ast* n2;
    struct ast* n3;
    struct ast* n4;
} NodeAST;

struct StructClass {
    enum NodeType nodeType;
    char* content1;
    char* content2;
};

struct StructPackage {
    enum NodeType nodeType;
    char* content1;
    char* content2;
    struct StructPackage* next;
};

struct StructIdentification {
    enum NodeType nodeType;
    char* title;
    char* author;
};

struct StructTextSubdivision {
    enum NodeType nodeType;
    char* content;
    struct ast* n1;
    struct ast* n2;
};

struct StructText {
    enum NodeType nodeType;
    char* content;
    struct StructText* next;
};

struct StructTextStyle {
    enum NodeType nodeType;
    char* content;
    enum TextStyle textStyle;
};

struct StructItens {
    enum NodeType nodeType;
    char* content;
    struct ast* next;
};

//* AST (Abstract Syntax Tree) *//
struct ast* newAST(enum NodeType nodeType, struct ast* n1, struct ast* n2, struct ast* n3, struct ast* n4);
struct ast* newClass(enum NodeType nodeType, char* content1, char* content2);
struct ast* newPackage(enum NodeType nodeType, char* content1, char* content2, struct ast* next);
struct ast* newIdentification(enum NodeType nodeType, char* n1, char* n2);
struct ast* newTextSubdivision(enum NodeType nodeType, char* content, struct ast* n1, struct ast* n2);
struct ast* newtext(enum NodeType nodeType, char* content, struct ast* next);
struct ast* newtextstyle(enum NodeType nodeType, char* content, enum TextStyle textStyle);
struct ast* newitens(enum NodeType nodeType, char* content, struct ast* next);
void evalAST(struct ast*);
void freeAST(struct ast*);

//* Utils *//
void copyStr(char** dest, char* src, bool removeBrackets);
char* numberToStr(long long int value);