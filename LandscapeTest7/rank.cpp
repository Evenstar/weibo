#include<string>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<ctime>
using namespace std;

struct Node{
  unsigned int id;
  unsigned int nsize;
};

unsigned int cnt;
unsigned int LEVEL;
const unsigned int USERNUM = 300000;

unsigned int* level = new unsigned int[USERNUM];
Node* orderlist;
unsigned int* iumap = new unsigned int[USERNUM];

// void read_map( string filename )
// {
//   ifstream infile( filename.c_str() );
//   infile>>cnt;
//   unsigned int a,b;
//   for (int i = 0; i < cnt; i++) {
//     infile >> a >> b;
//     iumap[i] = a;
//   }
//   infile.close();
// }
void read_result( string filename)
{
  memset(level, 0, sizeof(unsigned int)*USERNUM);
  ifstream infile( filename.c_str() );
  infile>>cnt>>LEVEL;
  orderlist = new Node[cnt];
  for (unsigned int i = 0; i < cnt; i++){
    orderlist[i].id = i;
    orderlist[i].nsize = 0;
  }
  unsigned int a,b,c;
  for (unsigned int i = 0; i < cnt; i++){
    infile>>a>>b>>c;
    orderlist[b].nsize++;
    iumap[i] = a;
    level[i] = c;
  }
  infile.close();
}

bool comp(const Node& a, const Node& b)
{
  return a.nsize > b.nsize;
}
void rank()
{
  sort(orderlist, orderlist + cnt, comp);
}

void output(string filename )
{
  ofstream outfile(filename.c_str());
  for (unsigned int i = 0; i < cnt; i++)
    {
      if (orderlist[i].nsize == 0 ) break;
      outfile<<setw(12)<<iumap[orderlist[i].id]
	     <<setw(12)<<orderlist[i].nsize
	     <<setw(12)<<level[i]<<endl;
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
void clean()
{
  delete [] orderlist;
  delete [] level;
  delete [] iumap;
}
int main(int argc, char** argv)
{
  if ( argc != 3) {
    cerr<<"Wrong input arguments. [.result] [.rank]"<<endl;
    exit(-1);
  }
  ofstream logfile;
  logfile.open("Timelog",ios::out | ios::app);
  logfile<<"Executing command: "<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<endl;
  clock_t start = clock();
  
  logfile.close();
  read_result(argv[1]);
  rank();
  output(argv[2]);
logfile<<"Time on computing ranking: "<<((double)clock() - start)/1000000<<"s"<<endl;
  cout<<"Processing completed"<<endl;
  clean();
  return 0;
}
