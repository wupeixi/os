#include <stdio.h>
#include<stdlib.h>

#define MEMORY 640 	//���õ��ڴ�ռ�Ϊ640KB
#define MINSIZE 2 	//�����ٷָ���ʣ������Ĵ�С
#define FREE 0 		//����
#define BUSY 1 		//ռ��
#define REQUEST 1	//�����ڴ�
#define RELEASE 0 	//�ͷ��ڴ�
#define ERROR 0		//����
#define OK 1		//����

//����������
typedef struct Body
{
	int name;				//��������
	int size;				//��������С
	int address;			//�������׵�ַ
	int status;				//������״̬�����У�FREE����ռ�ã�BUSY��

}Body;	

//���з�����
typedef struct Node
{
	Body data;				//����������
	struct Node *prior;		//ǰ��ָ��
	struct Node *next;		//����ָ��
}*DLinkList;				

//��ҵ����
typedef struct{
	int name;				//������ҵ��
	int type;				//����������ͣ����루REQUEST�����ͷţ�RELEASE��
	int size;				//����ռ��С
}Request;

DLinkList head; 			//ͷ��㣬��ʹ��
DLinkList first;  			//��һ�����ý��

void show();
void init();
void sortList();
int bestFit(int name, int size);
int firstFit(int name, int size);
int recycle(int name);
int allocate(int name, int type, int size);

//��ʾ��Ϣ
void show()
{
	printf("********************��ǰ�ڴ�������*********************\n");
	printf("������\t\tʼ��ַ\t\t������С\t״  ̬\n");
	printf("---------------------------------------------------------\n");
	Node *p=head->next;
	while(p)
	{
		if(p->data.name==-1){
			printf("null\t\t");
		}
		else{
			printf("%3d\t\t",p->data.name);
		}
		printf("%d\t\t",p->data.address);
		printf("%3dKB\t\t",p->data.size);
		if(p->data.status==FREE){
			printf("��  ��\n");
		}
		else if(p->data.status==BUSY){
			printf("�ѷ���\n");
		}
		
		p=p->next;
	}
	printf("---------------------------------------------------------\n\n");

}

//��ʼ�����з�����
void init()
{
	head=(DLinkList)malloc(sizeof(Node));
	first=(DLinkList)malloc(sizeof(Node));
	
	head->prior=NULL;
	head->next=first;
	head->data.status=BUSY;			//��֤�ڴ����ʱ�������жϲ�����
	
	first->prior=head;
	first->next=NULL;
	first->data.address=0;
	first->data.size=MEMORY;
	first->data.name=-1;
	first->data.status=FREE;
}

//������������������С��������
void sortList(){
	Node *p,*q;
	Body tmp;
	
	for(p=head->next;p!=NULL;p=p->next){
		if(p->data.status==FREE){
			for(q=p->next;q!=NULL;q=q->next){
				if(q->data.status==FREE && p->data.size > q->data.size){
					//�������
					tmp = p->data;
					p->data = q->data;
					q->data = tmp;
					
				}
			}
		}
	}
}

//�����Ӧ�㷨
int bestFit(int name, int size){
	sortList();			//����ǰ��������������
	
	if(firstFit(name,size)==ERROR)
		return ERROR;
	
	sortList();			//������ٽ�����������
	return OK;
}

//�״���Ӧ�㷨
int firstFit(int name, int size){
	Node *p=head->next;
	
	while(p)
	{
		//������Ҫ��Ŀ��з���
		if(p->data.status==FREE && p->data.size>size)
		{
			//�½���ҵ�Ľ��
			DLinkList NewNode=(DLinkList)malloc(sizeof(Node));
			NewNode->data.name=name;
			NewNode->data.size=size;
			NewNode->data.status=BUSY;
			
			if(p->data.size-size>MINSIZE){	//��������Ƭ������
				NewNode->prior=p->prior;
				NewNode->next=p;
				NewNode->data.address=p->data.address;
				p->prior->next=NewNode;
				p->prior=NewNode;
				p->data.address=NewNode->data.address+size;
				p->data.size-=size;
				
			}else{							//������Ƭ��ȫ������
				p->data.status=BUSY;
				p->data.name=name;
			}
			return OK;
		}
		p=p->next;
	}
	
	//����ʧ��
	return ERROR;
}

//�����ڴ�
int recycle(int name){
	Node *p=head->next;
	while(p)
	{
		//���ҿ��п�
		if(p->data.name==name)
		{
			p->data.status=FREE;
			p->data.name = -1;
			
			//��ǰ��Ŀ��п�����
			if((p->prior->data.status==FREE)&&(p->next->data.status==FREE))
			{
				p->prior->data.size=p->prior->data.size+p->data.size+p->next->data.size;
				//��p->next�����һ�����
				if(p->next->next==NULL)
				{
					p->prior->next=NULL;
				}
				else
				{
					p->prior->next=p->next->next;
					p->next->next->prior=p->prior;
				}
				return OK;
			}
			
			//��ǰ��Ŀ��п�����
			if(p->prior->data.status==FREE)
			{
				p->prior->data.size+=p->data.size;
				//��p�����һ�����
				if(p->next==NULL)
				{
					p->prior->next=NULL;
				}
				else
				{
					p->prior->next=p->next;
					p->next->prior=p->prior;
				}
				return OK;
			}

			//�����Ŀ��п�����
			if(p->next->data.status==FREE)
			{
				
				p->data.size+=p->next->data.size;
				//��p->next�����һ�����
				if(p->next->next==NULL){
					p->next=NULL;
				}
				else
				{
					p->next->next->prior=p;
					p->next=p->next->next;
				}
				return OK;
			}
			
			//ǰ����޿��п飬����ϲ���ֱ�ӷ���
			return OK;
		}
		p=p->next;
	}
	
	printf("����δ�ҵ���ҵ%d\n",name);
	return ERROR;
}

//�����ڴ�
int allocate(int name, int type, int size){
	//�������Ϸ���
	if(name<0)
	{
		printf("������ҵ��ֻ��Ϊ0����������\n");
		return ERROR;
	}
	if(size<=0)
	{
		printf("���󣺷�����ڴ��С��������������\n");
		return ERROR;
	}
	
	if(type==REQUEST){	//�����ڴ�
		printf("��ҵ%d��������ڴ�%dKB\n",name,size);
		// return firstFit(name,size);		//�״���Ӧ�㷨
		return bestFit(name,size);		//�����Ӧ�㷨
	}else{				//�����ڴ�
		printf("��ҵ%d�����ͷ��ڴ�%dKB\n",name,size);
		return recycle(name);
	}
	
	return ERROR;
}



int main(){
	
	//��ҵ��������
	Request reqList[]{
		{1,REQUEST,130},
		{2,REQUEST,60},
		{3,REQUEST,100},
		{2,RELEASE,60},
		{4,REQUEST,200},
		{3,RELEASE,100},
		{1,RELEASE,130},
		{5,REQUEST,140},
		{6,REQUEST,60},
		{7,REQUEST,50},
		{8,REQUEST,60},
		{-1}
	};
	
	init();		//��ʼ�����з�����
	
	printf("��ʼ��\n");
	show();
	
	int i=0;
	while(reqList[i].name>=0){
		if(allocate(reqList[i].name,reqList[i].type,reqList[i].size)==OK){
			printf("�����ɹ���\n");
		}else{
			printf("����ʧ�ܣ�\n");
		}
		show();
		i++;
	}
	
	printf("����ִ����ϡ�\n");
	getchar();
	return 0;
}
















