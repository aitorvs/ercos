    .text
    .align 4
    
    .global low_level_isr

    
low_level_isr:
	moveml %d0-%d7/%a0-%a6,-(%sp)
	pea (%sp,60)
	jsr low_level_isrW
	rte

   

