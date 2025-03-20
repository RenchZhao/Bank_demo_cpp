#include<cstdlib>
#include <string> 
#include"Bank_Account.h"
using namespace std;
int main()
{ fstream initializer;
  char file_way[500];
  char c;
  cout<<"请将数据库文件与主程序放置在同一个存储路径中，并将其文件名设为DataBase.dat\n"; 
  cout<<"请输入文件存储路径：";
  cin>>file_way;
  cout<<"文件是否已经初始化？是则输入Y,否则输入N:";
  cin>>c;
  if(c=='N'||c=='n'){cout<<"若已建立文件则将覆盖！输入N停止。按其它键继续。"<<endl;
                     cin>>c; 
                     if(c=='N') {
					             cout<<"操作结束。"<<endl; 
								 return 0;
                                }
                     initializer.open(file_way,ios::out|ios::binary);
                     if(!initializer)  {cerr<<"Initializer failed."<<endl;
                                        abort();
                                       }
                     initializer.seekp(0,ios::beg);
                     initializer.write((char*)&END,sizeof(Bank_Account));                     //写入数据库文件结束符 
                     cout<<"操作成功！"<<endl; 
                     initializer.close();
					}
  cout<<"是否已经建立\"账户流水\"文件夹?是则输入Y,否则输入N:";
  cin>>c;
  if(c=='N'||c=='n'){
  	  string path;
  	  path="md .\\用户流水";
  	  system(path.c_str());                          //在程序当前目录下创建“用户流水”文件夹 
  	  cout<<"已成功创建\"账户流水\"文件夹在程序当前目录!"<<endl;
  }
  return 0;
} 
