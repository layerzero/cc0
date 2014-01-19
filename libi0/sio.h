#ifndef SIO_H
#define SIO_H

#include "stdint.h"
#include "stddef.h"

#define no_SIO_DEBUG_ 1

#define sid_t uint64_t
#define sizeof_sid_t 8

// returned value is the number of bytes received, 
// or -1 if an error occurred. 
// The return value will be 0 when the peer  has  performed an orderly
// shutdown.

size_t sread(sid_t sid, char *addr, size_t nbyte)
{
    uint64_t n;

    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_SREAD;

    // set system call input arguments
    *(sid_t*) SYSCALL_COMM_AREA_ADDR = sid;

    // set system call input arguments
    *(size_t*) (SYSCALL_COMM_AREA_ADDR + sizeof_sid_t) = nbyte;

    // set system call input arguments
    *(char**) (SYSCALL_COMM_AREA_ADDR + 2 * sizeof_sid_t) = addr;

    // issue system call
    asm("int 0x80");

    // set return value

    n = *(size_t*) (SYSCALL_COMM_AREA_ADDR);

    // *addr = *(void**) (SYSCALL_COMM_AREA_ADDR + sizeof_size_t);

#ifdef _SIO_DEBUG_
    output_char('s');
    output_char('i');
    output_char('o');
    output_char(':');
    output_char('s');
    output_char('r');
    output_char(':');
    output_char(C_n);
    output_q(n);
    output_char(C_n);
    output_char_str((char*)addr, n);
    output_char(C_n);
#endif

    return n;
}

size_t swrite(sid_t sid, char *addr, size_t nbyte)
{
    long n;
    char c;
    
    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_SWRITE;

    // set system call input arguments
    *(sid_t*) SYSCALL_COMM_AREA_ADDR = sid;

    // set system call input arguments
    *(size_t*) (SYSCALL_COMM_AREA_ADDR + sizeof_sid_t) = nbyte;

    // set system call input arguments
    *(uint64_t*) (SYSCALL_COMM_AREA_ADDR + sizeof_sid_t + sizeof_size_t) = (uint64_t)addr;


#ifdef _SIO_DEBUG_
    output_char('s');
    output_char('i');
    output_char('o');
    output_char(':');
    output_char('s');
    output_char('w');
    output_char(':');
    output_char(C_n);
    output_q(nbyte);
    output_char(C_n);
    output_char_str(addr, nbyte);
    output_char(C_n);
#endif

    // make sure all data are accessible
    for (n = 0; n < nbyte; n = n + 1) {
        c = addr[n];
    }
    n = 0;

    // issue system call
    asm("int 0x80");

    n = *(uint64_t*)(SYSCALL_COMM_AREA_ADDR);

#ifdef _SIO_DEBUG_
    output_char('s');
    output_char('i');
    output_char('o');
    output_char(':');
    output_char('s');
    output_char('w');
    output_char(':');
    output_char('d');
    output_char('o');
    output_char('n');
    output_char('e');
    output_char(':');
    output_q(n);
    output_char(C_n);
#endif

    return n;
}
 
sid_t slisten(size_t port)
{
    sid_t sid;

    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_SLISTEN;

    // set system call input arguments
    *(size_t*) SYSCALL_COMM_AREA_ADDR = port;


    // issue system call
    asm("int 0x80");

    // set return value
    sid = *(sid_t*) SYSCALL_COMM_AREA_ADDR;

    return sid;
}

#endif // SIO_H

