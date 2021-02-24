#include <iostream>
#include <fstream>
#include "types.h"
#include "Memory.hpp"
#include "CPU.hpp"
#include "PPU.hpp"
#include "APU.hpp"
#include <bitset>
#include "spdlog/spdlog.h"


struct EmuConfig {
  int prg_rom_size;
  int prg_ram_size;
  int chr_rom_size;
  bool mirroring; // if true ,vertical mirroring is enabled, if false , horizontal mirroring is enabled.
  bool battery_prg_memory;
  bool trainer_present;
  bool four_vram; //4 screen VRAM
  Byte lower_nibble_mapper;
  Byte upper_nibble_mapper;
};

class NesEmu {

 public:
  NesEmu(string path);
  bool isRunning() const { return running; }

 private:
  EmuConfig m_emu_config;

  CPU m_cpu;
  PPU m_ppu;
  APU m_apu;


  shared_ptr<Memory> m_memory;

  bool running;

  bool load_game(string path, shared_ptr<Memory> mem);
  void power_up();
  void config_emu();

	void print_config();
};
