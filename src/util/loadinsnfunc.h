#pragma once

#define INSNFUNC_H
#ifdef INSNFUNC_H

#include "../opobj/ostype.h"
#include "../opobj/insndef.h"

#define QUADRANT(data) (((data) >>  0) & 0x3 )

/**
 * normal types
*/
#define OPCODE(data) (((data) >>  2) & 0x1f)
#define RD(data)     (((data) >>  7) & 0x1f)
#define RS1(data)    (((data) >> 15) & 0x1f)
#define RS2(data)    (((data) >> 20) & 0x1f)
#define RS3(data)    (((data) >> 27) & 0x1f)
#define FUNCT2(data) (((data) >> 25) & 0x3 )
#define FUNCT3(data) (((data) >> 12) & 0x7 )
#define FUNCT7(data) (((data) >> 25) & 0x7f)
#define IMM116(data) (((data) >> 26) & 0x3f)

static inline insn_t insn_utype_read(u32 data) {
    return (insn_t) {
        .rd = static_cast<i8>(RD(data)),
            .imm = static_cast<i32>((i32)data & 0xfffff000),
    };
}

static inline insn_t insn_itype_read(u32 data) {
    return (insn_t) {
        .rd = static_cast<i8>(RD(data)),
            .rs1 = static_cast<i8>RS1(data),
            .imm = static_cast<i32>((i32)data >> 20),
    };
}

static inline insn_t insn_jtype_read(u32 data) {
    u32 imm20 = (data >> 31) & 0x1;
    u32 imm101 = (data >> 21) & 0x3ff;
    u32 imm11 = (data >> 20) & 0x1;
    u32 imm1912 = (data >> 12) & 0xff;

    i32 imm = (imm20 << 20) | (imm1912 << 12) | (imm11 << 11) | (imm101 << 1);
    imm = (imm << 11) >> 11;

    return (insn_t) {
        .rd = static_cast<i8>(RD(data)),
            .imm = imm,
    };
}

static inline insn_t insn_btype_read(u32 data) {
    u32 imm12 = (data >> 31) & 0x1;
    u32 imm105 = (data >> 25) & 0x3f;
    u32 imm41 = (data >> 8) & 0xf;
    u32 imm11 = (data >> 7) & 0x1;

    i32 imm = (imm12 << 12) | (imm11 << 11) | (imm105 << 5) | (imm41 << 1);
    imm = (imm << 19) >> 19;

    return (insn_t) {
        .rs1 = static_cast<i8>RS1(data),
            .rs2 = static_cast<i8>(RS2(data)),
            .imm = imm,
    };
}

static inline insn_t insn_rtype_read(u32 data) {
    return (insn_t) {
        .rd = static_cast<i8>(RD(data)),
            .rs1 = static_cast<i8>RS1(data),
            .rs2 = static_cast<i8>(RS2(data)),
    };
}

static inline insn_t insn_stype_read(u32 data) {
    u32 imm115 = (data >> 25) & 0x7f;
    u32 imm40 = (data >> 7) & 0x1f;

    i32 imm = (imm115 << 5) | imm40;
    imm = (imm << 20) >> 20;
    return (insn_t) {
        .rs1 = static_cast<i8>RS1(data),
            .rs2 = static_cast<i8>(RS2(data)),
            .imm = imm,
    };
}

static inline insn_t insn_csrtype_read(u32 data) {
    return (insn_t) {
        .rd = static_cast<i8>(RD(data)),
            .rs1 = static_cast<i8>RS1(data),
            .csr = (i16)(data >> 20),
    };
}

static inline insn_t insn_fprtype_read(u32 data) {
    return (insn_t) {
        .rd = static_cast<i8>(RD(data)),
            .rs1 = static_cast<i8>RS1(data),
            .rs2 = static_cast<i8>(RS2(data)),
            .rs3 = (i8)RS3(data),
    };
}

