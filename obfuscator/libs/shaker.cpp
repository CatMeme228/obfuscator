#define _CRT_SECURE_NO_WARNINGS
#include "myHeader.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

int isInit(char* line);
int inFunction(char* line, int flag);
void collectData(FILE* inp);
void greatClean();

int everEnteredFunction = 0, amountOfBraces = 0, mainStatus = 0;
int amountOfInit = 0, amountOfFunctions = 0;
char** myInitializations = NULL;   // массив прототипов
char*** myFunctions = NULL;        // массив функций (каждая функция — массив строк)
int* myFunctionSizes = NULL;       // количество строк в каждой функции

// Вспомогательные функции для управления массивами
void addInitialization(char* line) {
    if (amountOfInit % 10 == 0) {
        myInitializations = (char**)realloc(myInitializations, (amountOfInit + 10) * sizeof(char*));
    }
    myInitializations[amountOfInit] = (char*)malloc(strlen(line) + 1);
    strcpy(myInitializations[amountOfInit], line);
    amountOfInit++;
}

void startNewFunction() {
    if (amountOfFunctions % 10 == 0) {
        myFunctions = (char***)realloc(myFunctions, (amountOfFunctions + 10) * sizeof(char**));
        myFunctionSizes = (int*)realloc(myFunctionSizes, (amountOfFunctions + 10) * sizeof(int));
    }
    myFunctions[amountOfFunctions] = NULL;
    myFunctionSizes[amountOfFunctions] = 0;
    amountOfFunctions++;   // ← теперь увеличиваем здесь
}

// addLineToCurrentFunction остаётся без изменений
void addLineToCurrentFunction(char* line) {
    int idx = amountOfFunctions - 1;   // теперь всегда корректный индекс
    myFunctions[idx] = (char**)realloc(myFunctions[idx], (myFunctionSizes[idx] + 1) * sizeof(char*));
    myFunctions[idx][myFunctionSizes[idx]] = (char*)malloc(strlen(line) + 1);
    strcpy(myFunctions[idx][myFunctionSizes[idx]], line);
    myFunctionSizes[idx]++;
}

// Перемешивание массива строк (алгоритм Фишера-Йетса)
void shuffleStringArray(char** arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char* tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

// Перемешивание массива функций (т.е. массивов строк)
void shuffleFunctionArray(char*** arr, int* sizes, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // меняем местами указатели на функции и их размеры
        char** tmpFunc = arr[i];
        arr[i] = arr[j];
        arr[j] = tmpFunc;
        int tmpSize = sizes[i];
        sizes[i] = sizes[j];
        sizes[j] = tmpSize;
    }
}

// Основная функция обфускации
int shaker(FILE* input, FILE* output) {
    if (!input || !output) return -1;

    srand((unsigned int)time(NULL));

    // 1. Сбор данных
    collectData(input);
    rewind(input);
    mainStatus = 0;
    everEnteredFunction = 0;
    amountOfBraces = 0;

    // 2. Перемешивание прототипов и функций
    if (amountOfInit > 0) shuffleStringArray(myInitializations, amountOfInit);
    if (amountOfFunctions > 0) shuffleFunctionArray(myFunctions, myFunctionSizes, amountOfFunctions);

    // 3. Генерация выходного файла
    char buffer[300];
    int protoIndex = 0;      // индекс следующего прототипа для подстановки
    int funcIndex = 0;       // индекс следующей функции для вывода после main
    int inMain = 0;          // 0 - до main, 1 - внутри main, 2 - после main (вывод функций)

    while (fgets(buffer, 300, input)) {
        if (inMain == 0) {
            // До main: копируем всё, кроме прототипов — их заменяем случайными
            if (isInit(buffer) && strchr(buffer, ';') != NULL && strstr(buffer, "main") == NULL) {
                // Заменяем прототип на следующий из перемешанного списка
                if (protoIndex < amountOfInit) {
                    fputs(myInitializations[protoIndex], output);
                    protoIndex++;
                }
                else fputs(buffer, output); // Если прототипов в списке меньше, чем в файле (странный случай) — оставляем как есть
                continue;
            }
            // Проверяем, не начался ли main
            if (strstr(buffer, "main") != NULL) {
                inMain = 1;
                mainStatus = inFunction(buffer, mainStatus);
            }
            fputs(buffer, output);
        }
        else if (inMain == 1) {
            // Внутри main: просто копируем
            fputs(buffer, output);
            mainStatus = inFunction(buffer, mainStatus);
            if (mainStatus == 0) inMain = 2;   // main закончился, переходим к выводу функций
        }
        else if (inMain == 2) {
            // После main: игнорируем исходные строки (там обычно определения функций)
            // и выводим перемешанные функции
            if (funcIndex < amountOfFunctions) {
                for (int i = 0; i < myFunctionSizes[funcIndex]; i++) fputs(myFunctions[funcIndex][i], output);
                funcIndex++;
            }
        }
    }

    // Если после main в файле не было ничего, а функции ещё остались — вывести их
    while (funcIndex < amountOfFunctions) {
        for (int i = 0; i < myFunctionSizes[funcIndex]; i++) fputs(myFunctions[funcIndex][i], output);
        funcIndex++;
    }

    greatClean();
    return 0;
}

