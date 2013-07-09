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
unsigned int cnt = 0;
unsigned int STEP = 20;

User* user = new User[USERNUM];
unsigned int* iumap = new unsigned int[USERNUM];
unsigned int* cursize = new unsigned int[USERNUM];
double alpha = 0.01;

void read_map(string infilename )
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
    iumap[k] = i;
    user[k].nsize = u;
  }
  fclose( infile );
}

void set_h()
{
  for (unsigned int i = 0; i < cnt; i++)
    {
      user[i].h = 1.0/cnt;
    }
}

void onestep()
{
  for (unsigned int i = 0; i < cnt; i++)
    {
      double s = 0;
      for (unsigned int j = 0; j < user[i].nsize; j++)
	{
	  s += user[j].h;
	}
      user[i].h = user[i].h*alpha + (1-alpha)*s;
    }
}

void init_network( string edgefilename, string idmapfilename )
{
  read_map( idmapfilename );
  for (unsigned int i = 0; i < cnt; i++) {
    user[i].idx = i;
    user[i].color = -1;
    user[i].level = 1e5;
  }
  set_h();
  
  for(unsigned int i = 0; i < cnt; i++) {
    user[i].nlist = new unsigned int[user[i].nsize];
  }
  
  FILE* instream;
  //create neighbor list
  memset(cursize, 0, sizeof(unsigned int)*cnt);
  instream = fopen( edgefilename.c_str(),"r" );
  unsigned int i,j;
  while ( fscanf(instream, "%d %d", &i, &j)!= EOF ) {
    *(user[i].nlist + cursize[i]) = j;
    *(user[j].nlist + cursize[j]) = i;
    cursize[i]++;
    cursize[j]++;
  }
  fclose( instream ); 
}


void iterate()
{
  for (int i = 0; i < STEP; i++)
    {
      onestep();
    }
}

void clean()
{
  for (unsigned int i = 0; i < cnt; i++){
    delete [] user[i].nlist;
  }
  delete [] user;
  delete [] iumap;
  delete [] cursize;
}

void output()
{
  for(unsigned int i = 0; i < cnt; i++)
    {
      cout<<user[i].h<<endl;
    }
}
int main( int argc, char** argv)
{
    if ( argc != 5) {
    cerr<<"Wrong input arguments. [.nedge] [.map] [.result] [step]"<<endl;
    exit( -1 );
  }
   
    STEP = atoi(argv[4]);
    init_network(argv[1], argv[2]);
    iterate();
    output();
    clean();
   
    return 0;
    
}
