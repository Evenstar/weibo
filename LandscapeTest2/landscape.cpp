#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include<cstdlib>
#include<set>
using namespace std;

struct node {
  int idx;
  vector<int> nlist;
  double h;
  int color;
};


node user[35];

bool comph(node a, node b)
{
  return a.h < b.h;
}

void init()
{
  ifstream infile;
  infile.open("karate.edge", ios::in);
  int i,j;
  while(infile>>i>>j)
    {
      i -=100;// index 
      j -= 100;// index
      user[i].nlist.push_back(j);
      user[j].nlist.push_back(i);
    }
  infile.close();
  srand(0);
  for (int i = 0; i < 34; i++) {
    user[i].idx = i;
    user[i].h = user[i].nlist.size() + (double)(rand()%1000) / 1000.0;
    user[i].color = -1;
  }
}
int main()
{
  init();
  int rank[34];
  sort(user, user+33, comph);//not using vector, requiring large amount of memory.
  for (int i = 0; i < 34; i++)
    {
      rank[user[i].idx] = i;
    }
  int cnt = 0;
  int level  = 0;
  vector< set<int> > levelset;
	  //test
	  for (int j = 0; j < 34; j++){
	    vector<int>::iterator tit = user[j].nlist.begin();
	    cout<<user[j].idx<<" : ";
	    for (; tit != user[j].nlist.end(); tit++)
	      cout<<(*tit)<<" ";
	    cout<<endl;
	    }
	  //test
  while( true ) 
    {
      set<int> lset;
      for (int i = 0; i < 34; i++) 
	{
	  if ( user[i].color != -1 ) {
	    continue;
	  }


	  vector<int>::iterator it = user[i].nlist.begin();
	  set<int> nset;
	  for (; it != user[i].nlist.end(); it++) {
	    if (user[rank[*it]].h < user[i].h) {
	      nset.insert(rank[*it]);
	    }
	  }
	  if ( nset.empty() ) {
	    lset.insert( user[i].idx );
	    user[i].color = user[i].idx;
	  } else {
	    if (nset.size() == 1) {
	      set<int>::iterator et = nset.begin();
	      user[i].color = user[rank[*et]].color;
	    }
	  }
	}

      set<int>::iterator it = lset.begin();
      for (; it != lset.end(); it++) 
	;
//	cout<<*it<<endl;
      break;
    }
  return 0;
}
