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

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

void Process::Pid(int pid) { pid_ = pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuUtil_; }

void Process::CpuUtilization(float cpuUtil) { cpuUtil_ = cpuUtil; }

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

void Process::Command(std::string command) { command_ = command; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return ram_; }

void Process::Ram(std::string ram) { ram_ = ram; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

void Process::User(std::string user) { user_ = user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return time_; }

void Process::UpTime(long int time) { time_ = time; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return CpuUtilization() < a.CpuUtilization();
}