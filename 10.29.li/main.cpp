#include <stdio.h>
#include <iostream>
//#include <malloc.h>
#define N 2                /*停车场内最多的停车数*/
#define M 41              /*候车场内最多的停车数*/
#define Price 2.5
using namespace std;
/*每单位时间停车费用*/
typedef struct Stack
{
    int carnumber[N];           /*车牌号*/
    int comeTime[N];            /*进场时间*/
    int top;                    /*栈指针*/
} Stack;                        /*定义顺序栈类型，用于描述停车场*/

typedef struct Queue
{
    int carnumber[M];           /*车牌号*/
    int front,rear;             /*队首和队尾指针*/
} Queue;                        /*定义循环队类型，用于描述候车场*/


Stack *St,*St1;  //St是停车场，St1是在有车离开时，记录为该车移开位置的车辆
Queue *Qu;   //Qu是候车场




/*以下为顺序栈的基本运算算法*/
void InitStack(Stack *&s)
{
    s=(Stack *)malloc(sizeof(Stack));
    s->top=-1;
}
int IsEmpty(Stack *s)
{
    return(s->top==-1);
}
int IsFull(Stack *s)
{
    return(s->top==N-1);
}

int Push(Stack *&s,int e1,int e2)
{
    if (s->top==N-1)
        return 0;
    
    s->top++;
    s->carnumber[s->top]=e1;
    cout<<s->carnumber[s->top]<<endl;
    s->comeTime[s->top]=e2;
    cout<<s->comeTime[s->top]<<endl;
    return 1;
}
int Pop(Stack *&s,int &e1,int &e2)
{
    if (s->top==-1)
        return 0;
    e1=s->carnumber[s->top];
    e2=s->comeTime[s->top];
    s->top--;
    return 1;
}
void DispStack(Stack *s)
{
    int i;
    for (i=s->top; i>=0; i--)
        printf("%d ",s->carnumber[i]);
    printf("\n");
}




/*以下为循环队列的基本运算算法*/
void InitQueue(Queue *&q)
{
    q=(Queue *)malloc (sizeof(Queue));
    q->front=q->rear=0;
}
int IsEmpty(Queue *q)
{
    return(q->front==q->rear);
}
int IsFull(Queue *q)       /*判断队满*/
{
    return ((q->rear+1)%M==q->front);
}
int AddQueue(Queue *&q,int e)      /*进队*/
{
    if ((q->rear+1)%M==q->front)    /*队满*/
        return 0;
    q->rear=(q->rear+1)%M;
    q->carnumber[q->rear]=e;
    return 1;
}
int deQueue(Queue *&q,int &e)     /*出队*/
{
    if (q->front==q->rear)          /*队空的情况*/
        return 0;
    q->front=(q->front+1)%M;
    e=q->carnumber[q->front];
    return 1;
}
void DispQueue(Queue *q)      /*输出队中元素*/
{
    int i;
    i=(q->front+1)%M;
    printf("%d ",q->carnumber[i]);
    while ((q->rear-i+M)%M>0)
    {
        i=(i+1)%M;
        printf("%d ",q->carnumber[i]);
    }
    printf("\n");
}


