#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//added constructor
Process::Process(int n):pid(n){};

//Return this process's ID
int Process::Pid() { return pid; }

// calculations are from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/
float Process::CpuUtilization() { 
  
  long uptime = LinuxParser::UpTime();
  long total_time= LinuxParser::ActiveJiffies(pid);
  long starttime = LinuxParser::UpTime(pid);
  float seconds = uptime - starttime;
  float cpu_usage = (total_time / (float)sysconf(_SC_CLK_TCK)) / seconds;

  return cpu_usage; 
}

//Return the command that generated this process
string Process::Command() { 
  return LinuxParser::Command(pid);
}

//Return this process's memory utilization
string Process::Ram(){ 
  return LinuxParser::Ram(pid); 
}

//Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid);
}

//Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

//comparision operator for sorting in descending order
bool Process::operator<(Process& a){
  return(CpuUtilization()>a.CpuUtilization());
}