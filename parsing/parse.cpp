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
  ofstream out_uid("uid.fd");
  ofstream out_msg("msg.fd");
  ofstream out_tsp("tsp.fd");
  ofstream out_rid("rid.fd");
  ofstream out_rsg("rsg.fd");
  string st = "{\"a\":\"中文\"}";

  json_t* root;
  json_t* uid, *msg, *rid,*rsg,*tsp;
  json_error_t error;
  while( getline(fin,st) ){
    root = json_loads(st.c_str(),0,&error);
    uid = json_object_get(root,"a");
    msg = json_object_get(root,"c");
    tsp = json_object_get(root,"e");
    rid = json_object_get(root,"i");
    rsg = json_object_get(root,"k");
    if ( json_is_string(uid) &&
	 json_is_integer(tsp) &&
	 json_is_string(rid)&&
	 json_is_string(rid)&&
	 json_is_string(rsg))
      {
	out_uid<<json_string_value(uid)<<endl;
	out_msg<<json_string_value(msg)<<endl;
	out_tsp<<json_integer_value(tsp)<<endl;
	out_rid<<json_string_value(rid)<<endl;
	out_rsg<<json_string_value(rsg)<<endl;
      }
   }
  fin.close();
  out_uid.close();
  out_msg.close();
  out_tsp.close();
  out_rid.close();
  out_rsg.close();
  cout<<((double)clock() - start)/1000000<<"s"<<endl;
  return 0;
}
