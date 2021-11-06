#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;                                // Getter for process pid
  void Pid(int pid);                              // Setter for process pid
  std::string User() const;                       // Getter for process user
  void User(std::string user);                    // Setter for process user
  std::string Command() const;                    // Getter for process command
  void Command(std::string command);              // Setter for process command
  float CpuUtilization() const;                   // Getter for process cpu utilization
  void CpuUtilization(float cpu_util);            // Getter for process cpu utilization
  std::string Ram() const;                        // Getter for process ram
  void Ram(std::string ram);                      // Setter for process ram
  long int UpTime() const;                        // Getter for process start time
  void UpTime(long int time);                     // Setter for process start time            
  bool operator<(Process const& a) const;         // Overlaod < operator

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  float cpuUtil_;
  std::string ram_;
  long int time_;
};

#endif