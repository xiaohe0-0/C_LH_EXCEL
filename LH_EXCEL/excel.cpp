#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "data.h"
#include "table.h"
#include "button.h"
int main()
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	screenSize();//���ô��ڴ�С
	display(hOutput);//��ʾ���
	display_mainButton(hOutput);//��ʾ�˵�

	/*��ʼ���������*/
	Table_head Th;
	initData(Th,hOutput);

	/*��ʼ�����ز˵�*/
	Button edit[INIT_SUB_NUM];
	edit->button_life = false;
	init_sub_edit(edit);
	Button file[INIT_FSUB_NUM];
	file->button_life = false;
	init_sub_file(file);

	/*����*/
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	INPUT_RECORD Buf;
	DWORD rec;
	SetConsoleMode(hInput, fdwMode);	//����ģʽ	

	while (true)
	{
		ReadConsoleInput(hInput,&Buf,1,&rec);
		dealEvent(Buf,hOutput,Th,edit,file);
	}
	CloseHandle(hInput);
	CloseHandle(hOutput);
	return 0;
}