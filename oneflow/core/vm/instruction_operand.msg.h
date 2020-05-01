#ifndef ONEFLOW_CORE_VM_INSTRUCTION_OPERAND_H_
#define ONEFLOW_CORE_VM_INSTRUCTION_OPERAND_H_

#include "oneflow/core/common/flat_msg.h"
#include "oneflow/core/vm/logical_object_id.h"
#include "oneflow/core/vm/mirrored_object_id.msg.h"

namespace oneflow {
namespace vm {

enum OperandAccessModifier {
  kInvalidOperandAccessModifier = 0,
  kConstModifier,
  kDataMutableModifier,
  kTypeAndDataMutableModifier,
};

enum OperandMemZoneModifier {
  kInvalidOperandMemZoneModifier = 0,
  kHostConstMemZoneModifier,
  kDeviceMemZoneModifier,
};

// clang-format off
template<OperandAccessModifier access_modifier, OperandMemZoneModifier mem_zone_modifier>
FLAT_MSG_BEGIN(ModifiedOperand);
  PUBLIC static const OperandAccessModifier operand_access_modifier = access_modifier;
  PUBLIC static const OperandMemZoneModifier operand_mem_zone_modifier = mem_zone_modifier;
  // methods
  PUBLIC int64_t logical_object_id() const { return operand().logical_object_id(); }
  // fields
  FLAT_MSG_DEFINE_OPTIONAL(Operand, operand);
FLAT_MSG_END(ModifiedOperand);

using ConstOperand = ModifiedOperand<kConstModifier, kDeviceMemZoneModifier>;
using MutOperand = ModifiedOperand<kDataMutableModifier, kDeviceMemZoneModifier>;
using Mut2Operand = ModifiedOperand<kTypeAndDataMutableModifier, kDeviceMemZoneModifier>;

using SymbolOperand = ModifiedOperand<kConstModifier, kHostConstMemZoneModifier>;
using InitSymbolOperand = ModifiedOperand<kDataMutableModifier, kHostConstMemZoneModifier>;

FLAT_MSG_BEGIN(OperandSeparator);
FLAT_MSG_END(OperandSeparator);

class InstructionOperandProto;

FLAT_MSG_BEGIN(InstructionOperand);
  // methods
  PUBLIC void __Init__(const InstructionOperandProto& proto);
  // fields
  FLAT_MSG_DEFINE_STRICT_ONEOF(_,
    FLAT_MSG_ONEOF_FIELD(ConstOperand, const_operand)
    FLAT_MSG_ONEOF_FIELD(MutOperand, mut_operand)
    FLAT_MSG_ONEOF_FIELD(Mut2Operand, mut2_operand)
    FLAT_MSG_ONEOF_FIELD(SymbolOperand, symbol_operand)
    FLAT_MSG_ONEOF_FIELD(InitSymbolOperand, init_symbol_operand)
    FLAT_MSG_ONEOF_FIELD(OperandSeparator, separator)
    FLAT_MSG_ONEOF_FIELD(double, double_operand)
    FLAT_MSG_ONEOF_FIELD(int64_t, int64_operand)
    FLAT_MSG_ONEOF_FIELD(uint64_t, uint64_operand)
    FLAT_MSG_ONEOF_FIELD(bool, bool_operand));
FLAT_MSG_END(InstructionOperand);
// clang-format on

}  // namespace vm
}  // namespace oneflow

#endif  // ONEFLOW_CORE_VM_INSTRUCTION_OPERAND_H_