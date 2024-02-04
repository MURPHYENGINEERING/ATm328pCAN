#include "types.h"
#include "bit.h"
#include "crc.h"
#include "fai.h"


#define BIT_ROM_START (SIZE_T) 0u
#define BIT_ROM_LEN   (SIZE_T) 16384u

/** `TRUE` if the initial ROM checksum has been computed. */
BOOL_T g_bit_rom_initial_done;
U32_T g_bit_rom_checksum;

/*******************************************************************************
 * Initialize the BIT system.
 ******************************************************************************/
void bit_init(void)
{
    g_bit_rom_checksum = (U32_T) 0u;
    g_bit_rom_initial_done = FALSE;
}


/*******************************************************************************
 * Run the ROM BIT, testing the computed CRC32 of the ROM against the stored
 * CRC32 computed at startup. 
 ******************************************************************************/
void task_bit_rom(void)
{
    U32_T checksum;

    checksum = crc_compute_checksum32(
        (U8_T*)(void*) BIT_ROM_START,
        BIT_ROM_LEN,
        0u
    );

    if (FALSE == g_bit_rom_initial_done) {
        g_bit_rom_checksum = checksum;
        g_bit_rom_initial_done = TRUE;
    } else {
        if (checksum != g_bit_rom_checksum) {
            fai_pass_fail_logger(FAI_FAULT_ID_BIT_ROM, FAIL, checksum);
        } else {
            fai_pass_fail_logger(FAI_FAULT_ID_BIT_ROM, PASS, (U32_T) 0u);
        }
    }
}


/*******************************************************************************
 * Retrieve the computed checksum of the ROM.
 * \return The 32-bit checksum of the ROM.
 ******************************************************************************/
U32_T bit_rom_get_checksum(void)
{
    return g_bit_rom_checksum;
}