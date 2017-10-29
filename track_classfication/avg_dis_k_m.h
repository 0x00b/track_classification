#ifndef _AVG_DIS_K_M_H_
#define _AVG_DIS_K_M_H_


#define GROUPCNT 20

struct PosData
{
	int groupcnt;			//��������
	POINT pt[GROUPCNT][200];//ÿ�����ߵĵ�����
	int ptcnt[GROUPCNT];	//ÿ�����ߵ���������
};

//
//  ����: ReadData(PosData& tracks, char* path)
//
//  Ŀ��:    ���ļ��ж�ȡ�������ꡣ
//
//  tracks		- ��������
//  path		- ·��
//
int ReadData(PosData& tracks, char* path);

//
//  ����: GetAllDis(PosData& tracks, double alldis[GROUPCNT][GROUPCNT])
//
//  Ŀ��:    ��ȡ��������֮������۾��롣
//
//  tracks		- ��������
//  alldis		- ��������֮������۾���()
//
void GetAllDis(PosData& tracks, double alldis[GROUPCNT][GROUPCNT]);

//
//  ����: GetAvgDis(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt)
//
//  Ŀ��:    ��ȡ��������֮������۾��롣
//
//  t1		- ����1
//  t2		- ����2
//  t1cnt	- ����1����������
//  t2cnt	- ����2����������
//
double GetAvgDis(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt);

//
//  ����: GetAvgDis_1(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt)
//
//  Ŀ��:    ��ȡ�������ߵ����۾��롣
//
//  t1		- ����1
//  t2		- ����2
//  t1cnt	- ����1����������
//  t2cnt	- ����2����������
//  step	- ����
//
double GetAvgDis_1(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt, int step);

//
//  ����:  TrackClassfy(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k);
//
//  Ŀ��:    1�����ѡȡ�����ߣ�
//			 2��Ȼ�����ÿ���ߵ������ߵľ��룬���߹��ൽ����������ߵ�һ���С�
//			 3�����¼����µķ����������
//			 4������23��������ֱ�������߲��䶯��������õ���һ�������ķ���
//
//  tracks	- ��������
//  cens	- ÿ�����ߵ�������
//  alldis	- ÿ������֮������۾���
//  classf	- ���߷�������
//  k		- k������
//
int TrackClassfy(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k);

//
//  ����:  TryBestClass(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k);

//
//  Ŀ��:    ��γ��Է��࣬ÿ�����ѡ�������ߣ��ҳ����ŵ���ϡ�
//
//  tracks	- ��������
//  cens	- ÿ�����ߵ�������
//  alldis	- ÿ������֮������۾���
//  classf	- ���߷�������
//  k		- k������
//
void TryBestClass(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k);


#endif // !_AVG_DIS_K_M_H_
