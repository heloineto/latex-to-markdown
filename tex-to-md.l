%option noyywrap nodefault yylineno
%{
    #include "tex-to-md.h"    
    #include "tex-to-md.tab.h"    
%}

letter  [a-zA-Z]
digit   [0-9]
space   " "
special_chars [\.@\-"<"">""("")"","\''"!""?"";"":"]
characters {digit}|{letter}|{special_chars}

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
\\newline|\\\\ { return NEWLINE; }

"{"document"}" { return DOCUMENT; }
"{"enumerate"}" { return ENUMERATE; }
"{"itemize"}" { return ITEMIZE; }

\\item" ".* {
    copyStr(&yylval.string, &yytext[6], false);
    return ITEM;
}

("{"|"[")(["\\"\n]|{characters})+("}"|"]") {
    copyStr(&yylval.string, yytext, true);
    return NAME;
}

"{"([" ""\\"\n]|{characters})+"}" {
    copyStr(&yylval.string, yytext, true);
    return CONTENT;
}

{characters}+ {
    copyStr(&yylval.string, yytext, false);
    return NAME;
}

\t|\n|{space}|\\maketitle {}

. { printf("Unknown character %c at line %d \n", *yytext, yylineno); }

%%