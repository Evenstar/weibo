// Install libjsoncpp first
// http://jsoncpp.sourceforge.net/
// http://blog.csdn.net/hzyong_c/article/details/7163589

// compile with
// g++ readweibo3.cpp -ljsoncpp

#include <iostream>
#include <fstream>
#include "json.h"

using namespace std;

int main()
{
  string st;
  ifstream fin("weibo_content_sample");
  Json::Reader reader; 
  Json::Value root;

  while (getline(fin, st)) {
    cout<<st<<endl;
    reader.parse(st,root);
    cout<<root["a"];
    cout<<root["b"];
    cout<<root["c"];
    cout<<endl;
  }

  return 0;
}
