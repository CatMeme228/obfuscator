#define _CRT_SECURE_NO_WARNINGS
#include "myHeader.h"

int junk_code(FILE* input, FILE* output) {
	if (!input || !output) return -1;
	int junkNumber = 0;

	//мусор
	const char* junk_function_init = "int veryUsefulFunction%d (int hihi%d, int haha%d);"; //вставка счётчика сюда
	const char* junk_function = 
		"int veryUsefulFunction%d (int hihi%d, int haha%d){" //вставка счётчика сюда
		"    int rofl;"
		"    while (haha != 0) {"
		"        rofl = haha;"
		"        haha = hihi % haha;"
		"        hihi = rofl;"
		"    }"
		"    return hihi;"
		"}"
	;
	const char* junk_for =
		"int count%d = 0;" //вставка счётчика сюда
		"for (int i = 1; i< 100000, i++){"
		"    if (i%(%d)==0){" //вставка счётчика сюда
		"        count%d = count%d + 1;" //вставка счётчика сюда
		"    }"
		"}"
	;
	const char* junk_var = "int trololo%d = veryUsefulFunction(%d+67, %d+173);"; //вставка счётчика сюда
	
	//вспомогатеьный код
	char buffer[300] = { '\0' };
	int inFunction = 0; //0- вне функции, 1- в функции
	while (fgets(buffer, 300, input)) {

	//вставка

	}
	return 0;
}

