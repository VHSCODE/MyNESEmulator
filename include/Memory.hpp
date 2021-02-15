//
// Created by vhs on 15/2/21.
//

#ifndef MYNESEMULATOR_SRC_MEMORY_HPP_
#define MYNESEMULATOR_SRC_MEMORY_HPP_
#define TOTAL_MEMORY_SIZE 0x0800 + 0x8 + 0x18 + 0x8 +0xBFE0
#include "types.h"

class Memory
{
 public:
	void write_data(int address, Byte data);
	Byte read_data(int address) const;
	vector<Byte> read_data(int from, int to);

	auto data() {return m_memory.data();}

 private:
	/*  memory Map
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
	array<Byte, TOTAL_MEMORY_SIZE> m_memory;
};

#endif //MYNESEMULATOR_SRC_MEMORY_HPP_
