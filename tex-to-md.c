#include "tex-to-md.h"

void yyerror(char const* s) {
    fprintf(stderr, "%s\n", s);
}

struct ast* newast(enum NodeType nodeType, struct ast* n1, struct ast* n2, struct ast* n3, struct ast* n4) {
    struct ast* a = malloc(sizeof(struct ast));

    if (!a) {
        printf("Error: memory allocation returned NULL pointer\n");
        exit(0);
    }

    a->nodeType = nodeType;
    a->n1 = n1;
    a->n2 = n2;
    a->n3 = n3;
    a->n4 = n4;

    return a;
}

struct ast* newclass(enum NodeType nodeType, char* content1, char* content2) {
    struct StructClass* a = malloc(sizeof(struct StructClass));


    if (!a) {
        printf("Error: memory allocation returned NULL pointer\n");
        exit(0);
    }

    a->nodeType = nodeType;
    a->content1 = content1;
    a->content2 = content2;

    return ((struct ast*)a);
}

struct ast* newpackage(enum NodeType nodeType, char* content1, char* content2, struct ast* next) {
    struct StructPackage* a = malloc(sizeof(struct StructPackage));

    if (!a) {
        printf("Error: memory allocation returned NULL pointer\n");
        exit(0);
    }

    a->nodeType = nodeType;
    a->content1 = content1;
    a->content2 = content2;
    a->next = (struct StructPackage*)next;

    return ((struct ast*)a);
}

struct ast* newidentification(enum NodeType nodeType, char* title, char* author) {
    struct StructIdentification* a = malloc(sizeof(struct StructIdentification));

    if (!a) {
        printf("Error: memory allocation returned NULL pointer\n");
        exit(0);
    }

    a->nodeType = nodeType;
    a->title = title;
    a->author = author;

    return ((struct ast*)a);
}

struct ast* newtextsubdivision(enum NodeType nodeType, char* content, struct ast* n1, struct ast* n2) {
    struct StructTextSubdivision* a = malloc(sizeof(struct StructTextSubdivision));

    if (!a) {
        printf("Error: memory allocation returned NULL pointer\n");
        exit(0);
    }

    a->nodeType = nodeType;
    a->content = content;
    a->n1 = n1;
    a->n2 = n2;

    return ((struct ast*)a);
}

struct ast* newtext(enum NodeType nodeType, char* content, struct ast* next) {
    struct StructText* a = malloc(sizeof(struct StructText));

    if (!a) {
        printf("Error: memory allocation returned NULL pointer\n");
        exit(0);
    }

    a->nodeType = nodeType;
    a->content = content;
    a->next = (struct StructText*)next;

    return ((struct ast*)a);
}

struct ast* newtextstyle(enum NodeType nodeType, char* content, enum TextStyle textStyle) {
    struct StructTextStyle* a = malloc(sizeof(struct StructTextStyle));

    if (!a) {
        printf("Error: memory allocation returned NULL pointer\n");
        exit(0);
    }

    a->nodeType = nodeType;
    a->content = content;
    a->textStyle = textStyle;

    return ((struct ast*)a);
}

struct ast* newitens(enum NodeType nodeType, char* content, struct ast* next) {
    struct StructItens* a = malloc(sizeof(struct StructItens));

    if (!a) {
        printf("Error: memory allocation returned NULL pointer\n");
        exit(0);
    }

    a->nodeType = nodeType;
    a->content = content;
    a->next = next;

    return ((struct ast*)a);
}

