#include <array>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "types.h"
using std::array;
using std::vector;
using std::string;

typedef char Byte;
struct CPURegisters {
  Byte A;    //Accumulator
  Byte X, Y; //Used for several adressing modes
  short PC;  //Program counter
  Byte S;    //Stack pointer
  Byte P;    //Status register
};

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
  CPURegisters m_cpu_regs;
  EmuConfig m_emu_config;

  /*  Memory Map
  Address range	Size	Device
  $0000-$07FF     $0800	2KB internal RAM
  $0800-$0FFF     $0800	Mirrors of $0000-$07FF
  $1000-$17FF     $0800   Mirrors of $0000-$07FF
  $1800-$1FFF     $0800   Mirrors of $0000-$07FF
  $2000-$2007     $0008	NES PPU registers
  $2008-$3FFF     $1FF8	Mirrors of $2000-2007 (repeats every 8 bytes)
  $4000-$4017     $0018	NES APU and I/O registers
  $4018-$401F     $0008	APU and I/O functionality that is normally disabled. See CPU Test Mode.
  $4020-$FFFF     $BFE0	Cartridge space: PRG ROM, PRG RAM, and mapper registers (See Note)
  */

  array<Byte, 0x0800> m_internal_ram;
  array<Byte, 0x8> m_ppu_registers;
  array<Byte, 0x18> m_apu_io_registers;
  array<Byte, 0x8> m_apu_io_func;
  array<Byte, 0xBFE0> m_cartridge_memory;

  bool running;



  bool load_game(string path);
  void power_up();
  void write_data(int const address, Byte const data);
  Byte read_data(int address);
  vector<Byte> read_data(int from, int to);
  void config_emu();

};
