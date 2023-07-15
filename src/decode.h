#pragma once

#define DECODE_H
#ifdef DECODE_H

#include "./util/loadinsnfunc.h"
#include "semu.h"


//* For this file, see reference RISC-V User-Level ISA V20180801-draft [chapter 22].
void insn_decode(insn_t* insn, u32 data) {
//* These macros are replaced by different macros according to the code segment.
    u32 quadrant = QUADRANT(data);
    switch (quadrant) {
    case 0x0: {
        u32 copcode = COPCODE(data);

        switch (copcode) {
        case 0x0: /* C.ADDI4SPN */
            *insn = insn_ciwtype_read(data);
            insn->rs1 = sp;
            insn->type = insn_addi;
            assert(insn->imm != 0);
            return;
        case 0x1: /* C.FLD */
            *insn = insn_cltype_read2(data);
            insn->type = insn_fld;
            return;
        case 0x2: /* C.LW */
            *insn = insn_cltype_read(data);
            insn->type = insn_lw;
            return;
        case 0x3: /* C.LD */
            *insn = insn_cltype_read2(data);
            insn->type = insn_ld;
            return;
        case 0x5: /* C.FSD */
            *insn = insn_cstype_read(data);
            insn->type = insn_fsd;
            return;
        case 0x6: /* C.SW */
            *insn = insn_cstype_read2(data);
            insn->type = insn_sw;
            return;
        case 0x7: /* C.SD */
            *insn = insn_cstype_read(data);
            insn->type = insn_sd;
            return;
        default: printf("data: %x\n", data); fatal("unimplemented");
        }
    }
            unreachable();
    case 0x1: {
        u32 copcode = COPCODE(data);

        switch (copcode) {
        case 0x0: /* C.ADDI */
            *insn = insn_citype_read(data);
            insn->rs1 = insn->rd;
            insn->type = insn_addi;
            return;
        case 0x1: /* C.ADDIW */
            *insn = insn_citype_read(data);
            assert(insn->rd != 0);
            insn->rs1 = insn->rd;
            insn->type = insn_addiw;
            return;
        case 0x2: /* C.LI */
            *insn = insn_citype_read(data);
            insn->rs1 = zero;
            insn->type = insn_addi;
            return;
        case 0x3: {
            i32 rd = RC1(data);
            if (rd == 2) { /* C.ADDI16SP */
                *insn = insn_citype_read3(data);
                assert(insn->imm != 0);
                insn->rs1 = insn->rd;
                insn->type = insn_addi;
                return;
            }
            else { /* C.LUI */
                *insn = insn_citype_read5(data);
                assert(insn->imm != 0);
                insn->type = insn_lui;
                return;
            }
        }
                unreachable();
        case 0x4: {
            u32 cfunct2high = CFUNCT2HIGH(data);

            switch (cfunct2high) {
            case 0x0:   /* C.SRLI */
            case 0x1:   /* C.SRAI */
            case 0x2: { /* C.ANDI */
                *insn = insn_cbtype_read2(data);
                insn->rs1 = insn->rd;

                if (cfunct2high == 0x0) {
                    insn->type = insn_srli;
                }
                else if (cfunct2high == 0x1) {
                    insn->type = insn_srai;
                }
                else {
                    insn->type = insn_andi;
                }
                return;
            }
                    unreachable();
            case 0x3: {
                u32 cfunct1 = CFUNCT1(data);

                switch (cfunct1) {
                case 0x0: {
                    u32 cfunct2low = CFUNCT2LOW(data);

                    *insn = insn_catype_read(data);
                    insn->rs1 = insn->rd;

                    switch (cfunct2low) {
                    case 0x0: /* C.SUB */
                        insn->type = insn_sub;
                        break;
                    case 0x1: /* C.XOR */
                        insn->type = insn_xor;
                        break;
                    case 0x2: /* C.OR */
                        insn->type = insn_or;
                        break;
                    case 0x3: /* C.AND */
                        insn->type = insn_and;
                        break;
                    default: unreachable();
                    }
                    return;
                }
                        unreachable();
                case 0x1: {
                    u32 cfunct2low = CFUNCT2LOW(data);

                    *insn = insn_catype_read(data);
                    insn->rs1 = insn->rd;

                    switch (cfunct2low) {
                    case 0x0: /* C.SUBW */
                        insn->type = insn_subw;
                        break;
                    case 0x1: /* C.ADDW */
                        insn->type = insn_addw;
                        break;
                    default: unreachable();
                    }
                    return;
                }
                        unreachable();
                default: unreachable();
                }
            }
                    unreachable();
            default: unreachable();
            }
        }
                unreachable();
        case 0x5: /* C.J */
            *insn = insn_cjtype_read(data);
            insn->rd = zero;
            insn->type = insn_jal;
            insn->cont = true;
            return;
        case 0x6: /* C.BEQZ */
        case 0x7: /* C.BNEZ */
            *insn = insn_cbtype_read(data);
            insn->rs2 = zero;
            insn->type = copcode == 0x6 ? insn_beq : insn_bne;
            return;
        default: fatal("unrecognized copcode");
        }
    }
            unreachable();
    case 0x2: {
        u32 copcode = COPCODE(data);
        switch (copcode) {
        case 0x0: /* C.SLLI */
            *insn = insn_citype_read(data);
            insn->rs1 = insn->rd;
            insn->type = insn_slli;
            return;
        case 0x1: /* C.FLDSP */
            *insn = insn_citype_read2(data);
            insn->rs1 = sp;
            insn->type = insn_fld;
            return;
        case 0x2: /* C.LWSP */
            *insn = insn_citype_read4(data);
            assert(insn->rd != 0);
            insn->rs1 = sp;
            insn->type = insn_lw;
            return;
        case 0x3: /* C.LDSP */
            *insn = insn_citype_read2(data);
            assert(insn->rd != 0);
            insn->rs1 = sp;
            insn->type = insn_ld;
            return;
        case 0x4: {
            u32 cfunct1 = CFUNCT1(data);

            switch (cfunct1) {
            case 0x0: {
                *insn = insn_crtype_read(data);

                if (insn->rs2 == 0) { /* C.JR */
                    assert(insn->rs1 != 0);
                    insn->rd = zero;
                    insn->type = insn_jalr;
                    insn->cont = true;
                }
                else { /* C.MV */
                    insn->rd = insn->rs1;
                    insn->rs1 = zero;
                    insn->type = insn_add;
                }
                return;
            }
                    unreachable();
            case 0x1: {
                *insn = insn_crtype_read(data);
                if (insn->rs1 == 0 && insn->rs2 == 0) { /* C.EBREAK */
                    fatal("unimplmented");
                }
                else if (insn->rs2 == 0) { /* C.JALR */
                    insn->rd = ra;
                    insn->type = insn_jalr;
                    insn->cont = true;
                }
                else { /* C.ADD */
                    insn->rd = insn->rs1;
                    insn->type = insn_add;
                }
                return;
            }
                    unreachable();
            default: unreachable();
            }
        }
                unreachable();
        case 0x5: /* C.FSDSP */
            *insn = insn_csstype_read(data);
            insn->rs1 = sp;
            insn->type = insn_fsd;
            return;
        case 0x6: /* C.SWSP */
            *insn = insn_csstype_read2(data);
            insn->rs1 = sp;
            insn->type = insn_sw;
            return;
        case 0x7: /* C.SDSP */
            *insn = insn_csstype_read(data);
            insn->rs1 = sp;
            insn->type = insn_sd;
            return;
        default: fatal("unrecognized copcode");
        }
    }
            unreachable();
    case 0x3: {
        u32 opcode = OPCODE(data);
        switch (opcode) {
        case 0x0: {
            u32 funct3 = FUNCT3(data);

            *insn = insn_itype_read(data);
            switch (funct3) {
            case 0x0: /* LB */
                insn->type = insn_lb;
                return;
            case 0x1: /* LH */
                insn->type = insn_lh;
                return;
            case 0x2: /* LW */
                insn->type = insn_lw;
                return;
            case 0x3: /* LD */
                insn->type = insn_ld;
                return;
            case 0x4: /* LBU */
                insn->type = insn_lbu;
                return;
            case 0x5: /* LHU */
                insn->type = insn_lhu;
                return;
            case 0x6: /* LWU */
                insn->type = insn_lwu;
                return;
            default: unreachable();
            }
        }
                unreachable();
        case 0x1: {
            u32 funct3 = FUNCT3(data);

            *insn = insn_itype_read(data);
            switch (funct3) {
            case 0x2: /* FLW */
                insn->type = insn_flw;
                return;
            case 0x3: /* FLD */
                insn->type = insn_fld;
                return;
            default: unreachable();
            }
        }
                unreachable();
        case 0x3: {
            u32 funct3 = FUNCT3(data);

            switch (funct3) {
            case 0x0: { /* FENCE */
                insn_t _insn = { 0 };
                *insn = _insn;
                insn->type = insn_fence;
                return;
            }
            case 0x1: { /* FENCE.I */
                insn_t _insn = { 0 };
                *insn = _insn;
                insn->type = insn_fence_i;
                return;
            }
            default: unreachable();
            }
        }
                unreachable();
        case 0x4: {
            u32 funct3 = FUNCT3(data);

            *insn = insn_itype_read(data);
            switch (funct3) {
            case 0x0: /* ADDI */
                insn->type = insn_addi;
                return;
            case 0x1: {
                u32 imm116 = IMM116(data);
                if (imm116 == 0) { /* SLLI */
                    insn->type = insn_slli;
                }
                else {
                    unreachable();
                }
                return;
            }
                    unreachable();
            case 0x2: /* SLTI */
                insn->type = insn_slti;
                return;
            case 0x3: /* SLTIU */
                insn->type = insn_sltiu;
                return;
            case 0x4: /* XORI */
                insn->type = insn_xori;
                return;
            case 0x5: {
                u32 imm116 = IMM116(data);

                if (imm116 == 0x0) { /* SRLI */
                    insn->type = insn_srli;
                }
                else if (imm116 == 0x10) { /* SRAI */
                    insn->type = insn_srai;
                }
                else {
                    unreachable();
                }
                return;
            }
                    unreachable();
            case 0x6: /* ORI */
                insn->type = insn_ori;
                return;
            case 0x7: /* ANDI */
                insn->type = insn_andi;
                return;
            default: fatal("unrecognized funct3");
            }
        }
                unreachable();
        case 0x5: /* AUIPC */
            *insn = insn_utype_read(data);
            insn->type = insn_auipc;
            return;
        case 0x6: {
            u32 funct3 = FUNCT3(data);
            u32 funct7 = FUNCT7(data);

            *insn = insn_itype_read(data);

            switch (funct3) {
            case 0x0: /* ADDIW */
                insn->type = insn_addiw;
                return;
            case 0x1: /* SLLIW */
                assert(funct7 == 0);
                insn->type = insn_slliw;
                return;
            case 0x5: {
                switch (funct7) {
                case 0x0: /* SRLIW */
                    insn->type = insn_srliw;
                    return;
                case 0x20: /* SRAIW */
                    insn->type = insn_sraiw;
                    return;
                default: unreachable();
                }
            }
                    unreachable();
            default: fatal("unimplemented");
            }
        }
                unreachable();
        case 0x8: {
            u32 funct3 = FUNCT3(data);

            *insn = insn_stype_read(data);
            switch (funct3) {
            case 0x0: /* SB */
                insn->type = insn_sb;
                return;
            case 0x1: /* SH */
                insn->type = insn_sh;
                return;
            case 0x2: /* SW */
                insn->type = insn_sw;
                return;
            case 0x3: /* SD */
                insn->type = insn_sd;
                return;
            default: unreachable();
            }
        }
                unreachable();
        case 0x9: {
            u32 funct3 = FUNCT3(data);

            *insn = insn_stype_read(data);
            switch (funct3) {
            case 0x2: /* FSW */
                insn->type = insn_fsw;
                return;
            case 0x3: /* FSD */
                insn->type = insn_fsd;
                return;
            default: unreachable();
            }
        }
                unreachable();
        case 0xc: {
            *insn = insn_rtype_read(data);

            u32 funct3 = FUNCT3(data);
            u32 funct7 = FUNCT7(data);

            switch (funct7) {
            case 0x0: {
                switch (funct3) {
                case 0x0: /* ADD */
                    insn->type = insn_add;
                    return;
                case 0x1: /* SLL */
                    insn->type = insn_sll;
                    return;
                case 0x2: /* SLT */
                    insn->type = insn_slt;
                    return;
                case 0x3: /* SLTU */
                    insn->type = insn_sltu;
                    return;
                case 0x4: /* XOR */
                    insn->type = insn_xor;
                    return;
                case 0x5: /* SRL */
                    insn->type = insn_srl;
                    return;
                case 0x6: /* OR */
                    insn->type = insn_or;
                    return;
                case 0x7: /* AND */
                    insn->type = insn_and;
                    return;
                default: unreachable();
                }
            }
                    unreachable();
            case 0x1: {
                switch (funct3) {
                case 0x0: /* MUL */
                    insn->type = insn_mul;
                    return;
                case 0x1: /* MULH */
                    insn->type = insn_mulh;
                    return;
                case 0x2: /* MULHSU */
                    insn->type = insn_mulhsu;
                    return;
                case 0x3: /* MULHU */
                    insn->type = insn_mulhu;
                    return;
                case 0x4: /* DIV */
                    insn->type = insn_div;
                    return;
                case 0x5: /* DIVU */
                    insn->type = insn_divu;
                    return;
                case 0x6: /* REM */
                    insn->type = insn_rem;
                    return;
                case 0x7: /* REMU */
                    insn->type = insn_remu;
                    return;
                default: unreachable();
                }
            }
                    unreachable();
            case 0x20: {
                switch (funct3) {
                case 0x0: /* SUB */
                    insn->type = insn_sub;
                    return;
                case 0x5: /* SRA */
                    insn->type = insn_sra;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            default: unreachable();
            }
        }
                unreachable();
        case 0xd: /* LUI */
            *insn = insn_utype_read(data);
            insn->type = insn_lui;
            return;
        case 0xe: {
            *insn = insn_rtype_read(data);

            u32 funct3 = FUNCT3(data);
            u32 funct7 = FUNCT7(data);

            switch (funct7) {
            case 0x0: {
                switch (funct3) {
                case 0x0: /* ADDW */
                    insn->type = insn_addw;
                    return;
                case 0x1: /* SLLW */
                    insn->type = insn_sllw;
                    return;
                case 0x5: /* SRLW */
                    insn->type = insn_srlw;
                    return;
                default: unreachable();
                }
            }
                    unreachable();
            case 0x1: {
                switch (funct3) {
                case 0x0: /* MULW */
                    insn->type = insn_mulw;
                    return;
                case 0x4: /* DIVW */
                    insn->type = insn_divw;
                    return;
                case 0x5: /* DIVUW */
                    insn->type = insn_divuw;
                    return;
                case 0x6: /* REMW */
                    insn->type = insn_remw;
                    return;
                case 0x7: /* REMUW */
                    insn->type = insn_remuw;
                    return;
                default: unreachable();
                }
            }
                    unreachable();
            case 0x20: {
                switch (funct3) {
                case 0x0: /* SUBW */
                    insn->type = insn_subw;
                    return;
                case 0x5: /* SRAW */
                    insn->type = insn_sraw;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            default: unreachable();
            }
        }
                unreachable();
        case 0x10: {
            u32 funct2 = FUNCT2(data);

            *insn = insn_fprtype_read(data);
            switch (funct2) {
            case 0x0: /* FMADD.S */
                insn->type = insn_fmadd_s;
                return;
            case 0x1: /* FMADD.D */
                insn->type = insn_fmadd_d;
                return;
            default: unreachable();
            }
        }
                 unreachable();
        case 0x11: {
            u32 funct2 = FUNCT2(data);

            *insn = insn_fprtype_read(data);
            switch (funct2) {
            case 0x0: /* FMSUB.S */
                insn->type = insn_fmsub_s;
                return;
            case 0x1: /* FMSUB.D */
                insn->type = insn_fmsub_d;
                return;
            default: unreachable();
            }
        }
                 unreachable();
        case 0x12: {
            u32 funct2 = FUNCT2(data);

            *insn = insn_fprtype_read(data);
            switch (funct2) {
            case 0x0: /* FNMSUB.S */
                insn->type = insn_fnmsub_s;
                return;
            case 0x1: /* FNMSUB.D */
                insn->type = insn_fnmsub_d;
                return;
            default: unreachable();
            }
        }
                 unreachable();
        case 0x13: {
            u32 funct2 = FUNCT2(data);

            *insn = insn_fprtype_read(data);
            switch (funct2) {
            case 0x0: /* FNMADD.S */
                insn->type = insn_fnmadd_s;
                return;
            case 0x1: /* FNMADD.D */
                insn->type = insn_fnmadd_d;
                return;
            default: unreachable();
            }
        }
                 unreachable();
        case 0x14: {
            u32 funct7 = FUNCT7(data);

            *insn = insn_rtype_read(data);
            switch (funct7) {
            case 0x0:  /* FADD.S */
                insn->type = insn_fadd_s;
                return;
            case 0x1:  /* FADD.D */
                insn->type = insn_fadd_d;
                return;
            case 0x4:  /* FSUB.S */
                insn->type = insn_fsub_s;
                return;
            case 0x5:  /* FSUB.D */
                insn->type = insn_fsub_d;
                return;
            case 0x8:  /* FMUL.S */
                insn->type = insn_fmul_s;
                return;
            case 0x9:  /* FMUL.D */
                insn->type = insn_fmul_d;
                return;
            case 0xc:  /* FDIV.S */
                insn->type = insn_fdiv_s;
                return;
            case 0xd:  /* FDIV.D */
                insn->type = insn_fdiv_d;
                return;
            case 0x10: {
                u32 funct3 = FUNCT3(data);

                switch (funct3) {
                case 0x0: /* FSGNJ.S */
                    insn->type = insn_fsgnj_s;
                    return;
                case 0x1: /* FSGNJN.S */
                    insn->type = insn_fsgnjn_s;
                    return;
                case 0x2: /* FSGNJX.S */
                    insn->type = insn_fsgnjx_s;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x11: {
                u32 funct3 = FUNCT3(data);

                switch (funct3) {
                case 0x0: /* FSGNJ.D */
                    insn->type = insn_fsgnj_d;
                    return;
                case 0x1: /* FSGNJN.D */
                    insn->type = insn_fsgnjn_d;
                    return;
                case 0x2: /* FSGNJX.D */
                    insn->type = insn_fsgnjx_d;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x14: {
                u32 funct3 = FUNCT3(data);

                switch (funct3) {
                case 0x0: /* FMIN.S */
                    insn->type = insn_fmin_s;
                    return;
                case 0x1: /* FMAX.S */
                    insn->type = insn_fmax_s;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x15: {
                u32 funct3 = FUNCT3(data);

                switch (funct3) {
                case 0x0: /* FMIN.D */
                    insn->type = insn_fmin_d;
                    return;
                case 0x1: /* FMAX.D */
                    insn->type = insn_fmax_d;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x20: /* FCVT.S.D */
                assert(RS2(data) == 1);
                insn->type = insn_fcvt_s_d;
                return;
            case 0x21: /* FCVT.D.S */
                assert(RS2(data) == 0);
                insn->type = insn_fcvt_d_s;
                return;
            case 0x2c: /* FSQRT.S */
                assert(insn->rs2 == 0);
                insn->type = insn_fsqrt_s;
                return;
            case 0x2d: /* FSQRT.D */
                assert(insn->rs2 == 0);
                insn->type = insn_fsqrt_d;
                return;
            case 0x50: {
                u32 funct3 = FUNCT3(data);

                switch (funct3) {
                case 0x0: /* FLE.S */
                    insn->type = insn_fle_s;
                    return;
                case 0x1: /* FLT.S */
                    insn->type = insn_flt_s;
                    return;
                case 0x2: /* FEQ.S */
                    insn->type = insn_feq_s;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x51: {
                u32 funct3 = FUNCT3(data);

                switch (funct3) {
                case 0x0: /* FLE.D */
                    insn->type = insn_fle_d;
                    return;
                case 0x1: /* FLT.D */
                    insn->type = insn_flt_d;
                    return;
                case 0x2: /* FEQ.D */
                    insn->type = insn_feq_d;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x60: {
                u32 rs2 = RS2(data);

                switch (rs2) {
                case 0x0: /* FCVT.W.S */
                    insn->type = insn_fcvt_w_s;
                    return;
                case 0x1: /* FCVT.WU.S */
                    insn->type = insn_fcvt_wu_s;
                    return;
                case 0x2: /* FCVT.L.S */
                    insn->type = insn_fcvt_l_s;
                    return;
                case 0x3: /* FCVT.LU.S */
                    insn->type = insn_fcvt_lu_s;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x61: {
                u32 rs2 = RS2(data);

                switch (rs2) {
                case 0x0: /* FCVT.W.D */
                    insn->type = insn_fcvt_w_d;
                    return;
                case 0x1: /* FCVT.WU.D */
                    insn->type = insn_fcvt_wu_d;
                    return;
                case 0x2: /* FCVT.L.D */
                    insn->type = insn_fcvt_l_d;
                    return;
                case 0x3: /* FCVT.LU.D */
                    insn->type = insn_fcvt_lu_d;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x68: {
                u32 rs2 = RS2(data);

                switch (rs2) {
                case 0x0: /* FCVT.S.W */
                    insn->type = insn_fcvt_s_w;
                    return;
                case 0x1: /* FCVT.S.WU */
                    insn->type = insn_fcvt_s_wu;
                    return;
                case 0x2: /* FCVT.S.L */
                    insn->type = insn_fcvt_s_l;
                    return;
                case 0x3: /* FCVT.S.LU */
                    insn->type = insn_fcvt_s_lu;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x69: {
                u32 rs2 = RS2(data);

                switch (rs2) {
                case 0x0: /* FCVT.D.W */
                    insn->type = insn_fcvt_d_w;
                    return;
                case 0x1: /* FCVT.D.WU */
                    insn->type = insn_fcvt_d_wu;
                    return;
                case 0x2: /* FCVT.D.L */
                    insn->type = insn_fcvt_d_l;
                    return;
                case 0x3: /* FCVT.D.LU */
                    insn->type = insn_fcvt_d_lu;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x70: {
                assert(RS2(data) == 0);
                u32 funct3 = FUNCT3(data);

                switch (funct3) {
                case 0x0: /* FMV.X.W */
                    insn->type = insn_fmv_x_w;
                    return;
                case 0x1: /* FCLASS.S */
                    insn->type = insn_fclass_s;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x71: {
                assert(RS2(data) == 0);
                u32 funct3 = FUNCT3(data);

                switch (funct3) {
                case 0x0: /* FMV.X.D */
                    insn->type = insn_fmv_x_d;
                    return;
                case 0x1: /* FCLASS.D */
                    insn->type = insn_fclass_d;
                    return;
                default: unreachable();
                }
            }
                     unreachable();
            case 0x78: /* FMV_W_X */
                assert(RS2(data) == 0 && FUNCT3(data) == 0);
                insn->type = insn_fmv_w_x;
                return;
            case 0x79: /* FMV_D_X */
                assert(RS2(data) == 0 && FUNCT3(data) == 0);
                insn->type = insn_fmv_d_x;
                return;
            default: unreachable();
            }
        }
                 unreachable();
        case 0x18: {
            *insn = insn_btype_read(data);

            u32 funct3 = FUNCT3(data);
            switch (funct3) {
            case 0x0: /* BEQ */
                insn->type = insn_beq;
                return;
            case 0x1: /* BNE */
                insn->type = insn_bne;
                return;
            case 0x4: /* BLT */
                insn->type = insn_blt;
                return;
            case 0x5: /* BGE */
                insn->type = insn_bge;
                return;
            case 0x6: /* BLTU */
                insn->type = insn_bltu;
                return;
            case 0x7: /* BGEU */
                insn->type = insn_bgeu;
                return;
            default: unreachable();
            }
        }
                 unreachable();
        case 0x19: /* JALR */
            *insn = insn_itype_read(data);
            insn->type = insn_jalr;
            insn->cont = true;
            return;
        case 0x1b: /* JAL */
            *insn = insn_jtype_read(data);
            insn->type = insn_jal;
            insn->cont = true;
            return;
        case 0x1c: {
            if (data == 0x73) { /* ECALL */
                insn->type = insn_ecall;
                insn->cont = true;
                return;
            }

            u32 funct3 = FUNCT3(data);
            *insn = insn_csrtype_read(data);
            switch (funct3) {
            case 0x1: /* CSRRW */
                insn->type = insn_csrrw;
                return;
            case 0x2: /* CSRRS */
                insn->type = insn_csrrs;
                return;
            case 0x3: /* CSRRC */
                insn->type = insn_csrrc;
                return;
            case 0x5: /* CSRRWI */
                insn->type = insn_csrrwi;
                return;
            case 0x6: /* CSRRSI */
                insn->type = insn_csrrsi;
                return;
            case 0x7: /* CSRRCI */
                insn->type = insn_csrrci;
                return;
            default: unreachable();
            }
        }
                 unreachable();
        default: unreachable();
        }
    }
            unreachable();
    default: unreachable();
    }
}

#endif