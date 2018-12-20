#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

//���̿��ƿ�
typedef struct pcb
{
  int name;         //������
  int arrivetime;   //����ʱ��
  int needtime;     //����ʱ��
  int cputime;     	//������ʱ��
  int pri;          //���ȼ�(��Ӧ��)
  int endtime;      //���ʱ��
  char state;       //״̬λ,���� W��Wait��������R��Run���������F��Finish��
}plist;

int globaltime = 0;	//cpu������ʱ��

void creatPl(int n,plist * pl );
void show( int n,plist * pl);
void fcfs(int n,plist * pl);
void sjf(int n,plist * pl);
void rr(int n,plist * pl);
void hrrn(int n,plist * pl);

/*ִ�н���*/
void action(plist * nowpl)
{
	void show( int n,plist * pl);

	// Sleep(1);						//�ӳ�1s
	globaltime++;               		//cpu������ʱ��+1
	nowpl->cputime++;					//������ʱ��+1
	if(nowpl->needtime<=nowpl->cputime)		
	{
		nowpl->endtime = globaltime;	//���ý������ʱ��
		nowpl->state='F';				//���ý���״̬Ϊ���
		nowpl->pri = 0;					//���ý������ȼ�Ϊ0
	}else{
		nowpl->state='W';				//���ý���״̬Ϊ����
	}
}
/*�������̣�����Ϊn*/
void  creatPl(int n,plist * pl)
{
   int j;
   srand(time(0));  					//�����������
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
/*��ʾ������Ϊj�Ľ��̵�PCB*/
void showPCB(int j,plist * pl){
	printf("\t%d",pl[j].name);
	printf("\t%d",pl[j].pri);
	printf("\t%d",pl[j].arrivetime);
	printf("\t%d",pl[j].needtime);
	printf("\t  %d",pl[j].cputime);
	printf("\t    %c",pl[j].state);
	printf("\t  %d\n",pl[j].endtime);
}
/*��ʾ��������Ϣ*/
void show( int n,plist * pl)
{
    int j;
    printf("*******************************ʱ�̣�%3d******************************\n",globaltime);
    printf("\t���̺� ���ȼ� ����ʱ�� ����ʱ�� ������ʱ�� ״̬ ���ʱ��\n");
	printf("----------------------------------------------------------------------\n");
    printf("����\n");
    for (j=0 ;j<n;j++)
    {
        if(pl[j].state == 'R'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("����\n");
    for (j=0 ;j<n;j++)
    {
        if(pl[j].state == 'W'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
    printf("���\n");
    for (j=0 ;j<n;j++)
    {
        if(pl[j].state == 'F'){
            showPCB(j,pl);
        }
    }
	printf("----------------------------------------------------------------------\n");
	printf("**********************************************************************\n\n");
}
/*����ƽ����תʱ��*/
float *cal(int n, plist* pl){
	int i;
	float *res = (float *)malloc(2*sizeof(float));
	// float res[1];
	float Sumct = 0;	//��תʱ��
	float Sumwt = 0;	//��Ȩ��תʱ��
	
	for(i=0;i<n;i++){
		Sumct+=pl[i].endtime-pl[i].arrivetime;
		Sumwt+=(pl[i].endtime-pl[i].arrivetime)/pl[i].needtime;
	}
	res[0]=Sumct/n;
	res[1]=Sumwt/n;
	
	return res;
}

/*���ý�����Ϣ*/
void clrPro(int n, plist* pl){
	int i;
	for(i=0;i<n;i++){
		pl[i].endtime=-1;
		pl[i].cputime=-1;
		pl[i].state='W';
	}
}


/*�̽��������㷨*/
void sjf(int n,plist * pl)
{
   void show( int n,plist * pl);
   int i,j;
   int count = n;
   plist temp;

   //�����̳�������
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

	  //���ҵ�ǰ�ѵ�������δ��ɽ���
	  for(i=0;i<n;i++){
        if(pl[i].arrivetime<=globaltime && pl[i].state!='F'){
			pl[i].state = 'R';		//���ý���״̬Ϊ����
			// show(n,pl);
            action(&pl[i]);			//����ǰ����
			break;
        }
	  }

	  //����Ƿ���δ��ɽ���
	  for(i=0;i<n;i++){
		 if(pl[i].state=='F'){
		  count--;
		}
	  }
   }
   show(n,pl);
   
}
/*ʱ��Ƭ��ת�㷨*/
void rr(int n,plist * pl)
{
  void show( int n,plist * pl);

   int i,j;
   int time;	//ѭ������
   plist temp;
   
   //������ʱ������
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
			pl[i].state = 'R';	//���ý���״̬Ϊ����
			// show(n,pl);
            action(&pl[i]);
          }
      }
      time--;
   }
   show(n,pl);
   
}
/*����Ӧ�������㷨*/
void hrrn(int n,plist * pl){
	int i;
	int j;
	int count = n;
	int hrrPro = 0;		//��¼�����Ӧ�ȵĽ�����
	plist temp;
   
   //������ʱ������
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
		//���������ѵ�����δ��ɵĽ��̵����ȼ������������ȼ���ߵĽ���
		for(i=0;i<n;i++){
			if(pl[i].arrivetime<=globaltime && pl[i].state!='F'){
				//��Ӧ��=��Ӧʱ��/����ʱ��
				pl[i].pri = (globaltime-pl[i].arrivetime+pl[i].needtime)/pl[i].needtime;
				if(pl[hrrPro].pri<pl[i].pri){
					hrrPro=i;
				}
			}
			
		}
	  //���Ȳ��ҵ��Ľ����������
	  while(pl[hrrPro].state != 'F'){
			pl[i].state = 'R';				//���ý���״̬Ϊ����
			// show(n,pl);
			action(&pl[hrrPro]);			//����ǰ����
	  }

	  //����Ƿ���δ��ɽ���
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
	int n;   		//������
	plist pl[10];	//���̶���
	float * avgct[2];	//ƽ����תʱ��

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
	printf("*********************************ͳ��*********************************\n");
	printf("\tƽ����תʱ��\tƽ����Ȩ��תʱ��\n");
	printf("SJF\t %f\t %f\t\n",avgct[0][0],avgct[0][1]);
	printf("RR\t %f\t %f\t\n",avgct[1][0],avgct[1][1]);
	printf("HRRN\t %f\t %f\t\n",avgct[2][0],avgct[2][1]);
	
	printf("�������\n");
	getch();

   return 0;

}
