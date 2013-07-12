/*************************************************
Copyright:Yaning Liu@pku
Author: Yaning Liu
Date:2011-12
Description:计算新浪微博的pagerank
**************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <map>
#include <list>
#include <cstdlib>
#include <cstring>
#include <string>
#include <memory.h>
#include <algorithm>
#include <vector>
#include <cmath>
#define SIZE 10000  /*weibo的用户数量*/
#define FocusTweetNum 10   /*关注转发数前10的Tweet*/
using namespace std;

const int userNum = SIZE;

int scoreIdx; /*迭代后用于存储最后Pagerank得分相应的index*/

/*用户信息*/
struct user
{
  int idx;  /*用户的index，即id.final用户对应的行数，在这里作为程序中的索引*/
  string name; /*用户的昵称*/
  int outDegree; /*用户Follow的人数*/
  vector<int> followers; /*Follow该用户人的index列表*/
  double score[2]; /*用户的Pagerank分数，根据scoreIdx判断收敛时，即有意义的分数*/
  int folSz;
} users[SIZE + 10];

map<string,int> idToIndex; /*用户id到index的对应*/
map<string,int> nameToIndex; /*用户昵称到index的对应*/


double sum = 1.0; /*pagerank和*/
double rem = 0.15; /*pagerank保留比例*/
double bound = 0.000001; /*pagerank迭代终止值*/


bool followerNumCmp(user a, user b)
{
  return a.folSz > b.folSz;
}

bool scoreCmp(user a, user b)
{
  return a.score[scoreIdx] > b.score[scoreIdx];
}

bool idxCmp(user a, user b)
{
  return a.idx < b.idx;
}

/*************************************************
Description:    // 得到当前时间
Return:         // 当前时间的字符串类型
*************************************************/
string nowTime()
{
  char outTime[64];
  time_t t = time(0);
  strftime(outTime, sizeof(outTime), "%Y/%m/%d %X", localtime(&t));
  return outTime;
}

/*************************************************
Description:    // 统计用户基本信息以及与index对应关系
Input:          idPath：用户id路径
                namePath：用户namePath
Output:         // 执行状态至标准输出
Return:         // 是否成功执行
Others:         // 要求idPath与namePath中每行数据一一对应
*************************************************/
bool initUser(string idPath,string namePath)
{
  ifstream idIn(idPath.c_str());
  ifstream nameIn(namePath.c_str());
  if(idIn == NULL)
    {
      cout << "can't open id file, " << "path: " << idPath << endl;
      getchar();
      return 0;
    }

  cout << nowTime() << ": Begin init user id." << endl;
  for(int i = 0; i < userNum; i++)
    {
      string id;
      idIn >> id;
      string name;
      nameIn >> name;
      if(idToIndex.find(id) == idToIndex.end())
        {
	  idToIndex[id] = i;
        }
      users[i].idx = i;
      users[i].name = name;
      users[i].folSz = 0;
    }
  idIn.close();
  nameIn.close();
  cout << nowTime() << ": Complete init user id." << endl;
  return 1;
}

/*************************************************
Description:    // 构建用户间的Follow关系
Input:          // 用户间follow关系的文件路径
Output:         // 执行状态至标准输出
Return:         // 是否成功执行
Others:         // 用户间follow关系1 2表示id为2的用户follow id为1的用户
*************************************************/
bool initNet(string path)
{
  cout << nowTime() << ": Begin init net." << endl;
  for(int i =0; i < userNum; i++)
    {
      users[i].outDegree = 0;
    }
  ifstream in(path.c_str());
  if(in == NULL)
    {
      cout << "can't open net file, " << "path: " << path << endl;
      getchar();
      return 0;
    }
  string id, fol;
  int idx, folx;
  while(in >> fol >> id)
    {
      if(idToIndex.find(fol) == idToIndex.end() ||
	 idToIndex.find(id) == idToIndex.end()) {
	continue;
      }
      idx = idToIndex[id];
      folx = idToIndex[fol];
      users[idx].followers.push_back(folx);
      users[idx].folSz++;
      users[folx].outDegree++;
    }
  in.close();
  cout << nowTime() << ": Complete init net." << endl;
  return 1;
}

