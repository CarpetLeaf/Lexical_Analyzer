#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <locale.h>
#pragma warning(disable:4996)

char TW[8][6] = { "begin", "end", "and", "or", "not", "do", "until", "loop" };
char TD[12][3] = { ";", ":=", "+", "-", "*", "/", ">", "<", "=", "!=", "(", ")" };
int j, f = 0, f1 = 1, k = 1, k1;
FILE* F;
int tableNUM, number;
char s = ' ';
char word[256] = "";
int d = 0;

struct TID {
	char id[256];
	struct TID* Next;
};

struct TNUM {
	int num;
	struct TNUM* Next;
};

struct TID* tid, * headID;
struct TNUM* tnum, * headNUM;

int SearchID(struct TID* head, char s[]) {
	struct TID *el = head;
	int i = 0;
	while (el != NULL)
	{
		if (!strcmp(el->id, s))
			return i;
		i++;
		el=el->Next;
	}
	return -1;
}

int SearchW(char TW[8][6], char s[]) {
	for(int i=0; i<8; i++)
		if (!strcmp(TW[i], s))
			return i;
	return -1;
}

int SearchNUM(struct TNUM* head, int d) {
	struct TNUM* el = head;
	int i = 0;
	while (el != NULL)
	{
		if (el->num == d)
			return i;
		i++;
		el = el->Next;
	}
	return -1;
}

void PrintLists(char TW[8][6], char TD[12][3], struct TNUM* headNUM, struct TID* headID) {
	struct TID* tid;
	struct TNUM* tnum;
	printf("\nТаблица служебных слов (TW):\n");
	for (int i = 0; i < 8; i++) {
		printf("<1, %d> ", i);
		for (int j = 0; j < strlen(TW[i]); j++)
			printf("%c", TW[i][j]);
		printf("\n");
	}
	printf("\nТаблица ограничителей (TD):\n");
	for (int i = 0; i < 12; i++) {
		printf("<2, %d> ", i);
		for (int j = 0; j < strlen(TD[i]); j++)
			printf("%c", TD[i][j]);
		printf("\n");
	}
	printf("\nТаблица идентификаторов (TID):\n");
	tid = headID;
	int i = 0;
	while (tid->Next)
	{
		printf("<3, %d> ", i);
		i++;
		for (int j = 0; j < strlen(tid->id); j++)
			printf("%c", tid->id[j]);
		printf("\n");
		tid = tid->Next;
	}
	printf("\nТаблица чисел-констант (TNUM):\n");
	tnum = headNUM;
	i = 0;
	while (tnum->Next)
	{
		printf("<4, %d> %d\n", i, tnum->num);
		i++;
		tnum = tnum->Next;
	}
}

