#define _CRT_SECURE_NO_WARNINGS
#include "myHeader.h"

#include <string.h>
#include <stdlib.h>

#define SPACE   ' '
#define ASTERISK '*'
#define HASH    '#'

static int is_id_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
}

int renamer(FILE* inp, FILE* out) {
    if (!inp || !out) return 1;

    // Чтение всего файла
    fseek(inp, 0, SEEK_END);
    unsigned long long Size = ftell(inp);
    fseek(inp, 0, SEEK_SET);
    char* Text = (char*)malloc(Size + 1);
    fread(Text, 1, Size, inp);
    Text[Size] = '\0';

    // ---- Первый проход: сбор имён переменных (исключая #define и main) ----
    char VariablesArr[100][25] = { {0} };
    int varCount = 0;

    for (unsigned long long i = 0; i < Size; i++) {
        int Flag = 0;
        int isDefine = 0;

        // #define
        if (i + 7 < Size && Text[i] == HASH && Text[i + 1] == 'd' && Text[i + 2] == 'e' &&
            Text[i + 3] == 'f' && Text[i + 4] == 'i' && Text[i + 5] == 'n' && Text[i + 6] == 'e' && Text[i + 7] == SPACE) {
            Flag = 8;
            isDefine = 1;
        }
        // int
        else if (i + 4 < Size && Text[i] == 'i' && Text[i + 1] == 'n' && Text[i + 2] == 't' &&
            (Text[i + 3] == SPACE || Text[i + 3] == ASTERISK))
            Flag = 4;
        // void
        else if (i + 5 < Size && Text[i] == 'v' && Text[i + 1] == 'o' && Text[i + 2] == 'i' && Text[i + 3] == 'd' &&
            (Text[i + 4] == SPACE || Text[i + 4] == ASTERISK))
            Flag = 5;
        // char
        else if (i + 5 < Size && Text[i] == 'c' && Text[i + 1] == 'h' && Text[i + 2] == 'a' && Text[i + 3] == 'r' &&
            (Text[i + 4] == SPACE || Text[i + 4] == ASTERISK))
            Flag = 5;
        // FILE*
        else if (i + 5 < Size && Text[i] == 'F' && Text[i + 1] == 'I' && Text[i + 2] == 'L' && Text[i + 3] == 'E' && Text[i + 4] == ASTERISK)
            Flag = 5;
        // long
        else if (i + 5 < Size && Text[i] == 'l' && Text[i + 1] == 'o' && Text[i + 2] == 'n' && Text[i + 3] == 'g' &&
            (Text[i + 4] == SPACE || Text[i + 4] == ASTERISK))
            Flag = 5;
        // double
        else if (i + 7 < Size && Text[i] == 'd' && Text[i + 1] == 'o' && Text[i + 2] == 'u' && Text[i + 3] == 'b' &&
            Text[i + 4] == 'l' && Text[i + 5] == 'e' && (Text[i + 6] == SPACE || Text[i + 6] == ASTERISK))
            Flag = 7;
        // float
        else if (i + 6 < Size && Text[i] == 'f' && Text[i + 1] == 'l' && Text[i + 2] == 'o' && Text[i + 3] == 'a' &&
            Text[i + 4] == 't' && (Text[i + 5] == SPACE || Text[i + 5] == ASTERISK))
            Flag = 6;
        // long long
        else if (i + 10 < Size && Text[i] == 'l' && Text[i + 1] == 'o' && Text[i + 2] == 'n' && Text[i + 3] == 'g' &&
            Text[i + 4] == SPACE && Text[i + 5] == 'l' && Text[i + 6] == 'o' && Text[i + 7] == 'n' && Text[i + 8] == 'g' &&
            (Text[i + 9] == SPACE || Text[i + 9] == ASTERISK))
            Flag = 10;

        if (Flag) {
            i += Flag;  // пропускаем ключевое слово (и звёздочку, если была)
            // пропускаем пробелы и дополнительные звёздочки
            while (i < Size && (Text[i] == SPACE || Text[i] == ASTERISK)) i++;
            char String[25] = { 0 };
            int j = 0;
            while (i < Size && is_id_char(Text[i]) && j < 24) String[j++] = Text[i++];
            String[j] = '\0';
            i--; // коррекция после цикла

            if (j == 0) continue;
            if (isDefine) continue;          // не добавляем макросы
            if (strcmp(String, "main") == 0) continue; // не трогаем main

            // проверка на дубликат
            int duplicate = 0;
            for (int p = 0; p < varCount; p++) {
                if (strcmp(VariablesArr[p], String) == 0) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate && varCount < 100) {
                strcpy(VariablesArr[varCount], String);
                varCount++;
            }
        }
    }

    // ---- Генерация новых имён ----
    char Replacement[100][25] = { {0} };
    for (int i = 0; i < varCount; i++) sprintf(Replacement[i], "var_%d", i);

    // ---- Второй проход: замена идентификаторов ----
    for (unsigned long long i = 0; i < Size; i++) {
        // Пропускаем строковые литералы
        if (Text[i] == '"') {
            fputc('"', out);
            i++;
            while (i < Size && Text[i] != '"') {
                fputc(Text[i], out);
                i++;
            }
            if (i < Size) fputc('"', out);
            continue;
        }
        // Пропускаем символьные константы
        if (Text[i] == '\'') {
            fputc('\'', out);
            i++;
            while (i < Size && Text[i] != '\'') {
                fputc(Text[i], out);
                i++;
            }
            if (i < Size) fputc('\'', out);
            continue;
        }

        if (!is_id_char(Text[i])) {
            fputc(Text[i], out);
            continue;
        }

        // Извлекаем идентификатор
        char Word[25] = { 0 };
        int pos = 0;
        while (i < Size && is_id_char(Text[i]) && pos < 24) Word[pos++] = Text[i++];
        Word[pos] = '\0';
        i--;

        // Не заменяем main
        if (strcmp(Word, "main") == 0) {
            fputs(Word, out);
            continue;
        }

        // Ищем замену
        int replaced = 0;
        for (int p = 0; p < varCount; p++) {
            if (strcmp(Word, VariablesArr[p]) == 0) {
                fputs(Replacement[p], out);
                replaced = 1;
                break;
            }
        }
        if (!replaced) fputs(Word, out);
    }

    free(Text);
    return 0;
}