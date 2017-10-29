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
	//������������֮������۾���
	//�����߶���Ϊ allstep ����������Ӧ��֮��ľ���
	for (;;)
	{
		flag = false;
		step = i / (double)allstep;
		s1p = t1cnt * step;
		s2p = t2cnt * step;

		if (s1p < t1cnt && s2p < t2cnt)
		{
			//�����
			dis += sqrt(pow(t1[s1p].y - (double)t2[s2p].y, 2) + pow(t1[s1p].x - (double)t2[s2p].x, 2));
			cnt++;
			flag = true;
		}
		if (!flag)
		{
			//�������
			break;
		}
		i++;
	}
	//���������ֱ�ߵĵ�֮��ľ���
	//	return (dis);
	return (dis / cnt);
}
double GetAvgDis(POINT t1[200], POINT t2[200], int t1cnt, int t2cnt)
{
	double dis = 0;
	int s1 = 0, s2 = 0;
	double min_dis = pow(10, 10);

	//�ҵ��������о�����С������
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

	//�Ӿ�����С�ĵط���ʼ����ͷ���죬�𲽼������
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
	//���������ֱ�ߵĵ�֮��ľ���
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
			//���ÿ���ߺ������ߵĹ������
			//�����һ��ͼ�����ͼ��ÿ���߿���һ���ڵ㣬ÿ���ڵ�������ڵ㶼����ͨ�ģ����ϵ�ȨֵΪ���ߵ�ƽ������
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
	//�ȼ���������֮��ľ���
	GetAllDis(tracks, alldis);
	//������k�ֹ켣
	int cnt = 0;
	int cen;
	memset(cens, 0, sizeof(int)*GROUPCNT);

	//����������ֹ켣��������
	while (cnt < k)
	{
		srand((unsigned int)time(NULL) + INT_MAX%rand());
		cen = rand() % tracks.groupcnt;
		int i = 0;
		//���û���������ͬ��λ��
		for (; i < k; i++)
		{
			if (cen == cens[i])
			{
				break;
			}
		}
		if (i >= k)
		{
			cens[cnt++] = cen;//��������µ�������
		}
	}
	//����10��
	for (size_t m = 0; m < 10; m++)
	{
		memset(classf, 0, sizeof(int)*GROUPCNT*GROUPCNT);
		for (int j = 0; j < tracks.groupcnt; j++)
		{
			int kind = 0;
			int min = alldis[cens[0]][j];
			for (int i = 1; i < k; i++)
			{
				//����ÿ���ߵ������ߵľ������
				if (/*cens[i] != j &&*/ min > alldis[cens[i]][j])
				{
					min = alldis[cens[i]][j];
					kind = i;
				}
			}
			classf[cens[kind]][j] = 1;//���°�j����
		}

		double dis[GROUPCNT] = { 0 };

		for (int i = 0; i < k; i++)
		{
			//����ÿ���ߵ����������ߵľ��룬ѡ��̵���Ϊ������
			memset(dis, 0, sizeof(dis));
			for (int j = 0; j < tracks.groupcnt; j++)
			{
				if (0 != classf[cens[i]][j])//�������ھ�����
				{
					for (int n = 0; n < tracks.groupcnt; n++)
					{
						if (0 != classf[cens[i]][n])//�������ھ�����
						{
							dis[j] += alldis[j][n];
						}
					}

				}
			}
			//����ȷ��������
			double min = pow(10, 10);
			for (int n = 0; n < tracks.groupcnt; n++)
			{
				if (dis[n] > 0.000005 && min > dis[n])//����С�������
				{
					min = dis[n];	//������С����
					cens[i] = n;	//ȷ��Ϊ�µ�������
				}
			}
		}
	}

	return 0;
}
double GetAllGroupDis(int cens[GROUPCNT], double alldis[GROUPCNT][GROUPCNT], int classf[GROUPCNT][GROUPCNT], int groupcnt, int k)
{
	double dis = 0;

	//���������ĵ������ߵľ��룬�����
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
		//����10�Σ�Ȼ��ѡ��������õ�һ��
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
		fscanf(fp, "%s\n", line);//ÿ�ζ�ȡһ��
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
