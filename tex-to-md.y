%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "tex-to-md.h"
%}

%union {
    ASTNode *a;
    char *string;
};

%token <string> NAME
%token <string> CONTENT
%token <string> ITEM
%token DOCUMENT _BEGIN _END CLASS PACKAGE AUTHOR TITLE CHAPTER SECTION SUBSECTION
%token PAR BF UNDERLINE IT ENUMERATE ITEMIZE NUMBER NEWLINE

%type <a> documentoLatex identificacao configuracao class package main begin end corpoLista chapter section subsection corpo text textStyle lists numberedList itemList itens

%%

documentoLatex: configuracao identificacao main { $$ = newAST(NT_DOCUMENT, $1, $2, $3, NULL); evalAST($$); }
| configuracao main { $$ = newAST(NT_DOCUMENT, $1, $2, NULL, NULL); evalAST($$); }
;

configuracao: class package { $$ = newAST(NT_SETTINGS, $1, $2, NULL, NULL); }
| class { $$ = newAST(NT_SETTINGS, $1, NULL, NULL, NULL); }
;

identificacao: TITLE CONTENT AUTHOR CONTENT { $$ = newIdentification(NT_IDENTIFICATION, $2, $4); } 
| TITLE NAME AUTHOR CONTENT { $$ = newIdentification(NT_IDENTIFICATION, $2, $4); }
| TITLE CONTENT AUTHOR NAME { $$ = newIdentification(NT_IDENTIFICATION, $2, $4); }
| TITLE NAME AUTHOR NAME { $$ = newIdentification(NT_IDENTIFICATION, $2, $4); }
| TITLE CONTENT { $$ = newIdentification(NT_IDENTIFICATION, $2, NULL); } 
| TITLE NAME { $$ = newIdentification(NT_IDENTIFICATION, $2, NULL); }
;

begin: _BEGIN DOCUMENT { $$ = newAST(NT_BEGIN, NULL, NULL, NULL, NULL); }
;


end: _END DOCUMENT { $$ = newAST(NT_END, NULL, NULL, NULL, NULL); }
;

package: PACKAGE NAME { $$ = newPackage(NT_PACKAGE, $2, NULL, NULL); }
| PACKAGE NAME NAME { $$ = newPackage(NT_PACKAGE, $2, $3, NULL); } 
| PACKAGE NAME package { $$ = newPackage(NT_PACKAGE, $2, NULL, $3); }
| PACKAGE NAME NAME package { $$ = newPackage(NT_PACKAGE, $2, $3, $4); }
;

main: begin end { $$ = newAST(NT_MAIN, $1, $2, NULL, NULL); }
| begin corpoLista end { $$ = newAST(NT_MAIN, $1, $3, $2, NULL); }
;

corpoLista: chapter corpoLista { $$ = newAST(NT_BODYLIST, $1, $2, NULL, NULL); }
| chapter { $$ = newAST(NT_BODYLIST, $1, NULL, NULL, NULL); }
| section corpoLista { $$ = newAST(NT_BODYLIST, $1, $2, NULL, NULL); }
| section { $$ = newAST(NT_BODYLIST, $1, NULL, NULL, NULL); }
| subsection corpoLista { $$ = newAST(NT_BODYLIST, $1, $2, NULL, NULL); }
| subsection { $$ = newAST(NT_BODYLIST, $1, NULL, NULL, NULL); }
| corpo corpoLista { $$ = newAST(NT_BODYLIST, $1, $2, NULL, NULL); }
| corpo { $$ = newAST(NT_BODYLIST, $1, NULL, NULL, NULL); }
;

chapter: CHAPTER CONTENT { $$ = newTextSubdivision(NT_CHAPTER, $2, NULL, NULL); }
| CHAPTER NAME { $$ = newTextSubdivision(NT_CHAPTER, $2, NULL, NULL); }
;

class: CLASS NAME NAME { $$ = newClass(NT_CLASS, $2, $3); }
;

section: SECTION CONTENT { $$ = newTextSubdivision(NT_SECTION, $2, NULL, NULL); }
| SECTION NAME { $$ = newTextSubdivision(NT_SECTION, $2, NULL, NULL); }
;

subsection: SUBSECTION CONTENT { $$ = newTextSubdivision(NT_SUBSECTION, $2, NULL, NULL); } 
| SUBSECTION NAME { $$ = newTextSubdivision(NT_SUBSECTION, $2, NULL, NULL);} 
;

corpo: text { $$ = newAST(NT_BODY, $1, NULL, NULL, NULL); } 
| textStyle { $$ = newAST(NT_BODY, $1, NULL, NULL, NULL); } 
| lists { $$ = newAST(NT_BODY, $1, NULL, NULL, NULL); }
;

text: NAME text { $$ = newText(NT_TEXT, $1, $2); }
| NAME PAR { $$ = newText(NT_TEXT, $1, NULL); }
| NAME NEWLINE { $$ = newText(NT_TEXT, $1, NULL); }
| NAME { $$ = newText(NT_TEXT, $1, NULL); }
| PAR { $$ = newText(NT_TEXT, " ", NULL); }
| NEWLINE { $$ = newText(NT_TEXT, "\n", NULL); }
;

textStyle: BF NAME { $$ = newTextStyle(NT_TEXTSTYLE, $2, TS_BOLD); } 
| BF CONTENT { $$ = newTextStyle(NT_TEXTSTYLE, $2, TS_BOLD); } 
| UNDERLINE NAME { $$ = newTextStyle(NT_TEXTSTYLE, $2, TS_UNDERLINE); } 
| UNDERLINE CONTENT { $$ = newTextStyle(NT_TEXTSTYLE, $2, TS_UNDERLINE); } 
| IT NAME { $$ = newTextStyle(NT_TEXTSTYLE, $2, TS_ITALIC); }
| IT CONTENT { $$ = newTextStyle(NT_TEXTSTYLE, $2, TS_ITALIC); }
;

lists: numberedList { $$ = newAST(NT_LIST, $1, NULL, NULL, NULL); } 
| itemList { $$ = newAST(NT_LIST, $1, NULL, NULL, NULL); }
;

numberedList: _BEGIN ENUMERATE itens _END ENUMERATE { $$ = newAST(NT_NUMBEREDLIST, $3, NULL, NULL, NULL); }
;

itemList: _BEGIN ITEMIZE itens _END ITEMIZE { $$ = newAST(NT_ITEMLIST, $3, NULL, NULL, NULL); }
;

itens: ITEM { $$ = newItems(NT_ITENS, $1, NULL); } 
| ITEM itens { $$ = newItems(NT_ITENS, $1, $2);}
;
