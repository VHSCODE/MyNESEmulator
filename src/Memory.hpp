//
// Created by vhs on 15/2/21.
//

#ifndef MYNESEMULATOR_SRC_MEMORY_HPP_
#define MYNESEMULATOR_SRC_MEMORY_HPP_
#include "types.h"
class Memory
{

	unique_ptr<array<Byte>> get_region(int from, int to);
	array<Byte> get_region(int from);
 private:
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
};

#endif //MYNESEMULATOR_SRC_MEMORY_HPP_
