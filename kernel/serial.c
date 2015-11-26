#include <kernel/serial.h>
#include <kernel/debug.h>
#include <sys/types.h>

//------------------------------------------------------------------------------

/**
 * This routine is the serial line ISR
 * 
 * @param _level:   This parameter is the irq level
 * 
 * The routine waits for a ^R character and reset the platform when it arrives
 * 
 * @return The routine does not return any value
 */
static void OS_SerialISR (int _level);


//------------------------------------------------------------------------------

/*
 *  This is the serial driver's generic interrupt routine for ERC-32
 */
static void OS_SerialISR (int _level) {

    sint32 car;
    
    if(IS_SERIAL_DATA_READY()) {
        CPU_SerialRx(&car);
        if ((char)car == 0x12)  // ^R
        {
        	DEB("Reset\n");
            CPU_Reset();
        }
    }

}

void OS_InitSerialLine()
{
    /*  Register the General Purpose Timer ISR  */
    OS_RegisterIRQ(SERIAL_TRAP, OS_SerialISR);
}
