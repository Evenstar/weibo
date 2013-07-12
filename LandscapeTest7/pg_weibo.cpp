/* Copyright (c) 2013, Cheng Tai.
 
   All rights reserved.
  
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
 
   * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 
   * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.
  
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
   OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//ALGORITHM
//This program computes the pagerank of a given directed graph.
//The main formula is simply PG(i) = (1-q)/N + q*sum( PG(j)/Outdegree(j))*
//D(i,j). D(i,j) = 1 whenever there is an arc from j to i. 
//q is the damping factor, usually set to be 0.15.
//N is the number of nodes.
//
//FILE FORMAT
//The input files should be of the following format.
//1. An edge file that contains all the directed arcs. It looks like the 
//following:
// 12 13
// 12 14
// this means 13 and 14 are FOLLOWERS of 12. That is, this program assumes 
//there is a directed arc from the second to the first node.Pay attention 
//to the direction of the arc before using this program. 
//2. A map created by the program create_map.
//As the data to be processed is very large, to reduce redundent work, this 
//program reuses the preprocessing. Such reusing of the preprocessed files
//facilitates memory allocation and creation of neighborlist.
//
//PARAMETER SETTING
//USERNUM: this is the maximum number of user allowed. It should be set 
//         larger than the number of nodes in the input graph.
//ALPHA:   1-ALPHA is the damping factor. 
//EPSILON: criterion for convergence. If the L1 deviation of pageranks 
//         produced by two successive iterations is smaller than EPSILON, 
//         the program stops.
//STEP:    maximum steps allowed.



#include<vector>
#include<string.h>
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

using namespace std;

void outputpagerank(string);


//Data structure declaration.
struct User{
  unsigned int idx;
  unsigned int* outlist;
  unsigned int outsize;
  unsigned int* inlist;
  unsigned int insize;
  double h;
};

struct Node{
  unsigned int idx;
  double h;
};

//PARAMETERS
const unsigned int USERNUM = 1000000;
unsigned int cnt = 0;        
unsigned int STEP = 50;
const double ALPHA = 0.85;
const double EPSILON = 1e-6;

//allocate space
User* user = new User[USERNUM];
unsigned int* iumap = new unsigned int[USERNUM];
//actually not used in this program. The output of this
//program is in the order of user index, not the
//user name.

double res(User* user, double* old_pg)
{
  double s = 0;
  for (unsigned int i = 0; i < cnt; i++) {
    s += abs(old_pg[i] - user[i].h);
  }
  return s;
}

void read_map(string infilename )
{
  FILE* infile = fopen( infilename.c_str(), "r");
  if ( infile == NULL ) {
    cerr << "Fail to open file: "<<infilename<<endl;
    exit( -1 );
  }

  fscanf(infile, "%d", &cnt);
  //get number of users
    
  unsigned int i,u;
  for (unsigned int k = 0; k < cnt; k++) {
    fscanf(infile, "%d %d", &i, &u);
    iumap[k] = i;
    user[k].inlist = new unsigned int[u];
    user[k].outlist = new unsigned int[u];
    //allocate space to hold in and out neighbors.
  }
  fclose( infile );
}

void init_network( string edgefilename, string idmapfilename )
{
  read_map( idmapfilename );

  //Set initial value of pagerank.
  for (unsigned int i = 0; i < cnt; i++) {
    user[i].idx = i;
    user[i].h = 1.0/cnt;
  }
   
  unsigned int* curoutdegree = new unsigned int[USERNUM];
  unsigned int* curindegree = new unsigned int [USERNUM];

  //create neighbor list
  memset(curoutdegree, 0, sizeof(unsigned int)*cnt);
  memset(curindegree, 0, sizeof(unsigned int)*cnt);
  FILE* instream;
  instream = fopen( edgefilename.c_str(),"r" );
  unsigned int i,j;
    
  //For weibo data, the format is 1--->2 for each line.
  //That is, the second is the follower of the first.
  while ( fscanf(instream, "%d %d", &i, &j)!= EOF ) {
    *(user[i].outlist + curoutdegree[i]) = j;
    *(user[j].inlist + curindegree[j]) = i;
    curoutdegree[i]++;
    curindegree[j]++;
  }
    
  for (unsigned int i = 0; i < cnt; i++){
    user[i].outsize = curoutdegree[i];
    user[i].insize = curindegree[i];
    //     cout<<user[i].outsize<<" "<<user[i].insize<<endl;
  }
  fclose( instream );
  delete [] curindegree;
  delete [] curoutdegree;
}



void onestep(double* old_pg)
{
  double sum = 0;
   for (unsigned int i = 0; i < cnt; i++)
    {
      double s = 0;
                for (unsigned int j = 0; j < user[i].insize; j++)
        {
	  unsigned int k = user[i].inlist[j];
	  s += old_pg[k] / user[k].outsize;
        }
	    user[i].h = 1.0*(1-ALPHA)/(double)(cnt) + ALPHA*s;
	    sum += user[i].h;
    }
   //  for(unsigned int i = 0; i < cnt; i++) {
   //user[i].h /= sum;
   // }
  
}

void iterate(string filename)
{
  double* old_pg = new double[cnt];
  memset(old_pg,0, sizeof(double)*cnt);
  for (unsigned int i = 0; i < STEP; i++)
    {
      onestep(old_pg);
      double diff = res(user, old_pg);
      cout<<"Iteration step "<<i<<", L1 error is "<<diff<<endl;

      //If convergence criterion is met, stop.
      if ( diff < EPSILON )
	{
	    break;
	}
      for (unsigned int i = 0; i < cnt; i++) {
	old_pg[i] = user[i].h;
      }
    }
  //Write updated pagerank to file after each step.
  outputpagerank(filename);
}

void clean()
{
  for (unsigned int i = 0; i < cnt; i++){
    delete [] user[i].outlist;
    delete [] user[i].inlist;
  }
  delete [] user;
  delete [] iumap;

}

void outputpagerank(string filename)
{
  ofstream outfile(filename.c_str());
  for(unsigned int i = 0; i < cnt; i++)
    {
      outfile<<setiosflags(ios::fixed)<<setprecision(10);
      outfile<<iumap[user[i].idx]<<"\t"<<user[i].h<<endl;
    }
  outfile.close();
}

void outputfollowernumber( string filename )
{
  ofstream outfile(filename.c_str());
  for(unsigned int i = 0; i < cnt; i++)
    {
      outfile<<iumap[user[i].idx]<<"  "<<user[i].insize<<" "<<user[i].outsize<<endl;
    }
  outfile.close();
}


bool comph(const User& a, const User&b ){
  return a.h > b.h;
}

bool compf(const User& a, const User& b) {
  return a.insize > b.insize;
}

void sort_user_h()
{
  sort(user, user+cnt, comph);
}

void sort_user_f()
{
  sort(user,user+cnt, compf);
}

string getTime()
{
  char outTime[64];
  time_t t = time(0);
  strftime(outTime, sizeof(outTime), "%Y/%m/%d %X", localtime(&t));
  return outTime;
}

int main( int argc, char** argv)
{
  if ( argc != 5) {
    cerr<<"Wrong input arguments. [.nedge] [.map] [ ] [step]"<<endl;
    cerr<<"For example, if the name of the input edge file is weibo.edge."<<endl;
    cerr<<"Executing first the program create_map to get two files: weibo.nedge"<<endl;
    cerr<<"and weibo.map. Then executing command ./pg weibo.nedge weibo.map weibo 50"<<endl;
    cerr<<"will produce 4 output files: weibo.pagerank, weibo.pagerank.sorted"<<endl;
    cerr<<"weibo.followernumber and weibo.followernumber.sorted after at most 50"<<endl;
    cerr<<"iterations"<<endl;
    exit( -1 );
  }
  ofstream logfile;
  logfile.open("pagerank_weibo.log", ios::out | ios::app);
  logfile<<getTime()<<endl;
  logfile<<"Executing command: "<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<argv[4]<<endl;
  logfile.close();
  
  clock_t start = clock();
  STEP = atoi(argv[4]);

  init_network(argv[1], argv[2]);
  string filename = string(argv[3]) + string(".followernumber");
  outputfollowernumber(filename);

  logfile.open("pagerank_weibo.log", ios::out | ios::app);
  logfile<<"Time on initialization: "<<((double)clock() - start)/1000000<<"s"<<endl;
  logfile.close();

  start = clock();
  filename = string(argv[3]) + string(".pagerank");
  iterate( filename  );

  logfile.open("pagerank_weibo.log", ios::out | ios::app);
  logfile<<"Time on iteration: "<<((double)clock() - start)/1000000<<"s"<<endl;
  logfile.close();
  //Write sorted result to file.

  sort_user_h();
  filename = string(argv[3]) + string(".pagerank.sorted");
  outputpagerank(filename);

 
  filename = string(argv[3]) + string(".followernumber.sorted");
  sort_user_f();
  outputfollowernumber(filename);
  logfile.open("pagerank_weibo.log", ios::out | ios::app);
  logfile<<"Processing completed."<<endl<<endl;
  logfile.close();
  cout<<"Processing completed."<<endl;
  clean();
    
  return 0;
    
}
