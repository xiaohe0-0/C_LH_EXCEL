#include <windows.h>
#include <stdio.h>
#include "sort.h"
#include "button.h"
#include "data.h"

/************************************************************************/
//��������void display_mainButton(HANDLE hOutput)
//��������ֵ��void
//�������ܣ���ʾ����ť
/************************************************************************/
void display_mainButton(HANDLE hOutput)
{
	///////////////////////////�ļ���ť///////////////////////////////
	Button fileMain;
	DWORD NumberOfCharsWritten;

	fileMain.button_life = true;
	fileMain.button_pos.X = INIT_FILE_X;
	fileMain.button_pos.Y = INIT_MAIN_Y;
	fileMain.button_title = "���ļ���";



	WriteConsoleOutputCharacter(hOutput,fileMain.button_title,BUTTON_WIDTH,fileMain.button_pos,&NumberOfCharsWritten);
	///////////////////////////�༭��ť///////////////////////////////
	Button editMain;

	editMain.button_life = true;
	editMain.button_pos.X = INIT_EDIT_X;
	editMain.button_pos.Y = INIT_MAIN_Y;
	editMain.button_title = "���༭��";
	
	WriteConsoleOutputCharacter(hOutput,editMain.button_title,BUTTON_WIDTH,editMain.button_pos,&NumberOfCharsWritten);
}

/************************************************************************/
//��������void init_sub_edit()
//��������ֵ��void
//�������ܣ���ʼ�����ذ�ť
/************************************************************************/
void init_sub_edit(Button edit_sub[INIT_SUB_NUM])
{
	int i;
	if(edit_sub->button_life)
	{
		edit_sub[0].button_title = "���ϼơ�";
		edit_sub[1].button_title = "��ƽ����";
		edit_sub[2].button_title = "�����";
		edit_sub[3].button_title = "����С��";
		edit_sub[4].button_title = "������";
		edit_sub[5].button_title = "������";
	}
	else
	{
		edit_sub[0].button_title = "     ";
		edit_sub[1].button_title = "     ";
		edit_sub[2].button_title = "     ";
		edit_sub[3].button_title = "     ";
		edit_sub[4].button_title = "     ";
		edit_sub[5].button_title = "     ";
	}

	//edit_sub->button_life = false;
	for(i = 0;i < INIT_SUB_NUM;i++)
	{
		edit_sub[i].button_pos.X = INIT_EDIT_X;
		edit_sub[i].button_pos.Y = INIT_MAIN_Y + (i+1) * BUTTON_HEIGHT;
	}
}

/************************************************************************/
//��������void init_sub_edit()
//��������ֵ��void
//�������ܣ���ʼ�����ذ�ť
/************************************************************************/
void init_sub_file(Button file_sub[INIT_FSUB_NUM])
{
	int i;
	if(file_sub->button_life)
	{
		file_sub[0].button_title = "���򿪡�";
		file_sub[1].button_title = "�����桿";
	}
	else
	{
		file_sub[0].button_title = "     ";
		file_sub[1].button_title = "     ";
	}

	//edit_sub->button_life = false;
	for(i = 0;i < INIT_FSUB_NUM;i++)
	{
		file_sub[i].button_pos.X = INIT_FILE_X;
		file_sub[i].button_pos.Y = INIT_MAIN_Y + (i+1) * BUTTON_HEIGHT;
	}
}
/************************************************************************/
//��������void init_sub_edit()
//��������ֵ��void
//�������ܣ���ʾ���ر༭��ť
/************************************************************************/
void display_sub_button(HANDLE hOutput,Button sub_bt[INIT_SUB_NUM])
{
	int i;
	DWORD NumberOfCharsWritten;
	init_sub_edit(sub_bt);
	for (i = 0;i < INIT_SUB_NUM;i++)
			WriteConsoleOutputCharacter(hOutput,sub_bt[i].button_title,BUTTON_WIDTH,sub_bt[i].button_pos,&NumberOfCharsWritten);

}
/************************************************************************/
//��������void display_subFile_button(HANDLE hOutput,Button sub_bt[INIT_FSUB_NUM])
//��������ֵ��void
//�������ܣ���ʾ�����ļ���ť
/************************************************************************/
void display_subFile_button(HANDLE hOutput,Button sub_bt[INIT_FSUB_NUM])
{
	int i;
	DWORD NumberOfCharsWritten;
	init_sub_file(sub_bt);
	for (i = 0;i < INIT_FSUB_NUM;i++)
		WriteConsoleOutputCharacter(hOutput,sub_bt[i].button_title,BUTTON_WIDTH,sub_bt[i].button_pos,&NumberOfCharsWritten);

}
/************************************************************************/
//��������void data_output(Table_line record,HANDLE hOutput)
//��������ֵ��void
//�������ܣ�������
/************************************************************************/
void data_output(Table_line record,HANDLE hOutput)
{
	COORD curs;
	DWORD NumberOfCharsWritten;

	curs.X = record->i * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
	curs.Y = record->j * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

	strcat(record->data,"    ");
	SetConsoleCursorPosition(hOutput,curs);
	WriteConsole(hOutput,record->data,CELL_DATA_LEN,&NumberOfCharsWritten,NULL);//��괦���
}

