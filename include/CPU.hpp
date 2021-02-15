//
// Created by vhs on 15/2/21.
//

#ifndef MYNESEMULATOR_SRC_CPU_HPP_
#define MYNESEMULATOR_SRC_CPU_HPP_
#include "types.h"
#include "Memory.hpp"
#include <memory>

struct CPURegisters {
	Byte A;    //Accumulator
	Byte X, Y; //Used for several adressing modes
	short PC;  //Program counter
	Byte S;    //Stack pointer
	Byte P;    //Status register
};

class CPU
{

 public:
	CPU();
	~CPU();
 private:
	CPURegisters m_cpu_regs;
	void init();

	Byte read_data(int address, unique_ptr<Memory> mem);
	void write_data(int address,Byte data, unique_ptr<Memory> mem);
};

#endif //MYNESEMULATOR_SRC_CPU_HPP_
