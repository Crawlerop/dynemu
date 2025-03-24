/* (c) 2025 Wrapper Inc. - The New Emulation Library */

#pragma once
#include <format>
#include "dynarmic/interface/A32/a32.h"
#include "dynarmic/interface/exclusive_monitor.h"
#include "dynemu/export.h"
#include <iostream>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

namespace Dynemu {
    struct MemMap final {
        u64 start;
        u64 size;
        std::unique_ptr<u8[]> data;
    };

    class DYNEMU_EXPORT MyEmulator final : public Dynarmic::A32::UserCallbacks {
        private:
            u8 find_memory_map_id(u32 vaddr);
            bool is_mmap_exists(u32 vaddr);
            template <std::size_t N>
            bool CheckExecOverridePC(const std::array<u32, N> &list, u32 pc);

        public:
            std::array<MemMap, 64> mem_map;
            u32 map_index;
            u32 os_offset;
            Dynarmic::A32::Jit *cpu;

            /* 
                MyEmulator() 
                Constructor
            */
            MyEmulator() : map_index(0), os_offset(0xa8000000), mem_map() {};

            /* 
                void CreateMemoryMap(u32 vaddr, u32 size) 
                This function creates a memory map for this specific vaddr (virtual address) with specific size
            */
            void CreateMemoryMap(u32 vaddr, u32 size);

            /* 
                void RoutineReplaceWithBXLR(const std::array<u32, N> &list, bool is_thumb, bool is_big)
                This function replaces each instruction on the list with "bx lr" instruction
                Currently, this has no use outside for hooking functions
            */
            template <std::size_t N>
            void RoutineReplaceWithBXLR(const std::array<u32, N> &list, bool is_thumb, bool is_big);

            /*
                size_t UploadToMemory(std::string filename, std::int64_t dest, u32 size)
                This function uploads a file into memory, automatically creating a memory map if not exists.
                Setting dest to -1 will use a multi-map parser
                Returns: 
                    0: NOK
                    1 or FileSize: OK
            */
            size_t UploadToMemory(std::string filename, std::int64_t dest, u32 size);

            /*
                bool UploadFLSFromBytes(const u8 *fls)
                This function uploads a multi-part code file from memory.
                Returns: 
                    0: NOK
                    1: OK
            */
            bool UploadFLSFromBytes(const u8 *fls);

            /*
                bool UploadFLSFromBytes(const std::array<u8, N> &fls)
                This function uploads a multi-part code file from memory.
                Returns: 
                    0: NOK
                    1: OK
            */
            template <std::size_t N>
            bool UploadFLSFromBytes(const std::array<u8, N> &fls) {
                return UploadFLSFromBytes(fls.data());
            }

            /*
                void UploadOSStub(u32 dest, u32 mem_start, u32 mem_size, bool is_big)
                This function uploads a stub libc to a specific dest virtual address, along with setting up the regions assosiated with the dynamic memory functions.
                Endianness were determined by is_big parameter.
            */
            void UploadOSStub(u32 dest, u32 mem_start, u32 mem_size, bool is_big);

            /*
                void UploadOSStub(u32 mem_start, u32 mem_size, bool is_big)
                This function uploads a stub libc to the configured os_offset parameter virtual address, along with setting up the regions assosiated with the dynamic memory functions.
                Endianness were determined by is_big parameter.
            */
            void UploadOSStub(u32 mem_start, u32 mem_size, bool is_big) {
                UploadOSStub(os_offset, mem_start, mem_size, is_big);
            }

            /*
                bool PreCodeReadHook(bool is_thumb, u32 pc, Dynarmic::A32::IREmitter& ir)
                Custom function used for end of code detection.
            */
            bool PreCodeReadHook(bool is_thumb, u32 pc, Dynarmic::A32::IREmitter& ir) override;

            /*
                u8 MemoryRead8(u32 vaddr)
                Reads an 8-bit value from vaddr
                Returns: byte[vaddr]
            */
            u8 MemoryRead8(u32 vaddr) override;

            /*
                void MemoryReadBytes(u32 vaddr, u8 *output, u32 size)
                Copy data from memory map to a single u8 pointer.
            */
            void MemoryReadBytes(u32 vaddr, u8 *output, u32 size);
        
            /*
                u16 MemoryRead16(u32 vaddr)
                Reads a 16-bit value from vaddr
                Returns: short[vaddr]
            */
            u16 MemoryRead16(u32 vaddr) override;
        
            /*
                u32 MemoryRead32(u32 vaddr)
                Reads a 32-bit value from vaddr
                Returns: word[vaddr]
            */
            u32 MemoryRead32(u32 vaddr) override;
        
