#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef void Emulator;

Emulator *dynemu_open();
void dynemu_delete(Emulator *e);
void dynemu_mmap(Emulator *e, u32 vaddr, u32 size);
size_t dynemu_upload_file(Emulator *e, const char *filename, int64_t dest, u32 size);
u8 dynemu_upload_fls_memory(Emulator *e, const u8 *fls);
void dynemu_upload_memstub(Emulator *e, u32 dest, u32 mem_start, u32 mem_size, u8 is_big);
void dynemu_upload_memstub_nooffset(Emulator *e, u32 mem_start, u32 mem_size, u8 is_big);
u8 dynemu_read_u8(Emulator *e, u32 vaddr);
u16 dynemu_read_u16(Emulator *e, u32 vaddr, u8 is_big);
u32 dynemu_read_u32(Emulator *e, u32 vaddr, u8 is_big);
u64 dynemu_read_u64(Emulator *e, u32 vaddr, u8 is_big);
void dynemu_write_u8(Emulator *e, u32 vaddr, u8 value);
void dynemu_write_u16(Emulator *e, u32 vaddr, u16 value, u8 is_big);
void dynemu_write_u32(Emulator *e, u32 vaddr, u32 value, u8 is_big);
void dynemu_write_u64(Emulator *e, u32 vaddr, u64 value, u8 is_big);
void dynemu_read_bytes(Emulator *e, u32 vaddr, u8 *output, u32 size);
void dynemu_write_bytes(Emulator *e, u32 vaddr, const u8 *input, u32 size);
u32 dynemu_execute(Emulator *e, u32 pc, u8 is_big);
u32 dynemu_read_reg(Emulator *e, u8 reg);
void dynemu_write_reg(Emulator *e, u8 reg, u32 value);