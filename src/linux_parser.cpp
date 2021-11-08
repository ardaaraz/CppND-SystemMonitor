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

// An example of how to read data from the filesystem
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
  filestream.close();
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// Update this to use std::filesystem
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  string line;
  string key;
  int value;
  int MemTotal, MemFree;
  bool setFlagMemTotal = false; 
  bool setFlagMemFree = false;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line) && (!setFlagMemFree || !setFlagMemTotal))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == "MemTotal:")
        {
          MemTotal = value;
          setFlagMemTotal = true;
        }
        else if (key == "MemFree:")
        {
          MemFree = value;
          setFlagMemFree = true;
        }
      }
    }
  }
  stream.close();
  return static_cast<float>(MemTotal - MemFree) / MemTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{
  string line;
  long upTime, idleTime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  stream.close();
  return upTime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  int PID;
  std::string comm;
  char state;
  long uTime, sTime, cuTime, csTime;
  long value;
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 4;
    linestream >> PID >> comm >> state;
    while(linestream >> value)
    {
      if(i == kUTime) { uTime = value; }
      else if(i == kSTime)     { sTime = value; }
      else if(i == kCUTime)    { cuTime = value; }
      else if(i == kCSTme)     { csTime = value; break;}
      i++;
    }
  }
  stream.close();
  long total_time = uTime + sTime + cuTime + csTime;
  return total_time / sysconf(_SC_CLK_TCK);
}

// Read and return CPU utilization of a process
float LinuxParser::CpuUtilization(int pid) {
  float cpu_util = 0.0;
  long total_time = LinuxParser::ActiveJiffies(pid); //[s]
  long start_time = LinuxParser::UpTime(pid); //[s]
  long up_time    = LinuxParser::UpTime(); //[s]
  long seconds    = up_time - (start_time);
  if(total_time > 0 && seconds >0)
  {
    cpu_util = (float)((float)total_time / (float)seconds);
  }
  return cpu_util;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies_vec = LinuxParser::CpuUtilization();
  long activeJiffies = 0;
  vector<int> activeJiffiesMember = {CPUStates::kUser_, CPUStates::kNice_, CPUStates::kSystem_,
                                     CPUStates::kIRQ_, CPUStates::kSoftIRQ_, CPUStates::kSteal_,
                                     CPUStates::kGuest_, CPUStates::kGuestNice_};
  for(int i : activeJiffiesMember)
  {
    activeJiffies += std::stol(jiffies_vec[i]);
  }
  return activeJiffies; 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies_vec = LinuxParser::CpuUtilization();
  long idleJiffies = 0;
  vector<int> iddleJiffiesMember = {CPUStates::kIdle_, CPUStates::kIOwait_};
  for(int i : iddleJiffiesMember)
  {
    idleJiffies += std::stol(jiffies_vec[i]);
  }
  return idleJiffies;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string cpuName;
  string value;
  vector<string> cpuUtilVec;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    // Read only first line
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpuName;
    while(linestream >> value)
    {
      cpuUtilVec.push_back(value);
    }
  }
  stream.close();
  return cpuUtilVec;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == "processes")
        {
          return value;
        }
      }
    }
  }
  stream.close();
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == "procs_running")
        {
          return value;
        }
      }
    }
  }
  stream.close();
  return value; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
  }
  stream.close();
  return line; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename); 
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == "VmRSS:") // Use VmRSS instead of VmSize to get exact physical memory being used 
        {
          return std::to_string(value / 1000);
        }
      }
    }
  }
  stream.close();
  return std::to_string(value);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value1, value2, value3, value4;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename); 
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value1 >> value2 >> value3 >> value4)
      {
        if(key == "Uid:")
        {
          return value1;
        }
      }
    }
  }
  stream.close();
  return value1; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string Uid = LinuxParser::Uid(pid);
  string line;
  string user;
  string value;
  string x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> value) {
        if (value == Uid) {
          return user;
        }
      }
    }
  }
  filestream.close();
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  int PID;
  std::string comm;
  char state;
  long upTime;
  long value;
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 4;
    linestream >> PID >> comm >> state;
    while(linestream >> value)
    {
      if(i == kStartTime)
      {
        break;
      }
      i++;
    }
  }
  stream.close();
  upTime = UpTime() - (value / sysconf(_SC_CLK_TCK));
  return upTime; 
}
