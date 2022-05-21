#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  string v[3];
  v[0] = std::to_string(seconds/3600);
  v[1] = std::to_string((seconds%3600)/60); 
  v[2] = std::to_string((seconds%3600)%60);
  for(int i=0;i<sizeof(v);i++){
  	if(v[i].length()==1)
      v[i].insert(0,"0");
  }
  return v[0]+":"+v[1]+":"+v[2];
}