//main函数用于模拟停车场的工作
int main()
{
    cout<<"fjghd"<<endl;
    int gongneng;
    int number,time,e1,e2;
    int i,j,t;

    InitStack(St);
    InitStack(St1);
    InitQueue(Qu);
    do
    {
        printf("请输入需要执行的指令  1.到达 2.离开 3.显示停车场的情况 4.显示候车场的情况 0.退出:");
        scanf("%d",&gongneng);
        switch(gongneng)
        {
            case 1:     /*汽车到达*/
                printf("输入车号和时间(设车号和时间均为整数): ");
                scanf("%d%d",&number,&time);
                if (!IsFull(St))         /*停车场不满*/
                {
                    Push(St,number,time);
                    printf("  >>停车场位置:%d\n",St->top+1);
                }
                else                        /*停车场满*/
                {
                    if (!IsFull(Qu))     /*候车场不满*/
                    {
                        AddQueue(Qu,number);
                        printf("  >>候车场位置:%d\n",Qu->rear);
                    }
                    else
                        printf("  >>候车场已满,不能停车\n");
                }
                break;
            case 2:     /*汽车离开*/
                printf("输入车号和时间(设车号和时间均为整数): ");
                scanf("%d%d",&number,&time);
                for (i=0; i<=St->top && St->carnumber[i]!=number; i++);  //在栈中找
                if (i>St->top)
                    printf("  >>未找到该编号的汽车\n");
                else
                {
                    St1->top=0;
                    for(i=St->top-1;i>=0;i--)
                    {
                        if(St->carnumber[i]==number)
                        {
                            printf("  >>%d汽车停车费用:%.2f\n",number,1.0*(time-St->comeTime[i])*Price);
                            break;
                        }
                        else
                        {
                            St1->carnumber[St1->top] =St->carnumber[i];
                            St1->comeTime[St1->top]=St->comeTime[i];
                            St1->top++;
                        }
                    }
                    for(j=St1->top-1;j>=0;j--)
                    {
                        
                        St->carnumber[i]=St1->carnumber[j];
                        St->comeTime[i]=St1->comeTime[j];
                        i++;
                    }
                    
                    if(!IsEmpty(Qu))
                       {
                         
                           St->carnumber[i]=Qu->carnumber[(++Qu->front)%M];
                           cout<<Qu->carnumber[Qu->front]<<endl;
                           cout<<"请重新输入现在的时间"<<endl;
                           cin>>time;
                           St->comeTime[i]=time;
                           Qu->front=(Qu->front)%M;
                           
                           
                       }
                    if(IsEmpty(Qu))
                    {
                        St->top--;
                    }
                        
                        
                    
                    
                    
                    
                    
//                    t = St->top - i;  //需要出栈的车辆数目
//                    for (j=0; j<t; j++)  //for (j=i; j<=St->top; j++)1楼评论讲的原错误写法
//                    {
//                        Pop(St,e1,e2);
//                        Push(St1,e1,e2);        /*倒车到临时栈St1中*/
//                    }
//                    Pop(St,e1,e2);              /*该汽车离开*/
//                    printf("  >>%d汽车停车费用:%d\n",number,(time-e2)*Price);
//                    while (!IsEmpty(St1))      /*将临时栈St1重新回到St中*/
//                    {
//                        Pop(St1,e1,e2);
//                        Push(St,e1,e2);
//                    }
//                    if (!IsEmpty(Qu))           /*队不空时,将队头进栈St*/
//                    {
//                        deQueue(Qu,e1);
//                        Push(St,e1,time);       /*以当前时间开始计费*/
//                    }
                }
                break;
            case 3:     /*显示停车场情况*/
                if (!IsEmpty(St))
                {
                    printf("  >>停车场中的车辆:"); /*输出停车场中的车辆*/
                    DispStack(St);
                }
                else
                    printf("  >>停车场中无车辆\n");
                break;
            case 4:     /*显示候车场情况*/
                if (!IsEmpty(Qu))
                {
                    printf("  >>候车场中的车辆:"); /*输出候车场中的车辆*/
                    DispQueue(Qu);
                }
                else
                    printf("  >>候车场中无车辆\n");
                break;
                
            case 0:     /*结束*/
                if (!IsEmpty(St))
                {
                    printf("  >>停车场中的车辆:"); /*输出停车场中的车辆*/
                    DispStack(St);
                }
                if (!IsEmpty(Qu))
                {
                    printf("  >>候车场中的车辆:"); /*输出候车场中的车辆*/
                    DispQueue(Qu);
                }
                break;
            default:    /*其他情况*/
                printf("  >>输入的命令错误\n");
                 DispQueue(Qu);
                break;
        }
    }
    while(gongneng!=0);
    return 0;
}
