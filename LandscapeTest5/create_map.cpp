#include<vector>
#include<map>
#include<ext/hash_map>
#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include<cstdlib>
#include<set>
#include<string>
#include<iomanip>
#include<ctime>
using namespace __gnu_cxx;
using namespace std;

const int M = 20000;
const int N = 100;
bool* visited[N];
void create_maps( string infilename, string outfilename1, string outfilename2 )
{
  // hash_map<unsigned int, unsigned int>(10);
  for (int i = 0; i < N; i++){
    visited[i] = new bool[M];
  }
  for (int i = 0; i < N; i++){
    for (int j = 0; j < M; j++){
      *(visited[i]+j) = false;
    }
  }
  ifstream edgeinput( infilename.c_str() );
  ofstream edgeoutput( outfilename1.c_str());
  if ( edgeinput.fail() ) {
    cerr << "Fail to open file: "<< infilename<<endl;
    exit( -1 );
  }
  unsigned int a,b;
  vector<unsigned int> idlist;
  clock_t start = clock();
  cout<<"Processing file: "<<infilename<<endl;
  int i,j;
  int cur = 0;
  hash_map<unsigned int, unsigned int> utoi;
  hash_map<unsigned int,unsigned int> ncount;
  hash_map<unsigned int, unsigned int>(1000000);
  while( edgeinput >> a >> b )
    {
      i = a / M;
      j = a % M;
      if ( *(visited[i] + j) == false) {
	*(visited[i]+j) = true;
	idlist.push_back( a );
	utoi[a] = cur;
	ncount[utoi[a]] = 1;
	cur++;
      } else {
	ncount[utoi[a]]++;
      }
      i = b / M;
      j = b % M;
      if ( *(visited[i] + j) == false) {
	*(visited[i]+j) = true;
	idlist.push_back( b );
	utoi[b] = cur;
	ncount[utoi[b]] = 1;
	cur++;
      } else {
	ncount[utoi[b]]++;
      }
      edgeoutput<<utoi[a]<<" "<<utoi[b]<<endl;
    }
  edgeinput.close();
  edgeoutput.close();
  ofstream mapoutfile( outfilename2.c_str() );
  mapoutfile<<idlist.size()<<endl;
  vector<unsigned int>::iterator it = idlist.begin();
  for( ; it != idlist.end(); it++) {
    mapoutfile<<(*it)<<" "<<ncount[utoi[*it]]<<endl;
  }
  mapoutfile.close();
  cout<<"Processing completed in "<<((double)(clock()) - start)/1000000<<"s"<<endl;
}

int main(int argc, char** argv) {
  if ( argc != 4 ) {
    cerr<<"Wrong arguments. [.edge] [.nedge] [.map]"<<endl;
    exit(-1);
  }
  //  clock_t start = clock();
  create_maps(argv[1],argv[2],argv[3]);
  for (int i = 0; i < N; i++){
    delete [] visited[i];
  }
  //  cout<<((double)clock() - start)/1000000<<endl;
  return 0;
}
