// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <span>
#include "shader_recompiler/frontend/instruction.h"
#include "shader_recompiler/ir/basic_block.h"
#include "shader_recompiler/ir/ir_emitter.h"
#include "shader_recompiler/runtime_info.h"

namespace Shader {
struct Info;
struct Profile;
} // namespace Shader

namespace Shader::Gcn {

enum class ConditionOp : u32 {
    F,
    EQ,
    LG,
    GT,
    GE,
    LT,
    LE,
    TRU,
    U,
};

enum class AtomicOp : u32 {
    Swap,
    CmpSwap,
    Add,
    Sub,
    Smin,
    Umin,
    Smax,
    Umax,
    And,
    Or,
    Xor,
    Inc,
    Dec,
    FCmpSwap,
    Fmin,
    Fmax,
};

enum class NegateMode : u32 {
    None,
    Src1,
    Result,
};

class Translator {
public:
    explicit Translator(IR::Block* block_, Info& info, const Profile& profile);

    // Instruction categories
    void EmitPrologue();
    void EmitFetch(const GcnInst& inst);
    void EmitDataShare(const GcnInst& inst);
    void EmitVectorInterpolation(const GcnInst& inst);
    void EmitScalarMemory(const GcnInst& inst);
    void EmitVectorMemory(const GcnInst& inst);
    void EmitExport(const GcnInst& inst);
    void EmitFlowControl(u32 pc, const GcnInst& inst);
    void EmitScalarAlu(const GcnInst& inst);
    void EmitVectorAlu(const GcnInst& inst);

    // Scalar ALU
    void S_MOVK(const GcnInst& inst);
    void S_MOV(const GcnInst& inst);
    void S_MUL_I32(const GcnInst& inst);
    void S_CMP(ConditionOp cond, bool is_signed, const GcnInst& inst);
    void S_AND_SAVEEXEC_B64(const GcnInst& inst);
    void S_MOV_B64(const GcnInst& inst);
    void S_OR_B64(NegateMode negate, bool is_xor, const GcnInst& inst);
    void S_AND_B64(NegateMode negate, const GcnInst& inst);
    void S_ADD_I32(const GcnInst& inst);
    void S_AND_B32(const GcnInst& inst);
    void S_ASHR_I32(const GcnInst& inst);
    void S_OR_B32(const GcnInst& inst);
    void S_LSHR_B32(const GcnInst& inst);
    void S_CSELECT_B32(const GcnInst& inst);
    void S_CSELECT_B64(const GcnInst& inst);
    void S_BFE_U32(const GcnInst& inst);
    void S_LSHL_B32(const GcnInst& inst);
    void S_BFM_B32(const GcnInst& inst);
    void S_NOT_B64(const GcnInst& inst);
    void S_BREV_B32(const GcnInst& inst);
    void S_ADD_U32(const GcnInst& inst);
    void S_SUB_U32(const GcnInst& inst);
    void S_GETPC_B64(u32 pc, const GcnInst& inst);
    void S_ADDC_U32(const GcnInst& inst);
    void S_MULK_I32(const GcnInst& inst);
    void S_ADDK_I32(const GcnInst& inst);
    void S_MAX_U32(const GcnInst& inst);
    void S_MIN_U32(const GcnInst& inst);

    // Scalar Memory
    void S_LOAD_DWORD(int num_dwords, const GcnInst& inst);
    void S_BUFFER_LOAD_DWORD(int num_dwords, const GcnInst& inst);

