/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor

Developers' Names: Aditya Yadav, Dhruv Agarwal
Developers' Email ids: 2022meb1291@iitrpr.ac.in , 2022meb1307@iitrpr.ac.in

*/


/* main.cpp 
   Purpose of this file: The file handles the input and output, and
   invokes the simulator
*/


#include "builtin_funcs.hpp"

#include <stdio.h>
#include <stdlib.h>
#include "myRISCVSim.hpp"
#ifndef GLOBAL
#define GLOBAL
#include "global_variables.hpp"
#endif
int main(int argc, char** argv) {
  char* prog_mem_file; 
  if(argc < 2) {
    printf("Incorrect number of arguments. Please invoke the simulator \n\t./myRISCVSim <input mem file> \n");
    exit(1);
  }
  
  //reset the processor
  reset_proc();
  //load the program memory
  load_program_memory(argv[1]);
  //run the simulator
  run_riscvsim();

  return 1;
}
