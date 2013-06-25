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

int main()
{
  ofstream outfile("network.edge");
  long int n = 10000;
  srand(0);
  for (int k = 0; k < 500000; k++) {
  long int i,j;
  i = rand()%n;
  j = rand()%n;
  outfile<<i<<" "<<j<<endl;
  }
  outfile.close();
  return 0;
  
}
