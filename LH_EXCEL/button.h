#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <windows.h>
#include "data.h"
#include "sort.h"

#define BUTTON_WIDTH 8
#define BUTTON_HEIGHT 1
#define INIT_EDIT_X 20
#define INIT_FILE_X 10
#define INIT_MAIN_Y 0
#define INIT_SUB_NUM 6
#define INIT_FSUB_NUM 2
#define DATA_LENGTH 30


typedef struct button 
{
	COORD button_pos;
	char* button_title;
	bool button_life;
}Button;

void display_mainButton(HANDLE hOutput);
void init_sub_edit(Button edit_sub[INIT_SUB_NUM]);
void init_sub_file(Button file_sub[INIT_FSUB_NUM]);
void display_subFile_button(HANDLE hOutput,Button sub_bt[INIT_FSUB_NUM]);
void display_sub_button(HANDLE hOutput,Button sub_bt[INIT_SUB_NUM]);
void row_sum(Table_head &th,int row,HANDLE hOutput,int tag = 1);
void col_sum(Table_head &th,int col,HANDLE hOutput,int tag = 1);
void row_maxMin(Table_head &th,int row,HANDLE hOutput,int tag = 1);
void col_maxMin(Table_head &th,int col,HANDLE hOutput,int tag = 1);
void row_sort(Table_head &th,int row,HANDLE hOutput,int tag = 1);
void col_sort(Table_head &th,int col,HANDLE hOutput,int tag = 1);
void writeFile(Table_head th);
void readFile(Table_head &th);

#endif