//This program process the edge information, transforms the usernames to 
//index. It also counts the neighbor size of each user.
//Input: .edge file. A list of all edges between user id.
//Output: .nedge, .map.
//        .map contains the userid-index correspondence as well
//             as the neighborsize of each user. 
//        .nedge the edge list between user indexes.

//NOTICE: the user id is supposed to be of long int type. I.e.
//        it does not exceed 2^32, approximately 4.3Billion.
//        Before compiling this program, setting the M,N value 
//        such that M*N is larger than the maximum value of the
//        user id when interpreted as long int.

#include<vector>
#include<map>
#include<unordered_map>
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

const int M = 8000;
const int N = 10000;
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
  string a,b;
  vector<string> idlist;
  clock_t start = clock();
  cout<<"Processing file: "<<infilename<<endl;
  int i,j;
  int cur = 0;
  unordered_map<string, unsigned int> utoi;
  unordered_map<unsigned int ,unsigned int> ncount;
  unordered_map<unsigned int, unsigned int>(1000000);
  while( edgeinput >> a >> b )
    {
      if ( utoi[a] == 0) {
	idlist.push_back( a );
	utoi[a] = cur;
	ncount[utoi[a]] = 1;
	cur++;
      } else {
	ncount[utoi[a]]++;
      }
      if ( utoi[b] == false) {
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
  vector<string>::iterator it = idlist.begin();
  for( ; it != idlist.end(); it++) {
    mapoutfile<<(*it)<<" "<<ncount[utoi[*it]]<<endl;
  }
  mapoutfile.close();
  cout<<"Processing completed in "<<((double)(clock()) - start)/1000000<<"s"<<endl;
}

string getTime()
{
  char outTime[64];
  time_t t = time(0);
  strftime(outTime, sizeof(outTime), "%Y/%m/%d %X", localtime(&t));
  return outTime; 
}

int main(int argc, char** argv) {
  if ( argc != 4 ) {
    cerr<<"Wrong arguments. [.edge] [.nedge] [.map]"<<endl;
    exit(-1);
  }
  clock_t start = clock();
  ofstream logfile;
  logfile.open("Timelog",ios::out | ios::app);
  logfile<<getTime()<<endl;
  logfile<<"Executing command: "<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<endl;
  create_maps(argv[1],argv[2],argv[3]);
  logfile<<"Time on processing: "<<((double)clock() - start)/1000000<<"s"<<endl<<endl;
  logfile.close();
  for (int i = 0; i < N; i++){
    delete [] visited[i];
  }
  //  cout<<((double)clock() - start)/1000000<<endl;
  return 0;
}


