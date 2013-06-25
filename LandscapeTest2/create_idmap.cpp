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
  unsigned int cnt = 0;
  for( ; it != idlist.end(); it++) {
    mapoutfile<<cnt<< " "<<(*it)<<endl;
    cnt++;
  }
  mapoutfile.close();
  cout<<"Writing file completed."<<endl;
}

int main(int argc, char** argv) {
  if ( argc != 3 ) {
    cerr<<"Wrong arguments."<<endl;
    exit(-1);
  }
  create_idmap(argv[1],argv[2]);
  return 0;
}
