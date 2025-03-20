/*
*	Copyright (c) 2020,华工电信
*	All rights reserved.

*	文件名称：银行ATM机主程序.cpp
*	版本：1.0
*	作 者：赵任知 

*	Creation Date:     20200524 
*	Last Modified:     20200607 

*	摘 要：
*	本程序模拟银行ATM机实现对客户的服务 

*/
 
#include"LinkList.h"
using namespace std;
  
void Print_Welcome_info();                                        //制作界面，输出欢迎信息 
bool Account_search(char*a1,LinkList L);                          //查找账户名为字符串a1的账户是否在链表中 
int Locate_Account(char*a1,LinkList L,Bank_Account&account2);     //查找账户名为字符串a1的账户，找不到返回0，找到将该账户赋值给account2,返回它在链表中位置 
void Log_out(Bank_Account&account1,int&pos1,LinkList&L);          //account1登出系统的函数 
bool Register_account(LinkList&L);                                //注册账户函数 
void transfer_money(Bank_Account&account1,LinkList&L);            //转账函数 
void Log_in(Bank_Account&account1,int&pos1,LinkList&L);           //account1登录的函数 

int main()
{   
	 char database_name[15]="DataBase.dat";                    //设置数据库文件名	 
	 fstream dat;

	 dat.open(database_name,ios::in|ios::binary);              //检查数据库是否已被创建 
	 if(!dat){
	      cerr<<"File can not be open."<<endl;
	      cerr<<"请检查是否已经通过银行后端数据库初始化程序进行数据库初始化\n";
          abort();
      }
      dat.close();
	  
	  Print_Welcome_info();

	 Bank_Account account1;                                    //处理当前用户账户服务的变量
	 int pos1=0;                                               //当前用户账户在链表中的位置 
	 LinkList L;                                              //读入数据库账户信息的链表 
	 char button='0';                                         //获取用户输入办理业务序号变量 
	 
	 while(cin>>button)
	 {
	 	if(!read_account(database_name,L)){                      //读入账户信息入链表失败报错退出 
	 	cerr<<"Read_accounts failed!\n";
	 	abort();
	    }
	    
	    if(pos1!=0)GetElem_L(L,pos1,account1);
	    
	    start:
		if(button=='0'){
	 		Log_out(account1,pos1,L);
	 		if(!update_account(database_name,L)){             //更新数据库 
				  	  cerr<<"Update_accounts failed!\n";
				  }
			cout<<endl<<endl;
			Print_Welcome_info();
			continue;                                            //退出循环 
	 	}
	 	
	 	else{
	 		 if(button=='1'){
	 		 	  Log_out(account1,pos1,L);                     //防止未退出就注册另一账号 
				  if(!Register_account(L)){                               //输入账号错误
				      cout<<"操作已取消！\n"; 
				  	  if(!update_account(database_name,L)){             //更新数据库 
				  	      cerr<<"Update_accounts failed!\n";
				  	      abort();
				      }
		              cout<<"请输入办理业务序号：";
		              continue;
				  }
    	      }
    	      
			  else if(account1.Login){
			      double c1=0.0;
				  pos1=LocateElem_L(L,account1);          //获取account1在链表中位置
				  ListDelete_L(L,pos1,account1);              //提取账户信息进行操作，删除是为了跟后面重新插入更改后账户对应 
				  switch(button)
				  {
				  	  case '2':                             //实现功能，查询余额
						  account1.Show_Balance();
						  account1.Write_Record('2');       //记录日志 
                          break;
                      case '3':                             //实现功能，取款
					      cout<<"请输入取款金额"<<endl;
                          cin>>c1;
						  if(c1>0){
						  	if(account1.Balance_change(-c1))account1.Write_Record('3',c1);       //记录日志 
						  }
						  else cout<<"输入金额有误！"<<endl;
						  break;
                      case '4':                            //实现功能，存款 
					      cout<<"请输入存款金额"<<endl;      
                          cin>>c1;
                          if(c1>0){
                          	if(account1.Balance_change(c1))account1.Write_Record('4',c1);       //记录日志 
                          }
                          else cout<<"输入金额有误！"<<endl;
                          break;
                      case '5':                           //实现功能，转账 
                           transfer_money(account1,L);
						   break;
                       case '6':                        //实现功能，改密 
					       if(account1.Password_change())account1.Write_Record('6');       //记录日志 
                           break;
					   default:
					       ListInsert_L(L,pos1,account1);        //用户输入错误，未操作账户信息，将account1重新插回链表 
						   cout<<"输入无效！请重新输入：";
                           while(!(cin>>button)){
                           	   cin.clear();
                           	   cout<<"输入无效！退出请按\"0\"键，请重新输入：";
                           }
						   goto start;                                           //可以保持登陆状态重新输入 
                  }
                   ListInsert_L(L,pos1,account1);                                //将更改后的account1写入链表 
			  }
			  else Log_in(account1,pos1,L);
		 }
		 if(!update_account(database_name,L)){             //更新数据库 
				  	  cerr<<"Update_accounts failed!\n";
				  	  abort();
				  }
		 cout<<"请输入办理业务序号：";
	 }
	 return 0;
}

