#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
    long idleJiffies     = LinuxParser::IdleJiffies();
    long activeJiffies   = LinuxParser::ActiveJiffies();
    float CPU_Percentage = 0.0;
    long prevTotal       = idleJiffies_ + activeJiffies_;
    long Total           = idleJiffies + activeJiffies;
    long totald          = Total - prevTotal;
    long idled           = idleJiffies - idleJiffies_;
    if(totald != 0.0)
    {
        
        CPU_Percentage =  (float)(totald - idled) / totald;
    }
    idleJiffies_         = idleJiffies;
    activeJiffies_       = activeJiffies;
    return CPU_Percentage;
}