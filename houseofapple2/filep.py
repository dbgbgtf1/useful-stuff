class IOField:
    def __init__(self, name, offset, base_addr, value=None):
        self.name = name
        self.offset = offset
        self.address = base_addr + offset
        self.value = value

    def __repr__(self):
        return (f"<{self.name}: offset=0x{self.offset:x}, "
                f"address=0x{self.address:x}, value={self.value}>")

class Filep:
    def __init__(self, address):
        self.address = address

        offsets = {
            "_flags": 0x0,
            "_IO_read_ptr": 0x8,
            "_IO_read_end": 0x10,
            "_IO_read_base": 0x18,
            "_IO_write_base": 0x20,
            "_IO_write_ptr": 0x28,
            "_IO_write_end": 0x30,
            "_IO_buf_base": 0x38,
            "_IO_buf_end": 0x40,
            "_IO_save_base": 0x48,
            "_IO_backup_base": 0x50,
            "_IO_save_end": 0x58,
            "_markers": 0x60,
            "_chain": 0x68,
            "_fileno": 0x70,
            "_flags2": 0x74,
            "_old_offset": 0x78,
            "_cur_column": 0x80,
            "_vtable_offset": 0x82,
            "_shortbuf": 0x83,
            "_lock": 0x88,
            "_offset": 0x90,
            "_codecvt": 0x98,
            "_wide_data": 0xa0,
            "_freeres_list": 0xa8,
            "_freeres_buf": 0xb0,
            "__pad5": 0xb8,
            "_mode": 0xc0,
            "_unused2": 0xc4,
        }

        for name, off in offsets.items():
            setattr(self, name, IOField(name, off, address))
