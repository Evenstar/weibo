#include<iostream>
#include<fstream>
#include<ctime>
using namespace std;

int main(int argc, char** argv) {
  clock_t start = clock();
  unsigned int max = 0;
  ifstream infile(argv[1]);
  unsigned int a;
  while( infile>> a){
    if ( a > max )  max = a;
  }
  infile.close();
  cout<<max<<endl;
  cout<<clock()-start<<endl;
  return 0;
}
