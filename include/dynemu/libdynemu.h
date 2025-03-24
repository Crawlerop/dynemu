#pragma once
#include <stdint.h>
#include "dynemu/export.h"

#ifdef __cplusplus
using namespace Dynemu;

extern "C" {
#endif
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    #ifndef __cplusplus
    typedef uint8_t bool;
    typedef struct Emulator Emulator;
    #endif

    DYNEMU_EXPORT Emulator *dynemu_open();
    DYNEMU_EXPORT void dynemu_delete(Emulator *e);
    DYNEMU_EXPORT void dynemu_mmap(Emulator *e, u32 vaddr, u32 size);
    DYNEMU_EXPORT size_t dynemu_upload_file(Emulator *e, const char *filename, int64_t dest, u32 size);
    DYNEMU_EXPORT bool dynemu_upload_fls_memory(Emulator *e, const u8 *fls);
    DYNEMU_EXPORT void dynemu_upload_memstub(Emulator *e, u32 dest, u32 mem_start, u32 mem_size, bool is_big);
    DYNEMU_EXPORT void dynemu_upload_memstub_nooffset(Emulator *e, u32 mem_start, u32 mem_size, bool is_big);
    DYNEMU_EXPORT u8 dynemu_read_u8(Emulator *e, u32 vaddr);
    DYNEMU_EXPORT u16 dynemu_read_u16(Emulator *e, u32 vaddr, bool is_big);
    DYNEMU_EXPORT u32 dynemu_read_u32(Emulator *e, u32 vaddr, bool is_big);
    DYNEMU_EXPORT u64 dynemu_read_u64(Emulator *e, u32 vaddr, bool is_big);
    DYNEMU_EXPORT void dynemu_write_u8(Emulator *e, u32 vaddr, u8 value);
    DYNEMU_EXPORT void dynemu_write_u16(Emulator *e, u32 vaddr, u16 value, bool is_big);
    DYNEMU_EXPORT void dynemu_write_u32(Emulator *e, u32 vaddr, u32 value, bool is_big);
    DYNEMU_EXPORT void dynemu_write_u64(Emulator *e, u32 vaddr, u64 value, bool is_big);
    DYNEMU_EXPORT void dynemu_read_bytes(Emulator *e, u32 vaddr, u8 *output, u32 size);
    DYNEMU_EXPORT void dynemu_write_bytes(Emulator *e, u32 vaddr, const u8 *input, u32 size);
    DYNEMU_EXPORT u32 dynemu_execute(Emulator *e, u32 pc, bool is_big);
    DYNEMU_EXPORT u32 dynemu_read_reg(Emulator *e, u8 reg);
    DYNEMU_EXPORT void dynemu_write_reg(Emulator *e, u8 reg, u32 value);
#ifdef __cplusplus
}
#endif