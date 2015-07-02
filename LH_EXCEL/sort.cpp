#include "sort.h"

/************************************************************************/
//函数名：void ShellInsert(float *ll,int dk,int length)
//函数返回值：void
//函数功能：一趟希尔升序排列
/************************************************************************/
void ShellInsert(float *ll,int dk,int length)
{
	int j;

    for (int i = dk + 1;i <= length;++i)
    {
		if(ll[i] < ll[i-dk])
		{
			ll[0] = ll[i];
			for (j = i-dk;j > 0 && ll[0] < ll[j];j -= dk)
				ll[j + dk] = ll[j];
			ll[j + dk] = ll[0];
		}
    }
}

/************************************************************************/
//函数名：void de_ShellInsert(float *ll,int dk,int length)
//函数返回值：void
//函数功能：一趟希尔降序排列
/************************************************************************/
void de_ShellInsert(float *ll,int dk,int length)
{
	int j;

	for (int i = dk + 1;i <= length;++i)
	{
		if(ll[i] > ll[i-dk])
		{
			ll[0] = ll[i];
			for (j = i-dk;j > 0 && ll[0] > ll[j];j -= dk)
				ll[j + dk] = ll[j];
			ll[j + dk] = ll[0];
		}
	}
}

/************************************************************************/
//函数名：void increase_Sort(float *ll, int dlta[],int t,int length)
//函数返回值：void
//函数功能：升序排列
/************************************************************************/
void increase_Sort(float *ll, int dlta[],int t,int length,int tag){
	for (int k = 0;k < t;++k)
	{
		if(tag == 1) ShellInsert(ll,dlta[k],length);
		if(tag == 2) de_ShellInsert(ll,dlta[k],length);
	}
}