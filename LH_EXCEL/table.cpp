#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "button.h"

/*��¼����¼�֮ǰ�Ķ���*/
static SMALL_RECT pre_cell = {0,0,0,0};
static int pre_state = 0;

//��Ԫ�������볤��
static int input_len = 0;

//��Ԫ������ʱ��λ��
SMALL_RECT cell_pos;

//�����Ѿ����������
static char data_record[6];

/************************************************************************/
//��������void screenSize()
//��������ֵ��void
//�������ܣ����ô��ڵĴ�С
/************************************************************************/
void screenSize()
{
	system ( "mode con:lines=35 " ); 
	system ( "mode con:cols=110 " );
}


/************************************************************************/
//��������void outputData(COORD coord,LPCWSTR data)
//��������ֵ��void
//�������ܣ������Ϣ
/************************************************************************/
void outputData(COORD coord,char* data,DWORD length,HANDLE hOutput)
{
	DWORD NumberOfCharsWritten;

	WriteConsoleOutputCharacter(hOutput, data, length,	
		coord,&NumberOfCharsWritten);
}

/************************************************************************/
//��������void setTable(Table &T)
//��������ֵ��void
//�������ܣ����ñ��Ļ�����Ϣ
/************************************************************************/
void setTable(Table &T)
{
	/*���Ŀ�ʼ����*/
	T.table_coord.x = INIT_TABLE_X;
	T.table_coord.y = INIT_TABLE_Y;

	/*�������*/
	int i,j;
	for (i = 0;i < ROW_MAX;i++)
	{
		for (j = 0;j < COL_MAX;j++)
		{
			/*�ؼ������Ʊ��*/
			if (i == 0)//��һ��
			{
				if(j == 0) T.table_char[i][j] = "��";
				else if(j == COL_MAX - 1) T.table_char[i][j] = "��";
				else T.table_char[i][j] = "��";
			}
			else if(i == ROW_MAX -1)//���һ��
			{
				if (j == 0)	T.table_char[i][j] = "��";
				else if(j == COL_MAX - 1)T.table_char[i][j] = "��";
				else T.table_char[i][j] = "��";
			}
			else if (j == 0)	T.table_char[i][j] = "��";//��һ��
			else if(j == COL_MAX - 1) T.table_char[i][j] = "��";//���һ��
			else T.table_char[i][j] = "��";//�м��
		}
	}
}



/************************************************************************/
//��������void display()
//��������ֵ��void
//�������ܣ���ʾ���
/************************************************************************/
void display(HANDLE hOutput)
{
	/************************��ʾ���***************************/
	Table T;
	setTable(T);

	DWORD NumberOfCharsWritten;

	/*����еĿհױ߿�����*/
	char * map_row = "����";
	char* map_col = "��";

	int row,col;
	for (row = 0;row < ROW_MAX;row++)
	{
		for (col = 0;col < COL_MAX;col++)
		{
			COORD Cur;
			Cur.X = T.table_coord.x + col * COL_SIZE;
			Cur.Y = T.table_coord.y + row * ROW_SIZE;

			/*������*/
			outputData(Cur,T.table_char[row][col],2,hOutput);

			/*����հױ߿�*/
			COORD blank_row;
			COORD blank_col;
			blank_row.X = Cur.X + ROW_CHANGE*4;
			blank_row.Y = Cur.Y;
			blank_col.X = Cur.X ;
			blank_col.Y = Cur.Y + ROW_CHANGE*2;

			if (row != ROW_MAX - 1)
			{
				outputData(blank_col,map_col,2,hOutput);
			}

			if(col != COL_MAX - 1)
			{
				outputData(blank_row,map_row,4,hOutput);

				blank_row.X += ROW_CHANGE*8;

				outputData(blank_row,map_row,2,hOutput);
			}
		}
	}
}
/************************************************************************/
//��������bool data_pos_exist(SMALL_RECT pos,Table_head th)
//��������ֵ��void
//�������ܣ�ĳһλ�õ������Ƿ����
/************************************************************************/
bool data_pos_exist(SMALL_RECT pos,Table_head th)
{
	COORD cur_cell;
	cur_cell.X = (pos.Left - INIT_TABLE_X)/COL_SIZE;//X��
	cur_cell.Y = (pos.Right - INIT_TABLE_Y)/ROW_SIZE; //Y��

	Table_line th_temp = th.table_row[cur_cell.Y].right;
	while(th_temp)
	{
		if(th_temp->i == cur_cell.X)	return true;
		th_temp = th_temp->right;
	}
	return false;
}

