/*	Entry point	*/
ENTRY(startup)

MCTRL = 0x80000000;
UARTA = 0x80000100;
IRQMP = 0x80000200;
GPTIMER = 0x80000300;

LEON3_ROM_SIZE = 2M;
LEON3_RAM_SIZE = 4M;

LEON3_ROM_START = 0x00000000;
LEON3_RAM_START = 0x40000000;

LEON3_ROM_END = LEON3_ROM_START + LEON3_ROM_SIZE;
LEON3_RAM_END = LEON3_RAM_START + LEON3_RAM_SIZE;

TASK_STACKS = LEON3_RAM_END;

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
