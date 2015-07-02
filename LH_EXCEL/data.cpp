#include <stdlib.h>
#include <windows.h>
#include "data.h"

/************************************************************************/
//函数名：void initData()
//函数返回值：void
//函数功能：初始化表格数据
/************************************************************************/
void initData(Table_head &Tdata,HANDLE hOutput)
{
	Tdata.row_num = INIT_ROW_NUM - 1;//行数
	Tdata.col_num = INIT_COL_NUM - 1;//列数
	Tdata.table_row = (Table_line)malloc(Tdata.row_num * sizeof(Table_data));//第一列
	int i,row_title;
	DWORD NumberOfCharsWritten;
	COORD position;//坐标位置

	/*************************第一行************************/
	Tdata.table_row[0].i = 0;//[0][0]单元
	Tdata.table_row[0].j = 0;//[0][0]单元

	/*[1][0]单元*/
	Table_line next_p = (Table_data*)malloc(sizeof(Table_data));
	next_p->right = NULL;
	next_p->data = "A";
	next_p->i = 1;
	next_p->j = 0;
	Tdata.table_row[0].right = next_p;
	Table_line temp_p = next_p;//记录当前指针

	/*[1][0]位置显示“A”*/
	position.X = INIT_TABLE_X + COL_SIZE + 8 * COL_CHANGE;
	position.Y = INIT_TABLE_Y  + 2*COL_CHANGE;
	WriteConsoleOutputCharacter(hOutput,temp_p->data,1,
		position, &NumberOfCharsWritten);

	/*2到第一行最后一列*/
	for (i = 2;i < Tdata.col_num;i++)
	{
		/*显示位置*/
		position.X = INIT_TABLE_X + i * COL_SIZE + 8 * COL_CHANGE;
		char col_data = 'A' + i - 1;//显示的字母

		/*链成首行*/
		next_p = (Table_line)malloc(sizeof(Table_data));
		next_p->right = NULL;
		next_p->data = &col_data;
		next_p->i = i;
		next_p->j = 0;

		temp_p->right = next_p;
		temp_p = next_p;

		/*显示*/
		WriteConsoleOutputCharacter(hOutput, temp_p->data,1,
			position, &NumberOfCharsWritten);
	}

	/************************第一列*************************/
	for (row_title = 1;row_title < Tdata.row_num ;row_title++)
	{
		COORD position = {INIT_TABLE_X + 4*ROW_CHANGE,INIT_TABLE_Y + row_title* ROW_SIZE + 2*COL_CHANGE};

		Tdata.table_row[row_title].right = NULL;
		Tdata.table_row[row_title].i = 0;
		Tdata.table_row[i].j = row_title;

		Tdata.table_row[row_title].data = (char *)malloc((Tdata.row_num - 2) * sizeof(char));//第一列的数据“1,2,3...”
		itoa(row_title,Tdata.table_row[row_title].data,10);

		WriteConsoleOutputCharacter(hOutput,Tdata.table_row[row_title].data,1,
			position, &NumberOfCharsWritten);
	}
}

void display_cell(Table_head &Tdata,HANDLE hOutput)
{
	int row_num;
	COORD Cur;
	DWORD NumberOfCharsWritten;
	WORD wd;
	
	for (row_num = 1; row_num < Tdata.row_num;row_num++)
	{
		Table_line p = Tdata.table_row[row_num].right;
		while (p)
		{
			Cur.X = INIT_TABLE_X + p->i * COL_SIZE + ROW_CHANGE *4;
			
			Cur.Y = INIT_TABLE_Y + p->j * ROW_SIZE + COL_CHANGE *2;
		
			SetConsoleCursorPosition(hOutput,Cur);
			WriteConsole(hOutput,p->data,CELL_DATA_LEN,&NumberOfCharsWritten,NULL);//光标处输出
	
			//WriteConsoleOutputCharacter(hOutput, p->data, CELL_DATA_LEN,	
				//Cur,&NumberOfCharsWritten);
			p = p->right;
		}
		
	}
}