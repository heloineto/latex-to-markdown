%option noyywrap nodefault yylineno
%{
    #include "tex-to-md.h"    
    #include "tex-to-md.tab.h"    
%}

letter  [a-zA-Z]
word    letter+
digit   [0-9]
number  [0-9]+
space   " " 

%%

"%".* /* Ignore comments */

\\author { return AUTHOR; }
\\begin { return _BEGIN; }
\\bf { return BF; } /*bold*/
\\chapter { return CHAPTER; }
\\documentclass { return CLASS; }
\\end { return _END; }
\\it { return IT; } /*italic*/
\\par { return PAR;} /* Nparagraph */
\\section { return SECTION; }
\\subsection { return SUBSECTION; }
\\title { return TITLE; }
\\underline { return UNDERLINE; }
\\usepackage { return PACKAGE; }

"{"document"}" { return DOCUMENT; }
"{"enumerate"}" { return ENUMERATE; }
"{"itemize"}" { return ITEMIZE; }

\\item" "["" "a-zA-Z0-9"\\"\.@\-"<"">""("")"","\''"!""?"";"":"]+ {
    //! HUMM Make this better
    copyStr(&yylval.string, &yytext[6], false);
    return ITEM;
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

\t|\n|{space} {}

. {/* printf("Unknown character %c at %d \n", *yytext, yylineno); */ }

%%