/**
 * compressed types
*/
#define COPCODE(data)     (((data) >> 13) & 0x7 )
#define CFUNCT1(data)     (((data) >> 12) & 0x1 )
#define CFUNCT2LOW(data)  (((data) >>  5) & 0x3 )
#define CFUNCT2HIGH(data) (((data) >> 10) & 0x3 )
#define RP1(data)         (((data) >>  7) & 0x7 )
#define RP2(data)         (((data) >>  2) & 0x7 )
#define RC1(data)         (((data) >>  7) & 0x1f)
#define RC2(data)         (((data) >>  2) & 0x1f)

static inline insn_t insn_catype_read(u16 data) {
    return (insn_t) {
        .rd = static_cast<i8>(RP1(data) + 8),
            .rs2 = static_cast<i8>(RP2(data) + 8),
            .rvc = true,
    };
}

static inline insn_t insn_crtype_read(u16 data) {
    return (insn_t) {
        .rs1 = static_cast<i8>(RC1(data)),
            .rs2 = static_cast<i8>(RC2(data)),
            .rvc = true,
    };
}

static inline insn_t insn_citype_read(u16 data) {
    u32 imm40 = (data >> 2) & 0x1f;
    u32 imm5 = (data >> 12) & 0x1;
    i32 imm = (imm5 << 5) | imm40;
    imm = (imm << 26) >> 26;

    return (insn_t) {
        .rd = static_cast<i8>(RC1(data)),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_citype_read2(u16 data) {
    u32 imm86 = (data >> 2) & 0x7;
    u32 imm43 = (data >> 5) & 0x3;
    u32 imm5 = (data >> 12) & 0x1;

    i32 imm = (imm86 << 6) | (imm43 << 3) | (imm5 << 5);

    return (insn_t) {
        .rd = static_cast<i8>(RC1(data)),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_citype_read3(u16 data) {
    u32 imm5 = (data >> 2) & 0x1;
    u32 imm87 = (data >> 3) & 0x3;
    u32 imm6 = (data >> 5) & 0x1;
    u32 imm4 = (data >> 6) & 0x1;
    u32 imm9 = (data >> 12) & 0x1;

    i32 imm = (imm5 << 5) | (imm87 << 7) | (imm6 << 6) | (imm4 << 4) | (imm9 << 9);
    imm = (imm << 22) >> 22;

    return (insn_t) {
        .rd = static_cast<i8>(RC1(data)),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_citype_read4(u16 data) {
    u32 imm5 = (data >> 12) & 0x1;
    u32 imm42 = (data >> 4) & 0x7;
    u32 imm76 = (data >> 2) & 0x3;

    i32 imm = (imm5 << 5) | (imm42 << 2) | (imm76 << 6);

    return (insn_t) {
        .rd = static_cast<i8>(RC1(data)),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_citype_read5(u16 data) {
    u32 imm1612 = (data >> 2) & 0x1f;
    u32 imm17 = (data >> 12) & 0x1;

    i32 imm = (imm1612 << 12) | (imm17 << 17);
    imm = (imm << 14) >> 14;
    return (insn_t) {
        .rd = static_cast<i8>(RC1(data)),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_cbtype_read(u16 data) {
    u32 imm5 = (data >> 2) & 0x1;
    u32 imm21 = (data >> 3) & 0x3;
    u32 imm76 = (data >> 5) & 0x3;
    u32 imm43 = (data >> 10) & 0x3;
    u32 imm8 = (data >> 12) & 0x1;

    i32 imm = (imm8 << 8) | (imm76 << 6) | (imm5 << 5) | (imm43 << 3) | (imm21 << 1);
    imm = (imm << 23) >> 23;

    return (insn_t) {
        .rs1 = static_cast<i8>(RP1(data) + 8),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_cbtype_read2(u16 data) {
    u32 imm40 = (data >> 2) & 0x1f;
    u32 imm5 = (data >> 12) & 0x1;
    i32 imm = (imm5 << 5) | imm40;
    imm = (imm << 26) >> 26;

    return (insn_t) {
        .rd = static_cast<i8>(RP1(data) + 8),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_cstype_read(u16 data) {
    u32 imm76 = (data >> 5) & 0x3;
    u32 imm53 = (data >> 10) & 0x7;

    i32 imm = ((imm76 << 6) | (imm53 << 3));

    return (insn_t) {
        .rs1 = static_cast<i8>(RP1(data) + 8),
            .rs2 = static_cast<i8>(RP2(data) + 8),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_cstype_read2(u16 data) {
    u32 imm6 = (data >> 5) & 0x1;
    u32 imm2 = (data >> 6) & 0x1;
    u32 imm53 = (data >> 10) & 0x7;

    i32 imm = ((imm6 << 6) | (imm2 << 2) | (imm53 << 3));

    return (insn_t) {
        .rs1 = static_cast<i8>(RP1(data) + 8),
            .rs2 = static_cast<i8>(RP2(data) + 8),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_cjtype_read(u16 data) {
    u32 imm5 = (data >> 2) & 0x1;
    u32 imm31 = (data >> 3) & 0x7;
    u32 imm7 = (data >> 6) & 0x1;
    u32 imm6 = (data >> 7) & 0x1;
    u32 imm10 = (data >> 8) & 0x1;
    u32 imm98 = (data >> 9) & 0x3;
    u32 imm4 = (data >> 11) & 0x1;
    u32 imm11 = (data >> 12) & 0x1;

    i32 imm = ((imm5 << 5) | (imm31 << 1) | (imm7 << 7) | (imm6 << 6) |
        (imm10 << 10) | (imm98 << 8) | (imm4 << 4) | (imm11 << 11));
    imm = (imm << 20) >> 20;
    return (insn_t) {
        .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_cltype_read(u16 data) {
    u32 imm6 = (data >> 5) & 0x1;
    u32 imm2 = (data >> 6) & 0x1;
    u32 imm53 = (data >> 10) & 0x7;

    i32 imm = (imm6 << 6) | (imm2 << 2) | (imm53 << 3);

    return (insn_t) {
        .rd = static_cast<i8>(RP2(data) + 8),
            .rs1 = static_cast<i8>(RP1(data) + 8),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_cltype_read2(u16 data) {
    u32 imm76 = (data >> 5) & 0x3;
    u32 imm53 = (data >> 10) & 0x7;

    i32 imm = (imm76 << 6) | (imm53 << 3);

    return (insn_t) {
        .rd = static_cast<i8>(RP2(data) + 8),
            .rs1 = static_cast<i8>(RP1(data) + 8),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_csstype_read(u16 data) {
    u32 imm86 = (data >> 7) & 0x7;
    u32 imm53 = (data >> 10) & 0x7;

    i32 imm = (imm86 << 6) | (imm53 << 3);

    return (insn_t) {
        .rs2 = static_cast<i8>(RC2(data)),
            .imm = imm,

            .rvc = true,
    };
}

static inline insn_t insn_csstype_read2(u16 data) {
    u32 imm76 = (data >> 7) & 0x3;
    u32 imm52 = (data >> 9) & 0xf;

    i32 imm = (imm76 << 6) | (imm52 << 2);

    return (insn_t) {
        .rs2 = static_cast<i8>(RC2(data)),
            .imm = imm,
            .rvc = true,
    };
}

static inline insn_t insn_ciwtype_read(u16 data) {
    u32 imm3 = (data >> 5) & 0x1;
    u32 imm2 = (data >> 6) & 0x1;
    u32 imm96 = (data >> 7) & 0xf;
    u32 imm54 = (data >> 11) & 0x3;

    i32 imm = (imm3 << 3) | (imm2 << 2) | (imm96 << 6) | (imm54 << 4);

    return (insn_t) {
        .rd = static_cast<i8>(RP2(data) + 8),
            .imm = imm,
            .rvc = true,
    };
}

#endif