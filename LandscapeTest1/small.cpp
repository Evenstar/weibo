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
using namespace std;

struct User{
  int idx;
  set<int> nset;
  double h;
  int color;
  int level;
};

struct Node{
  int idx;
  double h;
};

vector<User> user;
map<int, int> uimap;
map<int, int> iumap;
unsigned cnt = 0;
vector<Node> orderlist;

void create_idmap( string infilename, string outfilename )
{
  ifstream edgeinput( infilename.c_str() );
  if ( edgeinput.fail() ) {
    cerr << "Fail to open file: "<< infilename<<endl;
    exit( -1 );
  }
  int a;
  set<int> idset;
  vector<int> idlist;
  cout<<"Processing file: "<<infilename<<endl;
  while( edgeinput >> a )
    {
      if ( idset.find( a ) == idset.end() ) {
	idset.insert( a );
	idlist.push_back( a );
      }
    }
  edgeinput.close();
  cout<<"Processing completed."<<endl;
  cout<<"Writing file: "<<outfilename<<endl;
  ofstream mapoutfile( outfilename.c_str() );
  mapoutfile<<idlist.size()<<endl<<endl;
  vector<int>::iterator it = idlist.begin();
  for( ; it != idlist.end(); it++) {
    mapoutfile<<cnt<< " "<<(*it)<<endl;
    cnt++;
  }
  mapoutfile.close();
  cout<<"Writing file completed."<<endl;
}

void read_idmap( string infilename )
{
  ifstream infile( infilename.c_str() );
  if ( infile.fail() ) {
    cerr<<"Fail to open file: "<<infilename<<endl;
    exit(-1);
  }
  infile>>cnt;
  int i,u;
  while( infile>>i>>u )
    {
      uimap[u] = i;
      iumap[i] = u;
    }
  infile.close();
  //  map<int,int>::iterator it = uimap.begin();
  // for(; it != uimap.end(); it++) {
  // cout<<it->first<<" "<<it->second<<endl;
  // }
}


void init_network( string edgefilename, string idmapfilename)
{
  read_idmap( idmapfilename );
  for (int i = 0; i < cnt; i++){
    User node;
    node.idx = i;
    user.push_back(node);
  }
  
  int ui,uj;
  ifstream edgeinput( edgefilename.c_str() );
  if ( edgeinput.fail() ) {
    cerr << "Fail to open file: "<< edgefilename<<endl;
    exit( -1 );
  }
  while ( edgeinput >> ui >> uj)
    {
      user[uimap[ui]].nset.insert(uimap[uj]);
      user[uimap[uj]].nset.insert(uimap[ui]);
    }
  // vector<Node>::iterator it = user.begin();
  // for (; it != user.end(); it++){
  //   cout<< it->idx<<":";
  // set<int>::iterator sit = it->nset.begin();
  // for(; sit !=  it->nset.end(); sit++)
  //   cout<<(*sit)<<" ";
  // cout<<endl;
  // }
}

void init_user()
{
  srand(0);
  vector<User>::iterator it = user.begin();
  for(; it != user.end(); it++){
    it->h = -(double) it->nset.size() + (double)(rand()%1000-500)/5000;
    it->color = -1;
    it->level = 1e6;
    //  cout<<it->idx<<" "<<it->h<<endl;
  }
  user[4].h = user[10].h + 0.01;
  user[20].h = user[18].h + 0.01;
  
}

bool comph( const Node &a, const Node &b)
{
  return a.h < b.h;
}

void sort_user()
{
  for (int i = 0; i < user.size(); i++){
    Node node;
    node.idx = i;
    node.h = user[i].h;
    orderlist.push_back(node);
  }
  vector<Node>::iterator it = orderlist.begin();

  sort(orderlist.begin(), orderlist.end(), comph);
  // for( ; it != orderlist.end(); it++) {
  //   cout<<iumap[it->idx]-100<< " "<<it->h<<" ";
  //   set<int>::iterator sit = user[it->idx].nset.begin();
  //   for(; sit != user[it->idx].nset.end(); sit++) {
  //     cout<< iumap[(*sit)]-100<<" ";
  //   }
  //   cout<<endl;
  // }
}

set<int> Color[34];
void paint()
{
  int level = 0;
  bool finish = false;
  int cnt = 0;
  for (int k = 0; k < 3; k++) {
    set<int> C;
    for (int i = 0; i < user.size(); i++) {
      int idx = orderlist[i].idx;
      if ( user[idx].color != -1 ) {
	continue;
      }
      set<int> fset; //feasible set
      set<int>::iterator it = user[idx].nset.begin();
      for( ; it != user[idx].nset.end(); it++){
	int j = (*it);
	if ( user[j].h < user[idx].h &&  user[j].level >= level) {
	  fset.insert( user[j].color );
	}
      }
      if ( fset.empty() ) {
	C.insert( idx );
	user[idx].color = user[idx].idx;
	user[idx].level = level;
	cnt++;
      } else if ( fset.size()  == 1 && *(fset.begin()) != -1) {
	int j = *(fset.begin());
	user[idx].color = user[j].color;
	user[idx].level = level;
	cnt++;
      }
    }      
    level++;
    if (cnt == user.size()) finish = true;
    }
}


int main()
{
  // create_idmap("karate.edge","karate.idmap");
  // read_idmap("karate.idmap");
  init_network("karate.edge","karate.idmap");
  init_user();
  sort_user();
  // for (int i = 0; i < 34; i++)
  //   cout<<iumap[i]-100<<" "<<user[i].level<<endl;
  paint();
    for (int i = 0; i < 34; i++) 
      cout<<setw(6)<<iumap[i]-100<<setw(6)<<user[i].color<<setw(6)<<user[i].level<<endl;
     
   return 0;
}
