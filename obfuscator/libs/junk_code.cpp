#define _CRT_SECURE_NO_WARNINGS
#include "myHeader.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

int braces = 0, everEnteredMain=0;
int mainCheck(char* buffer, int flag);
int isInitialization(char* line, int firstInit=1);

int junk_code(FILE* input, FILE* output) {
	if (!input || !output) return -1;
	int junkNumber = 1, AmountOfJunkFunctions=0;

	//мусор
	char junk_function_init[] = "int veryUsefulFunction%d (int hihi, int haha);\n";
	char junk_function[][100] = {
		"\nint veryUsefulFunction%d (int hihi, int haha){\n", //вставка счётчика сюда
		"    int rofl;\n",
		"    while (haha != 0) {\n",
		"        rofl = haha;\n",
		"        haha = hihi %% haha;\n",
		"        hihi = rofl;\n",
		"    }\n",
		"    return hihi;\n",
		"}\n"
	};
	char junk_for [][50] = {
		"    int count%d = 0;\n", //вставка счётчика сюда
		"    for (int i = 1; i< 100000; i++){\n",
		"        if (i%%(%d)==0){\n", //вставка счётчика сюда
		"            count%d = count%d + 1;\n", //вставка счётчика сюда
		"        }\n",
		"    }\n"
	};
	char junk_var[] = "    int trololo%d = veryUsefulFunction%d(%d+67, %d+173);\n"; //вставка счётчика сюда
	
	//вспомогательный код
	char buffer[300] = { '\0' };
	int mainFlag = 0; //0- before main, 1- in main 2- after main
	srand(time(NULL)); //ГПСЧ

	//проход по файлу
	while (fgets(buffer, 300, input)) {
		mainFlag = mainCheck(buffer, mainFlag);
		if (mainFlag == 0 && isInitialization(buffer)) {
			fputs(buffer, output);
			fprintf(output, junk_function_init, junkNumber);
			junkNumber++;
			AmountOfJunkFunctions++;
		}

		else if (mainFlag == 1) {
			fputs(buffer, output);
			if (rand() % 2 == 0 && junkNumber>=1 && strstr(buffer, "if")==NULL && strstr(buffer, "if") == NULL) {//случайная вставка + защита от разрыва if else
				if (junkNumber % 2 == 0) fprintf(output, junk_var, junkNumber, junkNumber, junkNumber, junkNumber); //переменная
				else for (int i=0; i<6; i++) fprintf(output, junk_for[i], junkNumber, junkNumber);
				junkNumber--;
			}
		}

		else if (mainFlag == 2 && isInitialization(buffer, 0)) {
			for (int i = 0; i < 9; i++) fprintf(output, junk_function[i], AmountOfJunkFunctions);
			AmountOfJunkFunctions--;
			fputs(buffer, output); //основная (имеющая смысл) функция
		}
		else fputs(buffer, output);
	}
	return 0;
}

int isInitialization(char* line, int firstInit) {
	char keyWords[][20] = {"int", "long", "unsigned", "char", "float", "double", "void", "static", "short", "signed"};
	char* tmp = line;
	while (*tmp == ' ') tmp++;
	int keyWordFound = 0, semicolonFound=0;
	for (int i = 0; i < 9; i++) {
		if (strncmp(tmp, keyWords[i], strlen(keyWords[i])) == 0) {
			keyWordFound=1;
			break;
		}
	}
	if (strchr(tmp, ';') != NULL || firstInit==0) semicolonFound = 1;
	return keyWordFound && semicolonFound;
}

int mainCheck(char* line, int flag) {
	if (flag == 0 && strstr(line, "main") != NULL) { 
		if (strchr(line, '{')) braces++;
		if (braces == 1 && everEnteredMain == 0) everEnteredMain = 1;
		return 1;
	}
	if (flag == 1) {
		/*
		* everEnteredMain помогает правильно обрабатывать начало функции
		* если фигурная скобка появляется не сразу, а спустя несколько строк
		* программа думает, что баланс скобок == 0 -> main закончился, хотя он даже не начинался)
		* everEnteredMain смотрит, был ли совершён вход в функцию вообще, и только тогда есть смысл смотреть на баланс скобок
		*/
		char* tmp = line;
		while (*tmp) {
			if (everEnteredMain == 0 && braces == 1) everEnteredMain = 1;
			if (*tmp=='{') braces++;
			if (*tmp == '}') braces--;
			tmp++;
		}
		if (everEnteredMain == 1 && braces == 0) return 2;
		//выход
	}
	return flag;
}