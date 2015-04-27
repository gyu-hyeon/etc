#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning (disable : 4996)

#define TRUE 1
#define FALSE 0

typedef struct _calendar calendar;

typedef struct _calendar
{
	// member variable
	int year, month;
	int arr[6][7];
	int months[13];

	// member function pointer
	int (*isLeapYear)(const calendar * const);
	int (*getDayOfWeek)(const calendar * const);
	int (*setCalendar)(calendar *);
	void (*printCalendar)(calendar *);
	void (*outFile)(calendar *);
	char* (*getFileName)(calendar *);
};

///////////////////// function list //////////////////////
int isLeapYear(const calendar * const);
int getDayOfWeek(const calendar * const);

int setCalendar(calendar *);
void printCalendar(calendar *);
void outFile(calendar *);
char* getFileName(calendar *);

calendar* constructor(void);
void destructor(calendar *);
////////////////////////// end ///////////////////////////

////////////////////// MAIN BEGIN ////////////////////////
int main(void)
{
	calendar * cal = constructor();
	cal->setCalendar(cal);
	cal->printCalendar(cal);
	destructor(cal);

	return 0;
}
/////////////////////// MAIN END /////////////////////////

// 윤년 여부 검사
int isLeapYear(const calendar * const THIS)
{
	return ((THIS->year % 4 == 0) && (THIS->year % 100 != 0) || (THIS->year % 400 == 0)) ? TRUE : FALSE;
}

// 그 해의 월 첫번째 날의 요일 반환
int getDayOfWeek(const calendar * const THIS)
{
	int year, month;
	int y1, y2;

	year = THIS->year;
	month = THIS->month;

	if (month <= 2) { month += 12; year--; }

	y1 = THIS->year / 100;
	y2 = THIS->year % 100;

	return ((21 * y1 / 4) + (5 * y2 / 4) + (26 * (month + 1) / 10)) % 7;
}

// 달력 설정
int setCalendar(calendar * THIS)
{
	int day = 1;
	int row = 0, column = THIS->getDayOfWeek(THIS);

	THIS->arr[row][column++] = day++;
	while (day <= THIS->months[THIS->month])
	{
		if (column % 7 == 0) { row++; column = 0; }
		THIS->arr[row][column++] = day++;
	}

	return row;
}

// 달력 출력
void printCalendar(calendar * THIS)
{
	int row, limitRow;
	int column;

	limitRow = THIS->setCalendar(THIS);
	printf("\n%d년 %d월\n", THIS->year, THIS->month);

	for (row = 0; row <= limitRow; ++row)
		for (column = 0; column < 7; ++column)
			if ((THIS->arr)[row][column] != 0) printf("%2d%s", (THIS->arr)[row][column], column == 6 ? "\n" : " ");
			else printf("   ");

	printf("\n\n");

	THIS->outFile(THIS);
}

// 생성자
calendar* constructor(void)
{
	calendar * temp = (calendar*)malloc(sizeof(calendar));
	int months[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int i;

	temp->isLeapYear = isLeapYear;
	temp->getDayOfWeek = getDayOfWeek;
	temp->setCalendar = setCalendar;
	temp->printCalendar = printCalendar;
	temp->outFile = outFile;
	temp->getFileName = getFileName;

	printf("input year, month : ");
	scanf("%d %d", &(temp->year), &(temp->month));

	for (i = 0; i < sizeof(months) / sizeof(int); ++i)
		temp->months[i] = months[i];

	if (temp->isLeapYear(temp)) temp->months[2] = 29;
	else temp->months[2] = 28;

	memset(temp->arr, 0, sizeof(int) * 6 * 7);

	return temp;
}

// 소멸자
void destructor(calendar * THIS)
{
	free(THIS);
}

// 파일 출력
void outFile(calendar * THIS)
{
	char * filename = THIS->getFileName(THIS);
	FILE * fp = fopen(filename, "w");
	int row, limitRow;
	int column;

	if (fp == NULL) exit(1);

	limitRow = THIS->setCalendar(THIS);
	fprintf(fp, "%d년 %d월\n", THIS->year, THIS->month);

	for (row = 0; row <= limitRow; ++row)
		for (column = 0; column < 7; ++column)
			if ((THIS->arr)[row][column] != 0) fprintf(fp, "%2d%s", (THIS->arr)[row][column], column == 6 ? "\n" : " ");
			else fprintf(fp, "   ");

	fclose(fp);
	free(filename);
}

// 파일 이름 얻기
char* getFileName(calendar * THIS)
{
	char year[10], month[10], name[20] = { 0 };
	char * retname;

	itoa(THIS->year, year, 10);
	itoa(THIS->month, month, 10);

	strcpy(name, year);
	strcat(name, "_");
	strcat(name, month);
	strcat(name, ".txt");

	retname = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(retname, name);
	return retname;
}