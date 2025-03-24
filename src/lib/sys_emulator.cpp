/* (c) 2025 Wrapper Inc. - The New Emulation Library */

#include <iostream>
#include <fstream>
#include <format>

#include "shared/os_wrapper.hpp"
#include "dynemu/sys_emulator.hpp"

#include "dynarmic/interface/A32/a32.h"
#include "dynarmic/interface/A32/config.h"
#include "dynarmic/interface/exclusive_monitor.h"

#include "mcl/assert.hpp"

#define DEBUG 0
#define BLOCK_ALLIGN 0x10000

namespace Dynemu {
    struct FlashPartition {
        u32 start_vaddr;
        u32 size;
        std::unique_ptr<u8[]> data;
    };

    struct FlashLoader {
        u16 magic;
        u32 count;
        std::unique_ptr<FlashPartition[]> data;
    };

    /* 01 - Private */
    u8 MyEmulator::find_memory_map_id(u32 vaddr) {
        for (int i = 0; i < mem_map.size(); i++) {
            if (vaddr >= mem_map[i].start && vaddr < (mem_map[i].start + mem_map[i].size)) return i;
        }
        std::cerr << std::format("Memory map {:#08x} OOB, Emulation cannot continue", vaddr) << std::endl;
        throw std::out_of_range("Data abort");
    }

    bool MyEmulator::is_mmap_exists(u32 vaddr) {
        for (int i = 0; i < mem_map.size(); i++) {
            if (vaddr >= mem_map[i].start && vaddr < (mem_map[i].start + mem_map[i].size)) return true;
        }
        return false;
    }

    template <std::size_t N>
    bool MyEmulator::CheckExecOverridePC(const std::array<u32, N> &list, u32 pc) {
        for (int i = 0; i < N; i++) {
            if (pc == list[i]) return true;
        }

        return false;
    }

    /* 02 - Public */
    void MyEmulator::CreateMemoryMap(u32 vaddr, u32 size) {
        if (!is_mmap_exists(vaddr)) mem_map[map_index++] = {vaddr, size, std::make_unique<u8[]>(size)};
    }

    template <std::size_t N>
    void MyEmulator::RoutineReplaceWithBXLR(const std::array<u32, N> &list, bool is_thumb, bool is_big) {
        for (int i = 0; i < N; i++) {
            if (is_thumb) {
                MemoryWrite16(list[i], is_big ? 0x7047 : 0x4770);
            } else {
                MemoryWrite32(list[i], is_big ? 0x1eff2fe1 : 0xe12fff1e);
            }
        }
    }

    size_t MyEmulator::UploadToMemory(std::string filename, std::int64_t dest, u32 size) {
        std::ifstream file(filename, std::ios::binary);
        u8 buf[2048];
        u32 offset = 0;
        int to_read = 0;

        if (!file.is_open()) {
            std::cerr << std::format("Unable to open {}: {}", filename, std::strerror(errno)) << std::endl;
            return 0;
        }

        if (dest != -1) {
            file.seekg(0, file.end);
            size_t fileSize = file.tellg();
            file.seekg(0, file.beg);

            if (size < fileSize && !is_mmap_exists((u32)dest)) {
                std::cerr << "Code file too large to fit into memory" << std::endl;
                return 0;
            }

            CreateMemoryMap((u32)dest, size);

            while ((size_t)file.tellg() < fileSize) {
                to_read = (int)((fileSize - file.tellg()) >= 2048 ? 2048 : (fileSize - file.tellg()));
                file.read((char *)buf, to_read);
                MemoryWriteBytes((u32)dest + offset, buf, to_read);
                offset += to_read;
            }

            return fileSize;
        } else {
            FlashLoader fl{};
            file.read((char *)&fl.magic, sizeof(fl.magic));
            file.read((char *)&fl.count, sizeof(fl.count));

            if (fl.magic != 0x4c46) {
                std::cerr << "Invalid FL magic check" << std::endl;
                return 0;
            }

            fl.data = std::make_unique<FlashPartition[]>(fl.count);

            for (u32 i = 0; i < fl.count; i++) {
                file.read((char *)&fl.data.get()[i].start_vaddr, sizeof(fl.data.get()[i].start_vaddr));
                file.read((char *)&fl.data.get()[i].size, sizeof(fl.data.get()[i].size));
                fl.data.get()[i].data = std::make_unique<u8[]>(fl.data.get()[i].size);
                file.read((char *)fl.data.get()[i].data.get(), fl.data.get()[i].size);

                CreateMemoryMap(fl.data.get()[i].start_vaddr, fl.data.get()[i].size + ((fl.data.get()[i].size % BLOCK_ALLIGN) ? (BLOCK_ALLIGN - (fl.data.get()[i].size % BLOCK_ALLIGN)) : 0));
                MemoryWriteBytes(fl.data.get()[i].start_vaddr, fl.data.get()[i].data.get(), fl.data.get()[i].size);
            }

            return 1;
        }
    }

