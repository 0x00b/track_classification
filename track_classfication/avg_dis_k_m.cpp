#include "stdafx.h"
#include "avg_dis_k_m.h"


double GetAvgDis_1(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt, int allstep)
{
	double dis = 0;

	int i = 0;
	bool flag;
	int cnt = 0;
	int s1p, s2p;
	double step;
	//计算两条曲线之间的评价距离
	//两条线都分为 allstep 步，计算相应步之间的距离
	for (;;)
	{
		flag = false;
		step = i / (double)allstep;
		s1p = t1cnt * step;
		s2p = t2cnt * step;

		if (s1p < t1cnt && s2p < t2cnt)
		{
			//求距离
			dis += sqrt(pow(t1[s1p].y - (double)t2[s2p].y, 2) + pow(t1[s1p].x - (double)t2[s2p].x, 2));
			cnt++;
			flag = true;
		}
		if (!flag)
		{
			//计算完成
			break;
		}
		i++;
	}
	//估算出两条直线的点之间的距离
	//	return (dis);
	return (dis / cnt);
}
double GetAvgDis(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt)
{
	double dis = 0;
	int s1 = 0, s2 = 0;
	double min_dis = pow(10, 10);

	//找到两条线中距离最小的坐标
	for (int i = 0; i < t1cnt; i++)
	{
		for (int j = 0; j < t2cnt; j++)
		{
			dis = sqrt(pow(t1[i].y - t2[j].y, 2) + pow(t1[i].x - t2[j].x, 2));
			if (dis < min_dis)
			{
				s1 = i;
				s2 = j;
				min_dis = dis;
			}
		}
	}
	///SelectObject(g_hdc, CreatePen(PS_SOLID, 2, 0xeeeeee));
	//MoveToEx(g_hdc, t1[s1].x, t1[s1].y, NULL);
	//LineTo(g_hdc, t2[s2].x, t2[s2].y);

	int i = 1;
	dis = 0;
	int s1p, s2p;
	bool flag = false;
	int cnt = 0;
	double step = 0;

	double rate1 = 0;
	double rate2 = 0;
	if (s1 > s2)
	{
		rate1 = (s1 + 1.) / (s2 + 1.);
		rate2 = (t1cnt - s1 + 1.) / (t2cnt - s2 + 1.);
	}
	else
	{
		rate1 = (s2 + 1.) / (s1 + 1.);
		rate2 = (t2cnt - s2 + 1.) / (t1cnt - s1 + 1.);
	}

	//从距离最小的地方开始向两头延伸，逐步计算距离
	for (;;)
	{
		step = i / (double)t1cnt;
		flag = false;

		s1p = s1 - step;
		s2p = s2 - step * rate1;

		if (s1p >= 0 && s2p >= 0)
		{
			dis += sqrt(pow(t1[s1p].y - (double)t2[s2p].y, 2) + pow(t1[s1p].x - (double)t2[s2p].x, 2));
			cnt++;
			flag = true;
		}
		s1p = s1 + step;
		s2p = s2 + step * rate2;

		if (s1p < t1cnt && s2p < t2cnt)
		{
			dis += sqrt(pow(t1[s1p].y - (double)t2[s2p].y, 2) + pow(t1[s1p].x - (double)t2[s2p].x, 2));
			cnt++;
			flag = true;
		}
		if (!flag)
		{
			break;
		}
		i++;
	}
	//估算出两条直线的点之间的距离
	//	return (dis);
	return (dis / cnt);
}

void GetAllDis(PosData& tracks, double alldis[GROUPCNT][GROUPCNT])
{
	int minsetp = tracks.ptcnt[0];
	for (int i = 1; i < tracks.groupcnt; i++)
	{
		if (minsetp >  tracks.ptcnt[i])
		{
			minsetp = tracks.ptcnt[i];
		}
	}
	for (size_t i = 0; i < tracks.groupcnt - 1; i++)
	{
		for (size_t j = i + 1; j < tracks.groupcnt; j++)
		{
			//获得每条线和其他线的估算距离
			//构造出一个图，这个图中每条线看作一个节点，每个节点和其他节点都是连通的，边上的权值为两线的平均距离
			//alldis[i][j] = GetAvgDis(tracks.pt[i], tracks.pt[j], tracks.ptcnt[i], tracks.ptcnt[j]);
			alldis[i][j] = GetAvgDis_1(tracks.pt[i], tracks.pt[j], tracks.ptcnt[i], tracks.ptcnt[j], minsetp);
			alldis[j][i] = alldis[i][j];
		}
	}
	/*
	FILE* fp = fopen("TrackData\\d.txt", "w");

	for (size_t i = 0; i < tracks.groupcnt; i++)
	{
		for (size_t j = 0; j < tracks.groupcnt; j++)
		{
			fprintf(fp, "[%d,%d]%-6.2lf ", i + 1, j + 1, alldis[i][j]);
		}
		fprintf(fp, "\n ");
	}
	fclose(fp);*/
}

