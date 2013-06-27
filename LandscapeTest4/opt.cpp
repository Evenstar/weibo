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
  unsigned int idx;
  unsigned int* nlist;
  unsigned int nsize;
  double h;
  int color;
  unsigned int level;
};

struct Node{
  unsigned int idx;
  double h;
};

const unsigned int USERNUM = 1000000;
map<unsigned int, unsigned int> uimap;
map<unsigned int, unsigned int> iumap;
unsigned int cnt = 0;

User user[USERNUM];
Node orderlist[USERNUM];
unsigned int LEVEL = 0;
unsigned int cursize[USERNUM];
void read_idmap( string infilename )
{
  FILE* infile = fopen( infilename.c_str(), "r");
  if ( infile == NULL ) {
    cerr << "Fail to open file: "<<infilename<<endl;
    exit( -1 );
  }
  fscanf(infile, "%d", &cnt);
  unsigned int i,u;
  for (unsigned int k = 0; k < cnt; k++) {
    fscanf(infile, "%d %d", &i, &u);
    uimap[u] = i;
    iumap[i] = u;
  }
  fclose( infile );
}
void set_h()
{
  srand(0);
  for (unsigned int i = 0; i < cnt; i++){
    user[i].h = -(float)(user[i].nsize)+ (float)(rand()%1000-500)/5000;
  }
}

void init_network( string edgefilename, string idmapfilename )
{
  read_idmap( idmapfilename );
  for (unsigned int i = 0; i < cnt; i++) {
    user[i].idx = i;
    user[i].nsize = 0;
    user[i].color = -1;
    user[i].level = 1e5;
  }
  
  //get neighbor list size, set h
  unsigned int ui,uj;
  FILE* instream = fopen( edgefilename.c_str(),"r" );
  while ( fscanf(instream, "%d %d", &ui, &uj)!= EOF ) {
    user[uimap[ui]].nsize++;
    user[uimap[uj]].nsize++;
  }
  fclose(instream);
  for(unsigned int i = 0; i < cnt; i++) {
    user[i].nlist = new unsigned int[user[i].nsize];
  }
  set_h();
  user[4].h = user[10].h + 0.01;
  user[20].h = user[18].h + 0.01;
  
  //create neighbor list
  memset(cursize, 0, sizeof(unsigned int)*cnt);
  instream = fopen( edgefilename.c_str(),"r" );
  unsigned int i,j;
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
  for (unsigned int i = 0; i < cnt; i++){
    delete [] user[i].nlist;
  }
}



bool comph( const Node &a, const Node &b)
{
  return a.h < b.h;
}

void sort_user()
{
  for (unsigned int i = 0; i < cnt; i++){
    orderlist[i].idx = i;
    orderlist[i].h = user[i].h;
  }
  sort(orderlist, orderlist+cnt, comph);
}



void paint()
{
  unsigned int level = 0;
  bool finish = false;
  unsigned int cur = 0;
  while (!finish){
    for (unsigned int i = 0; i < cnt; i++){
      unsigned int idx = orderlist[i].idx;
      if( user[idx].color != -1 ) {
	continue;
      }
      

      //empty
      unsigned int j = 0;
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
    if ( LEVEL == 0 && cnt == cur) {
      finish = true;
    } else if ( LEVEL != 0 && level == LEVEL + 1 ) {
      finish = true;
    }
        
  }
}

void output(string filename)
{
  ofstream outfile( filename.c_str() );
  for (unsigned int i = 0; i < cnt; i++){
    outfile<<setw(8)<<iumap[i]<<setw(8)<<user[i].color<<setw(8)<<user[i].level<<endl;
  }
  outfile.close();
}


int main(int argc, char** argv)
{
  if ( argc != 5 ) {
    cerr<<"Wrong input arguments. [.edge] [.idmap] [.result] [level 0 for all]"<<endl;
    exit( -1 );
  }
  LEVEL = atoi( argv[4] );
  clock_t start = clock();
  init_network( argv[1], argv[2]);
  sort_user();
  cout<<"Time on initialization: "<<((double)clock() - start)/1000000<<"s"<<endl;
  start = clock();
  paint();
  cout<<"Time on coloring: "<<((double)clock() - start)/1000000<<"s"<<endl;
  output( argv[3] );
  clean();
  return 0;
}
