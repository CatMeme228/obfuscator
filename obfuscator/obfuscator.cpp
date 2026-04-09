#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/comment_remover.h"

void readConfing(FILE* conf, char** settings) {
	char buffer[100] = { '\0' };
	int paramId = 0;
	while (fgets(buffer, 100, conf)) {

		//делим строку на слова
		buffer[strcspn(buffer, "\r\n")] = 0;
		if (buffer[0] == '\0' || buffer[0]=='#') continue;

		char* words[4] = { 0 };
		int tokencount = 0;
		char* token = strtok(buffer, " ");
		while (token != NULL && tokencount < 4) {
			words[tokencount++] = token;
			token = strtok(NULL, " ");
		}
		
		settings[paramId] = (char*) malloc(strlen(words[2])*sizeof(char)+1);
		strcpy(settings[paramId], words[2]);
		paramId++;
	}
}

int main() {
	//читаем конфиг
	FILE* config = fopen("config.txt", "r");
	char** configs = (char**)malloc(sizeof(char*)*7);
	readConfing(config, configs);
	fclose(config);
	//ПРИ ВСЕХ FALSE БУДЕТ ПУСТОЙ ФАЙЛ
	FILE* input = fopen(configs[0], "r");
	FILE* output = fopen(configs[1], "w");

	if (strcmp(configs[3], "true")==0)remove_comments(input, output);
	//if (strcmp(configs[3], "true") == 0)remove_comments(input, output);

	fclose(input);
	fclose(output);
	return 0;
}