/************************************************************************/
//��������void cellBground(SMALL_RECT rect)
//��������ֵ��void
//�������ܣ����Ʊ��հױ���
/************************************************************************/
void cellBground(SMALL_RECT rect,HANDLE hOutput,WORD wd)
{
	COORD CursorPosition ={ rect.Left,rect.Right};
	DWORD dwWritten;

	::FillConsoleOutputAttribute(hOutput, wd,
		6,CursorPosition,&dwWritten);
}


/************************************************************************/
//��������void fillBlack(HANDLE hOutput)
//��������ֵ��void
//�������ܣ���ָ������ˢ��
/************************************************************************/
void fillBlack(HANDLE hOutput,Table_head th)
{
	//ˢ������Ϊһ��
	if (pre_state == 1)
	{
		for (int j = 0;j < COL_MAX - 2;j++)
		{
			if (data_pos_exist(pre_cell,th))	display_cell(th,hOutput);
			else cellBground(pre_cell,hOutput,0);
			pre_cell.Left -= COL_SIZE;
		}
	}

	//ˢ������Ϊһ��
	if (pre_state == 2)
	{
		for (int i = 0;i < ROW_MAX - 2;i++)
		{
			if (data_pos_exist(pre_cell,th)) display_cell(th,hOutput);
			else cellBground(pre_cell,hOutput,0);
			pre_cell.Right -= ROW_SIZE;
		}
	}

	//ˢ������Ϊһ����Ԫ�� ����ʾ����
	if (pre_state == 3)	
	{
		if(data_pos_exist(pre_cell,th)) display_cell(th,hOutput);
		else cellBground(pre_cell,hOutput,0);
	}
}

/************************************************************************/
//��������void open_cursor()
//��������ֵ��void
//�������ܣ��򿪹��
/************************************************************************/
void open_cursor(HANDLE hOutput)
{
	CONSOLE_CURSOR_INFO cur;
	GetConsoleCursorInfo(hOutput,&cur);
	cur.bVisible=true;
	SetConsoleCursorInfo(hOutput,&cur);
}

/************************************************************************/
//��������void close_cursor()
//��������ֵ��void
//�������ܣ��رչ��
/************************************************************************/
void close_cursor(HANDLE hOutput)
{
	CONSOLE_CURSOR_INFO cur;
	GetConsoleCursorInfo(hOutput,&cur);
	cur.bVisible=false;
	SetConsoleCursorInfo(hOutput,&cur);
}


