#pragma once

#define SEMU_H 
#ifdef SEMU_H 

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "./opobj/reg.h"
#include "./opobj/ostype.h"


#define fatalf(fmt, ...) (fprintf(stderr, "fatal: %s:%d " fmt "\n", __FILE__, __LINE__, __VA_ARGS__), exit(1))

#define fatal(msg) fatalf("%s", msg)

#define unreachable() (fatal("unreachable"), __builtin_unreachable())

#define ROUNDDOWN(x,y)   ((x) & -(y))
#define ROUNDUP(x,y)     (((x) + (y)-1) & -(y))
#define MIN(x,y)         ((x) < (y) ? (x) : (y))
#define MAX(x,y)         ((x) > (y) ? (x) : (y))

#define GUEST_MEMORY_OFFSET 0x088800000000ULL

#define TO_HOST(addr) (addr + GUEST_MEMORY_OFFSET)
#define TO_GUEST(addr) (addr - GUEST_MEMORY_OFFSET)

#define REG_INTEGRAL_X32 num_gp_regs     
#define REG_FLOAT_X32 num_fp_regs

#define ARRAY_SIZE(x)   (sizeof(x)/sizeof((x)[0]))

struct mmu_t {
    u64 entry;
    u64 host_alloc;
    u64 alloc;
    u64 base;
};

enum exit_reason_t {
    none,
    direct_branch,
    indirect_branch,
    interp,
    ecall,
};

enum csr_t {
    fflags = 0x001,
    frm = 0x002,
    fcsr = 0x003,
};

struct state_t {
    exit_reason_t exit_reason;

    u64 gp_regs[REG_INTEGRAL_X32];
    fp_reg_t fp_regs[REG_FLOAT_X32];

    // pos next pc 
    u64 reenter_pc;

    u64 pc;
};

struct machine_t {
    state_t state;
    mmu_t mmu;
};


#endif