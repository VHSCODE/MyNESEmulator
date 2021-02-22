//
// Created by vhs on 15/2/21.
//

#include <CPU.hpp>
CPU::CPU()
{
	init();
}
CPU::~CPU()
{

}

void CPU::init()
{
	//The NES needs some specific values be written in order to boot up.
	m_cpu_regs.P = 0x34;
	m_cpu_regs.A = 0;
	m_cpu_regs.X = 0;
	m_cpu_regs.Y = 0;
	m_cpu_regs.S = 0xfd;
}
Byte CPU::read_data(int address, shared_ptr<Memory> mem)
{
	return mem->read_data(address);
}

void CPU::write_data(int address, Byte data ,shared_ptr<Memory> mem)
{
	mem->write_data(address,data);
}
