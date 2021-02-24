//
// Created by vhs on 15/2/21.
//

#include <Memory.hpp>
#include <iostream>
#include "spdlog/spdlog.h"
#include "assert.h"
#include "utils.hpp"
Byte Memory::read_data(int const address) const
{
	assert(address <= 0xFFFF);
	spdlog::info("Reading from memory address 0x{0:x}", address);


	if (in_range(address, 0x0000, 0x07FF))
	{
		//CPU internal RAM
		return m_memory.at(address);
	}
	else if (in_range(address, 0x0800, 0x0FFF))
	{
		//First mirror of 0x0000 - 0x07FFF
		Byte real_address = address - 0x0800;
		return m_memory.at(real_address);
	}
	else if (in_range(address, 0x1000, 0x17FF))
	{
		//Second mirror of 0x0000 - 0x07FFF
		Byte real_address = address - (0x0800 * 2);
		return m_memory.at(real_address);
	}
	else if (in_range(address, 0x1800, 0x1FFF))
	{
		//Third mirror of 0x0000 - 0x07FFF
		Byte real_address = address - (0x0800 * 3);
		return m_memory.at(real_address);
	}
	else if (in_range(address, 0x2008, 0x3FFF))
	{
		//Mirrors of PPU registers. Repeats every 8 bytes
		Byte offset = address - 0x2000;
		return m_memory.at(address - offset);
	}

	return m_memory.at(address);


}

vector<Byte> Memory::read_data(int from, int to)
{
	assert(to > from);
	spdlog::info("Reading chunk of size {0:d} from address 0x{1:x} to 0x{2:x}", to - from, from, to);
	std::vector<Byte> data;
	data.reserve(to - from);

	//Do not allow to read from other memory regions except the one that the from address is referring to.
	if (in_range(from, 0x0000, 0x07FF) && in_range(to, 0x0000, 0x07FF))
	{
		for (int i = from; i < to; i++)
		{
			data.push_back(m_memory.at(i));
		}
		return data;
	}
	else if (in_range(from, 0x4020, 0xFFFF) && in_range(to, 0x4020, 0xFFFF))
	{
		//Cartridge space
		for(int i = from; i < to; i++)
		{
			data.push_back(m_memory.at(i));
		}
		return data;
	}
}


void Memory::write_data(int const address, Byte const data)
{
	spdlog::info("Writing to memory address 0x{0:x}", address);

	if (in_range(address, 0x0800, 0x0FFF))
	{
		//First mirror of 0x0000 - 0x07FFF
		Byte real_address = address - 0x0800;
		m_memory.at(real_address) = data;
	}
	else if (in_range(address, 0x1000, 0x17FF))
	{
		//Second mirror of 0x0000 - 0x07FFF
		Byte real_address = address - (0x0800 * 2);
		m_memory.at(real_address) = data;
	}
	else if (in_range(address, 0x1800, 0x1FFF))
	{
		//Third mirror of 0x0000 - 0x07FFF
		Byte real_address = address - (0x0800 * 3);
		m_memory.at(real_address) = data;
	}
	else if (in_range(address, 0x2008, 0x3FFF))
	{
		//Mirrors of PPU registers. Repeats every 8 bytes
		Byte offset = address - 0x2000;
		m_memory.at(address - offset) = data;
	}
	m_memory.at(address) = data;
}
void Memory::write_data_chunk(vector<Byte> data,int from, int size)
{
	spdlog::info("Writing chunk of data of size {0} from address 0x{1:x}",size,from);

	int to = size;


	assert(size <= 0xBFE0);
	//TODO Add all the other memory regions. We shouldn't allow to write to other memory regions other that
	// the one we are writing from. :P
	if (in_range(from, 0x4020, 0xFFFF) && in_range(to, 0x4020, 0xFFFF))
	{
		int index = 0;

		for(int i = from; i < to; i++)
		{
			m_memory.at(i) = data.at(index);
			index++;
		}
	}

}
