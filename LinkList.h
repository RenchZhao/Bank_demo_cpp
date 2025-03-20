#include<cstdlib>
#include"Bank_Account.h"
using namespace std;                            /*辅助工具：带头结点单链表*/ 

#define OK       true
#define ERROR    false

typedef Bank_Account ElemType;  //ElemType 可在此改变类型 

typedef struct LNode
{
     ElemType   data;       //数据域
     struct LNode  *next;   //指向后继的指针域
}LNode,*LinkList;   

bool InitList_L(LinkList &L);                        //初始化单链表 
void DestroyList_L(LinkList &L);                     //销毁单链表 
bool ListEmpty(LinkList L);                         //判断链表是否为空 
int ListLength_L(LinkList L);                       //求链表长 
bool GetElem_L(LinkList L,int i,ElemType &e);      //将链表指定位置的元素放入e中 
int LocateElem_L (LinkList L, ElemType e);         //找跟元素e账户名相同的元素 
bool ListInsert_L(LinkList &L,int i, ElemType e); //链表插入操作 
bool ListDelete_L(LinkList &L,int i,ElemType&e);  //链表删除操作
bool read_account(char*database,LinkList&L);     //将数据库数据读入链表 
bool update_account(char*database,LinkList&L);   //将更改后链表数据写入数据库
bool compare(Bank_Account B1,Bank_Account B2);  //比较两个账户是否账户名是否相同的函数 


bool InitList_L(LinkList &L)
{
	L=new LNode;
	if(!L)return ERROR;
	L->next=NULL;
	return OK;
}

void DestroyList_L(LinkList &L)
{
	LinkList p;
	while(L)
	{
		p=L;
		L=L->next;
		delete p;
	}
}

bool ListEmpty(LinkList L)
{
	if(L->next==NULL)return true;
	return false;
}

int ListLength_L(LinkList L)
{
	//返回L中数据元素个数
    LinkList p;
    int i = 0;
    p=L->next;  //p指向第一个结点
    while(p){//遍历单链表,统计结点数
           i++;
           p=p->next;    } 
    return i;                             
}

bool GetElem_L(LinkList L,int i,ElemType &e)
{
	LinkList p=L->next;
	int j=1;
	while(p&&j<i){	//向后扫描，直到p指向第i个元素或p为空 
       p=p->next; ++j; 
     } 
    if(!p || j>i) return ERROR; 
    e=p->data; 
    return OK; 
}

int LocateElem_L (LinkList L, ElemType e)
{   
    LinkList p;
    int i=1;
	if(!ListEmpty(L)){
		  p=L->next;
        while(p &&!compare(e,p->data)) {
         p=p->next; 
         i++;
		 }
                       		
    if(p!=NULL)return i; 
	}
	return 0;
}

bool ListInsert_L(LinkList &L,int i, ElemType e)
{
	LinkList p=L,s=NULL; int j=1; 
    while(p&&j<i)
      {p=p->next;++j;}	
    if(!p||j>i)return ERROR;	
    while(s==NULL)s=new LNode;
    s->data=e;      		
    s->next=p->next;	   	
    p->next=s; 
    return OK;
}

bool ListDelete_L(LinkList &L,int i,ElemType&e)
{
    LinkList p=L; int j=1; 
    while(p->next &&j<i)
     {p=p->next; ++j;} 
    if(!(p->next)||j>i) 
       return ERROR; 
    LNode *q=p->next;  
    p->next=q->next; 	                   
    e=q->data; 	 
    delete q;                                 
    return OK; 

}

bool compare(Bank_Account B1,Bank_Account B2)
{
	return strcmp(B1.Account,B2.Account)==0;
}

bool read_account(char*database,LinkList&L)
{
	if(!InitList_L(L))return ERROR;
	fstream dat;
	dat.open(database,ios::in|ios::out|ios::binary);
    if(!dat){cerr<<"File can not be open."<<endl;
             return ERROR;
            }
    Bank_Account account1;
    dat.seekg(0,ios::beg);
    int i=0;
		       do
               {
			    account1.mod_number(i);                                           //更新账户序号
				if(i!=0&&!ListInsert_L(L,i,account1))return ERROR;                 //将数据移入链表
               	i++;
 	            dat.read((char*)&account1,sizeof(Bank_Account));             //读入文件 
 	           }while(!(End_mark(account1)));
 	dat.close();
 	return OK;
}

bool update_account(char*database,LinkList&L)
{
	fstream dat;
	Bank_Account account1;
	dat.open(database,ios::out|ios::binary);
    if(!dat){cerr<<"File can not be open."<<endl;
             return ERROR;
            }
    dat.seekp(0,ios::beg);
    LinkList p=L->next;
	while(p)
	{
		dat.write((char*)&(p->data),sizeof(Bank_Account));           //将链表内容写入文件 
		p=p->next;
	}
	dat.write((char*)&END,sizeof(Bank_Account));                   //写入结束符 
    dat.close();
    DestroyList_L(L);
    return OK;
}
