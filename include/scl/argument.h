#ifndef SCL_ARGUMENT_H
#define SCL_ARGUMENT_H

#include "inplace.h"

#ifdef __cplusplus
extern "C" {
#endif


/** SCLArgumentDescriptor
 *
 *  Information for constructing and paring arguments
 */
typedef struct
{
    /** destruct
     *
     *  @param obj Pointer to object data
     *
     *  @note Constructor must be called in parse()
     */
    void (*destruct)(void* obj);

    /** Parse
     *
     *  @param  opaque Opaque @see SCLCommandDescriptor::arguments_opaques
     *  @param  obj    Raw pointer to an object, must be constructed
     *  @param  token  Token for parsing
     *  @return SCLError
     */
    uint8_t (*parse)(const void* opaque, void* obj, SHLITokenInfo token);

    /** completes
     *
     *  @param  opaque      Opaque @see SCLCommandDescriptor::arguments_opaques
     *  @param  buffer      Buffer to completes where must be placed and separated by '\n'
     *  @param  buffer_size Completes buffer size
     *  @param  token       Text to be completed
     *  @param  size        Size of token
     *  @return Completes size
     */
    size_t (*completes)(
        const void* opaque, char* buffer, size_t buffer_size, const char* token, size_t size);

    /** obj_size
     *
     *  Size of argument type object
     */
    uint16_t obj_size;

    /** obj_offset
     *
     *  Will be moved to SCLCommandDescriptor
     */
    uint16_t obj_offset;

    /** padding
     *
     *  Padding to 8 bytes
     */
    //uint16_t padding[1 + (sizeof(void*) == 8 ? 2 : 0)];
} SCLArgumentDescriptor;


#ifdef __cplusplus
}
#endif

#endif // SCL_ARGUMENT_H