void Print_Welcome_info()
{
	//界面制作 
     cout<<"华工银行欢迎您光临"<<endl;
	 cout<<"* 本机可办业务如下："<<endl;
	 cout<<"* 1.注册"<<endl; 
	 cout<<"* 2.查询"<<endl;
	 cout<<"* 3.取款"<<endl;
	 cout<<"* 4.存款"<<endl;
	 cout<<"* 5.转账"<<endl;
	 cout<<"* 6.改密"<<endl;
	 cout<<"* 0.退出"<<endl;
	 cout<<setw(30)<<setfill('*')<<" "<<endl;
     cout<<"请输入办理业务序号：";
}

bool Account_search(char*a1,LinkList L)
{
	Bank_Account account1(0,a1);
	if(LocateElem_L(L,account1)!=0)return OK;             //LocateElem_L不等于0说明该带账户名账户在链表中 
	return ERROR;
}

int Locate_Account(char*a1,LinkList L,Bank_Account&account2)
{
	Bank_Account account1(0,a1);
	int position=LocateElem_L(L,account1);                 //获取账户位置 
	if(position==0)return 0;                               //找不到该账户,返回0 
	else{
		GetElem_L(L,position,account2);                    //将找到的对应账户写入account2 
		return position;                                   //返回位置 
	}
}

void Log_out(Bank_Account&account1,int&pos1,LinkList&L)
{
	if(account1.get_number()!=0){                     //account1的number不为0则用户已经登陆 
	 			account1.Login_fail=0;
	 			account1.Login=false;
	 			ListInsert_L(L,pos1,account1);                //更新退出系统信息 
	 			ListDelete_L(L,pos1+1,account1);              //删除原账户信息 
	 			account1=END;                                 //置服务未占用状态
	 			pos1=0;
	 		 }
}

bool Register_account(LinkList&L)
{
	char k1[31];
	char k2[31];
	cout<<"请输入账号（30位以内字母数字符号组合）：";
	cin>>k1;
	if(cin.eof()){
		cin.clear();
		return ERROR;
	}
	while(Account_search(k1,L)){
				          cout<<"该账号已被注册！请重新输入！"<<endl;
                          cin>>k1;
                          if(cin.eof()){
		                          cin.clear();
		                          return ERROR;
	                      }
   }
   cout<<"请输入密码（30位以内字母数字符号组合）：";
   cin>>k2;
   if(cin.eof()){
		cin.clear();
		return ERROR;
   }
   Bank_Account account2;
   int rear=ListLength_L(L);
   GetElem_L(L,rear,account2);        //获得链表最后一个元素即最后一个用户信息 
   Bank_Account account3(account2.get_number()+1,k1,k2);
   if(!ListInsert_L(L,rear+1,account3)){             //将新注册用户插入链表 
   cerr<<"Register_account failed!\n";
   abort();
   }
   account3.Write_Record('1');                        //记录日志 
   cout<<"注册成功!\n";
   return OK;
}

void transfer_money(Bank_Account&account1,LinkList&L)
{
	double c1=0.0;
	char d1[31];
	Bank_Account account2;
	cout<<"请输入转账目的账户：";
	cin>>d1;
	if(!strcmp(account1.Account,d1)){cout<<"不能转账至自己的账户！！"<<endl;
                                     return;
                                    }
    if(Account_search(d1,L)){
                    strcpy(account2.Account,d1);                                           //将账户名存入account2中
					int pos2=LocateElem_L(L,account2);
                    ListDelete_L(L,pos2,account2);                                       //提取对应账户信息 
					cout<<"请输入转账金额"<<endl;
					cin>>c1;
					cout<<endl;
					cout<<"当前账户："<<endl;
					if(c1>0&&account1.Balance_change(-c1)){               //余额改变函数会输出余额信息 
							       cout<<endl;
							       cout<<"被转入账户：\n";
								   account2.Balance_change(c1,true);                 //true传给参数protection,即不输出转入账户的信息
								   account1.Write_Record('5',c1,account2);       //记录日志  
					}
					else if(c1<=0)cout<<"转账金额有误！\n";
					ListInsert_L(L,pos2,account2);                                     //写入转账后的account2信息 
    }
    else cout<<"对应账户不存在！"<<endl;
}

void Log_in(Bank_Account&account1,int&pos1,LinkList&L)
{
	char a1[31];
	cout<<"请先登录账号！"<<endl;
	cout<<"请输入账户名：";
	cin>>a1;                                                           //登录 
	pos1=Locate_Account(a1,L,account1);
	if(pos1!=0&&!cin.eof()){   
			    char b1[31];
				if(account1.Login_fail==3)cout<<"密码错误次数过多，卡已被冻结，请往前台解冻。"<<endl;
				else{
				     cout<<"请输入密码：";
					 cin>>b1;
					 while(!(account1.ID_Check(b1))&&account1.Login_fail<3)cin>>b1;
					 if(account1.Login_fail==3){
							account1.Write_Record('7');       //记录日志 
							cout<<"密码错误次数过多，卡已被冻结，请往前台解冻。"<<endl;
					 }
				Bank_Account account2;
				ListInsert_L(L,pos1,account1);   //更改账户登陆相关信息插入链表 
				ListDelete_L(L,pos1+1,account2); //删除链表中的原来账户
				} 
	}
	else cout<<"账户不存在！"<<endl;
}
