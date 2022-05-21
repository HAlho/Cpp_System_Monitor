#include "processor.h"
#include "linux_parser.h"
#include <vector>

// calculations are from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux 
float Processor::Utilization() { 
  //get vector values from linux parser
  //0.user 1.nice 2.system 3.idle 4.iowait 5.irq 
  //6.softirq 7.steal 8.guest 9.guest_nice
  preValue=std::vector<int>(10,0);
  std::vector<int> value = LinuxParser::CpuUtilization();

  int PrevIdle = preValue[3] + preValue[4];
  int Idle = value[3] + value[4];

  int PrevNonIdle = preValue[0] + preValue[1] + preValue[2] + preValue[5] + preValue[6] + preValue[7];
  int NonIdle = value[0] + value[1] + value[2] + value[5] + value[6] + value[7];

  int PrevTotal = PrevIdle + PrevNonIdle;
  int Total = Idle + NonIdle;

  //# differentiate: actual value minus the previous one
  float totald = Total - PrevTotal;
  int idled = Idle - PrevIdle;

  //save previous
  preValue=value;

  return (totald - idled)/totald;
}