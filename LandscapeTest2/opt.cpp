#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include<cstdlib>
#include<set>
#include<string>
#include<iomanip>
#include<stdio.h>
using namespace std;


struct User{
  int idx;
  int* nlist;
  int nsize;
  double h;
  int color;
  int level;
};

struct Node{
  int idx;
  double h;
};

const int USERNUM = 10000;
map<int, int> uimap;
map<int, int> iumap;
unsigned int cnt = 0;

User user[USERNUM];
Node orderlist[USERNUM];

int cursize[USERNUM];
void read_idmap( string infilename )
{
  FILE* infile = fopen( infilename.c_str(), "r");
  if ( infile == NULL ) {
    cerr << "Fail to open file: "<<infilename<<endl;
    exit( -1 );
  }
  fscanf(infile, "%d", &cnt);
  int i,u;
  for (int k = 0; k < cnt; k++) {
    fscanf(infile, "%d %d", &i, &u);
    uimap[u] = i;
    iumap[i] = u;
  }
  fclose( infile );
}
void set_h()
{
  srand(0);
  for (int i = 0; i < cnt; i++){
    user[i].h = -(float)(user[i].nsize)+ (float)(rand()%1000-500)/5000;
  }
}

void init_network( string edgefilename, string idmapfilename )
{
  read_idmap( idmapfilename );
  for (int i = 0; i < cnt; i++) {
    user[i].idx = i;
    user[i].nsize = 0;
    user[i].color = -1;
    user[i].level = 1e5;
  }
  
  //get neighbor list size, set h
  int ui,uj;
  FILE* instream = fopen( edgefilename.c_str(),"r" );
  while ( fscanf(instream, "%d %d", &ui, &uj)!= EOF ) {
    user[uimap[ui]].nsize++;
    user[uimap[uj]].nsize++;
  }
  fclose(instream);
  for(int i = 0; i < cnt; i++) {
    user[i].nlist = new int[user[i].nsize];
  }
  set_h();
  user[4].h = user[10].h + 0.01;
  user[20].h = user[18].h + 0.01;
  
  //create neighbor list
  memset(cursize, 0, sizeof(int)*cnt);
  instream = fopen( edgefilename.c_str(),"r" );
  int i,j;
  while ( fscanf(instream, "%d %d", &ui, &uj)!= EOF ) {
    i = uimap[ui];
    j = uimap[uj];
    *(user[i].nlist + cursize[i]) = j;
    *(user[j].nlist + cursize[j]) = i;
    cursize[i]++;
    cursize[j]++;
  }
  fclose( instream ); 
}

void clean()
{
  for (int i = 0; i < cnt; i++){
    delete [] user[i].nlist;
  }
}



bool comph( const Node &a, const Node &b)
{
  return a.h < b.h;
}

void sort_user()
{
  for (int i = 0; i < cnt; i++){
    orderlist[i].idx = i;
    orderlist[i].h = user[i].h;
  }
  sort(orderlist, orderlist+cnt, comph);
}



void paint()
{
  int level = 0;
  bool finish = false;
  int cur = 0;
  while ( !finish ){
    for (int i = 0; i < cnt; i++){
      int idx = orderlist[i].idx;
      if( user[idx].color != -1 ) {
	continue;
      }
      

      //empty
      int j = 0;
      for (; j < user[idx].nsize; j++){
	if ( user[user[idx].nlist[j]].h< user[idx].h 
	     && user[user[idx].nlist[j]].level >= level) {
	  break;
	}
      }
      if ( j == user[idx].nsize) {
	user[idx].color = user[idx].idx; //change
	user[idx].level = level;
	cur++;	
	continue;
      }
      
      
      if ( user[user[idx].nlist[j]].color == -1 ) {
      	continue;
      }
      
      int a = user[user[idx].nlist[j]].color;
      for(; j <user[idx].nsize; j++)
      	{
      	  if ( user[user[idx].nlist[j]].h < user[idx].h 
      	       && user[user[idx].nlist[j]].level >= level 
      	       && user[user[idx].nlist[j]].color != a)
      	    {
      	      break;
      	    }
      	}
      if ( j != user[idx].nsize ) {
      	continue;
      } else {
      	user[idx].color = a;
      	user[idx].level = level;
      	cur++;
      }
    }
    level++;
    if ( cur == cnt) finish = true;
      
  }
}

void output()
{
  for (int i = 0; i < cnt; i++){
    cout<<setw(8)<<iumap[i]<<setw(8)<<user[i].color<<setw(8)<<user[i].level<<endl;
  }
}


int main()
{
  clock_t start = clock();
  init_network( "karate.edge", "karate.idmap");
  sort_user();
  //cout<<clock() - start<<endl;
  start = clock();

  // for (int i = 0; i < cnt; i++){
  //   cout<<iumap[i]<<" ";
  //   for (int j = 0; j < user[i].nsize; j++){
  //     cout<<setw(4)<<user[i].nlist[j];
  //   }
  //   cout<<endl;
  // }
  // for(int i = 0; i < cnt; i++){
  //   cout<<user[i].h<<endl;
  // }
  for (int i = 0;i < cnt;i++){
    cout<<orderlist[i].idx<<endl;
  }
  paint();
  //cout<<clock() - start<<endl;
  // output();
  clean();
  return 0;
}
