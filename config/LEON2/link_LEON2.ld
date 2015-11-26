/*	Entry point	*/
ENTRY(startup)

REGMAP = 0x80000000;

LEON2_ROM_SIZE = 2M;
LEON2_RAM_SIZE = 4M;

LEON2_ROM_START = 0x00000000;
LEON2_RAM_START = 0x40000000;

LEON2_ROM_END = LEON2_ROM_START + LEON2_ROM_SIZE;
LEON2_RAM_END = LEON2_RAM_START + LEON2_RAM_SIZE;

TASK_STACKS = LEON2_RAM_END;

MEMORY
{
	rom : ORIGIN = 0x00000000, LENGTH = 2M
	ram : ORIGIN = 0x40000000, LENGTH = 4M
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