void eval(struct ast* a) {
    if (!a) {
        return;
    }

    switch (a->nodeType) {
    case NT_DOCUMENT:
        eval(a->n1);
        treefree(a->n1);
        eval(a->n2);
        treefree(a->n2);
        eval(a->n3);
        treefree(a->n3);
        break;
    case NT_SETTINGS:
        eval(a->n1);
        eval(a->n2);
        break;
    case NT_CLASS:;
        struct StructClass* class = (struct StructClass*)a;

        fputs("[//]: # \"", outFilePtr);

        fputs("\\documentclass[", outFilePtr);
        fputs(class->content1, outFilePtr);
        fputs("]", outFilePtr);

        fputs("{", outFilePtr);
        fputs(class->content2, outFilePtr);
        fputs("}", outFilePtr);

        fputs("}\"", outFilePtr);

        fputs(ELEMENT_PADDING, outFilePtr);

        break;
    case NT_PACKAGE:;
        struct StructPackage* package = (struct StructPackage*)a;

        fputs("[//]: # \"\n", outFilePtr);

        while (package != NULL) {

            fputs("\\package[", outFilePtr);
            fputs(package->content1, outFilePtr);
            fputs("]", outFilePtr);

            if (package->content2) {
                fputs("{", outFilePtr);
                fputs(package->content2, outFilePtr);
                fputs("}", outFilePtr);
            }
            fputs(ELEMENT_PADDING, outFilePtr);

            package = (struct StructPackage*)package->next;
        }

        fputs("}\"", outFilePtr);

        fputs(ELEMENT_PADDING, outFilePtr);

        break;

    case NT_IDENTIFICATION:;
        struct StructIdentification* id = (struct StructIdentification*)a;

        fputs("# ", outFilePtr);
        fputs(id->title, outFilePtr);
        fputs(ELEMENT_PADDING, outFilePtr);

        if (id->author) {
            fputs("\t", outFilePtr);
            fputs(id->author, outFilePtr);
            fputs(ELEMENT_PADDING, outFilePtr);
            fputs(ELEMENT_PADDING, outFilePtr);
        }

        break;

    case NT_MAIN:
        eval(a->n3); /* eval bodyList */
        break;

    case NT_BODYLIST:
        eval(a->n1);
        if (a->n2) /* se tem pelo menos o nó 2, tem todos os 4 */
            eval(a->n2);

        break;

    case NT_CHAPTER:;
        struct StructTextSubdivision* chap = (struct StructTextSubdivision*)a;

        currSection = 1;
        currSubSection = 1;

        fputs("## ", outFilePtr);
        fputs(chap->content, outFilePtr);

        fputs(ELEMENT_PADDING, outFilePtr);

        currChapter++;

        break;
    case NT_SECTION:;
        struct StructTextSubdivision* sec = (struct StructTextSubdivision*)a;

        currSubSection = 1;

        fputs("#### **", outFilePtr);
        fputs(numberToStr(currChapter), outFilePtr);
        fputs(".", outFilePtr);
        fputs(numberToStr(currSection), outFilePtr);
        fputs("\t", outFilePtr);
        fputs(sec->content, outFilePtr);
        fputs("**\n", outFilePtr);

        fputs(ELEMENT_PADDING, outFilePtr);

        currSection++;

        break;
    case NT_SUBSECTION:;
        struct StructTextSubdivision* subsec = (struct StructTextSubdivision*)a;

        fputs("##### **", outFilePtr);
        fputs(numberToStr(currChapter), outFilePtr);
        fputs(".", outFilePtr);
        fputs(numberToStr(currSection), outFilePtr);
        fputs(".", outFilePtr);
        fputs(numberToStr(currSubSection), outFilePtr);
        fputs("\t", outFilePtr);
        fputs(subsec->content, outFilePtr);
        fputs("**\n", outFilePtr);

        fputs(ELEMENT_PADDING, outFilePtr);

        currSubSection++;

        break;
    case NT_BODY:
        eval(a->n1);
        break;

    case NT_TEXT:;
        struct StructText* txt = (struct StructText*)a;
        while (txt != NULL) {
            fputs(txt->content, outFilePtr);
            txt = (struct StructText*)txt->next;
            fputs(" ", outFilePtr);
        }

        fputs(ELEMENT_PADDING, outFilePtr);

        break;

    case NT_TEXTSTYLE:;
        struct StructTextStyle* txtst = (struct StructTextStyle*)a;

        switch (txtst->textStyle) {
        case TS_BOLD:
            fputs("**", outFilePtr);
            fputs(txtst->content, outFilePtr);
            fputs("**", outFilePtr);
            fputs(" ", outFilePtr);
            break;
        case TS_ITALIC:
            fputs("*", outFilePtr);
            fputs(txtst->content, outFilePtr);
            fputs("*", outFilePtr);
            fputs(" ", outFilePtr);
            break;
        case TS_UNDERLINE:
            fputs("<ins>", outFilePtr);
            fputs(txtst->content, outFilePtr);
            fputs("</ins>", outFilePtr);
            fputs(" ", outFilePtr);
            break;
        default:
            break;
        }

        break;

    case NT_LIST:
        eval(a->n1);

        break;

    case NT_NUMBEREDLIST:;
        struct StructItens* nlist = ((struct StructItens*)a->n1);

        while (nlist != NULL) {
            fputs("1. ", outFilePtr);
            fputs(nlist->content, outFilePtr);
            fputs("\n", outFilePtr);
            nlist = (struct StructItens*)nlist->next;
        }

        fputs(ELEMENT_PADDING, outFilePtr);

        break;

    case NT_ITEMLIST:;
        struct StructItens* ilist = ((struct StructItens*)a->n1);

        while (ilist != NULL) {
            fputs("* ", outFilePtr);
            fputs(ilist->content, outFilePtr);
            fputs("\n", outFilePtr);
            ilist = (struct StructItens*)ilist->next;
        }

        fputs(ELEMENT_PADDING, outFilePtr);

        break;
    case NT_ITENS:
        break;

    default:
        printf("erro interno: bad node %d\n", a->nodeType);
    }
}