// Сбор прототипов и определений функций
void collectData(FILE* inp) {
    char buffer[300];
    int inFunctionBody = 0;   // находимся ли мы внутри тела функции (после main)

    while (fgets(buffer, 300, inp)) {
        if (mainStatus == 0) {
            // До main: собираем прототипы
            if (strstr(buffer, "main") != NULL) {
                mainStatus = inFunction(buffer, mainStatus);
                continue;
            }
            if (isInit(buffer) && strchr(buffer, ';') != NULL) addInitialization(buffer);
        }
        else if (mainStatus == 1) {
            // Пропускаем тело main
            mainStatus = inFunction(buffer, mainStatus);
            if (mainStatus == 0) {
                mainStatus = 2;   // main закончился, начинается область определений функций
                inFunctionBody = 0; // готовимся к новому определению
            }
        }
        else if (mainStatus == 2) {
            // Собираем определения функций после main
            if (!inFunctionBody) {
                // Начало новой функции
                startNewFunction();
                inFunctionBody = 1;
            }
            addLineToCurrentFunction(buffer);
            // Проверяем, не закончилась ли функция
            int prevBraces = amountOfBraces;
            inFunction(buffer, 2);   // обновляем счётчик скобок
            if (prevBraces > 0 && amountOfBraces == 0 && everEnteredFunction) inFunctionBody = 0; // Функция завершилась 
        }
    }
}

// Проверка, является ли строка началом объявления/определения
int isInit(char* line) {
    char keyWords[][20] = { "int", "long", "unsigned", "char", "float", "double", "void", "static", "short", "signed", "const" };
    char* tmp = line;
    while (*tmp == ' ') tmp++;
    for (int i = 0; i < sizeof(keyWords) / sizeof(keyWords[0]); i++) {
        if (strncmp(tmp, keyWords[i], strlen(keyWords[i])) == 0) return 1;
    }
    return 0;
}

// Отслеживание входа/выхода из функции по фигурным скобкам
int inFunction(char* line, int flag) {
    if (flag == 0) {
        if (strchr(line, '{')) amountOfBraces++;
        if (amountOfBraces == 1 && everEnteredFunction == 0) everEnteredFunction = 1;
        return 1;
    }
    if (flag == 1 || flag == 2) {
        char* tmp = line;
        while (*tmp) {
            if (everEnteredFunction == 0 && amountOfBraces == 1) everEnteredFunction = 1;
            if (*tmp == '{') amountOfBraces++;
            if (*tmp == '}') amountOfBraces--;
            tmp++;
        }
        if (everEnteredFunction && amountOfBraces == 0) return 0;
    }
    return flag;
}

// Очистка выделенной памяти
void greatClean() {
    for (int i = 0; i < amountOfInit; i++) free(myInitializations[i]);
    free(myInitializations);
    for (int i = 0; i < amountOfFunctions; i++) {
        for (int j = 0; j < myFunctionSizes[i]; j++) free(myFunctions[i][j]);
        free(myFunctions[i]);
    }
    free(myFunctions);
    free(myFunctionSizes);
}