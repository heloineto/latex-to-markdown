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
\\bf|\\textbf { return BF; }
\\chapter { return CHAPTER; }
\\documentclass { return CLASS; }
\\end { return _END; }
\\it|\\textit { return IT; }
\\par { return PAR;}
\\section { return SECTION; }
\\subsection { return SUBSECTION; }
\\title { return TITLE; }
\\underline { return UNDERLINE; }
\\usepackage { return PACKAGE; }
\\newline|\\\\ { printf("NEWLINE\n"); return NEWLINE; }

"{"document"}" { return DOCUMENT; }
"{"enumerate"}" { return ENUMERATE; }
"{"itemize"}" { return ITEMIZE; }

\\item" "["" "a-zA-Z0-9"\\"\.@\-"<"">""("")"","\''"!""?"";"":"]+ {
    copyStr(&yylval.string, &yytext[6], false);
    return ITEM;
}

("{"|"[")[a-zA-Z0-9"\\"\.@\n\-"<"">""("")"","\''"!""?"";"":"]+("}"|"]") {
    copyStr(&yylval.string, yytext, true);
    return NAME;
}

"{"[a-zA-Z0-9" ""\\"\.@\n\-"<"">""("")"","\''"!""?"";"":"]+"}" {
    copyStr(&yylval.string, yytext, true);
    return CONTENT;
}

[a-zA-Z0-9\.@\-"<"">""("")"","\''"!""?"";"":"]+ {
    copyStr(&yylval.string, yytext, false);
    return NAME;
}

\t|\n|{space}|\\maketitle|\\maketitle {}

. { printf("Unknown character %c at line %d \n", *yytext, yylineno); }

%%