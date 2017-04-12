#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
#  define CDECLS_BEGIN extern "C" {
#  define CDECLS_END }
#else
#  define CDECLS_BEGIN
#  define CDECLS_END
#endif

#define mb() asm volatile ("" : : : "memory")
#define atomic_inc_fetch(ptr) __atomic_add_fetch((ptr), 1, __ATOMIC_SEQ_CST)

#endif