/************************************************************************/
//��������void row_sum(Table_head &th,int row)
//��������ֵ��void
//�������ܣ�һ�����
/************************************************************************/
void row_sum(Table_head &th,int row,HANDLE hOutput,int tag)
{
	Table_line p = th.table_row[row].right;
	Table_line last = p;
	Table_line last_elem;
	float row_totle = 0;
	bool last_col = false;
	int count = 0;

	/**���� �ж����һ�������ڣ�**/
	while (p)
	{
		row_totle += atof(p->data);
		if(p->i == th.col_num - 1)	
		{
			last_elem = p;
			last_col = true;
		}

		count++;
		last = p;
		p = p->right;
	}

	/**���һ����������**/
	if (!last_col)
	{
		Table_line tmp = (Table_line)malloc(sizeof(Table_data));
		tmp->j = row;
		tmp->i = th.col_num - 1;
		tmp->right = NULL;
		tmp->data = (char *)malloc(CELL_DATA_LEN * sizeof(char));
		if(tag == 1) sprintf(tmp->data,"%g",row_totle);
		if(tag == 2 && count) sprintf(tmp->data,"%g",row_totle/count);
		if(tag == 2 && !count) sprintf(tmp->data,"%g",0.0);

		if(last) last->right = tmp;
		else th.table_row[row].right = tmp;

		last = tmp;
	}
	else
	{
		last = last_elem;
		if(tag == 1) sprintf(last->data,"%g",row_totle);
		if(tag == 2) sprintf(last->data,"%g",row_totle/count);
	}
    
	data_output(last,hOutput);//��괦���
}

