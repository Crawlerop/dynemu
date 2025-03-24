#include "dynemu/sys_emulator.hpp"
using namespace Dynemu;

extern "C" {
     #include "dynemu/libdynemu.h"

     Emulator *dynemu_open() {
          auto temp = new Emulator();
          return temp;
     }

     void dynemu_delete(Emulator *e) {
          delete e;
     }

     void dynemu_mmap(Emulator *e, u32 vaddr, u32 size) {
          return e->env.CreateMemoryMap(vaddr, size);
     }

     size_t dynemu_upload_file(Emulator *e, const char *filename, std::int64_t dest, u32 size) {
          return e->env.UploadToMemory(filename, dest, size);
     }

     bool dynemu_upload_fls_memory(Emulator *e, const u8 *fls) {
          return e->env.UploadFLSFromBytes(fls);
     }

     void dynemu_upload_memstub(Emulator *e, u32 dest, u32 mem_start, u32 mem_size, bool is_big) {
          return e->env.UploadOSStub(dest, mem_start, mem_size, is_big);
     }

     void dynemu_upload_memstub_nooffset(Emulator *e, u32 mem_start, u32 mem_size, bool is_big) {
          return e->env.UploadOSStub(mem_start, mem_size, is_big);
     }

     u8 dynemu_read_u8(Emulator *e, u32 vaddr) {
          return e->env.MemoryRead8(vaddr);
     }

     u16 dynemu_read_u16(Emulator *e, u32 vaddr, bool is_big) {
          if (is_big) {
               return e->env.MemoryRead16_Big(vaddr);
          } else {
               return e->env.MemoryRead16(vaddr);
          }
     }

     u32 dynemu_read_u32(Emulator *e, u32 vaddr, bool is_big) {
          if (is_big) {
               return e->env.MemoryRead32_Big(vaddr);
          } else {
               return e->env.MemoryRead32(vaddr);
          }
     }

     u64 dynemu_read_u64(Emulator *e, u32 vaddr, bool is_big) {
          if (is_big) {
               return e->env.MemoryRead64_Big(vaddr);
          } else {
               return e->env.MemoryRead64(vaddr);
          }
     }

     void dynemu_write_u8(Emulator *e, u32 vaddr, u8 value) {
          e->env.MemoryWrite8(vaddr, value);
     }

     void dynemu_write_u16(Emulator *e, u32 vaddr, u16 value, bool is_big) {
          if (is_big) {
               e->env.MemoryWrite16_Big(vaddr, value);
          } else {
               e->env.MemoryWrite16(vaddr, value);
          }
     }

     void dynemu_write_u32(Emulator *e, u32 vaddr, u32 value, bool is_big) {
          if (is_big) {
               e->env.MemoryWrite32_Big(vaddr, value);
          } else {
               e->env.MemoryWrite32(vaddr, value);
          }
     }

     void dynemu_write_u64(Emulator *e, u32 vaddr, u64 value, bool is_big) {
          if (is_big) {
               e->env.MemoryWrite64_Big(vaddr, value);
          } else {
               e->env.MemoryWrite64(vaddr, value);
          }
     }

     void dynemu_read_bytes(Emulator *e, u32 vaddr, u8 *output, u32 size) {
          e->env.MemoryReadBytes(vaddr, output, size);
     }

     void dynemu_write_bytes(Emulator *e, u32 vaddr, const u8 *input, u32 size) {
          e->env.MemoryWriteBytes(vaddr, input, size);
     }

     u32 dynemu_execute(Emulator *e, u32 pc, bool is_big) {
          return e->env.Execute(pc, is_big);
     }

     u32 dynemu_read_reg(Emulator *e, u8 reg) {
          return e->env.cpu->Regs()[reg];
     }

     void dynemu_write_reg(Emulator *e, u8 reg, u32 value) {
          e->env.cpu->Regs()[reg] = value;
     }
}