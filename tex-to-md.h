#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef enum NodeType {
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
} NodeType;

enum TextStyle {
    TS_BOLD,
    TS_ITALIC,
    TS_UNDERLINE
};

typedef struct ASTNode {
    NodeType nodeType;
    struct ASTNode* n1;
    struct ASTNode* n2;
    struct ASTNode* n3;
    struct ASTNode* n4;
} ASTNode;

typedef struct Class {
    NodeType nodeType;
    char* content1;
    char* content2;
} Class;

typedef struct Identification {
    NodeType nodeType;
    char* title;
    char* author;
} Identification;

typedef struct Itens {
    NodeType nodeType;
    char* content;
    struct ASTNode* next;
} Itens;

typedef struct Package {
    NodeType nodeType;
    char* content1;
    char* content2;
    struct Package* next;
} Package;

typedef struct Text {
    NodeType nodeType;
    char* content;
    struct Text* next;
} Text;

typedef struct StructTextStyle {
    NodeType nodeType;
    char* content;
    enum TextStyle textStyle;
} TextStyle;

typedef struct TextSubdivision {
    NodeType nodeType;
    char* content;
    struct ASTNode* n1;
    struct ASTNode* n2;
} TextSubdivision;

//* AST (Abstract Syntax Tree) *//
struct ASTNode* newAST(NodeType nodeType, struct ASTNode* n1, struct ASTNode* n2, struct ASTNode* n3, struct ASTNode* n4);
struct ASTNode* newClass(NodeType nodeType, char* content1, char* content2);
struct ASTNode* newPackage(NodeType nodeType, char* content1, char* content2, struct ASTNode* next);
struct ASTNode* newIdentification(NodeType nodeType, char* n1, char* n2);
struct ASTNode* newTextSubdivision(NodeType nodeType, char* content, struct ASTNode* n1, struct ASTNode* n2);
struct ASTNode* newText(NodeType nodeType, char* content, struct ASTNode* next);
struct ASTNode* newTextStyle(NodeType nodeType, char* content, enum TextStyle textStyle);
struct ASTNode* newItems(NodeType nodeType, char* content, struct ASTNode* next);
void evalAST(struct ASTNode*);
void freeAST(struct ASTNode*);

//* Flex definitions *//
int yylineno;
FILE* yyin;
int yylex();
void yyerror(const char* s);
int yyparse();

//* Utils *//
void copyStr(char** dest, char* src, bool removeBrackets);
char* numberToStr(long long int value);
char* outFileName;
FILE* outFilePtr;
int currChapter;
int currSection;
int currSubSection;