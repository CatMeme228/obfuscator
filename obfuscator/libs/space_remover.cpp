#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include "myHeader.h"

int space_remover(FILE* input, FILE* output) {
    if (!input || !output) return -1;

    int stringStatus = 0;   // 0 - код, 1 - строка, 2 - символ; строки НЕ ТРОГАЕМ!!!

    char curSym, next;

    while ((curSym = getc(input)) != EOF) {
        switch (stringStatus) {
        case 0: 

            //подумайте

        case 1:
            putc(curSym, output);

            //обработка экранирования
            if (curSym == '\\') {
                next = getc(input);
                if (next != EOF) putc(next, output);
            }

            else if (curSym == '"' || curSym == '\n') stringStatus = 0;
            break;

        case 2:
            putc(curSym, output);

            //обработка экранирования
            if (curSym == '\\') {
                next = getc(input);
                if (next != EOF) putc(next, output);
            }

            else if (curSym == '\'' || curSym == '\n') stringStatus = 0;
            break;
        }
    }

    return 0;
}