/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include <tracer/tracer.h>
#include <target.h>

/**
 * This static routine sends the stream information through the serial port
 * 
 * @param car:  This parameter is the caracter to be sent
 */
static void serial_retrieve(uint8 car);

//------------------------------------------------------------------------------

/** This is the stream buffer   */
struct buffer tracer_buffer;

/** This array stores the retrieve routines for each level  */
static OS_Trace_Retrieve_t trace_retrieve_fn[KCONFIG_TRACER_NLEVELS];

/** This variable stores the read pointer of the circular buffer    */
volatile unsigned int read_pos = 0;

/** This variable stores the write pointer of the circular buffer    */
volatile unsigned int write_pos = 0;

//------------------------------------------------------------------------------

void OS_TracerInit(void)
{
    int i;
    
    /*  The kernel register a serial retrieve routine for all levels    */
    for (i = 0; i < KCONFIG_TRACER_NLEVELS; ++i)
        trace_retrieve_fn[i].retrieveFn = serial_retrieve;
    
    /*  Stablish the buffer policy to overwrite in case of full */
    tracer_buffer.full_policy = _TRACE_UNTIL_FULL;
//    tracer_buffer.full_policy = _TRACE_OVERWRITTING;
    
    /*  Disables the stream buffer writting  */
    ENABLE_WRITING();
    
    /*  
     * The buffer reading is not allow until some write operation is 
     * performed
     */
    DISABLE_READING();
}

void inline OS_TracerStart(void)
{
    ENABLE_WRITING();
}

void OS_TracerRetrieveEvent(void)
{
    char car;
        
    /*  This method retrieves an event through the apropiate port   */
    while(1)
    {
        if(IS_READ_ENABLED) {
            /*  Extract the character fromt he stream buffer    */
            car = tracer_buffer.buf[read_pos];
            read_pos = (read_pos + 1) & bsm;
            
            /*  Sends the character */
            serial_retrieve(car);
            
            if(read_pos == write_pos) {
                DISABLE_READING();
            }
            
            ENABLE_WRITING();
        }
    }
}

static void serial_retrieve(uint8 car)
{
    CPU_SerialRxDebug(car);
}




