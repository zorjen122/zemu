#pragma once

#define MMU_H
#ifdef MMU_H

#include "semu.h"
#include "./opobj/elfdef.h"

class MMU {
protected:
    static void mmu_load_phdr(elf64_phdr_t*, elf64_ehdr_t*, i64, FILE*);
    static void mmu_load_segment(mmu_t*, elf64_phdr_t*, int);
    static int flags_to_mmap_prot(u32);

public:
    static void mmu_load_elf(mmu_t*, int);

    static u64 mmu_alloc(mmu_t*, i64);
    static inline void mmu_write(u64, u8*, size_t);
};

void MMU::mmu_load_elf(mmu_t* m, int fd) {
    u8 buf[sizeof(elf64_ehdr_t)];
    FILE* file = fdopen(fd, "rb");
    if (fread(buf, 1, sizeof(elf64_ehdr_t), file) != sizeof(elf64_ehdr_t))
        fatal("file too small");

    elf64_ehdr_t* ehdr = reinterpret_cast<elf64_ehdr_t*>(buf);
    if (*(reinterpret_cast<u32*>(ehdr)) != *(u32*)ELFMAG)
        fatal("bad elf file");

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS64 || ehdr->e_machine != EM_RISCV)
        fatal("Not supported by RISCV-64");

    m->entry = static_cast<u64>(ehdr->e_entry);

    elf64_phdr_t phdr;
    for (i64 i = 0;i < ehdr->e_phnum;++i) {
        mmu_load_phdr(&phdr, ehdr, i, file);
        if (phdr.p_type == PT_LOAD)
            mmu_load_segment(m, &phdr, fd);
    }
}

void MMU::mmu_load_phdr(elf64_phdr_t* phdr, elf64_ehdr_t* ehdr, i64 idx, FILE* file) {
    if (fseek(file, ehdr->e_phoff + (ehdr->e_phentsize * idx), SEEK_SET) != 0)
        fatal("seek file failed");

    if (fread(reinterpret_cast<void*>(phdr), 1, sizeof(elf64_phdr_t), file) != sizeof(elf64_phdr_t))
        fatal("file too small");
}

int MMU::flags_to_mmap_prot(u32 flags) {
    return (flags & PF_R ? PROT_READ : 0) |
        (flags & PF_W ? PROT_WRITE : 0) |
        (flags & PF_X ? PROT_EXEC : 0);
}


void MMU::mmu_load_segment(mmu_t* mmu, elf64_phdr_t* phdr, int fd) {
    int page_size = getpagesize();
    u64 offset = phdr->p_offset;
    u64 vaddr = TO_HOST(phdr->p_vaddr);
    u64 aligned_vaddr = ROUNDDOWN(vaddr, page_size);
    u64 filesz = phdr->p_filesz + (vaddr - aligned_vaddr);
    u64 memsz = phdr->p_memsz + (vaddr - aligned_vaddr);

    int prot = flags_to_mmap_prot(phdr->p_flags);

    u64 addr = reinterpret_cast<u64>(mmap(reinterpret_cast<void*>(aligned_vaddr), filesz,
        prot, MAP_PRIVATE | MAP_FIXED, fd, ROUNDDOWN(offset, page_size)));

    assert(addr == aligned_vaddr);

    u64 datafiled = ROUNDUP(filesz, page_size);
    u64 remaining_bss = ROUNDUP(memsz, page_size) - datafiled;

    // .bss section exist, so memsz with filesz no alwasys same.
    if (remaining_bss > 0) {
        u64 addr = reinterpret_cast<u64>(mmap(
            reinterpret_cast<void*>(aligned_vaddr + datafiled), remaining_bss,
            prot, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0));
        assert(addr == aligned_vaddr + datafiled);
    }

    mmu->host_alloc = MAX(mmu->host_alloc, aligned_vaddr + ROUNDUP(memsz, page_size));
    mmu->base = mmu->alloc = TO_GUEST(mmu->host_alloc);
}


u64 MMU::mmu_alloc(mmu_t* m, i64 sz) {

    assert(m->alloc >= m->base);

    int page_size = getpagesize();
    u64 new_base = m->alloc;

    m->alloc += sz;
    assert(m->alloc >= m->base);

    if (sz > 0 && m->alloc > TO_GUEST(m->host_alloc)) {
        if (mmap(reinterpret_cast<void*>(m->host_alloc), ROUNDUP(sz, page_size),
            PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
            -1, 0) == MAP_FAILED) {
            fatal("mmap fatal");
        }
        m->host_alloc += ROUNDUP(sz, page_size);
    }
    else if (sz < 0 && ROUNDUP(m->alloc, page_size) < TO_GUEST(m->host_alloc)) {
        u64 len = TO_GUEST(m->host_alloc) - ROUNDUP(m->alloc, page_size);
        if (munmap(reinterpret_cast<void*>(m->host_alloc), len) == -1)
            fatal(strerror(errno));
        m->host_alloc -= len;
    }

    return new_base;
}

inline void MMU::mmu_write(u64 addr, u8* data, size_t len) {
    std::memcpy(reinterpret_cast<void*>(TO_HOST(addr)),
        reinterpret_cast<void*>(data), len);
}

#endif