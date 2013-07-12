#include<vector>
#include<string.h>
#include<map>
#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include<cstdlib>
#include<set>
#include<time.h>
#include<string>
#include<iomanip>
#include<stdio.h>
using namespace std;

struct Node{
  unsigned int id;
  unsigned int h;
};

const unsigned int USERNUM = 200000;
Node* user = new Node[USERNUM];
unsigned int cnt;

void read_map( string filename )
{
  fstream infile(filename.c_str());
  infile>>cnt;
  unsigned int a,b;
  for (int i = 0; i < cnt; i++){
    infile>>a>>b;
    user[i].id = a;
    user[i].h = b;
  }
  infile.close();
}

bool comp(const Node& a, const Node& b){
  return a.h > b.h;
}

void sort_user()
{
  sort(user, user+cnt, comp);
}

void output( string filename )
{
  ofstream outfile( filename.c_str() );
  for (int i = 0; i < cnt; i++){
   outfile<<setw(12)<<user[i].id<<setw(12)<<user[i].h<<endl;
  }
}
int main(int argc, char** argv)
{
  if ( argc != 3 ) {
    cerr<<"Wrong input arguments.[.map] [.degree]"<<endl;
    exit(-1);
  }
  read_map( argv[1] );
  sort_user();
  output(argv[2]);
  return 0;
}
