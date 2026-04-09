#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void encryption(char* pass) {
	for (; *pass; pass++) *pass = *pass ^ 101;
}

/*
многострочный
комментарий
*/

//Перенос\
строки

int passCheck(char* pass) {
	FILE* input = fopen("D:\\OiB\\lab 6\\mypass\\xor.txt", "r"); //адресс
	char correctPassword[512] = { '\0' };
	fgets(correctPassword, 512, input);
	encryption(pass);
	if (strcmp(pass, correctPassword) == 0) return 1;
	return 0;
}

int main() {
	char inp[512] = { '\0' };
	printf("Enter password: ");
	scanf("%s", inp);
	if (passCheck(inp)) printf("Access Granted\n");
	else printf("Access Denied\n");
	getchar();
	getchar();
	return 0;
}
