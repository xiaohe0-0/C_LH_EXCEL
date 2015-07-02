#include <stdlib.h>
#include <windows.h>
#include "data.h"

/************************************************************************/
//��������void initData()
//��������ֵ��void
//�������ܣ���ʼ���������
/************************************************************************/
void initData(Table_head &Tdata,HANDLE hOutput)
{
	Tdata.row_num = INIT_ROW_NUM - 1;//����
	Tdata.col_num = INIT_COL_NUM - 1;//����
	Tdata.table_row = (Table_line)malloc(Tdata.row_num * sizeof(Table_data));//��һ��
	int i,row_title;
	DWORD NumberOfCharsWritten;
	COORD position;//����λ��

	/*************************��һ��************************/
	Tdata.table_row[0].i = 0;//[0][0]��Ԫ
	Tdata.table_row[0].j = 0;//[0][0]��Ԫ

	/*[1][0]��Ԫ*/
	Table_line next_p = (Table_data*)malloc(sizeof(Table_data));
	next_p->right = NULL;
	next_p->data = "A";
	next_p->i = 1;
	next_p->j = 0;
	Tdata.table_row[0].right = next_p;
	Table_line temp_p = next_p;//��¼��ǰָ��

	/*[1][0]λ����ʾ��A��*/
	position.X = INIT_TABLE_X + COL_SIZE + 8 * COL_CHANGE;
	position.Y = INIT_TABLE_Y  + 2*COL_CHANGE;
	WriteConsoleOutputCharacter(hOutput,temp_p->data,1,
		position, &NumberOfCharsWritten);

	/*2����һ�����һ��*/
	for (i = 2;i < Tdata.col_num;i++)
	{
		/*��ʾλ��*/
		position.X = INIT_TABLE_X + i * COL_SIZE + 8 * COL_CHANGE;
		char col_data = 'A' + i - 1;//��ʾ����ĸ

		/*��������*/
		next_p = (Table_line)malloc(sizeof(Table_data));
		next_p->right = NULL;
		next_p->data = &col_data;
		next_p->i = i;
		next_p->j = 0;

		temp_p->right = next_p;
		temp_p = next_p;

		/*��ʾ*/
		WriteConsoleOutputCharacter(hOutput, temp_p->data,1,
			position, &NumberOfCharsWritten);
	}

	/************************��һ��*************************/
	for (row_title = 1;row_title < Tdata.row_num ;row_title++)
	{
		COORD position = {INIT_TABLE_X + 4*ROW_CHANGE,INIT_TABLE_Y + row_title* ROW_SIZE + 2*COL_CHANGE};

		Tdata.table_row[row_title].right = NULL;
		Tdata.table_row[row_title].i = 0;
		Tdata.table_row[i].j = row_title;

		Tdata.table_row[row_title].data = (char *)malloc((Tdata.row_num - 2) * sizeof(char));//��һ�е����ݡ�1,2,3...��
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
			WriteConsole(hOutput,p->data,CELL_DATA_LEN,&NumberOfCharsWritten,NULL);//��괦���
	
			//WriteConsoleOutputCharacter(hOutput, p->data, CELL_DATA_LEN,	
				//Cur,&NumberOfCharsWritten);
			p = p->right;
		}
		
	}
}