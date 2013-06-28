#include<jansson.h>
#include<iostream>
#include<fstream>
#include<ctime>
#include<string>
using namespace std;

string getTime()
{
  char outTime[64];
  time_t t = time(0);
  strftime(outTime, sizeof(outTime), "%Y/%m/%d %X", localtime(&t));
  return outTime; 
}

int main(int argc, char** argv)
{
  if (argc != 2 ){
    cerr<<"Wrong arguments. [.input]"<<endl;
    exit(-1);
  }
  
  clock_t start  = clock();
  ifstream fin(argv[1]);
  ofstream timelog( "process.log" );
  string name_a = string(argv[1]) + ".fda";
  string name_b = string(argv[1]) + ".fdb";
  string name_c = string(argv[1]) + ".fdc";
  string name_e = string(argv[1]) + ".fde";
  string name_i = string(argv[1]) + ".fdi";
  string name_j = string(argv[1]) + ".fdj";
  string name_k = string(argv[1]) + ".fdk";

  ofstream out_a( name_a.c_str() );
  ofstream out_b( name_b.c_str() );
  ofstream out_c( name_c.c_str() );
  ofstream out_e( name_e.c_str() );
  ofstream out_i( name_i.c_str() );
  ofstream out_j( name_j.c_str() );
  ofstream out_k( name_k.c_str() );

  string st;
  json_t* root;
  json_t *a, *b, *c, *e, *i, *j, *k;
  json_error_t error;
  while( getline(fin,st) ){
    root = json_loads(st.c_str(),0,&error);
    if(!root)
      {
	fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
	return -1;
      }
    a = json_object_get(root,"a");
    b = json_object_get(root,"b");
    c = json_object_get(root,"c");
    e = json_object_get(root,"e");
    i = json_object_get(root,"i");
    j = json_object_get(root,"j");
    k = json_object_get(root,"k");

    if ( json_is_string( a ) 
	 &&json_is_string( b ) 
	 &&json_is_string( c )
	 &&json_is_integer( e )
	 &&json_is_string( i )
	 &&json_is_string( j )
	 &&json_is_string( k )
	 )
      {
	out_a<<json_string_value( a ) <<endl;
       	out_b<<json_string_value( b ) <<endl;
	out_c<<json_string_value( c ) <<endl;
	out_e<<json_integer_value( e ) <<endl;
	out_i<<json_string_value( i ) <<endl;
	out_j<<json_string_value( j ) <<endl;
	out_k<<json_string_value( k ) <<endl;
      }
  }
  fin.close();

  out_a.close();
  out_b.close();
  out_c.close();
  out_e.close();
  out_i.close();
  out_j.close();
  out_k.close();
  
  timelog<<getTime()<<endl;
  timelog<<"Executing command: "<< argv[0]<<" "<<argv[1]<<endl;
  timelog<<"Processing completed in: "<< ((double)clock() - start)/1000000<<"s"<<endl<<endl;
  cout<<((double)clock() - start)/1000000<<"s"<<endl;

  return 0;
}
