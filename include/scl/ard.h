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
 *    if key == 0 is EOF
 *
 *  2: has if hia
 *    0[0] byte is id after command
 *
 */

#define ARD_EOF 0
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


/** @brief ard_find_rule
 * @param  data Rule map that ends with key = 0, otherwise UB
 * @param  key  Key for searching on the map
 * @return The value that corresponds to the key if there is one, otherwise the first pair where key = 0
 */
ARDRule ard_find_rule(const uint8_t* data, uint8_t key);


#ifdef __cplusplus
}
#endif
