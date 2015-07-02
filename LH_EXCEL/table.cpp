#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "button.h"

/*记录鼠标事件之前的动作*/
static SMALL_RECT pre_cell = {0,0,0,0};
static int pre_state = 0;

//单元格中输入长度
static int input_len = 0;

//单元格输入时的位置
SMALL_RECT cell_pos;

//保存已经输入的数据
static char data_record[6];

/************************************************************************/
//函数名：void screenSize()
//函数返回值：void
//函数功能：设置窗口的大小
/************************************************************************/
void screenSize()
{
	system ( "mode con:lines=35 " ); 
	system ( "mode con:cols=110 " );
}


/************************************************************************/
//函数名：void outputData(COORD coord,LPCWSTR data)
//函数返回值：void
//函数功能：输出信息
/************************************************************************/
void outputData(COORD coord,char* data,DWORD length,HANDLE hOutput)
{
	DWORD NumberOfCharsWritten;

	WriteConsoleOutputCharacter(hOutput, data, length,	
		coord,&NumberOfCharsWritten);
}

/************************************************************************/
//函数名：void setTable(Table &T)
//函数返回值：void
//函数功能：设置表格的基本信息
/************************************************************************/
void setTable(Table &T)
{
	/*表格的开始坐标*/
	T.table_coord.x = INIT_TABLE_X;
	T.table_coord.y = INIT_TABLE_Y;

	/*表格的组成*/
	int i,j;
	for (i = 0;i < ROW_MAX;i++)
	{
		for (j = 0;j < COL_MAX;j++)
		{
			/*关键处的制表符*/
			if (i == 0)//第一行
			{
				if(j == 0) T.table_char[i][j] = "┏";
				else if(j == COL_MAX - 1) T.table_char[i][j] = "┓";
				else T.table_char[i][j] = "┳";
			}
			else if(i == ROW_MAX -1)//最后一行
			{
				if (j == 0)	T.table_char[i][j] = "┗";
				else if(j == COL_MAX - 1)T.table_char[i][j] = "┛";
				else T.table_char[i][j] = "┻";
			}
			else if (j == 0)	T.table_char[i][j] = "┣";//第一列
			else if(j == COL_MAX - 1) T.table_char[i][j] = "┫";//最后一列
			else T.table_char[i][j] = "╋";//中间的
		}
	}
}



