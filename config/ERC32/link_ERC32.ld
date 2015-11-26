/*	Entry point	*/
ENTRY(startup)

ERC32_MEC = 0x01f80000;
CLOCK_REG = 0x10000300;

ERC32_ROM_SIZE = 512K;
ERC32_RAM_SIZE = 4M;

ERC32_ROM_START = 0x00000000;
ERC32_RAM_START = 0x02000000;

ERC32_ROM_END = ERC32_ROM_START + ERC32_ROM_SIZE;
ERC32_RAM_END = ERC32_RAM_START + ERC32_RAM_SIZE;

TASK_STACKS = ERC32_RAM_END;

MEMORY
{
	rom : ORIGIN = 0x00000000, LENGTH = 512K
	ram : ORIGIN = 0x02000000, LENGTH = 4M
}

SECTIONS
{
  .text :
  {
    CODE_START = .;
  
    *(.romvec)
    *(.text)
    *(.rodata*)
    *(.gnu.linkonce.*)

    . = ALIGN(0x10);
	
    CODE_END = .;

  } > ram

  .data :
  {
  	DATA_START = .;
  	
    *(.data)

    . = ALIGN(0x10);
	__ctors_GLOBAL__ = .;
    *(.ctors)
	QUAD(0)
	. = ALIGN(0x10);
    
    DATA_END = .;
    
  } > ram

  .bss :
  {
    bss_start = .;
    *(.bss)
    *(COMMON)
    end = .;
  } > ram
    
  /DISCARD/ :
  {
    *(*)
  }
}
