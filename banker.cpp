#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define false 0
#define true !false

#define PROCESSES_NUMBER 5	//ϵͳ�����н�������

typedef struct {
	int	A;
	int	B;
	int	C;
}RESOURCE;

//����������
RESOURCE Max[PROCESSES_NUMBER];

//�ѷ�����Դ������
RESOURCE Allocation[PROCESSES_NUMBER];

//�������
RESOURCE Need[PROCESSES_NUMBER];

//������Դ����,A��10������B��15������C��12����
RESOURCE Available = {10,15,12};

//��ȫ����
// int safe[PROCESSES_NUMBER];

//PCB
typedef struct pcb{
	int name;				//������
	RESOURCE pMax;			//��Ҫ����Դ����
	RESOURCE pAllocation;	//�ѷ������Դ��
	char state;				//����״̬,���� W��Wait��������R��Run��������B��Block�������F��Finish��
}plist;

void ProbeAlloc(plist* proP,RESOURCE *res);
void RollBack(plist* proP,RESOURCE *res);
bool request(plist* proP,RESOURCE *res);
bool SafeCheck();
void creatPl(int n,plist * pl);
void show( int n,plist * pl);
void showPCB(int j,plist * pl);
void release(plist* proP);


//�ͷ���Դ
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
//��̽����
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
//����̽�������벻��ȫ״̬��������ع�
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
//��Դ��������
bool request(plist* proP,RESOURCE *res){
	int process = proP->name;
	//request������С��Need�����ж�Ӧ������
	if(res->A <= Need[process].A && res->B <= Need[process].B && res->C <= Need[process].C){
		//request������С��Available����
		if(res->A <= Available.A && res->B <= Available.B && res->C <= Available.C){
			//��̽����
			ProbeAlloc(proP,res);

			//�����ȫ��������������ɹ������򽫷���ع�������ʧ��
			if(SafeCheck()){
				return true;
			}else{
				printf("��Դ����ʧ�ܡ�ԭ��ϵͳ�����벻��ȫ״̬���п�������������\n");
				// printf("���ڻع�...\n");
				RollBack(proP,res);
				proP->state = 'W';
			}
		}else{
			// printf("��ȫ�Լ��ʧ�ܡ�ԭ�������������ڿ�������Դ������\n");
			printf("�����㹻��Դ������%d��ȴ���\n",process);
			proP->state = 'B';
		}
	}else{
		printf("��ȫ�Լ��ʧ�ܡ�ԭ������������������������\n");
	}
	return false;
}
//��ȫ�Լ��
bool SafeCheck(){
	RESOURCE Work = Available;
	bool Finish[PROCESSES_NUMBER] = {false,false,false,false,false};
	int i;
	// int j = 0;

	for (i = 0; i < PROCESSES_NUMBER; i++){
		//�Ƿ��Ѽ���
		if(Finish[i] == false){
			//�Ƿ����㹻����Դ������ý���
			if(Need[i].A <= Work.A && Need[i].B <= Work.B && Need[i].C <= Work.C){
				//����ʹ��ִ����ɣ������ѷ�����ý��̵���Դȫ������
				Work.A += Allocation[i].A;
				Work.B += Allocation[i].B;
				Work.C += Allocation[i].C;
				Finish[i] = true;
				// safe[j++] = i;		//���밲ȫ����
				i = -1;				//���½��б���
			}
		}
	}

	//������н��̵�Finish������Ϊtrue���ڰ�ȫ״̬������Ϊ����ȫ״̬
	for (i = 0; i < PROCESSES_NUMBER; i++){
		if (Finish[i] == false){
			return false;
		}
	}
	return true;
}
/*��ʾ������Ϊj�Ľ��̵�PCB*/
void showPCB(int j,plist * pl){
	printf("\t%d",pl[j].name);
	printf("\t{%2d,%2d,%2d}",pl[j].pMax.A,pl[j].pMax.B,pl[j].pMax.C);
	printf("\t{%2d,%2d,%2d}",Allocation[j].A,Allocation[j].B,Allocation[j].C);
	printf("\t%c\n",pl[j].state);
}
/*��ʾ����*/
void show( int n,plist * pl){
    int j;
    printf("\n\t���̺� ���������Դ�� �ѷ�����Դ�� ����״̬\n");	
	printf("----------------------------------------------------------------------\n");
    printf("����\n");
    for (j=0 ;j<n;j++){
        if(pl[j].state == 'R'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("����\n");
    for (j=0 ;j<n;j++){
        if(pl[j].state == 'W'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("����\n");
    for (j=0 ;j<n;j++){
        if(pl[j].state == 'B'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("���\n");
    for (j=0 ;j<n;j++){
        if(pl[j].state == 'F'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n\n");
}
/*�������̣�����Ϊn*/
void creatPl(int n,plist * pl){
	int j;
	srand(time(0));  					//�����������
	for(j=0;j<n;j++)
	{
		pl[j].name= j;
		pl[j].pMax.A=rand()%11;			//A��Դ��������
		pl[j].pMax.B=rand()%16;			//B��Դ��������
		pl[j].pMax.C=rand()%13;			//C��Դ��������
		
		//��ʼ������ԴΪ0
		pl[j].pAllocation.A=0;	
		pl[j].pAllocation.B=0;	
		pl[j].pAllocation.C=0;	
		
		pl[j].state='W';
	}
}


int main()
{
	int i;
	int runNum = PROCESSES_NUMBER;		//δ��ɵĽ�������
	RESOURCE res;						//���̵�������Դ
	
	int count=0;						//��Դ�������
	
	//��������
	plist pl[PROCESSES_NUMBER];
	creatPl(PROCESSES_NUMBER,pl);
	//��ʼ����Դ����
	for(i=0;i<PROCESSES_NUMBER;i++){
		Need[i].A = Max[i].A = pl[i].pMax.A;
		Need[i].B = Max[i].B = pl[i].pMax.B;
		Need[i].C = Max[i].C = pl[i].pMax.C;
	}
	
	printf("******************************��ʼ��**********************************\n");	
	show(PROCESSES_NUMBER,pl);			//��ʾ����
	
	i=0;
	while(runNum>0){
		if(pl[i].state!='F'){
			count++;
			//�������������Դ
			srand(time(0));  			//�����������
			res.A = (Need[i].A==0) ? 0 : (rand()% Need[i].A + 1);
			res.B = (Need[i].B==0) ? 0 : (rand()% Need[i].B + 1);
			res.C = (Need[i].C==0) ? 0 : (rand()% Need[i].C + 1);
			
			//��Դ����
			printf("******************************��%d�η���******************************\n",count);
			printf("����%d������䣺{%d,%d,%d}\n",i,res.A,res.B,res.C);
			if(request(&pl[i],&res)){
				printf("����ɹ���\n");
				pl[i].state = 'R';		//���н���
				
				// printf("��ȫ������{P%d,P%d,P%d,P%d,P%d}��\n",safe[0],safe[1],safe[2],safe[3],safe[4]);
			}
			
			show(PROCESSES_NUMBER,pl);	//��ʾ����
			
			//�������ѷ�����Դ�������������Դ����������
			if(Max[i].A==Allocation[i].A && Max[i].B==Allocation[i].B && Max[i].C==Allocation[i].C){
				pl[i].state = 'F';
				runNum--;
				
				release(&pl[i]);		//�ͷ���Դ
			}else{
				pl[i].state = 'W';		//���̱�Ϊ����
			}
			
		}
		
		//�л���һ����
		i=(i+1)%PROCESSES_NUMBER;
	}
	
	printf("*************************���н���ִ�����*****************************\n");	
	show(PROCESSES_NUMBER,pl);			//��ʾ����
	
	getchar();
	return 0;
}