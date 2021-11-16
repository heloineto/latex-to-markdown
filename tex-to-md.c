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

        appendOutput("[//]: # \""); /* um comentário em Markdown */

        appendOutput("\\documentclass[");
        appendOutput(class->content1);
        appendOutput("]");

        appendOutput("{");
        appendOutput(class->content2);
        appendOutput("}");

        appendOutput("}\"");

        appendOutput(ELEMENT_PADDING);

        break;
    case NT_PACKAGE:;
        struct StructPackage* package = (struct StructPackage*)a;

        appendOutput("[//]: # \"\n"); /* um comentário em Markdown */

        while (package != NULL) {

            appendOutput("\\package[");
            appendOutput(package->content1);
            appendOutput("]");

            if (package->content2) {
                appendOutput("{");
                appendOutput(package->content2);
                appendOutput("}");
            }
            appendOutput(ELEMENT_PADDING);

            package = (struct StructPackage*)package->next;
        }

        appendOutput("}\"");

        appendOutput(ELEMENT_PADDING);

        break;

    case NT_IDENTIFICATION:;
        struct StructIdentification* id = (struct StructIdentification*)a;

        appendOutput("# ");
        appendOutput(id->title);
        appendOutput(ELEMENT_PADDING);

        if (id->author) {
            appendOutput("\t");
            appendOutput(id->author);
            appendOutput(ELEMENT_PADDING);
            appendOutput(ELEMENT_PADDING);
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

        appendOutput("## ");
        appendOutput(chap->content);

        appendOutput(ELEMENT_PADDING);

        currChapter++;

        break;
    case NT_SECTION:;
        struct StructTextSubdivision* sec = (struct StructTextSubdivision*)a;

        currSubSection = 1;

        appendOutput("#### **");
        appendOutput(number_to_str(currChapter));
        appendOutput(".");
        appendOutput(number_to_str(currSection));
        appendOutput("\t");
        appendOutput(sec->content);
        appendOutput("**\n");

        appendOutput(ELEMENT_PADDING);

        currSection++;

        break;
    case NT_SUBSECTION:;
        struct StructTextSubdivision* subsec = (struct StructTextSubdivision*)a;

        appendOutput("##### **");
        appendOutput(number_to_str(currChapter));
        appendOutput(".");
        appendOutput(number_to_str(currSection));
        appendOutput(".");
        appendOutput(number_to_str(currSubSection));
        appendOutput("\t");
        appendOutput(subsec->content);
        appendOutput("**\n");

        appendOutput(ELEMENT_PADDING);

        currSubSection++;

        break;
    case NT_BODY:
        eval(a->n1);
        break;

    case NT_TEXT:;
        struct StructText* txt = (struct StructText*)a;
        while (txt != NULL) {
            appendOutput(txt->content);
            txt = (struct StructText*)txt->next;
            appendOutput(" ");
        }

        appendOutput(ELEMENT_PADDING);

        break;

    case NT_TEXTSTYLE:;
        struct StructTextStyle* txtst = (struct StructTextStyle*)a;

        switch (txtst->textStyle) {
        case TS_BOLD:
            appendOutput("**");
            appendOutput(txtst->content);
            appendOutput("**");
            appendOutput(" ");
            break;
        case TS_ITALIC:
            appendOutput("*");
            appendOutput(txtst->content);
            appendOutput("*");
            appendOutput(" ");
            break;
        case TS_UNDERLINE:
            appendOutput("<ins>");
            appendOutput(txtst->content);
            appendOutput("</ins>");
            appendOutput(" ");
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
            appendOutput("1. ");
            appendOutput(nlist->content);
            appendOutput("\n");
            nlist = (struct StructItens*)nlist->next;
        }

        appendOutput(ELEMENT_PADDING);

        break;

    case NT_ITEMLIST:;
        struct StructItens* ilist = ((struct StructItens*)a->n1);

        while (ilist != NULL) {
            appendOutput("* ");
            appendOutput(ilist->content);
            appendOutput("\n");
            ilist = (struct StructItens*)ilist->next;
        }

        appendOutput(ELEMENT_PADDING);

        break;
    case NT_ITENS:
        break;

    default:
        printf("erro interno: bad node %d\n", a->nodeType);
    }
}

/* libera uma árvore de AST */
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

    default:;
        // printf("erro interno: free bad node\n");
    }

    if (a)
        free(a); /* sempre libera o próprio nó */
}