    // Vector ALU
    void V_MOV(const GcnInst& inst);
    void V_SAD(const GcnInst& inst);
    void V_MAC_F32(const GcnInst& inst);
    void V_CVT_PKRTZ_F16_F32(const GcnInst& inst);
    void V_CVT_F32_F16(const GcnInst& inst);
    void V_CVT_F16_F32(const GcnInst& inst);
    void V_MUL_F32(const GcnInst& inst);
    void V_CNDMASK_B32(const GcnInst& inst);
    void V_OR_B32(bool is_xor, const GcnInst& inst);
    void V_AND_B32(const GcnInst& inst);
    void V_LSHLREV_B32(const GcnInst& inst);
    void V_LSHL_B32(const GcnInst& inst);
    void V_ADD_I32(const GcnInst& inst);
    void V_ADDC_U32(const GcnInst& inst);
    void V_CVT_F32_I32(const GcnInst& inst);
    void V_CVT_F32_U32(const GcnInst& inst);
    void V_MAD_F32(const GcnInst& inst);
    void V_FRACT_F32(const GcnInst& inst);
    void V_ADD_F32(const GcnInst& inst);
    void V_CVT_OFF_F32_I4(const GcnInst& inst);
    void V_MED3_F32(const GcnInst& inst);
    void V_FLOOR_F32(const GcnInst& inst);
    void V_SUB_F32(const GcnInst& inst);
    void V_RCP_F32(const GcnInst& inst);
    void V_FMA_F32(const GcnInst& inst);
    void V_CMP_F32(ConditionOp op, bool set_exec, const GcnInst& inst);
    void V_MAX_F32(const GcnInst& inst, bool is_legacy = false);
    void V_MAX_U32(bool is_signed, const GcnInst& inst);
    void V_RSQ_F32(const GcnInst& inst);
    void V_SIN_F32(const GcnInst& inst);
    void V_LOG_F32(const GcnInst& inst);
    void V_EXP_F32(const GcnInst& inst);
    void V_SQRT_F32(const GcnInst& inst);
    void V_MIN_F32(const GcnInst& inst, bool is_legacy = false);
    void V_MIN3_F32(const GcnInst& inst);
    void V_MIN3_I32(const GcnInst& inst);
    void V_MADMK_F32(const GcnInst& inst);
    void V_CUBEMA_F32(const GcnInst& inst);
    void V_CUBESC_F32(const GcnInst& inst);
    void V_CUBETC_F32(const GcnInst& inst);
    void V_CUBEID_F32(const GcnInst& inst);
    void V_CVT_U32_F32(const GcnInst& inst);
    void V_SUBREV_F32(const GcnInst& inst);
    void V_SUBREV_I32(const GcnInst& inst);
    void V_MAD_U64_U32(const GcnInst& inst);
    void V_CMP_U32(ConditionOp op, bool is_signed, bool set_exec, const GcnInst& inst);
    void V_LSHRREV_B32(const GcnInst& inst);
    void V_MUL_HI_U32(bool is_signed, const GcnInst& inst);
    void V_SAD_U32(const GcnInst& inst);
    void V_BFE_U32(bool is_signed, const GcnInst& inst);
    void V_MAD_I32_I24(const GcnInst& inst, bool is_signed = true);
    void V_MUL_I32_I24(const GcnInst& inst);
    void V_SUB_I32(const GcnInst& inst);
    void V_LSHR_B32(const GcnInst& inst);
    void V_ASHRREV_I32(const GcnInst& inst);
    void V_MAD_U32_U24(const GcnInst& inst);
    void V_RNDNE_F32(const GcnInst& inst);
    void V_BCNT_U32_B32(const GcnInst& inst);
    void V_COS_F32(const GcnInst& inst);
    void V_MAX3_F32(const GcnInst& inst);
    void V_MAX3_U32(const GcnInst& inst);
    void V_CVT_I32_F32(const GcnInst& inst);
    void V_MIN_I32(const GcnInst& inst);
    void V_MUL_LO_U32(const GcnInst& inst);
    void V_TRUNC_F32(const GcnInst& inst);
    void V_CEIL_F32(const GcnInst& inst);
    void V_MIN_U32(const GcnInst& inst);
    void V_CMP_NE_U64(const GcnInst& inst);
    void V_BFI_B32(const GcnInst& inst);
    void V_NOT_B32(const GcnInst& inst);
    void V_CVT_F32_UBYTE(u32 index, const GcnInst& inst);
    void V_BFREV_B32(const GcnInst& inst);
    void V_LDEXP_F32(const GcnInst& inst);
    void V_CVT_FLR_I32_F32(const GcnInst& inst);
    void V_CMP_CLASS_F32(const GcnInst& inst);
    void V_FFBL_B32(const GcnInst& inst);
    void V_MBCNT_U32_B32(bool is_low, const GcnInst& inst);

