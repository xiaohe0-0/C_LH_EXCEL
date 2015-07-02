#ifndef _DATA_H_
#define _DATA_H_

/**************��������***************/
#define CELL_DATA_LEN 6//��Ԫ�������ݳ���
#define FORGROUND_WHITE 0X000f//��ɫ
#define INIT_TABLE_X 10 //����λ��X
#define INIT_TABLE_Y 8  //����λ��Y
#define ROW_MAX 10	//��ʼ����������
#define COL_MAX 11	//��ʼ����������
#define ROW_SIZE 2	//��ʼ�������и�
#define COL_SIZE 8	//��ʼ�������п�
#define ROW_CHANGE 0.5	//���Ʊ��ʱ���и߿հ׵Ļ�����λ
#define COL_CHANGE 0.5	//���Ʊ��ʱ���п�հ׵Ļ�����λ

#define INIT_ROW_NUM /*ROW_MAX*/10
#define INIT_COL_NUM /*COL_MAX*/11

/*���Ԫ��*/
typedef struct table_data
{
	int i,j;//����
	char *data;//����
	struct table_data *right;//��
	//struct table_data *down;//��
}Table_data,*Table_line;

/*�С���*/
typedef struct{
	Table_line table_row;
	int row_num,col_num;//���и���
}Table_head;

void initData(Table_head &Tdata,HANDLE hOutput);
void display_cell(Table_head &Tdata,HANDLE hOutput);

#endif