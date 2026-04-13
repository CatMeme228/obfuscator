#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


int veryUsefulFunction1 (int hihi, int haha);
void encryption(char* pass);
int veryUsefulFunction2 (int hihi, int haha);
int    passCheck(char* pass);
int veryUsefulFunction3 (int hihi, int haha);
int a() {
	return 0;
}







int main() {
	char inp[512] = { '\0' };
	printf("Enter password: ");
    int count3 = 0;
    for (int i = 1; i< 100000; i++){
        if (i%(3)==0){
            count3 = count3 + 1;
        }
    }
	scanf("%s", inp);
	if (passCheck(inp)) printf("Access Granted\n");
	else printf("Access Denied\n");
	getchar();
	getchar();
	return 0;
    int trololo2 = veryUsefulFunction2(2+67, 2+173);
}


int veryUsefulFunction3 (int hihi, int haha){
    int rofl;
    while (haha != 0) {
        rofl = haha;
        haha = hihi % haha;
        hihi = rofl;
    }
    return hihi;
}
int    passCheck(char* pass) {
	FILE* input = fopen("D:\\OiB\\lab 6\\mypass\\xor.txt", "r"); 
	char correctPassword[512] = { '\0' };
	fgets(correctPassword, 512, input);
	encryption(pass);
	if (strcmp(pass, correctPassword) == 0) return 1;
	return 0;
}


int veryUsefulFunction2 (int hihi, int haha){
    int rofl;
    while (haha != 0) {
        rofl = haha;
        haha = hihi % haha;
        hihi = rofl;
    }
    return hihi;
}
void encryption(char* pass) {
	for (; *pass; pass++) *pass = *pass ^ 101;
}