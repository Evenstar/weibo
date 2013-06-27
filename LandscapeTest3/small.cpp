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
  vector<int> nset;
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
unsigned int cnt = 0;
vector<Node> orderlist;

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
    // FILE* instream;
    // instream = fopen(edgefilename.c_str(),"rb");
    // while( fscanf(instream, "%d %d", &ui,&uj)!= EOF) {
    //     user[uimap[ui]].nset.push_back(uimap[uj]);
    //     user[uimap[uj]].nset.push_back(uimap[ui]);
    // }
  while ( edgeinput >> ui >> uj)
    {
      user[uimap[ui]].nset.push_back(uimap[uj]);
      user[uimap[uj]].nset.push_back(uimap[ui]);
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

void set_h()
{
  srand(0);
  vector<User>::iterator it = user.begin();
  for(; it != user.end(); it++){
    it->h = -(double) it->nset.size() + (double)(rand()%1000-500)/5000;
  }
}

void init_user()
{
 
  vector<User>::iterator it = user.begin();
  for(; it != user.end(); it++){
    it->color = -1;
    it->level = 1e5;
  }
  set_h();
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

void paint()
{
  int level = 0;
  bool finish = false;
  int cnt = 0;
  while (!finish) {
      for (int i = 0; i < user.size(); i++) {
      int idx = orderlist[i].idx;
      if ( user[idx].color != -1 ) {
	continue;
      }

      //Most TIME consuming!!!
      set<int> fset; //feasible set
      vector<int>::iterator it = user[idx].nset.begin();
      for( ; it != user[idx].nset.end(); it++){
	int j = (*it);
	if ( user[j].h < user[idx].h &&  user[j].level >= level) {
	  fset.insert( user[j].color );
	  if ( fset.size() > 1) {
	    break;
	  }
	} //This guarantees the processing are on the restricted set.
      }
      //--M

      if ( fset.empty() ) {
	user[idx].color = user[idx].idx;
	user[idx].level = level;
	cnt++;
      } else if ( fset.size()  == 1 && *(fset.begin()) != -1) {//effect blank
	int j = *(fset.begin());
	user[idx].color = user[j].color;
	user[idx].level = level;
	cnt++;
      }
    }      
    level++;
    if (cnt == user.size()) finish = true;
  }
  // cout<<"Level"<<level<<endl;
}

void output()
{
  for (int i = 0; i < cnt; i++) 
    cout<<setw(8)<<iumap[i]
	<<setw(8)<<user[i].color<<setw(8)<<user[i].level<<endl;
}

int main(int argc, char** argv)
{
  //  if ( argc != 3 ) {
  //  cerr<<"Wrong arguments. [.edge] [.idmap]"<<endl;
  // exit(-1);
  // }
   clock_t start = clock();
  //init_network(argv[1],argv[2]);
  init_network("network.edge","network.idmap");
   clock_t end = clock();
   cout<<end - start<<endl;
  start = clock();
  init_user();
   cout<<clock() - start<<endl;
  start = clock();
  sort_user();
  end = clock();
  cout<<end - start<<endl;
  start = clock();

  // for (int i = 0;i < user.size(); i++){
  //   cout<<iumap[i]<<" ";
  //   vector<int>::iterator et = user[i].nset.begin();
  //   for (; et != user[i].nset.end(); et++) {
  //     cout<<setw(4)<<(*et);
  //   }
  //   cout<<endl;
  // }

  // for (int i = 0; i < user.size(); i++){
  //   cout<<user[i].h<<endl;
  // }
  // for (int i = 0; i < user.size(); i++){
  //   cout<<orderlist[i].idx<<endl;
  // }
  paint();
  end = clock();

   cout<<end- start <<endl;
   output();
  return 0;
}