void copyStr(char** dest, char* src, bool takeOffBrackets) {
    const int n = (takeOffBrackets ? (strlen(src) - 2) /* ignoring '{''}' */ : strlen(src));
    (*dest) = (char*)malloc((sizeof(char) * n) + 1); /* adding 1 for the '\0' */

    strncpy((*dest), &src[(takeOffBrackets ? 1 : 0)], n);
    (*dest)[n] = '\0';
}

void clearOutput() {
    output = fopen(mdFileName, "w");
    fclose(output);
}

void appendOutput(char* str) {
    fputs(str, output);
}

struct StackChar* new_node_stack_char(char data) {
    struct StackChar* node = (struct StackChar*)malloc(sizeof(struct StackChar));
    node->data = data;
    node->next = NULL;

    return node;
}

void push_stack_char(struct StackChar** stack, char data) {
    if (stack == NULL)
        return;

    struct StackChar* node = new_node_stack_char(data);

    if ((*stack) == NULL) {
        (*stack) = node;
    }
    else {
        struct StackChar* aux = (*stack);
        (*stack) = node;
        node->next = aux;
    }
}

char top_stack_char(struct StackChar* stack) {
    if (stack == NULL) {
        // printf("Can't return top! Stack is empty.\n");
    }
    else {
        return stack->data;
    }
}

void pop_stack_char(struct StackChar** stack) {
    if (stack == NULL || (*stack) == NULL) {
        // printf("Can't pop! Empty stack.\n");
        return;
    }

    struct StackChar* aux = (*stack);
    (*stack) = (*stack)->next;
    free(aux);
    aux = NULL;
}

bool is_empty_stack_char(struct StackChar* stack) {
    return (stack == NULL);
}

void print_stack_char(struct StackChar** stack) {
    if (stack == NULL || (*stack) == NULL) {
        // printf("Can't print! Empty stack.\n");
        return;
    }

    while ((*stack) != NULL) {
        printf("%c", top_stack_char(*stack));
        pop_stack_char(stack);
    }
    printf("\n");
}

char* get_string(long long unsigned int value, bool isNegative) {
    if (value == 0) {
        char* zero = (char*)malloc(sizeof(char) * 2);
        zero[0] = '0';
        zero[1] = '\0';

        return zero;
    }

    struct StackChar* strStack = NULL;
    int strStackSize = (isNegative ? 1 : 0);

    while (value > 0) {
        int digit = (value % 10);

        char digitChar = (digit + '0');
        push_stack_char(&strStack, digitChar);
        value /= 10;
        strStackSize++;
    }
    char* str = (char*)malloc(sizeof(char) * (strStackSize + 1));

    int i = (isNegative ? 1 : 0);
    if (isNegative) {
        str[0] = '-';
    }

    for (i; !is_empty_stack_char(strStack); i++) {
        str[i] = top_stack_char(strStack);
        pop_stack_char(&strStack);
    }
    const int lastStrPosition = strStackSize;
    str[lastStrPosition] = '\0'; /*Last position*/

    return str;
}

char* number_to_str(long long int value) {
    bool isNegative = (value < 0);

    if (isNegative) {
        value *= -1;
    }

    char* str = get_string(value, isNegative);
}

long long int str_to_number(const char* str) {
    long long int number = 0;
    bool isNegative = (str[0] == '-');

    int strLength = strlen(str);
    long long unsigned int decimalPlace = 1;
    const int stopFlag = (isNegative ? 0 : -1);
    for (int i = strLength - 1; i != stopFlag; i--) {
        int digit = (str[i] - '0');
        number += (digit * decimalPlace);
        decimalPlace *= 10;
    }

    if (isNegative) {
        number *= -1;
    }

    return number;
}

char* removeFileExt(char* str) {
    char* resultStr;
    char* lastExt;
    if (!str) return NULL;

    if ((resultStr = malloc(strlen(str) + 1)) == NULL)
        return NULL;

    strcpy(resultStr, str);

    lastExt = strrchr(resultStr, '.');

    if (lastExt)
        *lastExt = '\0';

    return resultStr;
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

    mdFileName = (char*)malloc(sizeof(char) * strlen(argv[1]));
    strcpy(mdFileName, removeFileExt(argv[1]));
    strcat(mdFileName, ".md");

    clearOutput();

    output = fopen(mdFileName, "a");

    currChapter = 0;
    currSection = 1;
    currSubSection = 1;

    return yyparse();
}
