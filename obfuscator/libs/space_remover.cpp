#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include "myHeader.h"

int space_remover(FILE* input, FILE* output) {
    if (!input || !output) return -1;

    int stringStatus = 0;   // 0 - код, 1 - строка, 2 - символ; строки НЕ ТРОГАЕМ!!!
    int needSpace = 0;
    int headFlag = 0;
    char curSym, next;

    while ((curSym = getc(input)) != EOF) {
        switch (stringStatus) {
        case 0: 

            //define include
            if (curSym == '#') headFlag = 1;

            if (isspace(curSym)) {

                if (headFlag && curSym == '\n') {
                    headFlag=0;
                    putc(curSym, output);
                    needSpace = 0;   // сбросить флаг пробела
                    break;
                }
                needSpace = 1;
                break;
            }

            // Непробельный символ – сначала, если нужно, выводим пробел
            if (needSpace) {
                putc(' ', output);
                needSpace = 0;
            }

            // смена состояния
            if (curSym == '"') {
                stringStatus = 1;
                putc(curSym, output);
            }
            else if (curSym == '\'') {
                stringStatus = 2;
                putc(curSym, output);
            }


            //просто код
            else putc(curSym, output);
            break;

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