void treefree(struct ast* a) {
    if (!a)
        return;

    switch (a->nodeType) {
    case NT_DOCUMENT:
        treefree(a->n1);
        treefree(a->n2);
        treefree(a->n3);
        break;
    case NT_SETTINGS:
        treefree(a->n1);
        treefree(a->n2);
        break;
    case NT_CLASS:;
        struct StructClass* class = (struct StructClass*)a;

        if (class->content1) {
            free(class->content1);
            class->content1 = NULL;
        }
        if (class->content2) {
            free(class->content2);
            class->content2 = NULL;
        }

        break;
    case NT_PACKAGE:;
        struct StructPackage* package = (struct StructPackage*)a;

        if (package->content1) {
            free(package->content1);
            package->content1 = NULL;
        }
        if (package->content2) {
            free(package->content2);
            package->content2 = NULL;
        }

        eval((struct ast*)package->next);
        package->next = NULL;

        break;
    case NT_IDENTIFICATION:;
        struct StructIdentification* id = (struct StructIdentification*)a;

        if (id->title) {
            free(id->title);
            id->title = NULL;
        }
        if (id->author) {
            free(id->author);
            id->author = NULL;
        }

        break;
    case NT_MAIN:
        treefree(a->n1);
        treefree(a->n2);
        treefree(a->n3);
        break;
    case NT_BEGIN:
        /* sem nós filhos */
        break;
    case NT_END:
        /* sem nós filhos */
        break;
    case NT_BODYLIST:
        treefree(a->n1);

        if (a->n2)
            treefree(a->n2);

        break;
    case NT_CHAPTER:;
        struct StructTextSubdivision* chapter = (struct StructTextSubdivision*)a;

        if (chapter->content) {
            free(chapter->content);
            chapter->content = NULL;
        }

        if (chapter->n1) /* se tiver n1, tem n2 */
        {
            treefree(chapter->n1);
            treefree(chapter->n2);
        }
        break;
    case NT_SUBSECTION:;
        struct StructTextSubdivision* subsection = (struct StructTextSubdivision*)a;

        if (subsection->content) {
            free(subsection->content);
            subsection->content = NULL;
        }

        treefree(subsection->n1); /* sempre vai ter n1 */

        if (subsection->n2) /* se tiver n1, tem n2 */
        {
            treefree(subsection->n2);
        }
        break;
    case NT_SECTION:;
        struct StructTextSubdivision* section = (struct StructTextSubdivision*)a;

        if (section->content) {
            free(section->content);
            section->content = NULL;
        }

        treefree(section->n1); /* sempre vai ter n1 */

        if (section->n2) /* se tiver n1, tem n2 */
        {
            treefree(section->n2);
        }
        break;
    case NT_BODY:
        treefree(a->n1);
        treefree(a->n2);
        break;
    case NT_TEXT:;
        struct StructText* text = (struct StructText*)a;

        if (text->content) {
            free(text->content);
            text->content = NULL;
        }

        treefree((struct ast*)text->next);
        break;
    case NT_TEXTSTYLE:;
        struct StructTextStyle* textStyle = (struct StructTextStyle*)a;

        if (textStyle->content) {
            free(textStyle->content);
            textStyle->content = NULL;
        }
        break;
    case NT_LIST:
        treefree(a->n1);
        break;
    case NT_NUMBEREDLIST:
        treefree(a->n1);
        break;
    case NT_ITEMLIST:
        treefree(a->n1);
        break;
    case NT_ITENS:;
        struct StructItens* itens = (struct StructItens*)a;

        if (itens->content) {
            free(itens->content);
            itens->content = NULL;
        }

        if (itens->next) {
            treefree(itens->next);
            itens->next = NULL;
        }
        break;
    default:
        break;
    }

    if (a) free(a); /* sempre libera o próprio nó */
}

void copyStr(char** dest, char* src, bool removeBrackets) {
    const int n = removeBrackets ? strlen(src) - 2 : strlen(src);
    *dest = (char*)malloc((sizeof(char) * n) + 1);

    strncpy(*dest, &src[(removeBrackets ? 1 : 0)], n);
    (*dest)[n] = '\0';
}

FILE* getFilePtr(char* inFileName) {
    outFileName = (char*)malloc(sizeof(char) * strlen(inFileName));

    /* Change extension (.tex -> .md) */
    strcpy(outFileName, inFileName);
    char* lastExt = strrchr(inFileName, '.');
    if (lastExt) *lastExt = '\0';
    strcat(outFileName, ".md");

    /* Clear file contents */
    outFilePtr = fopen(outFileName, "w");
    fclose(outFilePtr);

    return fopen(outFileName, "a");
}

char* numberToStr(long long int number) {
    char* str = (char*)malloc(sizeof(char) * (ceil(log10(number)) + 1));
    sprintf(str, "%llu", number);

    return str;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("1 argument expected, got %d\n", argc);
        return 1;
    }

    yyin = fopen(argv[1], "r");

    if (!yyin) {
        printf("Error: failed to read file %s\n", argv[1]);
        return 1;
    }

    outFilePtr = getFilePtr(argv[1]);

    currChapter = 0;
    currSection = 1;
    currSubSection = 1;

    return yyparse();
}