int TrackClassfy(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k)
{
	//先计算所有线之间的距离
	GetAllDis(tracks, alldis);
	//假设有k种轨迹
	int cnt = 0;
	int cen;
	memset(cens, 0, sizeof(int)*GROUPCNT);

	//线随机出几种轨迹的中心线
	while (cnt < k)
	{
		srand((unsigned int)time(NULL) + INT_MAX%rand());
		cen = rand() % tracks.groupcnt;
		int i = 0;
		//检查没有随机到相同的位置
		for (; i < k; i++)
		{
			if (cen == cens[i])
			{
				break;
			}
		}
		if (i >= k)
		{
			cens[cnt++] = cen;//随机到的新的中心线
		}
	}
	//迭代10次
	for (size_t m = 0; m < 10; m++)
	{
		memset(classf, 0, sizeof(int)*GROUPCNT*GROUPCNT);
		for (int j = 0; j < tracks.groupcnt; j++)
		{
			int kind = 0;
			int min = alldis[cens[0]][j];
			for (int i = 1; i < k; i++)
			{
				//根据每条线到中心线的距离分类
				if (/*cens[i] != j &&*/ min > alldis[cens[i]][j])
				{
					min = alldis[cens[i]][j];
					kind = i;
				}
			}
			classf[cens[kind]][j] = 1;//重新把j归类
		}

		double dis[GROUPCNT] = { 0 };

		for (int i = 0; i < k; i++)
		{
			//计算每条线到组内其他线的距离，选最短的作为中心线
			memset(dis, 0, sizeof(dis));
			for (int j = 0; j < tracks.groupcnt; j++)
			{
				if (0 != classf[cens[i]][j])//不在组内就跳过
				{
					for (int n = 0; n < tracks.groupcnt; n++)
					{
						if (0 != classf[cens[i]][n])//不在组内就跳过
						{
							dis[j] += alldis[j][n];
						}
					}

				}
			}
			//重新确定中心新
			double min = pow(10, 10);
			for (int n = 0; n < tracks.groupcnt; n++)
			{
				if (dis[n] > 0.000005 && min > dis[n])//找最小距离的线
				{
					min = dis[n];	//保存最小距离
					cens[i] = n;	//确定为新的中心线
				}
			}
		}
	}

	return 0;
}
double GetAllGroupDis(int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int groupcnt, int k)
{
	double dis = 0;

	//求出所有组的到中心线的距离，并求和
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < groupcnt; j++)
		{
			if (classf[cens[i]][j])
			{
				dis += alldis[cens[i]][j];
			}
		}
	}

	return dis;
}
void TryBestClass(PosData& tracks, int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int k)
{
	int cens_t[GROUPCNT];
	int classf_t[GROUPCNT][GROUPCNT];

	TrackClassfy(tracks, cens, alldis, classf, k);

	double mindis = GetAllGroupDis(cens, alldis, classf, tracks.groupcnt, k);
	double tmindis;
	for (int i = 0; i < 10; i++)
	{
		//尝试10次，然后选出评价最好的一组
		TrackClassfy(tracks, cens_t, alldis, classf_t, k);

		tmindis = GetAllGroupDis(cens_t, alldis, classf_t, tracks.groupcnt, k);
		if (tmindis < mindis)
		{
			mindis = tmindis;
			memcpy(cens, cens_t, sizeof(int)*GROUPCNT);
			memcpy(classf, classf_t, sizeof(int)*GROUPCNT*GROUPCNT);
		}
	}
}

int ReadData(PosData& tracks, char* path)
{
	FILE* fp = fopen(path, "r");

	if (NULL == fp)
	{
		return 1;
	}
	char line[1024];
	char* posstr = line + 11;
	std::string pstr;
	int grouplen = 0;
	int ptcnt = 0;
	while (!feof(fp))
	{
		fscanf(fp, "%s\n", line);//每次读取一行
		char* result = NULL;
		char* str = NULL;
		char delms[] = "#";
		posstr = line + 11;
		str = strtok_s(posstr, delms, &result);
		ptcnt = 0;
		while (NULL != str && 0 != str[0])
		{
			pstr = str;

			size_t pos = pstr.find(",");
			str[pos] = 0;
			tracks.pt[grouplen][ptcnt].x = atoi(str);
			tracks.pt[grouplen][ptcnt].y = atoi(str + pos + 1);

			str = strtok_s(NULL, delms, &result);
			ptcnt++;
		}
		tracks.ptcnt[grouplen] = ptcnt;
		grouplen++;
	}
	tracks.groupcnt = grouplen;

	return 0;
}
