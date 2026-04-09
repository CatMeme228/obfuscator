#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/comment_remover.h"

void readConfig(char** settings) {
	//открываем конфиг
	FILE* config = fopen("config.txt", "r");

	char buffer[100] = { '\0' };
	int paramId = 0;
	while (fgets(buffer, 100, config)) {

		//чистим строку и коммы в конфиге (начинаются с #)
		buffer[strcspn(buffer, "\r\n")] = 0;
		if (buffer[0] == '\0' || buffer[0]=='#') continue;

		//делим строку на слова
		char* words[3] = { 0 };
		int tokencount = 0;
		char* token = strtok(buffer, " ");
		while (token != NULL && tokencount < 4) {
			words[tokencount++] = token;
			token = strtok(NULL, " ");
		}
		
		//записываем в массив
		settings[paramId] = (char*) malloc(strlen(words[2])*sizeof(char)+1);
		strcpy(settings[paramId], words[2]);
		paramId++;
	}
	fclose(config);
}

void clearConfig(char** conf) {
	for (int i = 0; i < 7; i++) free(conf[i]);
	free(conf);
}

int main() {
	//читаем конфиг
	char** configs = (char**)malloc(sizeof(char*)*7);
	readConfig(configs);

	//ПРИ ВСЕХ FALSE БУДЕТ ПУСТОЙ ФАЙЛ
	FILE* input = fopen(configs[0], "r");
	FILE* output = fopen(configs[1], "w");

	// 5 прог, которые делают что-то своё
	if (strcmp(configs[2], "true") == 0) {} //удалить пробелы

	if (strcmp(configs[3], "true") == 0) { //далить комментарии
		printf("Comments were deleted!\n");
		remove_comments(input, output); 
	}
	
	if (strcmp(configs[4], "true") == 0) {}//переименование функций

	if (strcmp(configs[5], "true") == 0) {}//перемешивание функций

	if (strcmp(configs[6], "true") == 0) {}//мусор

	clearConfig(configs);

	fclose(input);
	fclose(output);
	return 0;
}