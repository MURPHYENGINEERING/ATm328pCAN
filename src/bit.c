#include "types.h"
#include "bit.h"
#include "crc.h"
#include "fai.h"

/** Located at the starting address of the BIT ROM test. */
extern volatile U8_T __ld_bit_rom_start;
/** Located at the final address of the BIT ROM test. */
extern volatile U8_T __ld_bit_rom_end;

/** `TRUE` if the initial ROM checksum has been computed. */
BOOL_T g_bit_rom_initial_done;
/** Checksum of the ROM as computed at start-up. */
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
 * Perform the BIT task, running all continuous BITs.
 ******************************************************************************/
void task_bit(void)
{
    bit_rom();
}


/*******************************************************************************
 * Run the ROM BIT, testing the computed CRC32 of the ROM against the stored
 * CRC32 computed at startup. 
 ******************************************************************************/
void bit_rom(void)
{
    U32_T checksum;

    checksum = crc_compute_crc32(
        (U8_T*)(void*) &__ld_bit_rom_start,
        (SIZE_T)( &__ld_bit_rom_end - &__ld_bit_rom_start ),
        (U32_T) 0u,
        TRUE
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