/************************************************************************/
//函数名：void display()
//函数返回值：void
//函数功能：显示表格
/************************************************************************/
void display(HANDLE hOutput)
{
	/************************显示表格***************************/
	Table T;
	setTable(T);

	DWORD NumberOfCharsWritten;

	/*表格中的空白边框的填充*/
	char * map_row = "━━";
	char* map_col = "┃";

	int row,col;
	for (row = 0;row < ROW_MAX;row++)
	{
		for (col = 0;col < COL_MAX;col++)
		{
			COORD Cur;
			Cur.X = T.table_coord.x + col * COL_SIZE;
			Cur.Y = T.table_coord.y + row * ROW_SIZE;

			/*大体框架*/
			outputData(Cur,T.table_char[row][col],2,hOutput);

			/*补充空白边框*/
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
//函数名：bool data_pos_exist(SMALL_RECT pos,Table_head th)
//函数返回值：void
//函数功能：某一位置的数据是否存在
/************************************************************************/
bool data_pos_exist(SMALL_RECT pos,Table_head th)
{
	COORD cur_cell;
	cur_cell.X = (pos.Left - INIT_TABLE_X)/COL_SIZE;//X列
	cur_cell.Y = (pos.Right - INIT_TABLE_Y)/ROW_SIZE; //Y行

	Table_line th_temp = th.table_row[cur_cell.Y].right;
	while(th_temp)
	{
		if(th_temp->i == cur_cell.X)	return true;
		th_temp = th_temp->right;
	}
	return false;
}

/************************************************************************/
//函数名：void cellBground(SMALL_RECT rect)
//函数返回值：void
//函数功能：绘制表格空白背景
/************************************************************************/
void cellBground(SMALL_RECT rect,HANDLE hOutput,WORD wd)
{
	COORD CursorPosition ={ rect.Left,rect.Right};
	DWORD dwWritten;

	::FillConsoleOutputAttribute(hOutput, wd,
		6,CursorPosition,&dwWritten);
}


/************************************************************************/
//函数名：void fillBlack(HANDLE hOutput)
//函数返回值：void
//函数功能：将指定区域刷黑
/************************************************************************/
void fillBlack(HANDLE hOutput,Table_head th)
{
	//刷黑区域为一行
	if (pre_state == 1)
	{
		for (int j = 0;j < COL_MAX - 2;j++)
		{
			if (data_pos_exist(pre_cell,th))	display_cell(th,hOutput);
			else cellBground(pre_cell,hOutput,0);
			pre_cell.Left -= COL_SIZE;
		}
	}

	//刷黑区域为一列
	if (pre_state == 2)
	{
		for (int i = 0;i < ROW_MAX - 2;i++)
		{
			if (data_pos_exist(pre_cell,th)) display_cell(th,hOutput);
			else cellBground(pre_cell,hOutput,0);
			pre_cell.Right -= ROW_SIZE;
		}
	}

	//刷黑区域为一个单元格 或显示数据
	if (pre_state == 3)	
	{
		if(data_pos_exist(pre_cell,th)) display_cell(th,hOutput);
		else cellBground(pre_cell,hOutput,0);
	}
}

/************************************************************************/
//函数名：void open_cursor()
//函数返回值：void
//函数功能：打开光标
/************************************************************************/
void open_cursor(HANDLE hOutput)
{
	CONSOLE_CURSOR_INFO cur;
	GetConsoleCursorInfo(hOutput,&cur);
	cur.bVisible=true;
	SetConsoleCursorInfo(hOutput,&cur);
}

/************************************************************************/
//函数名：void close_cursor()
//函数返回值：void
//函数功能：关闭光标
/************************************************************************/
void close_cursor(HANDLE hOutput)
{
	CONSOLE_CURSOR_INFO cur;
	GetConsoleCursorInfo(hOutput,&cur);
	cur.bVisible=false;
	SetConsoleCursorInfo(hOutput,&cur);
}


/************************************************************************/
//函数名：void left_event(HANDLE hOutput,SMALL_RECT sc)
//函数返回值：void
//函数功能：鼠标左键事件
/************************************************************************/
void left_event(HANDLE hOutput,SMALL_RECT sc,Table_head Th,Button subEdit[INIT_SUB_NUM],Button subFile[INIT_FSUB_NUM])
{
	int edit_tag = 0;
	/**********************点击行首***************************/
	if (sc.Left < (INIT_TABLE_X + COL_SIZE) && sc.Left > INIT_TABLE_X 
		&& sc.Right > (INIT_TABLE_Y + ROW_SIZE) && sc.Right < (INIT_TABLE_Y + ROW_SIZE *(ROW_MAX-1)))
	{
		//将前区域刷黑
		fillBlack(hOutput,Th);

		//将新区域刷成白色
		for (int i = 1;i < COL_MAX - 1;i++)
		{
			sc.Left = ((sc.Left - INIT_TABLE_X)/COL_SIZE + 1) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
			sc.Right = ((sc.Right - INIT_TABLE_Y)/ROW_SIZE) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

			if(data_pos_exist(sc,Th))	display_cell(Th,hOutput);
			else cellBground(sc,hOutput);
		}

		//重新赋值前区域的值
		pre_state = 1;
		pre_cell.Left = sc.Left;
		pre_cell.Right = sc.Right;

		subEdit->button_life = false;
		subFile->button_life = false;
	}

	/**********************点击列首***************************/
	else if(sc.Left > (INIT_TABLE_X + COL_SIZE) && sc.Left < (INIT_TABLE_X + COL_SIZE * (COL_MAX-1))
		&& sc.Right > INIT_TABLE_Y  && sc.Right < (INIT_TABLE_Y + ROW_SIZE))
	{
		//将前区域刷黑
		fillBlack(hOutput,Th);

		//将新区域刷成白色
		for (int i = 1;i < ROW_MAX - 1;i++)
		{
			sc.Left = ((sc.Left - INIT_TABLE_X)/COL_SIZE ) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
			sc.Right = ((sc.Right - INIT_TABLE_Y)/ROW_SIZE+ 1) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

			if(data_pos_exist(sc,Th))	display_cell(Th,hOutput);
			else cellBground(sc,hOutput);
		}

		//重新赋值前区域的值
		pre_state = 2;
		pre_cell.Left = sc.Left;
		pre_cell.Right = sc.Right;

		subEdit->button_life = false;
		subFile->button_life = false;
	}

	/**********************点击单元格***************************/
	else if (sc.Left > (INIT_TABLE_X + COL_SIZE) && sc.Left < (INIT_TABLE_X + COL_SIZE * (COL_MAX-1))
		&& sc.Right > (INIT_TABLE_Y + ROW_SIZE) && sc.Right < (INIT_TABLE_Y + ROW_SIZE *(ROW_MAX-1)))
	{
		//刷黑
		fillBlack(hOutput,Th);

		/*微调显示区域坐标*/
		sc.Left = ((sc.Left - INIT_TABLE_X)/COL_SIZE) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
		sc.Right = ((sc.Right - INIT_TABLE_Y)/ROW_SIZE) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

		//将新区域刷成白色
		if(data_pos_exist(sc,Th))	display_cell(Th,hOutput);
		else cellBground(sc,hOutput);

		//重新赋值前区域的值
		pre_state = 3;
		pre_cell.Left = sc.Left;
		pre_cell.Right = sc.Right;

		subEdit->button_life = false;
		subFile->button_life = false;
	}

	/**********************点击编辑菜单***************************/
	else if(sc.Left >= INIT_EDIT_X && sc.Left <= (INIT_EDIT_X + BUTTON_WIDTH))
	{
		if(sc.Right == 0) 	subEdit->button_life = true;

		/////////////////////求和//////////////////////////
		else if (sc.Right == 1 && subEdit->button_life)
		{
			//计算一行的和
			if (pre_state == 1) 
				row_sum(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput);

			//计算一列的和
			if(pre_state == 2)
				col_sum(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput);

			subEdit->button_life = false;
			subFile->button_life = false;
		}

		/////////////////////平均//////////////////////////
		else if (sc.Right == 2 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_sum(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput,2);
			if (pre_state == 2)
				col_sum(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput,2);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		/////////////////////最大//////////////////////////
		else if (sc.Right == 3 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_maxMin(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput);
			if (pre_state == 2)
				col_maxMin(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		/////////////////////最小//////////////////////////
		else if (sc.Right == 4 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_maxMin(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput,2);
			if (pre_state == 2)
				col_maxMin(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput,2);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		/////////////////////升序//////////////////////////
		else if (sc.Right == 5 && subEdit->button_life)
		{
			if (pre_state == 1) 
				row_sort(Th,(pre_cell.Right- INIT_TABLE_Y)/ROW_SIZE,hOutput);
			if (pre_state == 2) 
				col_sort(Th,(pre_cell.Left- INIT_TABLE_X)/COL_SIZE,hOutput);
			subEdit->button_life = false;
			subFile->button_life = false;
		}
		/////////////////////降序//////////////////////////
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


	////////////////////////////////////文件/////////////////////////////////////
	else if(sc.Left >= INIT_FILE_X && sc.Left <= (INIT_FILE_X + BUTTON_WIDTH))
	{
		if(sc.Right == 0) subFile->button_life = true;
		//读文件
		else if(sc.Right == 1 && subFile->button_life)
		{
			readFile(Th);
			display_cell(Th,hOutput);

			subEdit->button_life = false;
			subFile->button_life = false;
		}

		//写文件
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
//函数名：void dealEvent(INPUT_RECORD buf)
//函数返回值：void
//函数功能：鼠标事件
/************************************************************************/
void dealEvent(INPUT_RECORD buf,HANDLE hOutput,Table_head &Th,Button subEdit[INIT_SUB_NUM],Button subFile[INIT_FSUB_NUM])
{

	//注意函数的顺序：DoubleClick与LeftButton竞争，MouseMove与MouseDrag竞争
	if (buf.EventType == MOUSE_EVENT)
	{
		SMALL_RECT sc;
		sc.Left = buf.Event.MouseEvent.dwMousePosition.X ;
		sc.Right = buf.Event.MouseEvent.dwMousePosition.Y;
		int i;
		
		//移动
		if (buf.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
		{
			
		}

		//双击
		else if (buf.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK
			&& buf.Event.MouseEvent.dwButtonState == VK_LBUTTON)
		{
			if (sc.Left > (INIT_TABLE_X + COL_SIZE) && sc.Left < (INIT_TABLE_X + COL_SIZE * (COL_MAX-1))
				&& sc.Right > (INIT_TABLE_Y + ROW_SIZE) && sc.Right < (INIT_TABLE_Y + ROW_SIZE *(ROW_MAX-1)))
			{
				//设置光标坐标
				COORD curs;
				curs.X = ((sc.Left - INIT_TABLE_X)/COL_SIZE) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
				curs.Y = ((sc.Right - INIT_TABLE_Y)/ROW_SIZE) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;

				//记录当前位置
				cell_pos = sc;

				/*打开光标*/
				open_cursor(hOutput);

				SetConsoleCursorPosition(hOutput,curs);
			}
		}

		//左键单击
		else if (buf.Event.MouseEvent.dwButtonState == VK_LBUTTON)
		{		
			for(i = 0;i < 6;i++) data_record[i] = ' ';

			/*将单元格数据长度清零*/
			input_len = 0;

			/*关闭光标*/
			close_cursor(hOutput);

			/*合法区域内显示空白背景*/
			left_event(hOutput,sc,Th,subEdit,subFile);

		}
		//右键单击
		else if (buf.Event.MouseEvent.dwButtonState == VK_RBUTTON)
		{
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//键盘事件//
	if (buf.EventType == KEY_EVENT)
	{
		//光标是否打开
		CONSOLE_CURSOR_INFO cur;
		GetConsoleCursorInfo(hOutput,&cur);

		//输入的字符
		char charData = buf.Event.KeyEvent.uChar.AsciiChar;
		DWORD NumberOfCharsWritten;

		//光标位置
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		COORD pos = {cursorPos.x,cursorPos.y};

		//键盘按下事件 && 光标打开 && 字符长度在范围之内
		if(buf.Event.KeyEvent.bKeyDown && cur.bVisible && input_len < CELL_DATA_LEN){
			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

			/*记录数据*/
			int data_exist = 0;//判断输入的位置的数据是否已经存在的标志
			COORD curent_pos = {(cell_pos.Left - INIT_TABLE_X)/COL_SIZE,(cell_pos.Right - INIT_TABLE_Y)/ROW_SIZE};//记录当前位置
			
			Table_line row_p =Th.table_row;//指向已有的数据表格
			Table_line row_temp = Th.table_row;//指向数据表格
			Table_line row_pre = Th.table_row;

			/*判断数据是否已经存在*/
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


			//不存在的话再开辟内存
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

			/*编辑删除*/
			if (buf.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
			{				
				COORD curs;//光标位置
				SMALL_RECT sc;//刷白位置
				int i;

				sc.Left = ((cell_pos.Left - INIT_TABLE_X)/COL_SIZE) * COL_SIZE + INIT_TABLE_X + ROW_CHANGE*4;
				sc.Right = ((cell_pos.Right - INIT_TABLE_Y)/ROW_SIZE) * ROW_SIZE + INIT_TABLE_Y + COL_CHANGE*2;
				
				//将该位置刷白
				cellBground(sc,hOutput);
				curs.X = sc.Left;
				curs.Y = sc.Right;
				
				//设置光标位置
				SetConsoleCursorPosition(hOutput,curs);

				int tmp;
				input_len = 0;//输入长度为0
				//清空输入
				for(i = 0;i < 6;i++) data_record[i] = ' ';
				row_p->data = "\0";
			}

			/*编辑输入*/
			else{
				SetConsoleTextAttribute(output,FORGROUND_WHITE);//输出字体颜色

				//记录输入的字符
				char *output_cell = (char *)malloc(sizeof(char));
				*output_cell = charData;


				//辅助存储
				row_p->data += charData;
				char * p = (char *)malloc((input_len+2)*sizeof(char));
				data_record[input_len] = charData;
				strcpy(p,data_record);
				row_p->data = p;

				WriteConsole(output,output_cell,1,&NumberOfCharsWritten,NULL);//光标处输出
				free(output_cell);
				input_len++;
			}
		}
	}
}
