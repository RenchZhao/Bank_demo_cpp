#include"LinkList.h" 
using namespace std;

bool print_record(char*Account);                           //打印用户的交易流水 
bool unfreeze(char*Account,LinkList&L);                   //解冻用户的账户 
bool delete_account(char*Account,LinkList&L);             //注销用户的账户 

int main() 
{
 LinkList L;
 char start;
 char database_name[15]="DataBase.dat";
 cout<<"银行前台后端控制系统"<<endl;
 cout<<"按Control+Z退出，按任意键继续:";
 while(cin>>start){
     char a1[31];
     char sign;
     
	 cout<<"请输入账户名：";
	 cin>>a1;	 	 
	 if(cin.eof())return 0;
	 Bank_Account account1(1,a1);

	 cout<<"* 1.解冻账户"<<endl;
	 cout<<"* 2.打印流水"<<endl;
	 cout<<"* 3.注销账户"<<endl;
	 cout<<"请输入：";
	 start1:cin>>sign;

	 switch(sign)
	 {
	 	case'1':
	 		if(!read_account(database_name,L)){
	 			cerr<<"Read_Account failed!"<<endl;
	 			cerr<<"按Control+Z退出，按任意键继续:";
	 			continue;
	 		}
	 		if(!unfreeze(a1,L)){
	 			cout<<"找不到该账户!"<<endl;
	 			cout<<"按Control+Z退出，按任意键继续:";
	 			continue;
	 		}
	 		if(!update_account(database_name,L)){
	 			cerr<<"Update_Account failed!"<<endl;
	 			cerr<<"按Control+Z退出，按任意键继续:";
	 			continue;
	 		}
			account1.Write_Record('8');       //记录日志 
	 		cout<<"操作成功！\n";
	 		break;
	 	case'2':
	 		if(!print_record(a1)){
	 			cout<<"找不到该账户!"<<endl;
	 			cout<<"按Control+Z退出，按任意键继续:";
	 			continue;
	 		}
	 		break;
	 	case'3':
	 		cout<<"是否确定注销该账户？输入\"N\"退出:";
	 		cin>>start;
	 		if(cin.eof()||start=='N'||start=='n'){
	 			cin.clear();
	 			cout<<"操作取消。\n";
				cout<<"按Control+Z退出，按任意键继续:";
				continue;
	 		}
	 		if(!read_account(database_name,L)){
	 			cerr<<"Read_Account failed!"<<endl;
	 			cerr<<"按Control+Z退出，按任意键继续:";
	 			continue;
	 		}
	 		if(!delete_account(a1,L)){
	 			cout<<"找不到该账户!"<<endl;
	 			cout<<"按Control+Z退出，按任意键继续:";
	 			continue;
	 		}
	 		else account1.Write_Record('9');       //记录日志，该账户解冻成功 
	 		if(!update_account(database_name,L)){
	 			cerr<<"Update_Account failed!"<<endl;
	 			cerr<<"按Control+Z退出，按任意键继续:";
	 			continue;
	 		}
	 		cout<<"操作成功！\n";
	 		break;
	 	default:
	 		if(cin.eof())return 0;
	 		cout<<"输入错误,请重新输入:"<<endl;
	 		goto start1;
	 }
	 cout<<"按Control+Z退出，按任意键继续:";
 }
 
}


bool print_record(char*Account)
{
	fstream file;                               //定义流水日志文件流类对象 
	char path[50]=".\\用户流水\\";              //流水日志文件所在文件夹路径 
	strcat(path,Account);                       //将用户账户名拼接到path里 
	strcat(path,".txt");                        //加文件扩展名
	file.open(path,ios::in);
	if(!file)return ERROR;
	file.seekg(0,ios::beg);
	char readlines[101];
	file.getline(readlines,100);               //略去标题行
	while(file.getline(readlines,100)){
		cout<<readlines<<endl;                 //打印流水文件信息，现实中应输出到打印机 
	} 
	file.close();
	return OK;
}

bool unfreeze(char*Account,LinkList&L)
{
	Bank_Account account1(0,Account);
	int position=LocateElem_L(L,account1);
	if(position==0)return ERROR;                     //找不到对应账户，报错 
	GetElem_L(L,position,account1);         //获取对应账户信息 
	if(account1.Login_fail==3){                      //解冻账户 
	   account1.Login_fail=0;
	   ListInsert_L(L,position,account1);        //将账户的信息更新 
	   ListDelete_L(L,position+1,account1);      //删除旧账户 
	}
	return OK;
}

bool delete_account(char*Account,LinkList&L)
{
	Bank_Account account1(0,Account);
	int position=LocateElem_L(L,account1);
	if(position==0)return ERROR;                             //找不到对应账户，报错
	ListDelete_L(L,position,account1);                       //删除旧账户 
	return OK;
}