/************************************************************************/
//��������void col_sum(Table_head &th,int row,HANDLE hOutput)
//��������ֵ��void
//�������ܣ�һ�����
/************************************************************************/
void col_sum(Table_head &th,int col,HANDLE hOutput,int tag)
{
	Table_line p = th.table_row[1].right;
	Table_line last = p;
	float col_totle = 0;
	bool last_row = false;
	int re;
	int count = 0;

	/**ѭ�����**/
	for (re = 1;re < th.row_num;re++)
	{
		p = th.table_row[re].right;
		while(p)
		{
			if(p->i == col)
			{ 
				col_totle += atof(p->data);
				if(p->j == th.row_num - 1) 
				{
					last_row = true;
					last = p;
				}
				count++;
			}
			p = p->right;
		}
	}

	/*���Ĳ�����*/
	if(!last_row)
	{
		Table_line tmp = (Table_line)malloc(sizeof(Table_data));
		tmp->j = th.row_num - 1;
		tmp->i = col;
		tmp->right = NULL;
		tmp->data = (char *)malloc(CELL_DATA_LEN * sizeof(char));
		if(tag == 1)	sprintf(tmp->data,"%g",col_totle);
		if(tag == 2 && count) sprintf(tmp->data,"%g",col_totle/count);
		if(tag == 2 && !count) sprintf(tmp->data,"%g",0.0);


		th.table_row[th.row_num - 1].right = tmp;

		last = tmp;
	}
	
	else
	{
		if(tag == 1) sprintf(last->data,"%g",col_totle);
		if(tag == 2 && count) sprintf(last->data,"%g",col_totle/count);
		if(tag == 2 && !count) sprintf(last->data,"%g",0.0);
	}

	data_output(last,hOutput);//��괦���
}
/************************************************************************/
//��������void row_maxMin(Table_head &th,int row,HANDLE hOutput,int tag)
//��������ֵ��void
//�������ܣ�һ���������С
/************************************************************************/
void row_maxMin(Table_head &th,int row,HANDLE hOutput,int tag)
{
	bool last_exist = false;
	Table_line last_elem;
	Table_line record;
	Table_line p = th.table_row[row].right;
	if(!p)	return;

	float max = atof(p->data);
	float min = atof(p->data);

	/*�ռ����� �ж����һ��λ���Ƿ����*/
	while(p)
	{
		float data = atof(p->data);
		if(data > max) max = data;
		if(data < min) min = data;
		if(p->i == th.col_num - 1) 
		{
			last_exist = true;
			last_elem = p;
		}
		record = p;
		p = p->right;
	}

	/*���һ��������*/
	if (!last_exist)
	{
		Table_line temp = (Table_line)malloc(sizeof(Table_data));
		temp->j = row;
		temp->i = th.col_num - 1;
		temp->right = NULL;

		temp->data = (char *)malloc(CELL_DATA_LEN * sizeof(char));
		if(tag == 1) sprintf(temp->data,"%g",max);
		if(tag == 2) sprintf(temp->data,"%g",min);

		record->right = temp;
		record = temp;
	}
	else
	{
		record = last_elem;
		if(tag == 1) sprintf(record->data,"%g",max);
		if(tag == 2) sprintf(record->data,"%g",min);

	}
	/*���*/
	data_output(record,hOutput);//��괦���
}
/************************************************************************/
//��������void col_maxMin(Table_head &th,int col,HANDLE hOutput,int tag)
//��������ֵ��void
//�������ܣ�һ���������С
/************************************************************************/
void col_maxMin(Table_head &th,int col,HANDLE hOutput,int tag)
{
	int re;
	Table_line p;
	Table_line record;
	bool last_exist = false;
	Table_line last_elem;
	static float max;
	static float min;


	/*�ռ����� �ж����һ��λ���Ƿ����*/
	for (re = 1;re < th.row_num;re++)
	{
		p = th.table_row[re].right;
		while(p)
		{
			if(p->i == col)
			{
				float data = atof(p->data);
				min = data;
				max = data;
			}
			p = p->right;
		}
	}

	for (re = 1;re < th.row_num;re++)
	{
		p = th.table_row[re].right;
		while(p)
		{
			if(p->i == col)
			{
				float data = atof(p->data);
				if(data > max) max = data;
				if(data < min) min = data;

				if(p->j == th.row_num - 1)
				{
					last_elem = p;
					last_exist = true;
				}
			}
			p = p->right;
		}
	}
	

	/*���һ��������*/
	if (!last_exist)
	{
		Table_line temp = (Table_line)malloc(sizeof(Table_data));
		temp->i = col;
		temp->j = th.row_num - 1;
		temp->right = NULL;
		temp->data = (char *)malloc(CELL_DATA_LEN * sizeof(char));

		if(!th.table_row[th.row_num - 1].right)		th.table_row[th.row_num - 1].right = temp;
		else
		{
			record = th.table_row[th.row_num - 1].right;
			while(record->right) record = record->right;
			record->right = temp;
		}
		record = temp;
	}
	else
	{
		record = th.table_row[th.row_num - 1].right;
		while(record->right) record = record->right;
	}


	if(tag == 1) sprintf(record->data,"%g",max);
	if(tag == 2) sprintf(record->data,"%g",min);

	/*���*/
	data_output(record,hOutput);//��괦���
}
/************************************************************************/
//��������void row_sort(Table_head &th,int row,HANDLE hOutput,int tag)
//��������ֵ��void
//�������ܣ�һ�н�������
/************************************************************************/
void row_sort(Table_head &th,int row,HANDLE hOutput,int tag)
{
	int re = 1;
	int judge;
	float sort[COL_MAX];
	Table_line p = th.table_row[row].right;
	COORD curs;
	DWORD NumberOfCharsWritten;

	if(!p) return;

	while(p)
	{
		sort[re] = atof(p->data);
		p = p->right;
		re++;
	}

	int order[3] = {5,3,1};
	increase_Sort(sort, order,3,re - 1,tag);
	p = th.table_row[row].right;

	re = 1;

	for(judge = 1;judge < th.col_num;judge++)
	{
		p = th.table_row[row].right;
		while(p)
		{
			if(p->i == judge)
			{
				sprintf(p->data,"%g",sort[re]);
				re++;

				curs.X = p->i * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
				curs.Y = p->j * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

				strcat(p->data,"   ");
				SetConsoleCursorPosition(hOutput,curs);
				WriteConsole(hOutput,p->data,CELL_DATA_LEN,&NumberOfCharsWritten,NULL);//��괦���
			}
			
			p = p->right;
		}
	}	
}
/************************************************************************/
//��������void col_sort(Table_head &th,int row,HANDLE hOutput,int tag)
//��������ֵ��void
//�������ܣ�һ�н�������
/************************************************************************/
void col_sort(Table_head &th,int col,HANDLE hOutput,int tag)
{
	Table_line p = th.table_row;
	COORD curs;
	DWORD NumberOfCharsWritten;
	float sort[ROW_MAX];
	int re = 1;
	int row_count;
	for (row_count = 1;row_count < th.row_num;row_count++)
	{
		p = th.table_row[row_count].right;
		while(p)
		{
			if(p->i == col)
			{
				sort[re] = atof(p->data);
				re++;
			}

			p = p->right;
		}
	}

	int order[3] = {5,3,1};

	increase_Sort(sort, order,3,re - 1,tag);

	re = 1;
	for (row_count = 1;row_count < th.row_num;row_count++)
	{
		p = th.table_row[row_count].right;
		while(p)
		{
			if(p->i == col)
			{
				sprintf(p->data,"%g",sort[re]);
				re++;

				curs.X = p->i * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
				curs.Y = p->j * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

				strcat(p->data,"   ");
				SetConsoleCursorPosition(hOutput,curs);
				WriteConsole(hOutput,p->data,CELL_DATA_LEN,&NumberOfCharsWritten,NULL);//��괦���
			}

			p = p->right;
		}
	}
}

