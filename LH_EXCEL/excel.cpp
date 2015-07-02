#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "data.h"
#include "table.h"
#include "button.h"
int main()
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	screenSize();//设置窗口大小
	display(hOutput);//显示表格
	display_mainButton(hOutput);//显示菜单

	/*初始化表格数据*/
	Table_head Th;
	initData(Th,hOutput);

	/*初始化隐藏菜单*/
	Button edit[INIT_SUB_NUM];
	edit->button_life = false;
	init_sub_edit(edit);
	Button file[INIT_FSUB_NUM];
	file->button_life = false;
	init_sub_file(file);

	/*输入*/
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	INPUT_RECORD Buf;
	DWORD rec;
	SetConsoleMode(hInput, fdwMode);	//输入模式	

	while (true)
	{
		ReadConsoleInput(hInput,&Buf,1,&rec);
		dealEvent(Buf,hOutput,Th,edit,file);
	}
	CloseHandle(hInput);
	CloseHandle(hOutput);
	return 0;
}