    // Vector Memory
    void BUFFER_LOAD_FORMAT(u32 num_dwords, bool is_typed, bool is_format, const GcnInst& inst);
    void BUFFER_STORE_FORMAT(u32 num_dwords, bool is_typed, const GcnInst& inst);

    // Vector interpolation
    void V_INTERP_P2_F32(const GcnInst& inst);
    void V_INTERP_MOV_F32(const GcnInst& inst);

    // Data share
    void DS_SWIZZLE_B32(const GcnInst& inst);
    void DS_READ(int bit_size, bool is_signed, bool is_pair, const GcnInst& inst);
    void DS_WRITE(int bit_size, bool is_signed, bool is_pair, const GcnInst& inst);
    void V_READFIRSTLANE_B32(const GcnInst& inst);
    void V_READLANE_B32(const GcnInst& inst);
    void V_WRITELANE_B32(const GcnInst& inst);
    void S_BARRIER();

    // MIMG
    void IMAGE_GET_RESINFO(const GcnInst& inst);
    void IMAGE_SAMPLE(const GcnInst& inst);
    void IMAGE_GATHER(const GcnInst& inst);
    void IMAGE_STORE(const GcnInst& inst);
    void IMAGE_LOAD(bool has_mip, const GcnInst& inst);
    void IMAGE_GET_LOD(const GcnInst& inst);
    void IMAGE_ATOMIC(AtomicOp op, const GcnInst& inst);

private:
    template <typename T = IR::F32>
    [[nodiscard]] T GetSrc(const InstOperand& operand);

    template <typename T = IR::F64>
    [[nodiscard]] T GetSrc64(const InstOperand& operand);

    void SetDst(const InstOperand& operand, const IR::U32F32& value);
    void SetDst64(const InstOperand& operand, const IR::U64F64& value_raw);

