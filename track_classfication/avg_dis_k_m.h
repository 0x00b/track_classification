#ifndef _AVG_DIS_K_M_H_
#define _AVG_DIS_K_M_H_


#define GROUPCNT 20

struct PosData
{
	int groupcnt;			//曲线条数
	POINT pt[GROUPCNT][200];//每条曲线的点坐标
	int ptcnt[GROUPCNT];	//每条曲线的坐标数量
};

//
//  函数: ReadData(PosData& tracks, char* path)
//
//  目的:    从文件中读取曲线坐标。
//
//  tracks		- 曲线数据
//  path		- 路径
//
int ReadData(PosData& tracks, char* path);

//
//  函数: GetAllDis(PosData& tracks, double alldis[GROUPCNT][GROUPCNT])
//
//  目的:    获取所有曲线之间的评价距离。
//
//  tracks		- 曲线数据
//  alldis		- 所有曲线之间的评价距离()
//
void GetAllDis(PosData& tracks, double alldis[GROUPCNT][GROUPCNT]);

//
//  函数: GetAvgDis(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt)
//
//  目的:    获取两条曲线之间的评价距离。
//
//  t1		- 曲线1
//  t2		- 曲线2
//  t1cnt	- 曲线1的坐标数量
//  t2cnt	- 曲线2的坐标数量
//
double GetAvgDis(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt);

//
//  函数: GetAvgDis_1(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt)
//
//  目的:    获取两条曲线的评价距离。
//
//  t1		- 曲线1
//  t2		- 曲线2
//  t1cnt	- 曲线1的坐标数量
//  t2cnt	- 曲线2的坐标数量
//  step	- 步长
//
double GetAvgDis_1(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt, int step);

//
//  函数:  TrackClassfy(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k);
//
//  目的:    1、随机选取中心线，
//			 2、然后计算每条线到中心线的距离，把线归类到最近的中心线的一类中。
//			 3、重新计算新的分类的中心线
//			 4、进行23步迭代，直到中心线不变动，这里采用迭代一定次数的方法
//
//  tracks	- 曲线数据
//  cens	- 每组曲线的中心线
//  alldis	- 每条曲线之间的评价距离
//  classf	- 曲线分类数据
//  k		- k个分类
//
int TrackClassfy(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k);

//
//  函数:  TryBestClass(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k);

//
//  目的:    多次尝试分类，每次随机选择中心线，找出最优的组合。
//
//  tracks	- 曲线数据
//  cens	- 每组曲线的中心线
//  alldis	- 每条曲线之间的评价距离
//  classf	- 曲线分类数据
//  k		- k个分类
//
void TryBestClass(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k);


#endif // !_AVG_DIS_K_M_H_