            /*
                u64 MemoryRead64(u32 vaddr)
                Reads a 64-bit value from vaddr
                Returns: dword[vaddr]
            */
            u64 MemoryRead64(u32 vaddr) override;

            /*
                u16 MemoryRead16_Big(u32 vaddr)
                Reads a 16-bit value from vaddr
                Returns: short[vaddr]
            */
            u16 MemoryRead16_Big(u32 vaddr);
        
            /*
                u32 MemoryRead32_Big(u32 vaddr)
                Reads a 32-bit value from vaddr
                Returns: word[vaddr]
            */
            u32 MemoryRead32_Big(u32 vaddr);
        
            /*
                u64 MemoryRead64_Big(u32 vaddr)
                Reads a 64-bit value from vaddr
                Returns: dword[vaddr]
            */
            u64 MemoryRead64_Big(u32 vaddr);
        
            /*
                void MemoryWrite8(u32 vaddr, u8 value)
                Writes an 8-bit value to vaddr
            */
            void MemoryWrite8(u32 vaddr, u8 value) override;

            /*
                void MemoryWriteBytes(u32 vaddr, const u8 *input, u32 size)
                Copy data from a single u8 pointer into memory map.
            */
            void MemoryWriteBytes(u32 vaddr, const u8 *input, u32 size);

            /*
                void MemoryWriteBytes(u32 vaddr, const std::array<u8, N> &data)
                Copy data from a single u8 pointer into memory map.
            */
            template <std::size_t N>
            void MemoryWriteBytes(u32 vaddr, const std::array<u8, N> &data) {
                MemoryWriteBytes(vaddr, data.data(), N);
            };

            /*
                void MemoryWrite16(u32 vaddr, u16 value)
                Writes an 16-bit value to vaddr
            */
            void MemoryWrite16(u32 vaddr, u16 value) override;
        
            /*
                void MemoryWrite32(u32 vaddr, u32 value)
                Writes an 32-bit value to vaddr
            */
            void MemoryWrite32(u32 vaddr, u32 value) override;
        
            /*
                void MemoryWrite64(u32 vaddr, u64 value)
                Writes an 64-bit value to vaddr
            */
            void MemoryWrite64(u32 vaddr, u64 value) override;

            /*
                void MemoryWrite16_Big(u32 vaddr, u16 value)
                Writes an 16-bit value to vaddr
            */
            void MemoryWrite16_Big(u32 vaddr, u16 value);
        
            /*
                void MemoryWrite32_Big(u32 vaddr, u32 value)
                Writes an 32-bit value to vaddr
            */
            void MemoryWrite32_Big(u32 vaddr, u32 value);
        
            /*
                void MemoryWrite64_Big(u32 vaddr, u64 value)
                Writes an 64-bit value to vaddr
            */
            void MemoryWrite64_Big(u32 vaddr, u64 value);
        
            /* 
                void InterpreterFallback(u32 pc, size_t num_instructions) 
            */
            void InterpreterFallback(u32 pc, size_t num_instructions) override {
                // This is never called in practice.
                std::terminate();
            }
        
            /*
                void CallSVC(u32 swi)
            */
            void CallSVC(u32 swi) override {
                // Do something.
            }

            /*
                u32 Execute(u32 pc, bool is_big)
                Executes from specific PC
                Returns: R0 register
            */
            u32 Execute(u32 pc, bool is_big);

            /*
                u32 Execute(u32 pc)
                Executes from specific PC
                Returns: R0 register
            */
            u32 Execute(u32 pc) {
                return Execute(pc, false);
            }
        
            /*
                void ExceptionRaised(u32 pc, Dynarmic::A32::Exception exception)
            */
            void ExceptionRaised(u32 pc, Dynarmic::A32::Exception exception) override {
                std::cerr << std::format("decoder: exception at {:#08x}: ", pc);
                std::cerr << std::format("{}", (int)exception) << std::endl;
                std::terminate();
                // Do something.
            }

            /*
                void AddTicks(u64 ticks)
            */
            void AddTicks(u64 ticks) override {}
        
            /*
                GetTicksRemaining()
            */
            u64 GetTicksRemaining() override {return 8000;}
    };

    class DYNEMU_EXPORT Emulator final {
        private:
            std::unique_ptr<Dynarmic::A32::Jit> cpu;
            std::unique_ptr<Dynarmic::ExclusiveMonitor> mon;

        public:
            MyEmulator env;

            Emulator();
    };
}