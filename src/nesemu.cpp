#include <bitset>
#include "nesemu.hpp"
#include "utils.hpp"
NesEmu::NesEmu(string path)
{
	power_up();

	running = load_game(path);
	config_emu();
	write_data(0, 0);

	auto tmp = read_data(0);
}

bool NesEmu::load_game(std::string path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
	std::streampos size;
	if (file.is_open())
	{
		size = file.tellg();
		file.seekg(0, std::ios::beg);
		if (size > this->m_cartridge_memory.size())
		{
			spdlog::critical("Filesize too big ! Aborting...");
			return false;
		}

		file.read(this->m_cartridge_memory.data(), size);
		file.close();

		spdlog::info("Rom loaded successfully!");
		return true;
	}
	else
		return false;
}

void NesEmu::power_up()
{
	spdlog::info("Power up sequence initiated");


	write_data(0x4017, 0x00);
	write_data(0x4015, 0x00);

	for (int i = 0x4000; i < 0x4013; i++)
	{
		write_data(i, 0x00);
	}
	//The powerup values of the internal vary between consoles, in this emulator, i'll fill it with zeroes.
	m_internal_ram.fill(0x0);
}

void NesEmu::write_data(int const address, Byte const data)
{
	spdlog::info("Writing to memory address 0x{0:x}", address);
	if (is_in_range(address, 0x0000, 0x07FF))
	{
		//CPU internal RAM
		m_internal_ram.at(address) = data;
	}
	else if (is_in_range(address, 0x0800, 0x0FFF))
	{
		//First mirror of 0x0000 - 0x07FFF
		Byte real_address = address - 0x0800;
		m_internal_ram.at(real_address) = data;
	}
	else if (is_in_range(address, 0x1000, 0x17FF))
	{
		//Second mirror of 0x0000 - 0x07FFF
		Byte real_address = address - (0x0800 * 2);
		m_internal_ram.at(real_address) = data;
	}
	else if (is_in_range(address, 0x1800, 0x1FFF))
	{
		//Third mirror of 0x0000 - 0x07FFF
		Byte real_address = address - (0x0800 * 3);
		m_internal_ram.at(real_address) = data;
	}
	else if (is_in_range(address, 0x2000, 0x2007))
	{
		//PPU Registers
		m_ppu_registers.at(address - 0x2000) = data;
	}
	else if (is_in_range(address, 0x2008, 0x3FFF))
	{
		//Mirrors of PPU registers. Repeats every 8 bytes
		Byte offset = address - 0x2000;
		m_ppu_registers.at(address - address - offset) = data;
	}
	else if (is_in_range(address, 0x4000, 0x4017))
	{
		//APU and i/o registers
		m_apu_io_registers.at(address - 0x4000) = data;
	}
	else if (is_in_range(address, 0x4018, 0x401F))
	{
		//APU and i/o functionalities
		m_apu_io_func.at(address - 0x4018) = data;
	}
	else if (is_in_range(address, 0x4020, 0xFFFF))
	{
		//APU and i/o registers
		m_cartridge_memory.at(address - 0x4020) = data;
	}
}
Byte NesEmu::read_data(int const address)
{
	assert(address <= 0xFFFF);
	spdlog::info("Reading from memory address 0x{0:x}", address);
	if (is_in_range(address, 0x0000, 0x07FF))
	{
		//CPU internal RAM
		return m_internal_ram.at(address);
	}
	else if (is_in_range(address, 0x0800, 0x0FFF))
	{
		//First mirror of 0x0000 - 0x07FFF
		Byte real_address = address - 0x0800;
		return m_internal_ram.at(real_address);
	}
	else if (is_in_range(address, 0x1000, 0x17FF))
	{
		//Second mirror of 0x0000 - 0x07FFF
		Byte real_address = address - (0x0800 * 2);
		return m_internal_ram.at(real_address);
	}
	else if (is_in_range(address, 0x1800, 0x1FFF))
	{
		//Third mirror of 0x0000 - 0x07FFF
		Byte real_address = address - (0x0800 * 3);
		return m_internal_ram.at(real_address);
	}
	else if (is_in_range(address, 0x2000, 0x2007))
	{
		//PPU Registers
		return m_ppu_registers.at(address - 0x2000);
	}
	else if (is_in_range(address, 0x2008, 0x3FFF))
	{
		//Mirrors of PPU registers. Repeats every 8 bytes
		Byte offset = address - 0x2000;
		return m_ppu_registers.at(address - address - offset);
	}
	else if (is_in_range(address, 0x4000, 0x4017))
	{
		//APU and i/o registers
		return m_apu_io_registers.at(address - 0x4000);
	}
	else if (is_in_range(address, 0x4018, 0x401F))
	{
		//APU and i/o functionalities
		return m_apu_io_func.at(address - 0x4018);
	}
	else if (is_in_range(address, 0x4020, 0xFFFF))
	{
		//APU and i/o registers
		return m_cartridge_memory.at(address - 0x4020);
	}
}

void NesEmu::config_emu()
{
	FileFormats file_format;
	vector<Byte> header = read_data(0x4020, 0x4030);

	//Check the constant
	if (header.at(0) == 'N' && header.at(1) == 'E' && header.at(2) == 'S' && header.at(3) == 0x1A)
	{
		file_format = FileFormats::iNES;

		if ((header.at(7) & 0x0C) == 0x08)
		{
			file_format = FileFormats::NES_20;
		}
	}
	else
	{
		spdlog::critical("NES CONSTANT NOT FOUND. ABORTING...");
		return;
	}

	if (file_format == FileFormats::iNES)
	{
		m_emu_config.prg_rom_size = header.at(4) * 16384;
		if (header.at(5) != 0)
		{
			m_emu_config.prg_ram_size = header.at(5) * 8192;
		}
		std::bitset<8> flags6 = header.at(6);


		m_emu_config.mirroring = flags6.test(0);
		m_emu_config.battery_prg_memory = flags6.test(1);
		m_emu_config.trainer_present = flags6.test(2);
		m_emu_config.four_vram = flags6.test(3);
		m_emu_config.lower_nibble_mapper = 0;

		std::bitset<4> lower_nibble_mapper;
		int index = 0;
		for (int i = 7; i >= 4; i--)
		{
			if (flags6.test(i))
			{
				lower_nibble_mapper.set(index);
				index++;
			}
		}
		m_emu_config.lower_nibble_mapper = lower_nibble_mapper.to_ulong();

		std::bitset<8> flags7 = header.at(7);

	}

}

vector<Byte> NesEmu::read_data(int from, int to)
{
	assert(to > from);

	spdlog::info("Reading chunk of size {0:d} from address 0x{1:x} to 0x{2:x}", to - from, from, to);
	std::vector<Byte> data;
	data.reserve(to - from);

	//Do not allow to read from other memory regions except the one that the from address is referring to.
	if (is_in_range(from, 0x0000, 0x07FF) && is_in_range(to, 0x0000, 0x07FF))
	{
		for (int i = from; i < to; i++)
		{
			data.push_back(m_internal_ram.at(i));
		}
		return data;
	}
	else if (is_in_range(from, 0x4020, 0xFFFF) && is_in_range(to, 0x4020, 0xFFFF))
	{
		//APU and i/o registers

		for(int i = from - 0x4020; i < (to - 0x4020); i++)
		{
			data.push_back(m_cartridge_memory.at(i));
		}
		return data;
	}
}
