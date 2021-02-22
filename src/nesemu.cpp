#include "nesemu.hpp"
NesEmu::NesEmu(string path)
{
	power_up();

	running = load_game(path);
	config_emu();
}

bool NesEmu::load_game(std::string path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
	std::streampos size;
	if (file.is_open())
	{
		size = file.tellg();
		file.seekg(0, std::ios::beg);
		if (size > MAX_GAME_SIZE)
		{
			spdlog::critical("Filesize too big ! Aborting...");
			return false;
		}

		file.read(m_memory.data(),size);
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
	spdlog::set_level(spdlog::level::off);
	m_memory.write_data(0x4017,0x00);
	m_memory.write_data(0x4015, 0x00);

	for (int i = 0x4000; i < 0x4013; i++)
	{
		m_memory.write_data(i, 0x00);
	}
	//The powerup values of the internal vary between consoles, in this emulator, i'll fill it with zeroes.
	for(int j = 0x0; j < 0x07FF; j++)
	{
		m_memory.write_data(j,0x0);

	}
	spdlog::set_level(spdlog::level::debug);

	spdlog::info("Power up sequence finished");
}


void NesEmu::config_emu()
{
	FileFormats file_format;
	vector<Byte> header = m_memory.read_data(0x4020, 0x4030);

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
			m_emu_config.chr_rom_size = header.at(5) * 8192;
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
		
		//We ignore the VS Unisystem bit and the playchoice one

		bool flags_8to15_nes2_format = flags7.test(3) && !flags7.test(2) ? true : false;
		std::bitset<4> upper_nibble_mapper;
		
		index = 0;
		for (int i = 7; i >= 4; i--)
		{
			if (flags7.test(i))
			{
				upper_nibble_mapper.set(index);
				index++;
			}
		}
		m_emu_config.upper_nibble_mapper = upper_nibble_mapper.to_ulong();

		if(flags_8to15_nes2_format)
		{
			//Nes2 format flags
		}
		else
		{
			m_emu_config.prg_ram_size = header.at(8) * 8192;

		}
	}
	else if(file_format == FileFormats::NES_20)
	{

	}

}