/*************************************************
Description:    // 计算每个用户的pagerank
Output:         // 执行状态至标准输出
Return:         // 迭代终止时score应对应的下标
*************************************************/
int calPagerank()
{
  cout << nowTime() << ": Begin calculate pagerank." << endl;
  int oldIdx = 0;
  int newIdx = 1;
  for(int i = 0; i < userNum; i++)
    {
      users[i].score[oldIdx] = sum/userNum;
    }
  for(int iteration=0; iteration <= 200; iteration++)
    {
      cout << "iteration: " << iteration << endl;
      for(int i =0;i<userNum;i++)
	{
	  users[i].score[newIdx] = 0;
	}
      double perSum = 0;
     
      for(int i = 0; i < userNum; i ++)
	{
	  users[i].score[newIdx] = sum*rem/userNum;
	  if(users[i].folSz > 0)
	    {
	      for(vector<int>::iterator iter = users[i].followers.begin(); iter != users[i].followers.end(); iter++)
		{
		  int idx = *iter;
		  users[i].score[newIdx] += (1-rem)*users[idx].score[oldIdx]/users[idx].outDegree;
		}
	    }
	  perSum += users[i].score[newIdx];
	}
      for(int i = 0; i < userNum; i ++)
	{
	  users[i].score[newIdx] = users[i].score[newIdx] / perSum;
	}

      double dif = 0;
      for(int i = 0; i < userNum; i ++)
	{
	  dif += abs(users[i].score[newIdx] - users[i].score[oldIdx]);
	}

      if( dif < bound )
	{
	  break;
	}
      oldIdx = newIdx;
      newIdx = 1 - oldIdx;
    }
  cout << nowTime() << ": Complete calculate pagerank." << endl;
  return newIdx;
}

/*************************************************
Description:    // 将pagerank数据写到文件中
Input:          // 输出到的文件路径
Output:         // 执行状态至标准输出，pagerank信息至文件
Return:         // 是否执行成功
Others:         // 输出形式：a  0.001 表示昵称为a的用户的pagerank值为0.001
*************************************************/
bool pagerankWriteToFile(string outPath)
{
  cout << nowTime() << ": Begin write pagerank to file." << endl;
  ofstream out(outPath.c_str(), ios::trunc|ios::out);
  if(out == NULL)
    {
      cout << "can't open pagerank file, path: " << outPath << endl;
      getchar();
      return 0;
    }
  out << setiosflags(ios::fixed) << setprecision(10);

  sort(users, users + userNum, scoreCmp);
  for(int i = 0; i < userNum; i++)
    {
      out << users[i].name << "\t" << users[i].score[scoreIdx] << endl;
    }
  sort(users,users+userNum, idxCmp);
  out.close();
  cout << nowTime() << ": Complete write pagerank to file." << endl;
  return 1;
}

/*************************************************
Description:    // 将Follower数量数据写到文件中
Input:          // 输出到的文件路径
Output:         // 执行状态至标准输出，Follower数量至文件
Return:         // 是否执行成功
Others:         // 输出形式：a  1000 表示昵称为a的用户的Follower数量为1000
*************************************************/
bool followerNumWriteToFile(string path)
{
  cout << nowTime() << ": Begin write follower number to file." << endl;
  ofstream out(path.c_str(),ios::trunc|ios::out);
  if(out == NULL)
    {
      cout << "can't open follower number file, path: " << path << endl;
      getchar();
      return 0;
    }
  sort(users, users+userNum, followerNumCmp);
  for(int i = 0; i < userNum; i++)
    {
      out << users[i].name << "\t" << users[i].folSz << endl;
    }
  sort(users, users+userNum, idxCmp);
  cout << nowTime() << ": Complete write follower number to file." << endl;
  out.close();
  return 1;
}

/*************************************************
Description:    // 主函数
Input:          // argv[1] 用户id文件路径，第i行用户的index为i
                // argv[2] 用户昵称文件路径，第i行用户的index为i
		// argv[3] 用户间的follow关系，1 2：表示id为2的用户follow id为1的用户
		// argv[4]--argv[10] 数量1至7，可选，tweets文件路径
Output:			// 函数执行状态至标准输出
Return:         // 是否执行成功
Others:         // 其它说明
*************************************************/
int main(int argc,char *argv[])
{
  if(argc != 4)
    {
      cout << "parameter input error."<<endl;
      return 0;
    }
  string idPath = argv[1];
  string namePath = argv[2];
  string netPath = argv[3];
  if(!initUser(idPath, namePath)) {
    return 0;
  }
  if(!initNet(netPath)) {
    return 0;
  }
  scoreIdx = calPagerank();
  if(!pagerankWriteToFile("finalpagerank.txt")) {
    return 0;
  }
  if(!followerNumWriteToFile("followerNum.final")){
    return 0;
  }
  cout << "All process complete."<< endl;
  return 1;
}

