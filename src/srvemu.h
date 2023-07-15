#pragma once

#define RVEMU_H
#ifdef RVEMU_H


#include <stdio.h>
#include <cstring>
#include <fcntl.h>

#include "./opobj/elfdef.h"
#include "semu.h"
#include "srvmmu.h"
#include "interp.h"

class Machine {
public:
    void machine_load_program(machine_t*, char*);
    exit_reason_t machine_step_program(machine_t*);

    void machine_setup(machine_t*, int, char**);
};

inline u64 machine_get_gp_reg(machine_t* m, i32 reg) {
    assert(reg >= 0 && reg < num_gp_regs);
    return m->state.gp_regs[reg];
}

inline void machine_set_gp_reg(machine_t* m, i32 reg, u64 data) {
    assert(reg >= 0 && reg < num_gp_regs);
    m->state.gp_regs[reg] = data;
}

exit_reason_t Machine::machine_step_program(machine_t* m) {
    while (true) {
        m->state.exit_reason = none;
        exec_block_interp(&m->state);

        // Maybe an accident happened.
        assert(m->state.exit_reason != none);

        if (m->state.exit_reason == direct_branch
            || m->state.exit_reason == indirect_branch) {
            m->state.pc = m->state.reenter_pc;
            continue;
        }

        break;
    }

    m->state.pc = m->state.reenter_pc;

    switch (m->state.exit_reason) {
    case direct_branch:
    case indirect_branch:
        // continue execution
        break;
    case ecall:
        return ecall;
    default:
        unreachable();
    }

    return ecall;
}

void Machine::machine_load_program(machine_t* m, char* prog) {
    int fd = open(prog, O_RDONLY);
    if (fd == -1)
        fatal(strerror(errno));

    MMU::mmu_load_elf(&m->mmu, fd);
    close(fd);

    m->state.pc = static_cast<u64>(m->mmu.entry);
}



void Machine::machine_setup(machine_t* m, int argc, char* argv[]) {

    size_t stack_size = 30 * 1024 * 1024;
    u64 stack = MMU::mmu_alloc(&m->mmu, stack_size);

    m->state.gp_regs[sp] = stack + stack_size;
    m->state.gp_regs[sp] -= 8;
    m->state.gp_regs[sp] -= 8;
    m->state.gp_regs[sp] -= 8;  // argv end

    // pop call parameter of argc
    int args = argc - 1;
    for (int i = args;i > 0;--i) {
        i64 len = strlen(argv[i]);
        u64 mmu_block = MMU::mmu_alloc(&m->mmu, len + 1);
        MMU::mmu_write(mmu_block, reinterpret_cast<u8*>(argv[i]), len);

        m->state.gp_regs[sp] -= 8;   // argv[i]

        MMU::mmu_write(m->state.gp_regs[sp],
            reinterpret_cast<u8*>(&mmu_block), sizeof(u64));
    }

    m->state.gp_regs[sp] -= 8;   // argc
    MMU::mmu_write(m->state.gp_regs[sp],
        reinterpret_cast<u8*>(&args), sizeof(u64));

}


#endif