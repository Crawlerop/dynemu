#include <cstdint>
#pragma once

#if defined(_MSC_VER)
//  Microsoft 
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

extern "C" {
    EXPORT Emulator *dynemu_open();
    EXPORT void dynemu_delete(Emulator *e);
    EXPORT void dynemu_mmap(Emulator *e, u32 vaddr, u32 size);
    EXPORT size_t dynemu_upload_file(Emulator *e, const char *filename, int64_t dest, u32 size);
    EXPORT bool dynemu_upload_fls_memory(Emulator *e, const u8 *fls);
    EXPORT void dynemu_upload_memstub(Emulator *e, u32 dest, u32 mem_start, u32 mem_size, bool is_big);
    EXPORT void dynemu_upload_memstub_nooffset(Emulator *e, u32 mem_start, u32 mem_size, bool is_big);
    EXPORT u8 dynemu_read_u8(Emulator *e, u32 vaddr);
    EXPORT u16 dynemu_read_u16(Emulator *e, u32 vaddr, bool is_big);
    EXPORT u32 dynemu_read_u32(Emulator *e, u32 vaddr, bool is_big);
    EXPORT u64 dynemu_read_u64(Emulator *e, u32 vaddr, bool is_big);
    EXPORT void dynemu_write_u8(Emulator *e, u32 vaddr, u8 value);
    EXPORT void dynemu_write_u16(Emulator *e, u32 vaddr, u16 value, bool is_big);
    EXPORT void dynemu_write_u32(Emulator *e, u32 vaddr, u32 value, bool is_big);
    EXPORT void dynemu_write_u64(Emulator *e, u32 vaddr, u64 value, bool is_big);
    EXPORT void dynemu_read_bytes(Emulator *e, u32 vaddr, u8 *output, u32 size);
    EXPORT void dynemu_write_bytes(Emulator *e, u32 vaddr, const u8 *input, u32 size);
    EXPORT u32 dynemu_execute(Emulator *e, u32 pc, bool is_big);
    EXPORT u32 dynemu_read_reg(Emulator *e, u8 reg);
    EXPORT void dynemu_write_reg(Emulator *e, u8 reg, u32 value);
}