int getlex() {
	while (1) {
		if (f1) if (fscanf(F, "%c", &s) == EOF)
		{
			break;
		}
		if (s != ' ') {
			if ((s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z'))
			{
				f1 = 0;
				memset(word, 0, strlen(word));
				j = 0;
				while ((s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z') || (s >= '0' && s <= '9'))
				{
					word[j] = s;
					j++;
					if (fscanf(F, "%c", &s) == EOF)
					{
						f1 = 1;
						if (SearchW(TW, word) >= 0) {
							printf("<1, %d>", SearchW(TW, word));
							tableNUM = 1;
							number = SearchW(TW, word);
							return 1;
						}
						else
							if (SearchID(headID, word) < 0)
							{
								strcpy_s(tid->id, word);
								printf("<3, %d>", SearchID(headID, word));
								tableNUM = 3;
								number = SearchID(headID, word);
								tid->Next = (struct TID*)malloc(sizeof(struct TID));
								tid = tid->Next;
								tid->Next = NULL;
								return 1;
							}
							else {
								printf("<3, %d>", SearchID(headID, word));
								tableNUM = 3;
								number = SearchID(headID, word);
								return 1;
							}
						return 0;
					}
				}
				if (SearchW(TW, word) >= 0) {
					printf("<1, %d>", SearchW(TW, word));
					tableNUM = 1;
					number = SearchW(TW, word);
					return 1;
				}
				else
					if (SearchID(headID, word) < 0)
					{
						strcpy_s(tid->id, word);
						printf("<3, %d>", SearchID(headID, word));
						tableNUM = 3;
						number = SearchID(headID, word);
						tid->Next = (struct TID*)malloc(sizeof(struct TID));
						tid = tid->Next;
						tid->Next = NULL;
						return 1;
					}
					else {
						printf("<3, %d>", SearchID(headID, word));
						tableNUM = 3;
						number = SearchID(headID, word);
						return 1;
					}
			}
			else if (s >= '0' && s <= '9') {
				f1 = 0;
				d = s - '0';
				if (fscanf(F, "%c", &s) == EOF)
				{
					f1 = 1;
					break;
				}
				while (s >= '0' && s <= '9') {
					d = d * 10 + s - '0';
					if (fscanf(F, "%c", &s) == EOF)
					{
						f1 = 1;
						return 0;
					}
				}
				if (SearchNUM(headNUM, d) >= 0) {
					printf("<4, %d>", SearchNUM(headNUM, d));
					tableNUM = 4;
					number = SearchNUM(headNUM, d);
					return 1;
				}
				else {
					tnum->num = d;
					printf("<4, %d>", SearchNUM(headNUM, d));
					tableNUM = 4;
					number = SearchNUM(headNUM, d);
					tnum->Next = (struct TNUM*)malloc(sizeof(struct TNUM));
					tnum = tnum->Next;
					tnum->Next = NULL;
					return 1;
				}
			}
			else if (s == '/')
			{
				f1 = 0;
				if (fscanf(F, "%c", &s) == EOF)
				{
					f1 = 1;
					printf("<2, 5>");
					tableNUM = 2;
					number = 5;
					return 1;
				}
				if (s != '*') {
					printf("<2, 5>");
					tableNUM = 2;
					number = 5;
					return 1;
				}
				else
				{
					k1 = k;
					while (1)
					{
						if (fscanf(F, "%c", &s) == EOF)
						{
							f1 = 1;
							printf("Ошибка в строке %d: ", k1);
							printf("комментарий не был закрыт\n");
							exit(0);
						}
						if (s == '\n')
							k++;
						if (s == '*') {
							if (fscanf(F, "%c", &s) == EOF)
							{
								f1 = 1;
								printf("Ошибка в строке %d: ", k1);
								printf("комментарий не был закрыт\n");
								exit(0);
							}
							else if (s == '/') {
								if (fscanf(F, "%c", &s) == EOF)
								{
									f1 = 1;
									return 0;
								}
								break;
							}
							else if (s == '\n')
								k++;
						}
					}
				}
			}
			else if (s == ':') {
				f1 = 0;
				if (fscanf(F, "%c", &s) == EOF)
				{
					f1 = 1;
					printf("Ошибка в строке %d: ", k);
					printf("втречен неизвестный символ\n");
					exit(0);
				}
				if (s == '=') {
					f1 = 1;
					printf("<2, 1>");
					tableNUM = 2;
					number = 1;
					return 1;
				}
				else {
					printf("Ошибка в строке %d: ", k);
					printf("втречен неизвестный символ\n");
					exit(0);
				}
			}
			else if (s == '!') {
				f1 = 0;
				if (fscanf(F, "%c", &s) == EOF)
				{
					f1 = 1;
					printf("Ошибка в строке %d: ", k);
					printf("втречен неизвестный символ\n");
					exit(0);
				}
				if (s == '=') {
					f1 = 1;
					printf("<2, 9>");
					tableNUM = 2;
					number = 9;
					return 1;
				}
				else {
					printf("Ошибка в строке %d: ", k);
					printf("втречен неизвестный символ\n");
					exit(0);
				}
			}
			else if (s == '\n')
			{
				printf("\n");
				k++;
				f1 = 1;
			}
			else {
				f1 = 1;
				if (!((s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z') || (s >= '0' && s <= '9'))) {
					f = 0;
					for (j = 0; j < 12; j++) {
						if (s == TD[j][0])
						{
							printf("<2, %d>", j);
							tableNUM = 2;
							number = j;
							f = 1;
							return 1;
						}
					}
					if (!f && s != ' ' && s != EOF && s != '\t' && s != '\n')
					{
						printf("Ошибка в строке %d: ", k);
						printf("встречен неизвестный символ\n");
						exit(0);
					}
				}
			}
		}
		else {
			if (fscanf(F, "%c", &s) == EOF)
			{
				f1 = 1;
				break;
			}
			f1 = 0;
		}
	}
	return 0;
}

void E();

void E3(void) {
	if (tableNUM == 3 || tableNUM == 4) {
		getlex();
		return;
	}
	if (tableNUM == 2 && number == 10) {
		getlex();
		E();
		if (!(tableNUM == 2 && number == 11)) {
			printf("\nОжидалось ) в строке: %d\n", k);
			exit(0);
		}
		getlex();
		return;
	}
	printf("\nОжидалось число или идентификатор в в строке: %d\n", k);
	exit(0);
}


void E2(void) {
	E3();
	while ((tableNUM == 2 && (number == 4 || number == 5)) || (tableNUM == 1 && number == 2)) {		
		getlex();
		E3();
	}
	return;
}


void E1(void) {
	E2();
	while ((tableNUM == 2 && (number == 2 || number == 3)) || (tableNUM == 1 && number == 3)) {	
		getlex();
		E2();
	}
	return;
}


void E(void) {
	E1();
	while (tableNUM == 2 && (number == 6 || number == 7 || number == 8 || number == 9)) {		
		getlex();
		E1();
	}
	return;
}


void D(void) {
	if (tableNUM == 3)
		return;
	printf("\nОжидался идентифкатор в строке: %d\n", k);
	exit(0);
}


void C(void) {
	if (tableNUM == 1 && number == 5) {
		getlex();
		if (tableNUM == 1 && number == 6) {
			getlex();
			E();
			C();
			while (tableNUM == 2 && number == 0) {
				getlex();
				C();
			}
			if (tableNUM == 1 && number == 7) {
				getlex();
				return;
			}
			printf("\nОжидалось слово loop в строке: %d\n", k);
			exit(0);
		}
		printf("\nОжидалось слово until в строке: %d\n", k);
		exit(0);
	}
	D();
	getlex();
	if (tableNUM == 2 && number == 1) {
		getlex();
		E();
		if (!((tableNUM == 2 && number == 0) || (tableNUM == 1 && number))) {
			printf("\nОжидалось ; в строке: %d\n", k-1);
			exit(0);
		}
		return;
	}
	printf("Ожидался оператор или цикл в строке: %d\n", k);
	exit(0);
}

void B(void) {
	if (tableNUM == 1 && number == 0) {
		getlex();
		C();		
		while (tableNUM == 2 && number == 0) {
			getlex();
			C();
		}
		getlex();
		if (tableNUM == 1 && number == 1)
			return;
	}
	printf("\nОшибка в операторных скобках в строке: %d\n", k);
	exit(0);
}


int main() {
	F = fopen("in.txt", "r");
	tid = (struct TID*)malloc(sizeof(struct TID));
	tid->Next = NULL;
	tnum = (struct TNUM*)malloc(sizeof(struct TNUM));
	tnum->Next = NULL;
	headID = tid;
	headNUM = tnum;
	setlocale(LC_ALL, "Russian");
	getlex();
	B();
	PrintLists(TW, TD, headNUM, headID);
	free(headID);
	free(headNUM);
	return 0;
}