#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define false 0
#define true !false

#define PROCESSES_NUMBER 5	//系统中所有进程数量

typedef struct {
	int	A;
	int	B;
	int	C;
}RESOURCE;

//最大需求矩阵
RESOURCE Max[PROCESSES_NUMBER];

//已分配资源数矩阵
RESOURCE Allocation[PROCESSES_NUMBER];

//需求矩阵
RESOURCE Need[PROCESSES_NUMBER];

//可用资源向量,A（10个）、B（15个）、C（12个）
RESOURCE Available = {10,15,12};

//安全序列
// int safe[PROCESSES_NUMBER];

//PCB
typedef struct pcb{
	int name;				//进程名
	RESOURCE pMax;			//需要的资源总数
	RESOURCE pAllocation;	//已分配的资源数
	char state;				//进程状态,就绪 W（Wait）、运行R（Run）、阻塞B（Block）或完成F（Finish）
}plist;

void ProbeAlloc(plist* proP,RESOURCE *res);
void RollBack(plist* proP,RESOURCE *res);
bool request(plist* proP,RESOURCE *res);
bool SafeCheck();
void creatPl(int n,plist * pl);
void show( int n,plist * pl);
void showPCB(int j,plist * pl);
void release(plist* proP);


//释放资源
void release(plist* proP){
	int process = proP->name;
	
	Available.A += proP->pAllocation.A;
	Available.B += proP->pAllocation.B;
	Available.C += proP->pAllocation.C;

	Allocation[process].A -= proP->pAllocation.A;
	Allocation[process].B -= proP->pAllocation.B;
	Allocation[process].C -= proP->pAllocation.C;
	
	proP->pAllocation.A = 0;
	proP->pAllocation.B = 0;
	proP->pAllocation.C = 0;
	
	Need[process].A += 0;
	Need[process].B += 0;
	Need[process].C += 0;
	
}
//试探分配
void ProbeAlloc(plist* proP,RESOURCE *res)
{
	int process = proP->name;
	
	Available.A -= res->A;
	Available.B -= res->B;
	Available.C -= res->C;
	
	Allocation[process].A += res->A;
	Allocation[process].B += res->B;
	Allocation[process].C += res->C;
	
	proP->pAllocation.A = Allocation[process].A;
	proP->pAllocation.B = Allocation[process].B;
	proP->pAllocation.C = Allocation[process].C;

	Need[process].A -= res->A;
	Need[process].B -= res->B;
	Need[process].C -= res->C;
	// Need[process].A = Max[process].A - Allocation[process].A;
	// Need[process].B = Max[process].B - Allocation[process].B;
	// Need[process].C = Max[process].C - Allocation[process].C;
}
//若试探分配后进入不安全状态，将分配回滚
void RollBack(plist* proP,RESOURCE *res)
{
	int process = proP->name;
	
	Available.A += res->A;
	Available.B += res->B;
	Available.C += res->C;

	Allocation[process].A -= res->A;
	Allocation[process].B -= res->B;
	Allocation[process].C -= res->C;
	
	proP->pAllocation.A = Allocation[process].A;
	proP->pAllocation.B = Allocation[process].B;
	proP->pAllocation.C = Allocation[process].C;
	
	Need[process].A += res->A;
	Need[process].B += res->B;
	Need[process].C += res->C;
}
//资源分配请求
bool request(plist* proP,RESOURCE *res){
	int process = proP->name;
	//request向量需小于Need矩阵中对应的向量
	if(res->A <= Need[process].A && res->B <= Need[process].B && res->C <= Need[process].C){
		//request向量需小于Available向量
		if(res->A <= Available.A && res->B <= Available.B && res->C <= Available.C){
			//试探分配
			ProbeAlloc(proP,res);

			//如果安全检查成立，则请求成功，否则将分配回滚并返回失败
			if(SafeCheck()){
				return true;
			}else{
				printf("资源请求失败。原因：系统将进入不安全状态，有可能引起死锁。\n");
				// printf("正在回滚...\n");
				RollBack(proP,res);
				proP->state = 'W';
			}
		}else{
			// printf("安全性检查失败。原因：请求向量大于可利用资源向量。\n");
			printf("尚无足够资源，进程%d须等待。\n",process);
			proP->state = 'B';
		}
	}else{
		printf("安全性检查失败。原因：请求向量大于需求向量。\n");
	}
	return false;
}
//安全性检查
bool SafeCheck(){
	RESOURCE Work = Available;
	bool Finish[PROCESSES_NUMBER] = {false,false,false,false,false};
	int i;
	// int j = 0;

	for (i = 0; i < PROCESSES_NUMBER; i++){
		//是否已检查过
		if(Finish[i] == false){
			//是否有足够的资源分配给该进程
			if(Need[i].A <= Work.A && Need[i].B <= Work.B && Need[i].C <= Work.C){
				//有则使其执行完成，并将已分配给该进程的资源全部回收
				Work.A += Allocation[i].A;
				Work.B += Allocation[i].B;
				Work.C += Allocation[i].C;
				Finish[i] = true;
				// safe[j++] = i;		//记入安全序列
				i = -1;				//重新进行遍历
			}
		}
	}

	//如果所有进程的Finish向量都为true则处于安全状态，否则为不安全状态
	for (i = 0; i < PROCESSES_NUMBER; i++){
		if (Finish[i] == false){
			return false;
		}
	}
	return true;
}
/*显示进程名为j的进程的PCB*/
void showPCB(int j,plist * pl){
	printf("\t%d",pl[j].name);
	printf("\t{%2d,%2d,%2d}",pl[j].pMax.A,pl[j].pMax.B,pl[j].pMax.C);
	printf("\t{%2d,%2d,%2d}",Allocation[j].A,Allocation[j].B,Allocation[j].C);
	printf("\t%c\n",pl[j].state);
}
/*显示数据*/
void show( int n,plist * pl){
    int j;
    printf("\n\t进程号 最大需求资源数 已分配资源数 进程状态\n");	
	printf("----------------------------------------------------------------------\n");
    printf("运行\n");
    for (j=0 ;j<n;j++){
        if(pl[j].state == 'R'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("就绪\n");
    for (j=0 ;j<n;j++){
        if(pl[j].state == 'W'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("阻塞\n");
    for (j=0 ;j<n;j++){
        if(pl[j].state == 'B'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("完成\n");
    for (j=0 ;j<n;j++){
        if(pl[j].state == 'F'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n\n");
}
/*创建进程，数量为n*/
void creatPl(int n,plist * pl){
	int j;
	srand(time(0));  					//设置随机种子
	for(j=0;j<n;j++)
	{
		pl[j].name= j;
		pl[j].pMax.A=rand()%11;			//A资源所需总数
		pl[j].pMax.B=rand()%16;			//B资源所需总数
		pl[j].pMax.C=rand()%13;			//C资源所需总数
		
		//初始分配资源为0
		pl[j].pAllocation.A=0;	
		pl[j].pAllocation.B=0;	
		pl[j].pAllocation.C=0;	
		
		pl[j].state='W';
	}
}


int main()
{
	int i;
	int runNum = PROCESSES_NUMBER;		//未完成的进程总数
	RESOURCE res;						//进程的请求资源
	
	int count=0;						//资源请求次数
	
	//创建进程
	plist pl[PROCESSES_NUMBER];
	creatPl(PROCESSES_NUMBER,pl);
	//初始化资源矩阵
	for(i=0;i<PROCESSES_NUMBER;i++){
		Need[i].A = Max[i].A = pl[i].pMax.A;
		Need[i].B = Max[i].B = pl[i].pMax.B;
		Need[i].C = Max[i].C = pl[i].pMax.C;
	}
	
	printf("******************************初始化**********************************\n");	
	show(PROCESSES_NUMBER,pl);			//显示数据
	
	i=0;
	while(runNum>0){
		if(pl[i].state!='F'){
			count++;
			//随机生成请求资源
			srand(time(0));  			//设置随机种子
			res.A = (Need[i].A==0) ? 0 : (rand()% Need[i].A + 1);
			res.B = (Need[i].B==0) ? 0 : (rand()% Need[i].B + 1);
			res.C = (Need[i].C==0) ? 0 : (rand()% Need[i].C + 1);
			
			//资源分配
			printf("******************************第%d次分配******************************\n",count);
			printf("进程%d请求分配：{%d,%d,%d}\n",i,res.A,res.B,res.C);
			if(request(&pl[i],&res)){
				printf("分配成功。\n");
				pl[i].state = 'R';		//运行进程
				
				// printf("安全序列是{P%d,P%d,P%d,P%d,P%d}。\n",safe[0],safe[1],safe[2],safe[3],safe[4]);
			}
			
			show(PROCESSES_NUMBER,pl);	//显示数据
			
			//若进程已分配资源等于最大需求资源，则进程完成
			if(Max[i].A==Allocation[i].A && Max[i].B==Allocation[i].B && Max[i].C==Allocation[i].C){
				pl[i].state = 'F';
				runNum--;
				
				release(&pl[i]);		//释放资源
			}else{
				pl[i].state = 'W';		//进程变为就绪
			}
			
		}
		
		//切换下一进程
		i=(i+1)%PROCESSES_NUMBER;
	}
	
	printf("*************************所有进程执行完毕*****************************\n");	
	show(PROCESSES_NUMBER,pl);			//显示数据
	
	getchar();
	return 0;
}