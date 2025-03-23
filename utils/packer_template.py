import struct
from keystone import *

# ("file_name", 0xOFFSET)
files = [

]

asm = Ks(KS_ARCH_ARM, KS_MODE_ARM)
asm_t = Ks(KS_ARCH_ARM, KS_MODE_THUMB)

asm_be = Ks(KS_ARCH_ARM, KS_MODE_ARM + KS_MODE_BIG_ENDIAN)
asm_be_t = Ks(KS_ARCH_ARM, KS_MODE_THUMB + KS_MODE_BIG_ENDIAN)

# Override addresses
MEMCPY_ADDRESS = []
MALLOC_ADDRESS = []
FREE_ADDRESS = []
MEMSET_ADDRESS = []
CALLOC_ADDRESS = []
REALLOC_ADDRESS = []
MEMMOVE_ADDRESS = []

MEMCPY_ADDRESS_T = []
MALLOC_ADDRESS_T = []
FREE_ADDRESS_T = []
MEMSET_ADDRESS_T = []
CALLOC_ADDRESS_T = []
REALLOC_ADDRESS_T = []
MEMMOVE_ADDRESS_T = []

ROUTE_INIT_REGION = 0x0
ROUTE_MALLOC = 0x4
ROUTE_MEMCPY = 0x8
ROUTE_FREE = 0xc
ROUTE_MEMSET = 0x10
ROUTE_CALLOC = 0x14
ROUTE_REALLOC = 0x18
ROUTE_MEMMOVE = 0x1c
ROUTE_EXIT = 0x20

ROUTE_MALLOC_T = 0x24
ROUTE_MEMCPY_T = 0x2c
ROUTE_FREE_T = 0x34
ROUTE_MEMSET_T = 0x3c
ROUTE_CALLOC_T = 0x44
ROUTE_REALLOC_T = 0x4c
ROUTE_MEMMOVE_T = 0x54
ROUTE_EXIT_T = 0x5c

ROUTE_MEM_BASE = 0x0

if __name__ == "__main__":
    f = open("(OUTPUT_FILE).fls", "wb")
    
    # 01 - Files
    f.write(b"FL")
    f.write(struct.pack("<L", 0))
    for fl in files:
        flashName, offset = fl
        flash = open(flashName, "rb").read()
        f.write(struct.pack("<LL", offset, len(flash))+flash)
        
    tCount = len(files)
        
    # 02 - Patching (ARM)
    for a in MALLOC_ADDRESS:
        b, c = asm.asm(f"b 0x{ROUTE_MEM_BASE + ROUTE_MALLOC:02x}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in MEMCPY_ADDRESS:
        b, c = asm.asm(f"b 0x{ROUTE_MEM_BASE + ROUTE_MEMCPY:02x}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in FREE_ADDRESS:
        b, c = asm.asm(f"b 0x{ROUTE_MEM_BASE + ROUTE_FREE:02x}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in MEMSET_ADDRESS:
        b, c = asm.asm(f"b 0x{ROUTE_MEM_BASE + ROUTE_MEMSET:02x}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in CALLOC_ADDRESS:
        b, c = asm.asm(f"b 0x{ROUTE_MEM_BASE + ROUTE_CALLOC:02x}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in REALLOC_ADDRESS:
        b, c = asm.asm(f"b 0x{ROUTE_MEM_BASE + ROUTE_REALLOC:02x}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in MEMMOVE_ADDRESS:
        b, c = asm.asm(f"b 0x{ROUTE_MEM_BASE + ROUTE_MEMMOVE:02x}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    # 03 - Patching thumb
    for a in MALLOC_ADDRESS_T:
        b, c = asm_t.asm("push {r4-r7, lr}; " + f"blx 0x{ROUTE_MEM_BASE + ROUTE_MALLOC:02x}; " + "pop {r4-r7, pc}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in MEMCPY_ADDRESS_T:
        b, c = asm_t.asm("push {r4-r7, lr}; " + f"blx 0x{ROUTE_MEM_BASE + ROUTE_MEMCPY:02x}; " + "pop {r4-r7, pc}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in FREE_ADDRESS_T:
        b, c = asm_t.asm("push {r4-r7, lr}; " + f"blx 0x{ROUTE_MEM_BASE + ROUTE_FREE:02x}; " + "pop {r4-r7, pc}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in MEMSET_ADDRESS_T:
        b, c = asm_t.asm("push {r4-r7, lr}; " + f"blx 0x{ROUTE_MEM_BASE + ROUTE_MEMSET:02x}; " + "pop {r4-r7, pc}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in CALLOC_ADDRESS_T:
        b, c = asm_t.asm("push {r4-r7, lr}; " + f"blx 0x{ROUTE_MEM_BASE + ROUTE_CALLOC:02x}; " + "pop {r4-r7, pc}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in REALLOC_ADDRESS_T:
        b, c = asm_t.asm("push {r4-r7, lr}; " + f"blx 0x{ROUTE_MEM_BASE + ROUTE_REALLOC:02x}; " + "pop {r4-r7, pc}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    for a in MEMMOVE_ADDRESS_T:
        b, c = asm_t.asm("push {r4-r7, lr}; " + f"blx 0x{ROUTE_MEM_BASE + ROUTE_MEMMOVE:02x}; " + "pop {r4-r7, pc}", a, True)
        f.write(struct.pack("<LL", a, len(b)) + b)
        tCount += 1
        
    f.seek(2)
    f.write(struct.pack("<L", tCount))
    f.close()

