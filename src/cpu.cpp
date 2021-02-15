//
// Created by vhs on 15/2/21.
//

#include "cpu.hpp"
void CPU::init()
{
	//The NES needs some specific values be written in order to boot up.
	m_cpu_regs.P = 0x34;
	m_cpu_regs.A = 0;
	m_cpu_regs.X = 0;
	m_cpu_regs.Y = 0;
	m_cpu_regs.S = 0xfd;
}
CPU::CPU()
{

}
CPU::~CPU()
{

}
void CPU::read_data(Byte data, int address, unique_ptr<Memory> mem)
{
}