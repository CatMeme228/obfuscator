#define _CRT_SECURE_NO_WARNINGS
#include "myHeader.h"

int comment_remover(FILE* input, FILE* output) {
    if (!input || !output) return -1;

    int stringStatus = 0;
    char curSym, next;

    while ((curSym = getc(input)) != EOF) {
        switch (stringStatus) {
        case 0: {
            if (curSym == '/') {
                next = getc(input);
                if (next == '/') {
                    stringStatus = 1;
                }
                else if (next == '*') {
                    stringStatus = 2;
                }
                else {
                    putc(curSym, output);
                    if (next != EOF) ungetc(next, input);
                }
            }
            else if (curSym == '"') {
                stringStatus = 3;
                putc(curSym, output);
            }
            else if (curSym == '\'') {
                stringStatus = 4;
                putc(curSym, output);
            }
            else {
                putc(curSym, output);
            }
            break;
        }

        case 1:
            if (curSym == '\\') {
                next = getc(input);
                if (next == EOF) break;
            }
            else if (curSym == '\n' || curSym == '\r') {
                stringStatus = 0;
                putc(curSym, output);

                if (curSym == '\r') {
                    next = getc(input);
                    if (next == '\n') {
                        putc(next, output);
                    }
                    else if (next != EOF) {
                        ungetc(next, input);
                    }
                }
            }
            break;

        case 2:
            if (curSym == '*') {
                next = getc(input);
                if (next == '/') {
                    stringStatus = 0;
                }
                else if (next != EOF) {
                    ungetc(next, input);
                }
            }
            else if (curSym == '\n' || curSym == '\r') {
                putc(curSym, output);


                if (curSym == '\r') {
                    next = getc(input);
                    if (next == '\n') {
                        putc(next, output);
                    }
                    else if (next != EOF) {
                        ungetc(next, input);
                    }
                }
            }
            break;

        case 3:
            putc(curSym, output);
            if (curSym == '\\') {
                next = getc(input);
                if (next != EOF) putc(next, output);
            }
            else if (curSym == '"' || curSym == '\n') {
                stringStatus = 0;
            }
            break;

        case 4:
            putc(curSym, output);
            if (curSym == '\\') {
                next = getc(input);
                if (next != EOF) {
                    putc(next, output);
                }
            }
            else if (curSym == '\'' || curSym == '\n') {
                stringStatus = 0;
            }
            break;
        }
    }
    return 0;
}