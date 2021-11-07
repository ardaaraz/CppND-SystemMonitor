#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

  // Declare any necessary private members
 private:
  long activeJiffies_ = 0.0;
  long idleJiffies_ = 0.0;
};

#endif