    bool MyEmulator::UploadFLSFromBytes(const u8 *fls) {
        FlashLoader fl{};

        memcpy(&fl.magic, fls, sizeof(fl.magic));
        memcpy(&fl.count, fls + sizeof(fl.magic), sizeof(fl.count));

        if (fl.magic != 0x4c46) {
            std::cerr << "Invalid FL magic check" << std::endl;
            return false;
        }
        
        fl.data = std::make_unique<FlashPartition[]>(fl.count);

        auto offset = sizeof(fl.magic) + sizeof(fl.count);

        for (u32 i = 0; i < fl.count; i++) {
            memcpy(&fl.data.get()[i].start_vaddr, fls + offset, sizeof(fl.data.get()[i].start_vaddr));
            memcpy(&fl.data.get()[i].size, fls + offset + sizeof(fl.data.get()[i].start_vaddr), sizeof(fl.data.get()[i].size));
            fl.data.get()[i].data = std::make_unique<u8[]>(fl.data.get()[i].size);
            memcpy((char *)fl.data.get()[i].data.get(), fls + offset + sizeof(fl.data.get()[i].start_vaddr) + sizeof(fl.data.get()[i].size), fl.data.get()[i].size);

            offset += sizeof(fl.data.get()[i].start_vaddr) + sizeof(fl.data.get()[i].size) + fl.data.get()[i].size;

            CreateMemoryMap(fl.data.get()[i].start_vaddr, fl.data.get()[i].size + ((fl.data.get()[i].size % BLOCK_ALLIGN) ? (BLOCK_ALLIGN - (fl.data.get()[i].size % BLOCK_ALLIGN)) : 0));
            MemoryWriteBytes(fl.data.get()[i].start_vaddr, fl.data.get()[i].data.get(), fl.data.get()[i].size);
        }

        return true;
    }

    void MyEmulator::UploadOSStub(u32 dest, u32 mem_start, u32 mem_size, bool is_big) {
        if (is_big) {
            MemoryWriteBytes(dest, OS_LoopLoader_BE.data(), (u32)OS_LoopLoader_BE.size());
            MemoryWrite32_Big(dest + MEM_INIT_REG_OFFSET, mem_start);
            MemoryWrite32_Big(dest + MEM_INIT_REG_SIZE, mem_size);
        } else {
            MemoryWriteBytes(dest, OS_LoopLoader.data(), (u32)OS_LoopLoader.size());
            MemoryWrite32(dest + MEM_INIT_REG_OFFSET, mem_start);
            MemoryWrite32(dest + MEM_INIT_REG_SIZE, mem_size);
        }
        os_offset = dest;
    }

    bool MyEmulator::PreCodeReadHook(bool is_thumb, u32 pc, Dynarmic::A32::IREmitter& ir) { 
    #if DEBUG            
        std::cout << std::format("executing at {:#08x}", pc) << std::endl;
        std::cout << std::format("REG P0 {:#08x}", cpu->Regs()[0]) << std::endl;
        std::cout << std::format("REG P1 {:#08x}", cpu->Regs()[1]) << std::endl;
        std::cout << std::format("REG P2 {:#08x}", cpu->Regs()[2]) << std::endl;
        std::cout << std::format("REG P12 {:#08x}", cpu->Regs()[12]) << std::endl;
        std::cout << std::format("REG P15 {:#08x}", cpu->Regs()[15]) << std::endl;
    #endif

        if (pc == (os_offset + ROUTE_EXIT)) cpu->HaltExecution();
        
        return true; 
    }

    u8 MyEmulator::MemoryRead8(u32 vaddr) {
    #if DEBUG
        if (cpu->IsExecuting()) printf("Reading at 0x%08x\n", vaddr);
    #endif
        try {
            u8 mem_now = find_memory_map_id(vaddr);
            return mem_map[mem_now].data.get()[vaddr - mem_map[mem_now].start];
        } catch (std::out_of_range) {
            cpu->HaltExecution(Dynarmic::HaltReason::MemoryAbort);
            return 0xff; // Undefined behavior can occur
        }
    }

    void MyEmulator::MemoryReadBytes(u32 vaddr, u8 *output, u32 size) {
        u8 mem_now = find_memory_map_id(vaddr);
        ASSERT(((vaddr - mem_map[mem_now].start) + size) <= mem_map[mem_now].size);
        memcpy(output, mem_map[mem_now].data.get() + (vaddr - mem_map[mem_now].start), size);
    }

