#include<iostream>
#include<cstring>
#include<ctime> 
#include<cstdio>
#include<fstream>
#include<iomanip>
using namespace std; 


class Bank_Account
{public:
	Bank_Account(int n=0,char*a="\0",char*b="\0",double c=0.0);         //构造函数 
	Bank_Account(const Bank_Account &);                                 //复制构造函数 
    void operator=(const Bank_Account&);                                //重载等号运算符，功能与复制构造函数类似 
    bool ID_Check(char*a);                                              //检查字符串a是否与密码相同，以此来操作Login和Login_fail,是判断登录是否成功的函数 
    bool Balance_change(double a,bool protection=false);                //更改账户余额的函数，返回更改是否成功，输出对应提示信息，成功按照是否保护显示操作后余额 
	bool Password_change();                                             //更改密码，如三次输密码错误则返回错误 
    void Show_Balance();                                                //按小数点后2位精度输出账户余额（单位：元） 
    void Write_Record(char sign,double money,Bank_Account account2);    //按照sign种类写用户操作记录进日志文件 
	int get_number(){return this->number;}                              //返回账户编号即在链表中的位置 
	void mod_number(int num){number=num;}                               //更改账户编号 
	
	char Account[31];                                                   //公有成数据员账户名 
	bool Login;                                                         //公有数据成员登录状态 
	short Login_fail;                                                   //公有数据成员登录失败次数 
	
 private:
 	int number;                                                         //私有数据成员编号 
	char Password[31];                                                  //私有数据成员密码 
	double Balance;                                                     //私有数据成员余额（单位：元） 
};

const Bank_Account END;                                                 //数据库结束标识记录
int End_mark(Bank_Account account);                                     //判断一个账户变量是否是数据库结束标识符 

Bank_Account::Bank_Account(int n,char*a,char*b,double c)
{number=n;
 strcpy(Account,a);
 strcpy(Password,b);
 Balance=c;
 Login_fail=0;
 Login=false;
}

Bank_Account::Bank_Account(const Bank_Account&obj1)
{  Login=obj1.Login;
   number=obj1.number;
   strcpy(Account,obj1.Account);
   strcpy(Password,obj1.Password);
   Balance=obj1.Balance;
   Login_fail=obj1.Login_fail;
}

void Bank_Account::operator=(const Bank_Account&obj1)
{  Login=obj1.Login;
   number=obj1.number;
   strcpy(Account,obj1.Account);
   strcpy(Password,obj1.Password);
   Balance=obj1.Balance;
   Login_fail=obj1.Login_fail;
}

bool Bank_Account::ID_Check(char*a)
{ if(!strcmp(a,Password)){cout<<"密码正确！"<<endl;
                               Login=true;}
  else{Login_fail++;
       cout<<"密码错误！这是第"<<Login_fail<<"次输错密码，3次错误将吞卡！"<<endl;
       Login=false;
	   }
  return Login;                              //返回登录状态 
}

bool Bank_Account::Balance_change(double a,bool protection)
{bool b;
	if(Balance+a<0){cout<<"余额不足！"<<endl;
	                b=false;}
	else {Balance+=a;
	     cout<<"操作成功！"<<endl;
		 b=true;}    
	if(!protection)Show_Balance();           //如果有保护则不显示余额，在转账的被转入账户时使用 
	return b;                                //返回是否成功操作账户余额 
}

void Bank_Account::Show_Balance()
{ 
 cout<<"该账户余额为"<<setiosflags(ios::fixed)<<setprecision(2)<<Balance<<"元"<<endl;
}

bool Bank_Account::Password_change()
{ char a[31],b[31];
  int i=0;
  cout<<"请输入原始密码"<<endl;
  cin>>a;
  while(strcmp(a,Password)!=0&&(++i)<3){cout<<"密码错误！请重新输入："<<endl;
                                      cin>>a;
						             }
  if(i!=3){cout<<"密码正确！请输入新密码："<<endl;
		   cin>>b;
		   if(!cin.eof()){
		   	   strcpy(Password,b);
			   cout<<"操作成功"<<endl;
			   return true;
		   }
		   cin.clear();
		   cout<<"操作取消！\n";
		   }
  else cout<<"连续3次错误！请返回重试！"<<endl;
  return false;
}

