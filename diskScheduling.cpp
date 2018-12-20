#include<stdio.h>
#include<stdlib.h>
#include<time.h>
 
#define MAXSIZE 200		//最大磁道号
#define NUM	10			//磁盘请求序列个数
#define ASC 0			//向磁道号增加方向
#define DEC 1			//向磁道号减少方向


int begin = 100;		//初始磁道号
int direction = ASC;	//初始方向为磁道号增加方向

/*********************冒泡排序算法**************************/
int *sort(int arr[])
{
    int i,j;
    int temp;
    for(i=0; i<NUM; i++) 
        for(j=i+1; j<NUM; j++)
        {
            if(arr[i]>arr[j])
            {
                temp=arr[i];
                arr[i]=arr[j];
                arr[j]=temp;
            }
        }
    return arr;
}

//显示磁头移动顺序，并计算平均寻道长度
void show(int arr[])
{
	printf("磁头移动顺序: ");
	
	int i;
	int sum = abs(begin-arr[0]);
	float avg = 0;
	
    for(i=0; i<NUM; i++) 
    {
        printf("%d  ",arr[i]);
    }
	printf("\n");
	
	//求平均寻道长度
    for(i=0; i<NUM-1; i++) 
    {
        sum+=abs(arr[i+1]-arr[i]);
        avg=(float)(sum)/(float)(NUM);
    }
    printf("磁头的平均移动磁道数: %f\n\n",avg);
}

//先来先服务调度算法
int* FCFS(int arr[])   
{
	printf("---------------------------------------------------\n");
	printf("先来先服务调度算法\n");
	return arr;
}

//最短寻道时间优先调度算法
int* SSTF(int arr[])
{
	printf("---------------------------------------------------\n");
	printf("最短寻道时间优先调度算法\n");
	
	sort(arr);						//升序排序
	
	int i,j,k;
	int current = begin;			//记录当前磁道
	int flag[NUM] = {0};			//记录相应请求是否已处理，0为未处理，1未已处理
	int *arr2 = (int*)malloc(NUM*sizeof(int));
	
	for(i=0;i<NUM;i++){
		j=0;
		k=NUM-1;
		//查找第一个大于或等于当前磁道且未处理的请求
		while((flag[j]==1 || arr[j]<current) && j<NUM-1) j++;	
		//查找第一个小于或等于当前磁道且未处理的请求
		while((flag[k]==1 || arr[k]>current) && k>0) k--;	
		
		if(j!=k){						//若j==k，说明arr[j]已是最近请求
			while(flag[j]==1) j--;		//若超出最大范围，则取未完成的最大磁道
			while(flag[k]==1) k++;		//若超出最小范围，则取未完成的最小磁道
			j = (abs(arr[j]-current) < abs(arr[k]-current)) ? j : k;
		}
		flag[j]=1;						//arr[j]为离当前磁道最近的请求，标记已处理
		arr2[i]=arr[j];
		current = arr[j];				//修改当前磁头
	}
	
	return arr2;
}

//扫描调度算法
int* SCAN(int arr[])  
{
	printf("---------------------------------------------------\n");
	printf("扫描调度算法\n");
	
	sort(arr);		//升序排序
	
	int i,j;
	int *arr2 = (int*)malloc(NUM*sizeof(int));
	
	if(direction==ASC){		//向磁道号增加方向
		//将大于或等于当前磁道的请求加入新数组
		for(i=0,j=0;i<NUM;i++){
			if(arr[i]>=begin){
				arr2[j++]=arr[i];
			}
		}
		// //调换磁头方向
		// direction=DEC;
		//将小于当前磁道的请求按逆序加入新数组
		for(i=NUM-j-1;i>=0;i--){
			arr2[j++]=arr[i];
		}
	}else{					//向磁道号减少方向
		//将小于或等于当前磁道的请求加入新数组
		for(i=NUM,j=0;i>0;i--){
			if(arr[i]<=begin){
				arr2[j++]=arr[i];
			}
		}
		// //调换磁头方向
		// direction=ASC;
		//将大于当前磁道的请求按逆序加入新数组
		while(j<NUM){
			arr2[j]=arr[j];
			j++;
		}
	}
	
	return arr2;
}

//循环扫描调度算法
int* CSCAN(int arr[])
{
	printf("---------------------------------------------------\n");
	printf("循环扫描调度算法\n");
	
	sort(arr);		//升序排序
	
	int i,j;
	int *arr2 = (int*)malloc(NUM*sizeof(int));
	
	if(direction==ASC){	//向磁道号增加方向
		//将大于或等于当前磁道的请求加入新数组
		for(i=0,j=0;i<NUM;i++){
			if(arr[i]>=begin){
				arr2[j++]=arr[i];
			}
		}
		//将小于当前磁道的请求加入新数组
		for(i=0;i<NUM;i++){
			arr2[j++]=arr[i];
		}
	}else{	//向磁道号减少方向
		//将小于或等于当前磁道的请求加入新数组
		for(i=NUM,j=0;i>0;i--){
			if(arr[i]>=begin){
				arr2[j++]=arr[i];
			}
		}
		//将大于当前磁道的请求加入新数组
		for(i=NUM;j<NUM;i--){
			arr2[j++]=arr[i];
		}
	}
	
	return arr2;
}

int main()
{
	int arr[NUM] = { 0 };
	// int arr[NUM] = {55,58,39,18,90,160,150,38,184};
	
	//随机生成磁盘请求序列
	int i = 0;
	printf("磁盘请求序列：");
	srand((unsigned int)time(0));
	for (i = 0; i < NUM; i++)
	{
		arr[i] = rand() % MAXSIZE + 1;
		printf("%d  ", arr[i]);
	}
	printf("\n\n");
	
	show(FCFS(arr));
	show(SSTF(arr));
	show(SCAN(arr));
	show(CSCAN(arr));
	
	printf("程序执行结束。\n");
	getchar();
	return 0;
}








