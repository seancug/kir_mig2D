/** Copyright (C) 2015, Qingdao Institute of Marine Geology    **
**  All right reserved                                         **
**  2D Kirchhoff post time migtation                           **
**  Current Version:Debug 1.0                                  **
**  Author:Sun BO(Sean)                                        **
**  Finish Date:2015.12                                        **
**  For More Information,please contact sunbo@cgs.cn           ** 
**
**��ά�����������ʱ��ƫ��
**���ߣ��ﲨ     �ൺ��������о���
**����:sunbo@cgs.cn
**/

#include "stdafx.h"
#include "kir.h"


int main(int agrc, char *agrv[])
{
	float **arryin = NULL;   //�����������
	float **arryvel = NULL;  //�������ٶ�
	float dt;         //ʱ����
	float dx;         //�����
	float aper;       //ƫ�ƿ׾���aperture
	float width_aper;      //�׾�ƫ��cos�߿��
	float ang_lim;    //�Ƕ�����
	float width_ang;  //�Ƕ�cos��
	int nx;     //�������
	int nt;     //��������
	float *x = NULL;
	float *t = NULL;
	float *para = NULL;
	int *size = NULL;
	para = vector(1, 6);
	size = ivector(1, 2);

	kir_init(arryin, arryvel, t, x, size, para);
	
	nt = size[1];	nx = size[2];	dt = para[1];	
	dx = para[2];	aper = para[3];	width_aper = para[4];	
	ang_lim = para[5];	width_ang = para[6];
	
	free_vector(para, 1, 6);
	free_ivector(size, 1, 2);

	system("pause");
    return 0;
}

