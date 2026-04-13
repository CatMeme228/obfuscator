#define _CRT_SECURE_NO_WARNINGS
#include "myHeader.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

int isInit(char* line);
int inFunction(char* line, int flag);
void collectData(FILE* inp);
void greatClean();

int everEnteredFunction = 0, amountOfBraces = 0, mainStatus=0;
int amountOfInit = 0, amountOfFunctions = 0;
char** myInitializations = NULL; //массив прототипов
char*** myFunctions = NULL; //массив функций

int shaker(FILE* input, FILE* output) {
	if (!input || !output) return -1;

	//сбор инфы и сброс
	collectData(input);
	rewind(input);
	mainStatus = 0;

/*	srand(time(NULL));
	char buffer[300];
	while (fgets(buffer, 300, input)) {
		if (mainStatus == 0) {
			mainStatus = inFunction(buffer, mainStatus);
			if (isInit(buffer)) {
				if (strstr(buffer, "main") != NULL) { mainStatus = 1; continue; }
				char* curLine= (char*)malloc

			}
		}

		else if (mainStatus == 1) {
			fputs(buffer, output); //вставка main без изменений
			mainStatus = inFunction(buffer, mainStatus);
		}

		else if (mainStatus == 2) {
			//вставка функции
		}

	}*/
	return 0;
}

void collectData(FILE* inp) {
	int currentSize = 10;
	myInitializations = (char**)malloc(currentSize*sizeof(char*));
	myFunctions = (char***)malloc(currentSize * sizeof(char**));

	char buffer[300];
	while (fgets(buffer, 300, inp)) {

		if (mainStatus == 0) { //копируем прототипы и функции в массивы, не трогая main
			if (strstr(buffer, "main") != NULL) { mainStatus = inFunction(buffer, mainStatus); continue; }
			if (isInit(buffer) && (strchr(buffer, ';') != NULL)) {
				char* curLine = (char*)malloc(strlen(buffer) + 1);
				strcpy(curLine, buffer);
				
			}
		}

		else if (mainStatus == 1) { //скип main
			mainStatus = inFunction(buffer, mainStatus);
			if (mainStatus == 0) mainStatus = 2;
		}

		else if (mainStatus == 2) { //как-то сдеать массив функций
			
		}

	}

}


int isInit(char* line) {
	char keyWords[][20] = { "int", "long", "unsigned", "char", "float", "double", "void", "static", "short", "signed", "const" };
	char* tmp = line;
	while (*tmp == ' ') tmp++;
	int keyWordFound = 0;
	for (int i = 0; i < 9; i++) {
		if (strncmp(tmp, keyWords[i], strlen(keyWords[i])) == 0) {
			keyWordFound = 1;
			break;
		}
	}
	return keyWordFound;
}

int inFunction(char* line, int flag) {
	if (flag == 0) {
		if (strchr(line, '{')) amountOfBraces++;
		if (amountOfBraces == 1 && everEnteredFunction == 0) everEnteredFunction = 1;
		return 1;
	}
	if (flag == 1) {
		char* tmp = line;
		while (*tmp) {
			if (everEnteredFunction == 0 && amountOfBraces == 1) everEnteredFunction = 1;
			if (*tmp == '{') amountOfBraces++;
			if (*tmp == '}') amountOfBraces--;
			tmp++;
		}
		if (everEnteredFunction == 1 && amountOfBraces == 0) return 0;
	}
	return flag;
}
