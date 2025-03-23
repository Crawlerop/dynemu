from ctypes import *
import enum

class Regs(enum.IntEnum):
    R0 = 0
    R1 = 1
    R2 = 2
    R3 = 3
    R4 = 4
    R5 = 5
    R6 = 6
    R7 = 7
    R8 = 8
    R9 = 9
    R10 = 10
    R11 = 11
    IP = 12
    SP = 13
    LR = 14
    PC = 15

class Dynemu():
    def __init__(self):
        self._dynemu = dynemu_open()
    
    def __del__(self):
        dynemu_delete(self._dynemu)
        
    def memory_map(self, vaddr: int, size: int):
        dynemu_mmap(self._dynemu, vaddr, size)
        
    def upload_file(self, filename: str, dest: int, size: int):
        return dynemu_upload_file(self._dynemu, filename, dest, size)
        
    def upload_fls_memory(self, data: bytes|bytearray):
        temp = (c_ubyte * len(data)).from_buffer(bytearray(data))
        return dynemu_upload_fls_memory(self._dynemu, temp)
        
    def upload_memstub(self, dest: int=-1, mem_start: int=0xa0000000, mem_size: int=0x2000000, is_big: bool=False):
        if dest == -1:
            dynemu_upload_memstub_nooffset(self._dynemu, mem_start, mem_size, is_big)
            
        else:
            dynemu_upload_memstub(self._dynemu, dest, mem_start, mem_size, is_big)
        
    def read_u8(self, vaddr: int):
        return dynemu_read_u8(self._dynemu, vaddr)
    
    def read_u16(self, vaddr: int, is_big: bool=False):
        return dynemu_read_u16(self._dynemu, vaddr, is_big)
    
    def read_u32(self, vaddr: int, is_big: bool=False):
        return dynemu_read_u32(self._dynemu, vaddr, is_big)
    
    def read_u64(self, vaddr: int, is_big: bool=False):
        return dynemu_read_u64(self._dynemu, vaddr, is_big)
    
    def write_u8(self, vaddr: int, value: int):
        return dynemu_write_u8(self._dynemu, vaddr, value)
    
    def write_u16(self, vaddr: int, value: int, is_big: bool=False):
        return dynemu_write_u16(self._dynemu, vaddr, value, is_big)
    
    def write_u32(self, vaddr: int, value: int, is_big: bool=False):
        return dynemu_write_u32(self._dynemu, vaddr, value, is_big)
    
    def write_u64(self, vaddr: int, value: int, is_big: bool=False):
        return dynemu_write_u64(self._dynemu, vaddr, value, is_big)
        
    def read_bytes(self, vaddr: int, size: int):
        temp = (c_ubyte * size)()
        dynemu_read_bytes(self._dynemu, vaddr, temp, size)
        return bytes(temp)
    
    def write_bytes(self, vaddr: int, data: bytes|bytearray):
        temp = (c_ubyte * len(data)).from_buffer(bytearray(data))
        dynemu_write_bytes(self._dynemu, vaddr, temp, len(data))
        
    def execute(self, pc: int, is_big: bool=False):
        return dynemu_execute(self._dynemu, pc, is_big)
        
    def get_reg(self, reg: int):
        return dynemu_read_reg(self._dynemu, reg)
    
    def set_reg(self, reg: int, value: int):
        dynemu_write_reg(self._dynemu, reg, value)
        
    