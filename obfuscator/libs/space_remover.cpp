#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include "myHeader.h"

static int is_id_char(int c) {
    return isalnum(c) || c == '_';
}

int space_remover(FILE* input, FILE* output) {
    if (!input || !output) return -1;

    int stringStatus = 0;   //0- код, 1- строка, 2- константа; 1-2 НЕ ТРОГАЕМ
    int hasWhitespace = 0;  // был ли пробел
    int lastChar = 0;       // последний не пробел
    int headFlag = 0;       // директивы
    char curSym, next;

    while ((curSym = getc(input)) != EOF) {
        switch (stringStatus) {
        case 0:

            //директива
            if (curSym == '#')
                headFlag = 1;

            if (isspace(curSym)) {
                // скип директив и сохранение перевода строк
                if (headFlag && curSym == '\n') {
                    putc(curSym, output);
                    headFlag = 0;
                    hasWhitespace = 0;   
                    break;
                }
                // нужен пробел (только 1)
                hasWhitespace = 1;
                break;
            }

            if (hasWhitespace) {
                //между ключевыми словами
                if (lastChar != 0 && is_id_char(lastChar) && is_id_char(curSym)) putc(' ', output);
                hasWhitespace = 0;
            }

            putc(curSym, output);
            lastChar = curSym;  //сохранить последний не пробел

           // смена состояния
            if (curSym == '"') stringStatus = 1;
            else if (curSym == '\'') stringStatus = 2;
            break;

        case 1: //строка
            putc(curSym, output);
            if (curSym == '\\') {
                next = getc(input);
                if (next != EOF) putc(next, output);
            }
            else if (curSym == '"' || curSym == '\n') stringStatus = 0; //вернуться в код
            break;

        case 2: //константа
            putc(curSym, output);
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