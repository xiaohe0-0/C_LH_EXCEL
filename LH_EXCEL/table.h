#ifndef _TABLE_H_
#define _TABLE_H_

#include <windows.h>
#include "data.h"
#include "button.h"

/*坐标*/
typedef struct coords
{
	int x;
	int y;
}Coords;

/*表格*/
typedef struct table 
{
	Coords table_coord;
	Coords cell[ROW_MAX][COL_MAX];
	char* table_char[ROW_MAX][COL_MAX];
}Table;

/*函数*/
void screenSize();
void outputData(COORD coord,LPCWSTR data,DWORD length,HANDLE hOutput);
void setTable(Table &T);
void display(HANDLE hOutput);
void cellBground(SMALL_RECT rect,HANDLE hOutput,WORD wd = 255);
void fillBlack(HANDLE hOutput);
void left_event(HANDLE hOutput,SMALL_RECT sc,Button subEdit[INIT_SUB_NUM],Button subFile[INIT_FSUB_NUM]);
void dealEvent(INPUT_RECORD buf,HANDLE hOutput,Table_head &Th,Button subEdit[INIT_SUB_NUM],Button subFile[INIT_FSUB_NUM]);

#endif