#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  float value[5];
  string temp;
  if(stream.is_open()){
  	string line;
    for(int i=0; i<5; i++){
        std::getline(stream, line);
    	std::istringstream linestream(line);
		linestream>>temp>>temp;
      	value[i]=std::stoi(temp);
    }
  }
  return (value[0]-value[1]-value[3]-value[4])/value[0];
}

// Read and return the system uptime(in seconds)
long LinuxParser::UpTime() { 
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  long value{0};
  if(stream.is_open()){
    string line;
  	std::getline(stream, line);
    std::istringstream linestream(line);
    string temp;
    linestream>>temp;
    value = std::stol(temp);
  }
  return value; 
}


//Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
   std::ifstream stream
    (kProcDirectory +"/"+to_string(pid)+ kStatFilename);
  long value{0};
  if(stream.is_open()){
    string line;
    string temp;
  	std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=0; i<17; i++){
      linestream>>temp;
      if(i>12)
    	value += std::stol(temp);
    }
  }
  return value; 
}


//return the ten values of the aggregate cpu
vector<int> LinuxParser::CpuUtilization() { 
  vector<int> values;
  std::string line, temp;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> temp;
    while(linestream>>temp){
      values.push_back(std::stoi(temp));
    }
  }
  return values; 
}

//find value of key (next string after the key)
int LinuxParser::findValue(string key, string dir){
  string line;
  string temp;
  std::ifstream stream(dir);
  int value{0};
  if(stream.is_open()){
  	while(std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream>>temp; 
      if(temp==key){
        linestream>>temp;
        value=std::stoi(temp);
        break;
      }
    }
  }
  return value; 
}

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return findValue("processes", kProcDirectory + kStatFilename); 
}

//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return findValue("procs_running", kProcDirectory + kStatFilename); 
}

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::ifstream stream(
    kProcDirectory + "/"+to_string(pid)+ kCmdlineFilename);
  string line{};
  if(stream.is_open())
  	stream>>line;
 
  return line; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  int mem = findValue("VmSize:",kProcDirectory +"/"
            +to_string(pid)+kStatusFilename);
  return to_string(mem/1024); 
}


// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  int uid = findValue("Uid",(kProcDirectory +"/"
            +to_string(pid)+kStatusFilename));
  return to_string(uid); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  //search for username
  string line;
  string name,temp,value;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      //search for third value
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>name>>temp>>value;
      if(value==uid) return name;
    }
   }
   
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  std::ifstream stream
    (kProcDirectory +"/"+to_string(pid)+ kStatFilename);
  long value{0};
  if(stream.is_open()){
    string line;
    string temp;
  	std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=0; i<22; i++)
      linestream>>temp;
    value = std::stol(temp);
    value/=sysconf(_SC_CLK_TCK);
  }
  return value; 
}