/************************************************************************/
//��������void left_event(HANDLE hOutput,SMALL_RECT sc)
//��������ֵ��void
//�������ܣ��������¼�
/************************************************************************/
void left_event(HANDLE hOutput,SMALL_RECT sc,Table_head Th,Button subEdit[INIT_SUB_NUM],Button subFile[INIT_FSUB_NUM])
{
	int edit_tag = 0;
	/**********************�������***************************/
	if (sc.Left < (INIT_TABLE_X + COL_SIZE) && sc.Left > INIT_TABLE_X 
		&& sc.Right > (INIT_TABLE_Y + ROW_SIZE) && sc.Right < (INIT_TABLE_Y + ROW_SIZE *(ROW_MAX-1)))
	{
		//��ǰ����ˢ��
		fillBlack(hOutput,Th);

		//��������ˢ�ɰ�ɫ
		for (int i = 1;i < COL_MAX - 1;i++)
		{
			sc.Left = ((sc.Left - INIT_TABLE_X)/COL_SIZE + 1) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
			sc.Right = ((sc.Right - INIT_TABLE_Y)/ROW_SIZE) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

			if(data_pos_exist(sc,Th))	display_cell(Th,hOutput);
			else cellBground(sc,hOutput);
		}

		//���¸�ֵǰ�����ֵ
		pre_state = 1;
		pre_cell.Left = sc.Left;
		pre_cell.Right = sc.Right;

		subEdit->button_life = false;
		subFile->button_life = false;
	}

	/**********************�������***************************/
	else if(sc.Left > (INIT_TABLE_X + COL_SIZE) && sc.Left < (INIT_TABLE_X + COL_SIZE * (COL_MAX-1))
		&& sc.Right > INIT_TABLE_Y  && sc.Right < (INIT_TABLE_Y + ROW_SIZE))
	{
		//��ǰ����ˢ��
		fillBlack(hOutput,Th);

		//��������ˢ�ɰ�ɫ
		for (int i = 1;i < ROW_MAX - 1;i++)
		{
			sc.Left = ((sc.Left - INIT_TABLE_X)/COL_SIZE ) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
			sc.Right = ((sc.Right - INIT_TABLE_Y)/ROW_SIZE+ 1) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

			if(data_pos_exist(sc,Th))	display_cell(Th,hOutput);
			else cellBground(sc,hOutput);
		}

		//���¸�ֵǰ�����ֵ
		pre_state = 2;
		pre_cell.Left = sc.Left;
		pre_cell.Right = sc.Right;

		subEdit->button_life = false;
		subFile->button_life = false;
	}

	/**********************�����Ԫ��***************************/
	else if (sc.Left > (INIT_TABLE_X + COL_SIZE) && sc.Left < (INIT_TABLE_X + COL_SIZE * (COL_MAX-1))
		&& sc.Right > (INIT_TABLE_Y + ROW_SIZE) && sc.Right < (INIT_TABLE_Y + ROW_SIZE *(ROW_MAX-1)))
	{
		//ˢ��
		fillBlack(hOutput,Th);

		/*΢����ʾ��������*/
		sc.Left = ((sc.Left - INIT_TABLE_X)/COL_SIZE) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
		sc.Right = ((sc.Right - INIT_TABLE_Y)/ROW_SIZE) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

		//��������ˢ�ɰ�ɫ
		if(data_pos_exist(sc,Th))	display_cell(Th,hOutput);
		else cellBground(sc,hOutput);

		//���¸�ֵǰ�����ֵ
		pre_state = 3;
		pre_cell.Left = sc.Left;
		pre_cell.Right = sc.Right;

		subEdit->button_life = false;
		subFile->button_life = false;
	}

	/**********************����༭�˵�***************************/
	else if(sc.Left >= INIT_EDIT_X && sc.Left <= (INIT_EDIT_X + BUTTON_WIDTH))
	{
		if(sc.Right == 0) 	subEdit->button_life = true;

		/////////////////////���//////////////////////////
		else if (sc.Right == 1 && subEdit->button_life)
		{
			//����һ�еĺ�
			if (pre_state == 1) 
				row_sum(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput);

			//����һ�еĺ�
			if(pre_state == 2)
				col_sum(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput);

			subEdit->button_life = false;
			subFile->button_life = false;
		}

		/////////////////////ƽ��//////////////////////////
		else if (sc.Right == 2 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_sum(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput,2);
			if (pre_state == 2)
				col_sum(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput,2);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		/////////////////////���//////////////////////////
		else if (sc.Right == 3 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_maxMin(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput);
			if (pre_state == 2)
				col_maxMin(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		/////////////////////��С//////////////////////////
		else if (sc.Right == 4 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_maxMin(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput,2);
			if (pre_state == 2)
				col_maxMin(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput,2);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		/////////////////////����//////////////////////////
		else if (sc.Right == 5 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_sort(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput);
			if (pre_state == 2) 
				col_sort(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		/////////////////////����//////////////////////////
		else if (sc.Right == 6 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_sort(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput,2);
			if (pre_state == 2) 
				col_sort(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput,2);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		else 
		{
			subEdit->button_life = false;
			subFile->button_life = false;
		}
	}


	////////////////////////////////////�ļ�/////////////////////////////////////
	else if(sc.Left >= INIT_FILE_X && sc.Left <= (INIT_FILE_X + BUTTON_WIDTH))
	{
		if(sc.Right == 0) subFile->button_life = true;
		//���ļ�
		else if(sc.Right == 1 && subFile->button_life)
		{
			readFile(Th);
			display_cell(Th,hOutput);

			subEdit->button_life = false;
			subFile->button_life = false;
		}

		//д�ļ�
		else if(sc.Right == 2 && subFile->button_life) 
		{
			writeFile(Th);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
	}
	else	
	{
		subEdit->button_life = false;
		subFile->button_life = false;
	}

	display_sub_button(hOutput,subEdit);
	display_subFile_button(hOutput,subFile);

}


/************************************************************************/
//��������void dealEvent(INPUT_RECORD buf)
//��������ֵ��void
//�������ܣ�����¼�
/************************************************************************/
void dealEvent(INPUT_RECORD buf,HANDLE hOutput,Table_head &Th,Button subEdit[INIT_SUB_NUM],Button subFile[INIT_FSUB_NUM])
{

	//ע�⺯����˳��DoubleClick��LeftButton������MouseMove��MouseDrag����
	if (buf.EventType == MOUSE_EVENT)
	{
		SMALL_RECT sc;
		sc.Left = buf.Event.MouseEvent.dwMousePosition.X ;
		sc.Right = buf.Event.MouseEvent.dwMousePosition.Y;
		int i;
		
		//�ƶ�
		if (buf.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
		{
			
		}

		//˫��
		else if (buf.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK
			&& buf.Event.MouseEvent.dwButtonState == VK_LBUTTON)
		{
			if (sc.Left > (INIT_TABLE_X + COL_SIZE) && sc.Left < (INIT_TABLE_X + COL_SIZE * (COL_MAX-1))
				&& sc.Right > (INIT_TABLE_Y + ROW_SIZE) && sc.Right < (INIT_TABLE_Y + ROW_SIZE *(ROW_MAX-1)))
			{
				//���ù������
				COORD curs;
				curs.X = ((sc.Left - INIT_TABLE_X)/COL_SIZE) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
				curs.Y = ((sc.Right - INIT_TABLE_Y)/ROW_SIZE) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

				//��¼��ǰλ��
				cell_pos = sc;

				/*�򿪹��*/
				open_cursor(hOutput);

				SetConsoleCursorPosition(hOutput,curs);
			}
		}

		//�������
		else if (buf.Event.MouseEvent.dwButtonState == VK_LBUTTON)
		{		
			for(i = 0;i < 6;i++) data_record[i] = ' ';

			/*����Ԫ�����ݳ�������*/
			input_len = 0;

			/*�رչ��*/
			close_cursor(hOutput);

			/*�Ϸ���������ʾ�հױ���*/
			left_event(hOutput,sc,Th,subEdit,subFile);

		}
		//�Ҽ�����
		else if (buf.Event.MouseEvent.dwButtonState == VK_RBUTTON)
		{
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//�����¼�//
	if (buf.EventType == KEY_EVENT)
	{
		//����Ƿ��
		CONSOLE_CURSOR_INFO cur;
		GetConsoleCursorInfo(hOutput,&cur);

		//������ַ�
		char charData = buf.Event.KeyEvent.uChar.AsciiChar;
		DWORD NumberOfCharsWritten;

		//���λ��
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		COORD pos = {cursorPos.x,cursorPos.y};

		//���̰����¼� && ���� && �ַ������ڷ�Χ֮��
		if(buf.Event.KeyEvent.bKeyDown && cur.bVisible && input_len < CELL_DATA_LEN){
			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

			/*��¼����*/
			int data_exist = 0;//�ж������λ�õ������Ƿ��Ѿ����ڵı�־
			COORD curent_pos = {(cell_pos.Left - INIT_TABLE_X)/COL_SIZE,(cell_pos.Right - INIT_TABLE_Y)/ROW_SIZE};//��¼��ǰλ��
			
			Table_line row_p =Th.table_row;//ָ�����е����ݱ��
			Table_line row_temp = Th.table_row;//ָ�����ݱ��
			Table_line row_pre = Th.table_row;

			/*�ж������Ƿ��Ѿ�����*/
			row_p = row_p[curent_pos.Y].right;
			while(row_p)
			{
				if(row_p->i == curent_pos.X) 
				{
					data_exist = 1;
					break;
				}
				row_p = row_p->right;
			}


			//�����ڵĻ��ٿ����ڴ�
			if(!data_exist)	
			{
				row_p = (Table_data*)malloc(sizeof(Table_data));
				row_p->data = (char *)malloc(CELL_DATA_LEN * sizeof(char));
				row_p->i = curent_pos.X;
				row_p->j = curent_pos.Y;
				row_p->right = NULL;

				row_temp = Th.table_row[curent_pos.Y].right;
				if (row_temp)
				{
					while(row_temp)	
					{
						row_pre = row_temp;
						row_temp = row_temp->right;
					}
					row_pre->right = row_p;
				}
				else		Th.table_row[curent_pos.Y].right = row_p;
				
			}

			/*�༭ɾ��*/
			if (buf.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
			{				
				COORD curs;//���λ��
				SMALL_RECT sc;//ˢ��λ��
				int i;

				sc.Left = ((cell_pos.Left - INIT_TABLE_X)/COL_SIZE) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
				sc.Right = ((cell_pos.Right - INIT_TABLE_Y)/ROW_SIZE) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;
				
				//����λ��ˢ��
				cellBground(sc,hOutput);
				curs.X = sc.Left;
				curs.Y = sc.Right;
				
				//���ù��λ��
				SetConsoleCursorPosition(hOutput,curs);

				int tmp;
				input_len = 0;//���볤��Ϊ0
				//�������
				for(i = 0;i < 6;i++) data_record[i] = ' ';
				row_p->data = "\0";
			}

			/*�༭����*/
			else{
				SetConsoleTextAttribute(output,FORGROUND_WHITE);//���������ɫ

				//��¼������ַ�
				char *output_cell = (char *)malloc(sizeof(char));
				*output_cell = charData;


				//�����洢
				row_p->data += charData;
				char * p = (char *)malloc((input_len+2)*sizeof(char));
				data_record[input_len] = charData;
				strcpy(p,data_record);
				row_p->data = p;

				WriteConsole(output,output_cell,1,&NumberOfCharsWritten,NULL);//��괦���
				free(output_cell);
				input_len++;
			}
		}
	}
}