    void LogMissingOpcode(const GcnInst& inst);

private:
    IR::IREmitter ir;
    Info& info;
    const Profile& profile;
    IR::U32 m0_value;
    bool opcode_missing = false;
};

void Translate(IR::Block* block, u32 block_base, std::span<const GcnInst> inst_list, Info& info,
               const Profile& profile);

template <typename T>
[[nodiscard]] T Translator::GetSrc(const InstOperand& operand) {
    constexpr bool integer = std::is_same_v<T, IR::U32>;
    if constexpr (!integer) {
        static_assert(std::is_same_v<T, IR::U32F32> || std::is_same_v<T, IR::F32>);
    }

    ASSERT(operand.type == ScalarType::Float32 || operand.type == ScalarType::Uint32 ||
           operand.type == ScalarType::Sint32);

    T value{};

    const bool is_float = operand.type == ScalarType::Float32;

    auto imm = [&]<typename V>(V value) -> T {
        if constexpr (std::is_same_v<T, IR::U32>) {
            if constexpr (std::is_same_v<V, u32> || std::is_same_v<V, s32>) {
                return ir.Imm32(value);
            } else if constexpr (std::is_same_v<V, f32>) {
                UNREACHABLE(); // Shouldn't have f32 immediate to integer operations
            } else {
                static_assert(false, "Unknown type");
            }
        } else if constexpr (std::is_same_v<T, IR::F32>) {
            if constexpr (std::is_same_v<V, f32>) {
                return ir.Imm32(value);
            } else if constexpr (std::is_same_v<V, u32> || std::is_same_v<V, s32>) {
                return ir.Imm32(f32(value));
            } else {
                static_assert(false, "Unknown type");
            }
        } else {
            static_assert(false, "Unknown type");
        }
    };

    switch (operand.field) {
    case OperandField::ScalarGPR:
        if constexpr (!integer) {
            value = ir.GetScalarReg<IR::F32>(IR::ScalarReg(operand.code));
        } else {
            value = ir.GetScalarReg<IR::U32>(IR::ScalarReg(operand.code));
        }
        break;
    case OperandField::VectorGPR:
        if constexpr (!integer) {
            value = ir.GetVectorReg<IR::F32>(IR::VectorReg(operand.code));
        } else {
            value = ir.GetVectorReg<IR::U32>(IR::VectorReg(operand.code));
        }
        break;
    case OperandField::ConstZero:
        if (is_float) {
            value = imm(0.f);
        } else {
            value = imm(0U);
        }
        break;
    case OperandField::SignedConstIntPos:
        ASSERT(!is_float);
        value = imm(operand.code - SignedConstIntPosMin + 1);
        break;
    case OperandField::SignedConstIntNeg:
        ASSERT(!is_float);
        value = imm(-s32(operand.code) + SignedConstIntNegMin - 1);
        break;
    case OperandField::LiteralConst:
        if (is_float) {
            value = imm(std::bit_cast<float>(operand.code));
        } else {
            value = imm(operand.code);
        }
        break;
    case OperandField::ConstFloatPos_1_0:
        value = imm(1.f);
        break;
    case OperandField::ConstFloatPos_0_5:
        value = imm(0.5f);
        break;
    case OperandField::ConstFloatPos_2_0:
        value = imm(2.0f);
        break;
    case OperandField::ConstFloatPos_4_0:
        value = imm(4.0f);
        break;
    case OperandField::ConstFloatNeg_0_5:
        value = imm(-0.5f);
        break;
    case OperandField::ConstFloatNeg_1_0:
        value = imm(-1.0f);
        break;
    case OperandField::ConstFloatNeg_2_0:
        value = imm(-2.0f);
        break;
    case OperandField::ConstFloatNeg_4_0:
        value = imm(-4.0f);
        break;
    case OperandField::VccLo:
        if constexpr (integer) {
            value = ir.GetVccLo();
        } else if (!is_float) {
            value = T{ir.GetVccLo()};
        } else {
            value = ir.ConvertUToF(32, 32, ir.GetVccLo());
        }
        break;
    case OperandField::VccHi:
        if constexpr (integer) {
            value = ir.GetVccHi();
        } else if (!is_float) {
            value = T{ir.GetVccHi()};
        } else {
            value = ir.ConvertUToF(32, 32, ir.GetVccHi());
        }
        break;
    case OperandField::M0:
        ASSERT(!is_float);
        if constexpr (integer) {
            return m0_value;
        } else {
            return T{m0_value};
        }
    default:
        UNREACHABLE();
    }

    if (operand.input_modifier.abs) {
        if constexpr (std::is_same_v<T, IR::U32>) {
            UNREACHABLE();
        } else {
            value = ir.FPAbs(value);
        }
    }
    if (operand.input_modifier.neg) {
        if constexpr (std::is_same_v<T, IR::U32>) {
            UNREACHABLE();
        } else {
            value = ir.FPNeg(value);
        }
    }

    return value;
}

template <typename T>
T Translator::GetSrc64(const InstOperand& operand) {
    constexpr bool integer = std::is_same_v<T, IR::U64>;
    if constexpr (!integer) {
        static_assert(std::is_same_v<T, IR::F64>);
    }
    using HalfT = std::conditional_t<std::is_same_v<T, IR::F64>, IR::F32, IR::U32>;

    ASSERT(operand.type == ScalarType::Float64 || operand.type == ScalarType::Uint64 ||
           operand.type == ScalarType::Sint64);

    HalfT value_hi{};
    HalfT value_lo{};
    T value_imm{};

    const bool is_float = operand.type == ScalarType::Float64 && !integer;
    bool immediate = false;

    switch (operand.field) {
    case OperandField::ScalarGPR:
        if constexpr (!integer) {
            value_lo = ir.GetScalarReg<IR::F32>(IR::ScalarReg(operand.code));
            value_hi = ir.GetScalarReg<IR::F32>(IR::ScalarReg(operand.code + 1));
        } else if (operand.type == ScalarType::Uint64 || operand.type == ScalarType::Sint64) {
            value_lo = ir.GetScalarReg<IR::U32>(IR::ScalarReg(operand.code));
            value_hi = ir.GetScalarReg<IR::U32>(IR::ScalarReg(operand.code + 1));
        } else {
            UNREACHABLE();
        }
        break;
    case OperandField::VectorGPR:
        if constexpr (!integer) {
            value_lo = ir.GetVectorReg<IR::F32>(IR::VectorReg(operand.code));
            value_hi = ir.GetVectorReg<IR::F32>(IR::VectorReg(operand.code + 1));
        } else if (operand.type == ScalarType::Uint64 || operand.type == ScalarType::Sint64) {
            value_lo = ir.GetVectorReg<IR::U32>(IR::VectorReg(operand.code));
            value_hi = ir.GetVectorReg<IR::U32>(IR::VectorReg(operand.code + 1));
        } else {
            UNREACHABLE();
        }
        break;
    case OperandField::ConstZero:
        immediate = true;
        if (is_float) {
            value_imm = T{ir.Imm64(0.0)};
        } else {
            value_imm = T{ir.Imm64(u64(0U))};
        }
        break;
    case OperandField::SignedConstIntPos:
        ASSERT(!is_float);
        immediate = true;
        value_imm = T{ir.Imm64(s64(operand.code) - SignedConstIntPosMin + 1)};
        break;
    case OperandField::SignedConstIntNeg:
        ASSERT(!is_float);
        immediate = true;
        value_imm = T{ir.Imm64(-s64(operand.code) + SignedConstIntNegMin - 1)};
        break;
    case OperandField::LiteralConst:
        immediate = true;
        if (is_float) {
            UNREACHABLE(); // There is a literal double?
        } else {
            value_imm = T{ir.Imm64(u64(operand.code))};
        }
        break;
    case OperandField::ConstFloatPos_1_0:
        immediate = true;
        if (is_float) {
            value_imm = T{ir.Imm64(1.0)};
        } else {
            value_imm = T{ir.Imm64(std::bit_cast<u64>(f64(1.0)))};
        }
        break;
    case OperandField::ConstFloatPos_0_5:
        immediate = true;
        value_imm = T{ir.Imm64(0.5)};
        break;
    case OperandField::ConstFloatPos_2_0:
        immediate = true;
        value_imm = T{ir.Imm64(2.0)};
        break;
    case OperandField::ConstFloatPos_4_0:
        immediate = true;
        value_imm = T{ir.Imm64(4.0)};
        break;
    case OperandField::ConstFloatNeg_0_5:
        immediate = true;
        value_imm = T{ir.Imm64(-0.5)};
        break;
    case OperandField::ConstFloatNeg_1_0:
        immediate = true;
        value_imm = T{ir.Imm64(-1.0)};
        break;
    case OperandField::ConstFloatNeg_2_0:
        immediate = true;
        value_imm = T{ir.Imm64(-2.0)};
        break;
    case OperandField::ConstFloatNeg_4_0:
        immediate = true;
        value_imm = T{ir.Imm64(-4.0)};
        break;
    case OperandField::VccLo: {
        value_lo = ir.GetVccLo();
        value_hi = ir.GetVccHi();
    } break;
    case OperandField::VccHi:
        UNREACHABLE();
    default:
        UNREACHABLE();
    }

    IR::Value value;

    if (immediate) {
        value = value_imm;
    } else if (is_float) {
        throw NotImplementedException("required OpPackDouble2x32 implementation");
    } else {
        IR::Value packed = ir.CompositeConstruct(value_lo, value_hi);
        value = ir.PackUint2x32(packed);
    }

    if (!integer) {
        if (operand.input_modifier.abs) {
            value = ir.FPAbs(IR::F32F64(value));
        }
        if (operand.input_modifier.neg) {
            value = ir.FPNeg(IR::F32F64(value));
        }
    }
    return T{value};
}

} // namespace Shader::Gcn
