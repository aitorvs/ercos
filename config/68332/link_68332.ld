/*	Entry point	*/
ENTRY(startup)

MEMORY
{
	/* rom : ORIGIN = 0x00000000, LENGTH = 2M */
	ram : ORIGIN = 0x00000000, LENGTH = 16M
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
	__dtors_GLOBAL__ = .;
    *(.dtors)
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
    
  .misc :
  {
  	*(*)
  } > ram

  /* /DISCARD/ :
  {
    *(*)
  } */
}