/************************************************************************/
//��������void row_sort(Table_head &th,int row,HANDLE hOutput,int tag)
//��������ֵ��void
//�������ܣ�д�ļ�
/************************************************************************/
void writeFile(Table_head th)
{
	char * fileName = "info.txt";
	FILE *fp = fopen(fileName,"w");
	Table_line p;
	int row_count;
	for (row_count = 1;row_count < th.row_num;row_count++)
	{
		p = th.table_row[row_count].right;
		while(p)	
		{
			fprintf(fp,"%d %d %s\n",p->i,p->j,p->data);
			p = p->right;
		}
	}

	fclose(fp);
}

/************************************************************************/
//��������void readFile(Table_head &th)
//��������ֵ��void
//�������ܣ����ļ�
/************************************************************************/
void readFile(Table_head &th)
{
	char * fileName = "info.txt";
	FILE *fp = fopen(fileName,"r");
	
	char form_data[ROW_MAX * COL_MAX][DATA_LENGTH];
	char *p;

	int re = 0;
	int row_count;
	int row[ROW_MAX * COL_MAX];
	int col[ROW_MAX * COL_MAX];
	char *data[ROW_MAX * COL_MAX];

	Table_line next,temp;
	while(!feof(fp))
	{
		fgets(form_data[re],DATA_LENGTH,fp);
		re++;
	}
	
	if(!re) return;

	for (row_count = 0;row_count < re - 1;row_count++)
	{
		p = form_data[row_count];
		row[row_count] = atoi(p);

		while(*p != ' ')
		{
			p++;
		}
		p++;
		
		col[row_count]= atoi(p);

		while(*p != ' ') 
		{
			p++;
		}
		p++;
		data[row_count] = p;
	}

	for (row_count = 0;row_count < re - 1;row_count++)
	{
		int col_p = col[row_count];
		temp = th.table_row[col_p].right;
		
		if(!temp)
		{
			temp = (Table_line)malloc(sizeof(Table_data));
			temp->i = row[row_count];
			temp->j = col[row_count];
			temp->data = data[row_count];
			temp->right = NULL;

			th.table_row[col_p].right = temp;
		}
		else
		{
			while(temp->right) temp = temp->right;
			next = (Table_line)malloc(sizeof(Table_data));
			next->i = row[row_count];
			next->j = col[row_count];
			next->data = data[row_count];
			next->right = NULL;

			temp->right = next;
		}
	}

	fclose(fp);

}