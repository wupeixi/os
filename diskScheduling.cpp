#include<stdio.h>
#include<stdlib.h>
#include<time.h>
 
#define MAXSIZE 200		//���ŵ���
#define NUM	10			//�����������и���
#define ASC 0			//��ŵ������ӷ���
#define DEC 1			//��ŵ��ż��ٷ���


int begin = 100;		//��ʼ�ŵ���
int direction = ASC;	//��ʼ����Ϊ�ŵ������ӷ���

/*********************ð�������㷨**************************/
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

//��ʾ��ͷ�ƶ�˳�򣬲�����ƽ��Ѱ������
void show(int arr[])
{
	printf("��ͷ�ƶ�˳��: ");
	
	int i;
	int sum = abs(begin-arr[0]);
	float avg = 0;
	
    for(i=0; i<NUM; i++) 
    {
        printf("%d  ",arr[i]);
    }
	printf("\n");
	
	//��ƽ��Ѱ������
    for(i=0; i<NUM-1; i++) 
    {
        sum+=abs(arr[i+1]-arr[i]);
        avg=(float)(sum)/(float)(NUM);
    }
    printf("��ͷ��ƽ���ƶ��ŵ���: %f\n\n",avg);
}

//�����ȷ�������㷨
int* FCFS(int arr[])   
{
	printf("---------------------------------------------------\n");
	printf("�����ȷ�������㷨\n");
	return arr;
}

//���Ѱ��ʱ�����ȵ����㷨
int* SSTF(int arr[])
{
	printf("---------------------------------------------------\n");
	printf("���Ѱ��ʱ�����ȵ����㷨\n");
	
	sort(arr);						//��������
	
	int i,j,k;
	int current = begin;			//��¼��ǰ�ŵ�
	int flag[NUM] = {0};			//��¼��Ӧ�����Ƿ��Ѵ���0Ϊδ����1δ�Ѵ���
	int *arr2 = (int*)malloc(NUM*sizeof(int));
	
	for(i=0;i<NUM;i++){
		j=0;
		k=NUM-1;
		//���ҵ�һ�����ڻ���ڵ�ǰ�ŵ���δ���������
		while((flag[j]==1 || arr[j]<current) && j<NUM-1) j++;	
		//���ҵ�һ��С�ڻ���ڵ�ǰ�ŵ���δ���������
		while((flag[k]==1 || arr[k]>current) && k>0) k--;	
		
		if(j!=k){						//��j==k��˵��arr[j]�����������
			while(flag[j]==1) j--;		//���������Χ����ȡδ��ɵ����ŵ�
			while(flag[k]==1) k++;		//��������С��Χ����ȡδ��ɵ���С�ŵ�
			j = (abs(arr[j]-current) < abs(arr[k]-current)) ? j : k;
		}
		flag[j]=1;						//arr[j]Ϊ�뵱ǰ�ŵ���������󣬱���Ѵ���
		arr2[i]=arr[j];
		current = arr[j];				//�޸ĵ�ǰ��ͷ
	}
	
	return arr2;
}

//ɨ������㷨
int* SCAN(int arr[])  
{
	printf("---------------------------------------------------\n");
	printf("ɨ������㷨\n");
	
	sort(arr);		//��������
	
	int i,j;
	int *arr2 = (int*)malloc(NUM*sizeof(int));
	
	if(direction==ASC){		//��ŵ������ӷ���
		//�����ڻ���ڵ�ǰ�ŵ����������������
		for(i=0,j=0;i<NUM;i++){
			if(arr[i]>=begin){
				arr2[j++]=arr[i];
			}
		}
		// //������ͷ����
		// direction=DEC;
		//��С�ڵ�ǰ�ŵ��������������������
		for(i=NUM-j-1;i>=0;i--){
			arr2[j++]=arr[i];
		}
	}else{					//��ŵ��ż��ٷ���
		//��С�ڻ���ڵ�ǰ�ŵ����������������
		for(i=NUM,j=0;i>0;i--){
			if(arr[i]<=begin){
				arr2[j++]=arr[i];
			}
		}
		// //������ͷ����
		// direction=ASC;
		//�����ڵ�ǰ�ŵ��������������������
		while(j<NUM){
			arr2[j]=arr[j];
			j++;
		}
	}
	
	return arr2;
}

//ѭ��ɨ������㷨
int* CSCAN(int arr[])
{
	printf("---------------------------------------------------\n");
	printf("ѭ��ɨ������㷨\n");
	
	sort(arr);		//��������
	
	int i,j;
	int *arr2 = (int*)malloc(NUM*sizeof(int));
	
	if(direction==ASC){	//��ŵ������ӷ���
		//�����ڻ���ڵ�ǰ�ŵ����������������
		for(i=0,j=0;i<NUM;i++){
			if(arr[i]>=begin){
				arr2[j++]=arr[i];
			}
		}
		//��С�ڵ�ǰ�ŵ����������������
		for(i=0;i<NUM;i++){
			arr2[j++]=arr[i];
		}
	}else{	//��ŵ��ż��ٷ���
		//��С�ڻ���ڵ�ǰ�ŵ����������������
		for(i=NUM,j=0;i>0;i--){
			if(arr[i]>=begin){
				arr2[j++]=arr[i];
			}
		}
		//�����ڵ�ǰ�ŵ����������������
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
	
	//������ɴ�����������
	int i = 0;
	printf("�����������У�");
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
	
	printf("����ִ�н�����\n");
	getchar();
	return 0;
}








