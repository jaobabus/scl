#pragma once

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/* Ascii Rule Dict
 *
 * Dict structure:
 *  Dict:
 *  0: [hia: bit] [key: bit[7]]
 *  1: [value: bit[8]]
 *  2: [id_after: bit[8]]
 *
 *  0:
 *    0[0] bit is hia (Has Id After)
 *    0[1] byte is ascii7 key
 *
 *    if not key is EOF
 *
 *  2: has if hia
 *    0[0] byte is id after command
 *
 */

#define ARD_EOF 0
#define ARD_EOT 0x80
#define ARD_HIA_MASK 0x80
#define ARD_HIA 0x80
#define ARD_KEY_MASK 0x7F


typedef struct {
    uint8_t value;
    uint8_t id_after;
} ARDRule;


typedef struct {
    uint8_t key;
    uint8_t value;
} ARDMiniRule;



ARDRule ard_find_rule(const uint8_t* data, uint8_t key);


#ifdef __cplusplus
}
#endif
