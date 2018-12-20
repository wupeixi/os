#include <stdio.h>
#include<stdlib.h>

#define MEMORY 640 	//可用的内存空间为640KB
#define MINSIZE 2 	//不可再分隔的剩余分区的大小
#define FREE 0 		//空闲
#define BUSY 1 		//占用
#define REQUEST 1	//申请内存
#define RELEASE 0 	//释放内存
#define ERROR 0		//错误
#define OK 1		//正常

//空闲链内容
typedef struct Body
{
	int name;				//空闲区号
	int size;				//空闲区大小
	int address;			//空闲区首地址
	int status;				//空闲区状态，空闲（FREE）或占用（BUSY）

}Body;	

//空闲分区链
typedef struct Node
{
	Body data;				//空闲链内容
	struct Node *prior;		//前向指针
	struct Node *next;		//后向指针
}*DLinkList;				

//作业请求
typedef struct{
	int name;				//请求作业名
	int type;				//请求操作类型，申请（REQUEST）或释放（RELEASE）
	int size;				//请求空间大小
}Request;

DLinkList head; 			//头结点，不使用
DLinkList first;  			//第一个可用结点

void show();
void init();
void sortList();
int bestFit(int name, int size);
int firstFit(int name, int size);
int recycle(int name);
int allocate(int name, int type, int size);

//显示信息
void show()
{
	printf("********************当前内存分配情况*********************\n");
	printf("分区号\t\t始地址\t\t分区大小\t状  态\n");
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
			printf("空  闲\n");
		}
		else if(p->data.status==BUSY){
			printf("已分配\n");
		}
		
		p=p->next;
	}
	printf("---------------------------------------------------------\n\n");

}

//初始化空闲分区链
void init()
{
	head=(DLinkList)malloc(sizeof(Node));
	first=(DLinkList)malloc(sizeof(Node));
	
	head->prior=NULL;
	head->next=first;
	head->data.status=BUSY;			//保证内存回收时，类型判断不出错
	
	first->prior=head;
	first->next=NULL;
	first->data.address=0;
	first->data.size=MEMORY;
	first->data.name=-1;
	first->data.status=FREE;
}

//将空闲链按空闲区大小升序排序
void sortList(){
	Node *p,*q;
	Body tmp;
	
	for(p=head->next;p!=NULL;p=p->next){
		if(p->data.status==FREE){
			for(q=p->next;q!=NULL;q=q->next){
				if(q->data.status==FREE && p->data.size > q->data.size){
					//交换结点
					tmp = p->data;
					p->data = q->data;
					q->data = tmp;
					
				}
			}
		}
	}
}

//最佳适应算法
int bestFit(int name, int size){
	sortList();			//分配前，将空闲链排序
	
	if(firstFit(name,size)==ERROR)
		return ERROR;
	
	sortList();			//分配后，再将空闲链排序
	return OK;
}

//首次适应算法
int firstFit(int name, int size){
	Node *p=head->next;
	
	while(p)
	{
		//有满足要求的空闲分区
		if(p->data.status==FREE && p->data.size>size)
		{
			//新建作业的结点
			DLinkList NewNode=(DLinkList)malloc(sizeof(Node));
			NewNode->data.name=name;
			NewNode->data.size=size;
			NewNode->data.status=BUSY;
			
			if(p->data.size-size>MINSIZE){	//不产生碎片，划分
				NewNode->prior=p->prior;
				NewNode->next=p;
				NewNode->data.address=p->data.address;
				p->prior->next=NewNode;
				p->prior=NewNode;
				p->data.address=NewNode->data.address+size;
				p->data.size-=size;
				
			}else{							//产生碎片，全部分配
				p->data.status=BUSY;
				p->data.name=name;
			}
			return OK;
		}
		p=p->next;
	}
	
	//分配失败
	return ERROR;
}

//回收内存
int recycle(int name){
	Node *p=head->next;
	while(p)
	{
		//查找空闲块
		if(p->data.name==name)
		{
			p->data.status=FREE;
			p->data.name = -1;
			
			//与前后的空闲块相连
			if((p->prior->data.status==FREE)&&(p->next->data.status==FREE))
			{
				p->prior->data.size=p->prior->data.size+p->data.size+p->next->data.size;
				//若p->next是最后一个结点
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
			
			//与前面的空闲块相连
			if(p->prior->data.status==FREE)
			{
				p->prior->data.size+=p->data.size;
				//若p是最后一个结点
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

			//与后面的空闲块相连
			if(p->next->data.status==FREE)
			{
				
				p->data.size+=p->next->data.size;
				//若p->next是最后一个结点
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
			
			//前后均无空闲块，无须合并，直接返回
			return OK;
		}
		p=p->next;
	}
	
	printf("错误：未找到作业%d\n",name);
	return ERROR;
}

//分配内存
int allocate(int name, int type, int size){
	//检查参数合法性
	if(name<0)
	{
		printf("错误：作业号只能为0或正整数！\n");
		return ERROR;
	}
	if(size<=0)
	{
		printf("错误：分配的内存大小必须是正整数！\n");
		return ERROR;
	}
	
	if(type==REQUEST){	//分配内存
		printf("作业%d申请分配内存%dKB\n",name,size);
		// return firstFit(name,size);		//首次适应算法
		return bestFit(name,size);		//最佳适应算法
	}else{				//回收内存
		printf("作业%d申请释放内存%dKB\n",name,size);
		return recycle(name);
	}
	
	return ERROR;
}



int main(){
	
	//作业请求序列
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
	
	init();		//初始化空闲分区链
	
	printf("初始化\n");
	show();
	
	int i=0;
	while(reqList[i].name>=0){
		if(allocate(reqList[i].name,reqList[i].type,reqList[i].size)==OK){
			printf("操作成功！\n");
		}else{
			printf("操作失败！\n");
		}
		show();
		i++;
	}
	
	printf("程序执行完毕。\n");
	getchar();
	return 0;
}
















