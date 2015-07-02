#ifndef _DATA_H_
#define _DATA_H_

/**************基本参数***************/
#define CELL_DATA_LEN 6//单元格内数据长度
#define FORGROUND_WHITE 0X000f//白色
#define INIT_TABLE_X 10 //表格的位置X
#define INIT_TABLE_Y 8  //表格的位置Y
#define ROW_MAX 10	//初始化表格的行数
#define COL_MAX 11	//初始化表格的列数
#define ROW_SIZE 2	//初始化表格的行高
#define COL_SIZE 8	//初始化表格的列宽
#define ROW_CHANGE 0.5	//绘制表格时补行高空白的基本单位
#define COL_CHANGE 0.5	//绘制表格时补列宽空白的基本单位

#define INIT_ROW_NUM /*ROW_MAX*/10
#define INIT_COL_NUM /*COL_MAX*/11

/*表格元素*/
typedef struct table_data
{
	int i,j;//坐标
	char *data;//数据
	struct table_data *right;//右
	//struct table_data *down;//下
}Table_data,*Table_line;

/*行、列*/
typedef struct{
	Table_line table_row;
	int row_num,col_num;//行列个数
}Table_head;

void initData(Table_head &Tdata,HANDLE hOutput);
void display_cell(Table_head &Tdata,HANDLE hOutput);

#endif