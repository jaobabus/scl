#pragma once

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/** SCLConsoleBufferContext
 */
typedef struct
{
    /** write
     *
     *  @param opaque SCLConsoleBufferContext::opaque
     *  @param data   Data to console
     *  @param size   size of data
     */
    void (*write)(void* opaque, const void* data, size_t size);

    /** execute
     *
     *  Will be called when reached '\n', '\r'
     *
     *  @param opaque SCLConsoleBufferContext::opaque
     *  @param buffer Command line buffer, after call buffer will be reused
     *  @param size   size of buffer
     */
    void (*execute)(void* opaque, void* buffer, size_t size);
#ifndef SCL_NO_COLORS
    /** colors
     *
     *  Table of 256 ansi colors for every token
     *  @see SHLToken
     */
    const uint8_t* colors;
#endif

    /** opaque
     *  User opaque
     */
    void* opaque;

    /** buffer
     *  Commands buffer
     */
    uint8_t* buffer;

    /** buffer_size
     *  Full buffer size
     */
    uint16_t buffer_size;

    /** size
     *  Used size
     */
    uint16_t size;

    /** cursor
     *  Cursor position
     */
    uint16_t cursor;

    /** escape_index
     *  Escape cursor position after SCLConsoleBufferContext::size
     */
    uint8_t escape_index;

    /** escape_stack_size
     *  Count of collected escapes after SCLConsoleBufferContext::size
     */
    uint8_t escape_stack_size;
} SCLConsoleBufferContext;


/** sclcb_make_context
 *
 * @param  buffer Command line buffer
 * @param  size   Size of buffer
 * @param  opaque User opaque
 * @param  colors 256 ansi colors
 * @return Completed context
 */
SCLConsoleBufferContext sclcb_make_context(void* buffer, uint16_t size,
                                           void* opaque,
                                           void (*write)(void* opaque, const void* data, size_t size),
                                           void (*execute)(void* opaque, void* buffer, size_t size),
                                           const uint8_t* colors);


/** sclcb_on_char
 *
 *  Put next char to buffer
 *
 * @param ctx Pointer to context
 * @param ch  char to put
 */
void sclcb_on_char(SCLConsoleBufferContext* ctx, char ch);

/** sclcb_clear
 *
 *  Clear buffer
 *
 * @param ctx Pointer to context
 */
void sclcb_clear(SCLConsoleBufferContext* ctx);


#ifdef __cplusplus
}
#endif