void Bank_Account::Write_Record(char sign,double money=0.0,Bank_Account account2=END)
{
	ofstream file;                               //定义流水日志文件流类对象 
	char path[50]=".\\用户流水\\";              //流水日志文件所在文件夹路径
	strcat(path,Account);                       //将用户账户名拼接到path里 
	strcat(path,".txt");                        //加文件扩展名 
	
	time_t t = time(0);                         //获取当前时间 
    char tmp[64]; 
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X ",localtime(&t) );            //将当前时间：年月日时分秒通过 strftime函数写入字符串tmp 
    
	if(!End_mark(account2)&&sign=='5')
	{
		ofstream file2;
	    char path2[50]=".\\用户流水\\";              //流水日志文件所在文件夹路径
	    strcat(path2,account2.Account);                       //将用户账户名拼接到path里 
		strcat(path2,".txt");                        //加文件扩展名
		file2.open(path2,ios::app);
		if(!file2){
				cerr<<"Record_write failed!"<<endl;
			}
		file2<<tmp<<"   "<<"账户名为  "<<Account<<"  的账户";
		file2<<"转账"<<setiosflags(ios::fixed)<<setprecision(2)<<money<<"元";
		file2<<"入本账户\n";
		file2.close();
	}
	
	switch(sign){
		case '1':
			file.open(path,ios::out);
			file<<"账户  "<<Account<<"  的流水\n";
			file<<tmp<<"   "<<"创建账户\n";
			if(!file){
				cerr<<"请先创建\"用户流水\"子目录!\n";
			}
			file.close();
			break;
		case '2':
			file.open(path,ios::app);
			if(!file){
				cerr<<"Record_write failed!"<<endl;
			}
			file<<tmp<<"   "<<"查询余额\n";
			file.close();
			break;
		case '3':
			file.open(path,ios::app);
			if(!file){
				cerr<<"Record_write failed!"<<endl;
			}
			file<<tmp<<"   "<<"取款:"<<setiosflags(ios::fixed)<<setprecision(2)<<money<<"元\n";
			file.close();
			break;
		case '4':
			file.open(path,ios::app);
			if(!file){
				cerr<<"Record_write failed!"<<endl;
			}
			file<<tmp<<"   "<<"存款:"<<setiosflags(ios::fixed)<<setprecision(2)<<money<<"元\n";
			file.close();
			break;
		case '5':
			file.open(path,ios::app);
			if(!file){
				cerr<<"Record_write failed!"<<endl;
			}
			file<<tmp<<"   "<<"转账"<<setiosflags(ios::fixed)<<setprecision(2)<<money<<"元";
			file<<"至  账户名为  "<<account2.Account<<"  的账户\n";
			file.close();
			break;
		case '6':
			file.open(path,ios::app);
			if(!file){
				cerr<<"Record_write failed!"<<endl;
			}
			file<<tmp<<"   "<<"更改密码\n";
			file.close();
			break;
		case '7':
			file.open(path,ios::app);
			if(!file){
				cerr<<"Record_write failed!"<<endl;
			}
			file<<tmp<<"   "<<"输错密码次数过多，账户被冻结\n";
			file.close();
			break;
		case '8':
			file.open(path,ios::app);
			if(!file){
				cerr<<"Record_write failed!"<<endl;
			}
			file<<tmp<<"   "<<"成功解冻账户\n";
			file.close();
			break;
		case '9':
			file.open(path,ios::app);
			if(!file){
				cerr<<"Record_write failed!"<<endl;
			}
			file<<tmp<<"   "<<"注销账户，不再使用\n";
			file<<endl;
			file<<"以上是该账户全部流水\n";
			file.close();
			break;
	}
} 

int End_mark(Bank_Account account)
{
	return !(account.get_number());                          //写入数据库的记录只有结束符number为0 
}
