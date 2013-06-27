#include<vector>
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
//#include<ext/hash_map>
using namespace std;
//using namespace __gnu_cxx;
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
//map<unsigned int, unsigned int> uimap;

unsigned int cnt = 0;

User user[USERNUM];
Node orderlist[USERNUM];
unsigned int iumap[USERNUM];
unsigned int LEVEL = 0;
unsigned int cursize[USERNUM];
void read_map( string infilename )
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
  srand(0);
  for (unsigned int i = 0; i < cnt; i++){
    user[i].h = -(float)(user[i].nsize)+ (float)(rand()%1000-500)/5000;
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
    // outfile<<iumap[i]<<endl;
  }
  outfile.close();
}

string getTime()
{
  char outTime[64];
  time_t t = time(0);
  strftime(outTime, sizeof(outTime), "%Y/%m/%d %X", localtime(&t));
  return outTime; 
}

int main(int argc, char** argv)
{
  if ( argc != 5 ) {
    cerr<<"Wrong input arguments. [.nedge] [.map] [.result] [level 0 for all]"<<endl;
    exit( -1 );
  }
  ofstream logfile;
  logfile.open("Timelog",ios::out | ios::app);
  LEVEL = atoi( argv[4] );
  clock_t start = clock();
  init_network( argv[1], argv[2]);
  sort_user();
  logfile<<getTime()<<endl;
  logfile<<"Executing command: "<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<argv[4]<<endl;
  logfile<<"Time on initialization: "<<((double)clock() - start)/1000000<<"s"<<endl;
  start = clock();
  paint();
  logfile<<"Time on coloring: "<<((double)clock() - start)/1000000<<"s"<<endl<<endl;
  cout<<"Processing completed."<<endl;
  logfile.close();
  output( argv[3] );
  clean();
  return 0;
}