    u16 MyEmulator::MemoryRead16(u32 vaddr) {
        return u16(MemoryRead8(vaddr)) | u16(MemoryRead8(vaddr + 1)) << 8;
    }

    u32 MyEmulator::MemoryRead32(u32 vaddr) {
        return u32(MemoryRead16(vaddr)) | u32(MemoryRead16(vaddr + 2)) << 16;
    }

    u64 MyEmulator::MemoryRead64(u32 vaddr) {
        return u64(MemoryRead32(vaddr)) | u64(MemoryRead32(vaddr + 4)) << 32;
    }

    u16 MyEmulator::MemoryRead16_Big(u32 vaddr) {
        return u16(MemoryRead8(vaddr + 1)) | u16(MemoryRead8(vaddr)) << 8;
    }

    u32 MyEmulator::MemoryRead32_Big(u32 vaddr) {
        return u32(MemoryRead16_Big(vaddr + 2)) | u32(MemoryRead16_Big(vaddr)) << 16;
    }

    u64 MyEmulator::MemoryRead64_Big(u32 vaddr) {
        return u64(MemoryRead32_Big(vaddr + 4)) | u64(MemoryRead32_Big(vaddr)) << 32;
    }

    void MyEmulator::MemoryWrite8(u32 vaddr, u8 value) {
    #if DEBUG
        if (cpu->IsExecuting()) printf("Writing at 0x%08x 0x%02x\n", vaddr, value);
    #endif
        try {
            u8 mem_now = find_memory_map_id(vaddr);
            mem_map[mem_now].data.get()[vaddr - mem_map[mem_now].start] = value;
        } catch (std::out_of_range) {
            cpu->HaltExecution(Dynarmic::HaltReason::MemoryAbort);
        }
    }

    void MyEmulator::MemoryWriteBytes(u32 vaddr, const u8 *input, u32 size) {
        u8 mem_now = find_memory_map_id(vaddr);
        ASSERT(((vaddr - mem_map[mem_now].start) + size) <= mem_map[mem_now].size);
        memcpy(mem_map[mem_now].data.get() + (vaddr - mem_map[mem_now].start), input, size);
    }

    void MyEmulator::MemoryWrite16(u32 vaddr, u16 value) {
        MemoryWrite8(vaddr, u8(value));
        MemoryWrite8(vaddr + 1, u8(value >> 8));
    }

    void MyEmulator::MemoryWrite32(u32 vaddr, u32 value) {
        MemoryWrite16(vaddr, u16(value));
        MemoryWrite16(vaddr + 2, u16(value >> 16));
    }

    void MyEmulator::MemoryWrite64(u32 vaddr, u64 value) {
        MemoryWrite32(vaddr, u32(value));
        MemoryWrite32(vaddr + 4, u32(value >> 32));
    }

    void MyEmulator::MemoryWrite16_Big(u32 vaddr, u16 value) {
        MemoryWrite8(vaddr + 1, u8(value));
        MemoryWrite8(vaddr, u8(value >> 8));
    }

    void MyEmulator::MemoryWrite32_Big(u32 vaddr, u32 value) {
        MemoryWrite16_Big(vaddr + 2, u16(value));
        MemoryWrite16_Big(vaddr, u16(value >> 16));
    }

    void MyEmulator::MemoryWrite64_Big(u32 vaddr, u64 value) {
        MemoryWrite32_Big(vaddr + 4, u32(value));
        MemoryWrite32_Big(vaddr, u32(value >> 32));
    }

    u32 MyEmulator::Execute(u32 pc, bool is_big) {
        cpu->Regs()[14] = os_offset + ROUTE_EXIT;
        cpu->Regs()[15] = pc & ~1;
        cpu->SetCpsr(((pc & 1) ? 0x30 : 0x1d0) | (is_big ? 0x200 : 0x0));

        Dynarmic::HaltReason returnCode = cpu->Run();
        cpu->ClearCache();

        return cpu->Regs()[0];
    }

    Emulator::Emulator() {
        Dynarmic::A32::UserConfig user_config;

        user_config.callbacks = &env;
        user_config.enable_cycle_counting = false;
        user_config.arch_version = Dynarmic::A32::ArchVersion::v7;

        mon = std::make_unique<Dynarmic::ExclusiveMonitor>(1);
        user_config.global_monitor = mon.get();

        cpu = std::make_unique<Dynarmic::A32::Jit>(user_config);
        env.cpu = cpu.get();
    };
}