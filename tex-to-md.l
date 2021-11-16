%option noyywrap nodefault yylineno
%{
    #include "tex-to-md.h"    
    #include "tex-to-md.tab.h"    
%}

letter [a-zA-Z]
word letter+
digit [0-9]
number [0-9]+
space " " 

%%

"%".* /* Ignore comments */

\\begin {
    return _BEGIN;
}

\\end {
    return _END;
}

"{"document"}" {
    return DOCUMENT;
}

\\documentclass {
    return CLASS;
}

\\usepackage {
    return PACKAGE;
}

\\title {
    return TITLE;
}

\\author {
    return AUTHOR;
}

\\chapter {
    return CHAPTER;
}

\\section {
    return SECTION;
}

\\subsection {
    return SUBSECTION;
}

\\par {
    return PAR;
}

\\bf {
    return BF;
}

\\underline {
    return UNDERLINE;
}

\\it {
    return IT;
}


\\item" "["" "a-zA-Z0-9"\\"\.@\-"<"">""("")"","\''"!""?"";"":"]+ {
    //! HUMM Make this better
    copyStr(&yylval.string, &yytext[6], false);
    return ITEM;
}

"{"enumerate"}" {
    return ENUMERATE;
}

"{"itemize"}" {
    return ITEMIZE;
}



"{"[a-zA-Z0-9"\\"\.@\n\-"<"">""("")"","\''"!""?"";"":"]+"}" {
    copyStr(&yylval.string, yytext, true);
    return NAME;
}

"{"[a-zA-Z0-9" ""\\"\.@\n\-"<"">""("")"","\''"!""?"";"":"]+"}" {
    copyStr(&yylval.string, yytext, true);
    return CONTENT;
}

"["[a-zA-Z0-9"\\"\.@\n\-"<"">""("")"","\''"!""?"";"":"]+"]" {
    copyStr(&yylval.string, yytext, true);
    return NAME;
}



[a-zA-Z0-9\.@\-"<"">""("")"","\''"!""?"";"":"]+ {
    copyStr(&yylval.string, yytext, false);
    return NAME;
}

\t|\n|{space} {

}

. {
    printf("Caracter não reconhecido %c\n", *yytext);
}

%%