from construct import *
import sys
import os

FlashLoader = Struct(
    "magic" / Const(b"FL"),
    "count" / Int32ul,
    "regions" / Array(this.count, Struct(
        "start" / Int32ul,
        "size" / Int32ul,
        "data" / Bytes(this.size)
    ))
)

if __name__ == "__main__":
    fls = FlashLoader.parse_file(sys.argv[1])
    for reg in fls.regions:
        open(os.path.splitext(sys.argv[1])[0] + f"_0x{reg.start:08x}.bin", "wb").write(reg.data)