#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include <iostream>
#include <fstream>
#include "types.h"
#include "Memory.hpp"
#include "cpu.hpp"
#include <bitset>


struct EmuConfig {
  int prg_rom_size;
  int prg_ram_size;
  bool mirroring; // if true vertical mirroring is enabled, if false horizontal mirroring is enabled.
  bool battery_prg_memory;
  bool trainer_present;
  bool four_vram; //4 screen VRAM
  Byte lower_nibble_mapper;
};

class NesEmu {

 public:
  NesEmu(string path);
  bool isRunning() const { return running; }

 private:
  EmuConfig m_emu_config;

  CPU m_cpu;
  Memory m_memory;

  bool running;

  bool load_game(string path);
  void power_up();
  void config_emu();

};
