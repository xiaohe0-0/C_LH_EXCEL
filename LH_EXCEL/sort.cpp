#include "sort.h"

/************************************************************************/
//��������void ShellInsert(float *ll,int dk,int length)
//��������ֵ��void
//�������ܣ�һ��ϣ����������
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
//��������void de_ShellInsert(float *ll,int dk,int length)
//��������ֵ��void
//�������ܣ�һ��ϣ����������
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
//��������void increase_Sort(float *ll, int dlta[],int t,int length)
//��������ֵ��void
//�������ܣ���������
/************************************************************************/
void increase_Sort(float *ll, int dlta[],int t,int length,int tag){
	for (int k = 0;k < t;++k)
	{
		if(tag == 1) ShellInsert(ll,dlta[k],length);
		if(tag == 2) de_ShellInsert(ll,dlta[k],length);
	}
}