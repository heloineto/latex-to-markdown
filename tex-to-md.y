%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "tex-to-md.h"
%}

%union
{   
    struct ast *a;
    char *string;
};

%token <string> NAME
%token <string> CONTENT
%token NUMBER
// keywords
%token DOCUMENT
%token _BEGIN
%token _END
%token CLASS
%token PACKAGE
%token AUTHOR 
%token TITLE
%token CHAPTER
%token SECTION
%token SUBSECTION
%token PAR
%token BF
%token UNDERLINE
%token IT
%token <string> ITEM
%token ENUMERATE
%token ITEMIZE

%type <a> documentLatex identification settings class package main begin end bodyList chapter section subsection body text textStyle lists numberedList itemList itens

%%

documentLatex: settings identification main {
    $$ = newAST(NT_DOCUMENT, $1, $2, $3, NULL);
    evalAST($$);
}
| settings main {
    $$ = newAST(NT_DOCUMENT, $1, $2, NULL, NULL);
    evalAST($$);
}
;

settings: class package {
    $$ = newAST(NT_SETTINGS, $1, $2, NULL, NULL);
}
| class {
    $$ = newAST(NT_SETTINGS, $1, NULL, NULL, NULL);
}
;

class: CLASS NAME NAME {
    $$ = newClass(NT_CLASS, $2, $3);   
}
;

package: PACKAGE NAME {
    $$ = newPackage(NT_PACKAGE, $2, NULL, NULL);  
}
| PACKAGE NAME NAME {
    $$ = newPackage(NT_PACKAGE, $2, $3, NULL);
} 
| PACKAGE NAME package {
    $$ = newPackage(NT_PACKAGE, $2, NULL, $3);
}
| PACKAGE NAME NAME package {
    $$ = newPackage(NT_PACKAGE, $2, $3, $4);
}
;

identification: TITLE CONTENT AUTHOR CONTENT {
    $$ = newIdentification(NT_IDENTIFICATION, $2, $4);
} 
| TITLE NAME AUTHOR CONTENT {
    $$ = newIdentification(NT_IDENTIFICATION, $2, $4);
}
| TITLE CONTENT AUTHOR NAME {
    $$ = newIdentification(NT_IDENTIFICATION, $2, $4);
}
| TITLE NAME AUTHOR NAME {
    $$ = newIdentification(NT_IDENTIFICATION, $2, $4);
}
| TITLE CONTENT {
    $$ = newIdentification(NT_IDENTIFICATION, $2, NULL);
} 
| TITLE NAME {
    $$ = newIdentification(NT_IDENTIFICATION, $2, NULL);
}
;

main: begin end {
    $$ = newAST(NT_MAIN, $1, $2, NULL, NULL);
}
| begin bodyList end {
    $$ = newAST(NT_MAIN, $1, $3, $2, NULL);
}
;

begin: _BEGIN DOCUMENT {
    $$ = newAST(NT_BEGIN, NULL, NULL, NULL, NULL);
}
;


end: _END DOCUMENT {
    $$ = newAST(NT_END, NULL, NULL, NULL, NULL);
}
;

bodyList: chapter bodyList {
    $$ = newAST(NT_BODYLIST, $1, $2, NULL, NULL);
}
| chapter {
    $$ = newAST(NT_BODYLIST, $1, NULL, NULL, NULL);
}
| section bodyList {
    $$ = newAST(NT_BODYLIST, $1, $2, NULL, NULL);
}
| section {
    $$ = newAST(NT_BODYLIST, $1, NULL, NULL, NULL);
}
| subsection bodyList {
    $$ = newAST(NT_BODYLIST, $1, $2, NULL, NULL);
}
| subsection {
    $$ = newAST(NT_BODYLIST, $1, NULL, NULL, NULL);
}
| body bodyList {
    $$ = newAST(NT_BODYLIST, $1, $2, NULL, NULL);
}
| body {
    $$ = newAST(NT_BODYLIST, $1, NULL, NULL, NULL);
}
;

chapter: CHAPTER CONTENT {
    $$ = newTextSubdivision(NT_CHAPTER, $2, NULL, NULL);
}
| CHAPTER NAME {
    $$ = newTextSubdivision(NT_CHAPTER, $2, NULL, NULL);
}
;

section: SECTION CONTENT {
    $$ = newTextSubdivision(NT_SECTION, $2, NULL, NULL);    
}
| SECTION NAME {
    $$ = newTextSubdivision(NT_SECTION, $2, NULL, NULL);    
}
;

subsection: SUBSECTION CONTENT {
    $$ = newTextSubdivision(NT_SUBSECTION, $2, NULL, NULL);
} 
| SUBSECTION NAME {
    $$ = newTextSubdivision(NT_SUBSECTION, $2, NULL, NULL);
} 
;

body: text {
    $$ = newAST(NT_BODY, $1, NULL, NULL, NULL);
} 
| textStyle {
    $$ = newAST(NT_BODY, $1, NULL, NULL, NULL);
} 
| lists {
    $$ = newAST(NT_BODY, $1, NULL, NULL, NULL);
}
;

text: NAME text {
    $$ = newtext(NT_TEXT, $1, $2);
}
| NAME PAR {
    $$ = newtext(NT_TEXT, $1, NULL);
}
| NAME {
    $$ = newtext(NT_TEXT, $1, NULL);
}
| PAR {
    char *space = (char *) malloc(sizeof(char));
    (*space) = ' '; 
    $$ = newtext(NT_TEXT, space, NULL);
}
;

textStyle: BF NAME {
    $$ = newtextstyle(NT_TEXTSTYLE, $2, TS_BOLD);
} 
| BF CONTENT {
    $$ = newtextstyle(NT_TEXTSTYLE, $2, TS_BOLD);
} 
| UNDERLINE NAME {
    $$ = newtextstyle(NT_TEXTSTYLE, $2, TS_UNDERLINE);
} 
| UNDERLINE CONTENT {
    $$ = newtextstyle(NT_TEXTSTYLE, $2, TS_UNDERLINE);
} 
| IT NAME {
    $$ = newtextstyle(NT_TEXTSTYLE, $2, TS_ITALIC);
}
| IT CONTENT {
    $$ = newtextstyle(NT_TEXTSTYLE, $2, TS_ITALIC);
}
;

lists: numberedList {
    $$ = newAST(NT_LIST, $1, NULL, NULL, NULL);
} | itemList {
    $$ = newAST(NT_LIST, $1, NULL, NULL, NULL);
}
;

numberedList: _BEGIN ENUMERATE itens _END ENUMERATE {
    $$ = newAST(NT_NUMBEREDLIST, $3, NULL, NULL, NULL);
}
;

itemList: _BEGIN ITEMIZE itens _END ITEMIZE {
    $$ = newAST(NT_ITEMLIST, $3, NULL, NULL, NULL);
}
;

itens: ITEM {
    $$ = newitens(NT_ITENS, $1, NULL);
} | ITEM itens {
    $$ = newitens(NT_ITENS, $1, $2);
}
;
