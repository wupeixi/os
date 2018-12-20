#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

//进程控制块
typedef struct pcb
{
  int name;         //进程名
  int arrivetime;   //到达时间
  int needtime;     //所需时间
  int cputime;     	//已运行时间
  int pri;          //优先级(响应比)
  int endtime;      //完成时间
  char state;       //状态位,就绪 W（Wait）、运行R（Run）、或完成F（Finish）
}plist;

int globaltime = 0;	//cpu运行总时间

void creatPl(int n,plist * pl );
void show( int n,plist * pl);
void fcfs(int n,plist * pl);
void sjf(int n,plist * pl);
void rr(int n,plist * pl);
void hrrn(int n,plist * pl);

/*执行进程*/
void action(plist * nowpl)
{
	void show( int n,plist * pl);

	// Sleep(1);						//延迟1s
	globaltime++;               		//cpu运行总时间+1
	nowpl->cputime++;					//已运行时间+1
	if(nowpl->needtime<=nowpl->cputime)		
	{
		nowpl->endtime = globaltime;	//设置进程完成时间
		nowpl->state='F';				//设置进程状态为完成
		nowpl->pri = 0;					//设置进程优先级为0
	}else{
		nowpl->state='W';				//设置进程状态为就绪
	}
}
/*创建进程，数量为n*/
void  creatPl(int n,plist * pl)
{
   int j;
   srand(time(0));  					//设置随机种子
   for(j=0;j<n;j++)
   {
     pl[j].name= j;
     pl[j].needtime=rand()%100+1;
     pl[j].arrivetime=rand()%100;
     pl[j].pri=0;
     pl[j].endtime=-1;
     pl[j].cputime=0;
     pl[j].state='W';
   }
   pl[0].arrivetime=0;
}
/*显示进程名为j的进程的PCB*/
void showPCB(int j,plist * pl){
	printf("\t%d",pl[j].name);
	printf("\t%d",pl[j].pri);
	printf("\t%d",pl[j].arrivetime);
	printf("\t%d",pl[j].needtime);
	printf("\t  %d",pl[j].cputime);
	printf("\t    %c",pl[j].state);
	printf("\t  %d\n",pl[j].endtime);
}
/*显示各进程信息*/
void show( int n,plist * pl)
{
    int j;
    printf("*******************************时刻：%3d******************************\n",globaltime);
    printf("\t进程号 优先级 到达时间 所需时间 已运行时间 状态 完成时间\n");
	printf("----------------------------------------------------------------------\n");
    printf("运行\n");
    for (j=0 ;j<n;j++)
    {
        if(pl[j].state == 'R'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("就绪\n");
    for (j=0 ;j<n;j++)
    {
        if(pl[j].state == 'W'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("完成\n");
    for (j=0 ;j<n;j++)
    {
        if(pl[j].state == 'F'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
	printf("**********************************************************************\n\n");
}
/*计算平均周转时间*/
float *cal(int n, plist* pl){
	int i;
	float *res = (float *)malloc(2*sizeof(float));
	// float res[1];
	float Sumct = 0;	//周转时间
	float Sumwt = 0;	//带权周转时间
	
	for(i=0;i<n;i++){
		Sumct+=pl[i].endtime-pl[i].arrivetime;
		Sumwt+=(pl[i].endtime-pl[i].arrivetime)/pl[i].needtime;
	}
	res[0]=Sumct/n;
	res[1]=Sumwt/n;
	
	return res;
}

/*重置进程信息*/
void clrPro(int n, plist* pl){
	int i;
	for(i=0;i<n;i++){
		pl[i].endtime=-1;
		pl[i].cputime=-1;
		pl[i].state='W';
	}
}


/*短进程优先算法*/
void sjf(int n,plist * pl)
{
   void show( int n,plist * pl);
   int i,j;
   int count = n;
   plist temp;

   //按进程长度排序
   for(i=0;i<n;i++)
   {
     for(j=i+1;j<n;j++)
     {
        if(pl[i].needtime>pl[j].needtime)
       {
          temp=pl[j];
		  pl[j]=pl[i];
		  pl[i]=temp;
       }
     }
   }

   while(count>0)
   {
      count=n;

	  //查找当前已到达的最短未完成进程
	  for(i=0;i<n;i++){
        if(pl[i].arrivetime<=globaltime && pl[i].state!='F'){
			pl[i].state = 'R';		//设置进程状态为运行
			// show(n,pl);
            action(&pl[i]);			//处理当前进程
			break;
        }
	  }

	  //检查是否有未完成进程
	  for(i=0;i<n;i++){
		 if(pl[i].state=='F'){
		  count--;
		}
	  }
   }
   show(n,pl);
   
}
/*时间片轮转算法*/
void rr(int n,plist * pl)
{
  void show( int n,plist * pl);

   int i,j;
   int time;	//循环次数
   plist temp;
   
   //按到达时间排序
   for(i=0;i<n;i++)
   {
     for(j=i+1;j<n;j++)
     {
        if(pl[i].arrivetime>pl[j].arrivetime)
       {
          temp=pl[j];
		  pl[j]=pl[i];
		  pl[i]=temp;
       }
     }
   }
   
   time=pl[0].needtime+pl[0].arrivetime;
   for(i=0;i<n;i++)
   {
      if(time<pl[i].needtime+pl[i].arrivetime)
      {
         time=pl[i].needtime+pl[i].arrivetime;
      }
   }
   while(time>0)
   {
      for(i=0;i<n;i++)
      {
          if(pl[i].arrivetime<=globaltime && pl[i].state!='F')
          {
			pl[i].state = 'R';	//设置进程状态为运行
			// show(n,pl);
            action(&pl[i]);
          }
      }
      time--;
   }
   show(n,pl);
   
}
/*高响应比优先算法*/
void hrrn(int n,plist * pl){
	int i;
	int j;
	int count = n;
	int hrrPro = 0;		//记录最高响应比的进程名
	plist temp;
   
   //按到达时间排序
   for(i=0;i<n;i++)
   {
     for(j=i+1;j<n;j++)
     {
        if(pl[i].arrivetime>pl[j].arrivetime)
       {
          temp=pl[j];
		  pl[j]=pl[i];
		  pl[i]=temp;
       }
     }
   }
   
   while(count>0)
   {
      count=n;
		//计算所有已到达且未完成的进程的优先级，并查找优先级最高的进程
		for(i=0;i<n;i++){
			if(pl[i].arrivetime<=globaltime && pl[i].state!='F'){
				//响应比=响应时间/服务时间
				pl[i].pri = (globaltime-pl[i].arrivetime+pl[i].needtime)/pl[i].needtime;
				if(pl[hrrPro].pri<pl[i].pri){
					hrrPro=i;
				}
			}
			
		}
	  //调度查找到的进程至其完成
	  while(pl[hrrPro].state != 'F'){
			pl[i].state = 'R';				//设置进程状态为运行
			// show(n,pl);
			action(&pl[hrrPro]);			//处理当前进程
	  }

	  //检查是否有未完成进程
	  for(i=0;i<n;i++){
		 if(pl[i].state=='F'){
		  count--;
		}
	  }
   }
   show(n,pl);
   
}

int main()
{
	int n;   		//进程数
	plist pl[10];	//进程队列
	float * avgct[2];	//平均周转时间

	printf("please input the number of pl between 0 and 10\n");
	printf("number of pl: ");
	scanf("%d",&n);
	creatPl(n,pl);
	show(n,pl);
	
	printf("*********************************SJF**********************************\n");
	sjf(n,pl); 
	avgct[0] = cal(n,pl);
	printf("**********************************RR**********************************\n");
	globaltime=0;
	clrPro(n,pl);
	rr(n,pl);
	avgct[1] = cal(n,pl);
	printf("*********************************HRRN*********************************\n");
	globaltime=0;
	clrPro(n,pl);
	hrrn(n,pl);
	avgct[2] = cal(n,pl);
	printf("*********************************统计*********************************\n");
	printf("\t平均周转时间\t平均带权周转时间\n");
	printf("SJF\t %f\t %f\t\n",avgct[0][0],avgct[0][1]);
	printf("RR\t %f\t %f\t\n",avgct[1][0],avgct[1][1]);
	printf("HRRN\t %f\t %f\t\n",avgct[2][0],avgct[2][1]);
	
	printf("程序结束\n");
	getch();

   return 0;

}
