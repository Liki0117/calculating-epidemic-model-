#include<iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define N 16000										//N为样本总量
#define w 0.1										//w为传播病毒概率 
struct People
{
	int u;																			//u为病情参数，0=健康，1=潜伏期，2=发病期，3=死亡
	int t;																			//t为病情周期长度
};


//根据发病时间切换身体状态
int conditionSwitch(People p[], int i){												//已知潜伏期为6天，发病期为8天，周期共14天，健康状况定为-1
	if (p[i].t == -1)p[i].u = 0;
	if (0<=p[i].t&&p[i].t<6) p[i].u = 1;
	if (6<=p[i].t&&p[i].t<14) p[i].u = 2;
	if (p[i].t >= 14)p[i].u = 3;
	return p[i].u;
}


//统计各种身体状况的人数
int totalNumber(People p[], int u)
{
	int i;
	int number0 = 0, number1 = 0, number2 = 0, number3 = 0;
	for (i = 0; i < N; i++)
	{
		switch (p[i].u)
		{
		case  0:
			number0++; break;
		case  1:
			number1++; break;
		case  2:
			number2++; break;
		case  3:
			number3++; break;
		default:
			break;
		}
	}
	if (u == 0)return number0;
	if (u == 1)return number1;
	if (u == 2)return number2;
	if (u == 3)return number3;
}

int precautionDelivery(People p[], int med,int m){										   //由于免疫药只对健康人有效，故可等效成随机分给健康人														   //med为免疫药品的支数
	int i, j = 0;
	int b[med];
	int number = totalNumer(p, 0);
	int allo = number / med;
	for (i = 0; i < (number - med); i + allo){
		b[j] = i;
		j++;
	}
	return b[m];
}

int isolation(People p[], int iso,int m){
	int i, j = 0;
	int b[iso];
	int number = totalNumber(p, 2);
	int allo = number / iso;
	for (int i = 0; i < (number - iso); i + allo){
		b[j] = i;
		j++;
	}
	return b[m];
}
	
int cureDelivery(People p[], int cure, int m) {
	int i, j = 0;
	int p = 0;
	int q = 0;
	int b[cure];
	int live[cure/2];
	int death[cure/2];
	int number = totalNumber(p, 2);
	int allo = number / cure;
	for (int i = 0; i < (number - cure); i + allo){
		b[j] = i;
		j++;
	}
	for (i = 0; i < j / 2; i++){
		death[p] = i;
		p++;
	}
	for (i = j / 2; i < j; i++){
		live[q] = i;
		q++;
	}
	return death[m]
}








int secondEffect(People p[],int i,int parent,int p1){
	int m;
	if (p[i].u == 0){
		if (p[3 * i + 1].u == 0 && p[3 * i + 2].u == 0 && p[3 * i + 3].u == 0 
			|| p[3 * i + 1].u == 3 && p[3 * i + 2].u == 3 && p[3 * i + 3].u == 3
			|| p[3 * i + 1].u == 0 && p[3 * i + 2].u == 3 && p[3 * i + 3].u == 3
			|| p[3 * i + 1].u == 3 && p[3 * i + 2].u == 0 && p[3 * i + 3].u == 3
			|| p[3 * i + 1].u == 3 && p[3 * i + 2].u == 3 && p[3 * i + 3].u == 0
			|| p[3 * i + 1].u == 3 && p[3 * i + 2].u == 0 && p[3 * i + 3].u == 0
			|| p[3 * i + 1].u == 0 && p[3 * i + 2].u == 3 && p[3 * i + 3].u == 0
			|| p[3 * i + 1].u == 0 && p[3 * i + 2].u == 0 && p[3 * i + 3].u == 3){
			p[i].u = 0;
		}
		else
		{
			if (p1 > 1 - w)p[i].u = 1;
			else p[i].u = 0;
		}
			
	}
	return p[i].u;
}
void main()
{
	srand((int)time(0));
	People p[3*N];
	int i, e, day;
	//p[0].u = 2;
	p[0].u = 2;
	for (i = 1; i < 3*N; i++)
		p[i].u = 0;
    /***************    免疫的部分***********************/
	int med = 10;												//med之后更换数据，代表免疫药的支数，免疫药假设全部给了健康人，潜伏期的支数自动省略（前期计算好）
	/***************    隔离的部分 **********************/
	int iso = 20;		                                        //iso之后更换数据，代表隔离的人数
	/***************    治疗的部分 **********************/
	int cure=20;											    //cure之后更换数据，代表治疗药品的数量
	/****************************************************/
	for (int i = 0; i < N; i++){
		if (p[i].u == 0)p[i].t = -1;
		if (p[i].u == 1)p[i].t = 0;
		if (p[i].u == 2)p[i].t = 6;
		if (p[i].u == 3)p[i].t = 14;
	}
	int parent = N;
	for (day = 1; day < 5; day++){
		for (i = 0; i < parent; i++){
			float p1 = rand() % 100 / 100.0;														//p1为对于下一个节点的作用概率
			switch (p[i].u){
			case 0: 
				for (int m = 0; m < med; m++){
					if (i != precautionDelivery(p, med, m)){
						p[i].u = secondEffect(p, i, parent, p1);
						if (p[i].u == 1){
							p[i].t++;
							p[i].u = conditionSwitch(p, i);

					    }
     			    }	
					else p[i].u = -2;												               //u=-2  代表该节点被  免疫
				}
				break;
			case 1:
				if (p1>1 - w){																	   //p1符合此条件才会对下一个节点造成影响
					for (e = 1; e < 4; e++){
						p[i * 3 + e].t++;
						p[i * 3 + e].u = conditionSwitch(p, i * 3 + e);
					}
				}
				p[i].t++;
				p[i].u = conditionSwitch(p, i); break;
			case 2:
				for (int m = 0; m<iso; m++){
						if (i != isolationDelivery(p, med, m)){
							if (p1>1 - w){
								for (e = 1; e < 4; e++){
									p[i * 3 + e].t++;
									p[i * 3 + e].u = conditionSwitch(p, i * 3 + e);
								}
							}
							p[i].t++;
							p[i].u = conditionSwitch(p, i);
						}
						else{
							p[i].u = 3;
						}
				}
				 break;
			case 3:break;
			case -2:break;
			default:break;
			}
		}
	}



	int res0 = totalNumber(p, 0);
	int res1 = totalNumber(p, 1);
	int res2 = totalNumber(p, 2);
	int res3 = totalNumber(p, 3);
	cout << res0 << "    " << res1 << "   " << res2 << "   " << res3 << "   " << endl;

	system("pause");
}

