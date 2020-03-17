#ifndef ONEFLOW_CORE_VM_VPU_INSTRUCTION_MSG_H_
#define ONEFLOW_CORE_VM_VPU_INSTRUCTION_MSG_H_

#include <cstring>
#include <mutex>
#include "oneflow/core/common/flat_msg.h"
#include "oneflow/core/common/object_msg.h"
#include "oneflow/core/vm/stream_desc.msg.h"
#include "oneflow/core/vm/mirrored_object.msg.h"
#include "oneflow/core/vm/stream_type.h"
#include "oneflow/core/vm/instruction_id.msg.h"
#include "oneflow/core/vm/instruction.pb.h"

namespace oneflow {
namespace vm {

class MirroredObject;

// clang-format off
OBJECT_MSG_BEGIN(InstructionMsg);
  // methods
  PUBLIC void __Init__() {}
  PUBLIC void __Init__(const InstructionProto& proto);
  PUBLIC InstructionOperand* add_operand();

  // fields
  OBJECT_MSG_DEFINE_FLAT_MSG(InstructionId, instr_id);
  OBJECT_MSG_DEFINE_STRUCT(std::vector<FlatMsg<InstructionOperand>>, operand);

  // links
  OBJECT_MSG_DEFINE_LIST_LINK(instr_msg_link);
OBJECT_MSG_END(InstructionMsg);
// clang-format on

// clang-format off
OBJECT_MSG_BEGIN(Instruction);
  // methods
  PUBLIC void __Init__(InstrChain* instr_chain, InstructionMsg* instr_msg) {
    set_instr_chain(instr_chain);
    reset_instr_msg(instr_msg);
  }
  PUBLIC MirroredObject* FindMirroredObjectByOperand(const MirroredObjectOperand& operand,
                                                     int64_t default_parallel_id);
  // fields
  OBJECT_MSG_DEFINE_OPTIONAL(InstructionMsg, instr_msg);
  OBJECT_MSG_DEFINE_PTR(InstrChain, instr_chain);

  // links
  OBJECT_MSG_DEFINE_LIST_LINK(instruction_link);
  OBJECT_MSG_DEFINE_SKIPLIST_HEAD(MirroredObjectAccess, mirrored_object_id, mirrored_object_id2access);
OBJECT_MSG_END(Instruction);
// clang-format on

static const int kInstructionStatusBufferBytes = 32;
// clang-format off
FLAT_MSG_BEGIN(InstructionStatusBuffer);
  FLAT_MSG_DEFINE_REPEATED(char, buffer, kInstructionStatusBufferBytes);
FLAT_MSG_END(InstructionStatusBuffer);
// clang-format on

class InstrChain;
// clang-format off
OBJECT_MSG_BEGIN(InstrChainEdge);
  // methods
  PUBLIC void __Init__(InstrChain* src_instr_chain, InstrChain* dst_instr_chain) {
    set_src_instr_chain(src_instr_chain);
    set_dst_instr_chain(dst_instr_chain);
  }
  // links
  OBJECT_MSG_DEFINE_SKIPLIST_KEY(10, InstrChain*, src_instr_chain);
  OBJECT_MSG_DEFINE_SKIPLIST_KEY(10, InstrChain*, dst_instr_chain);
OBJECT_MSG_END(InstrChainEdge);
// clang-format on

class Stream;
// clang-format off
OBJECT_MSG_BEGIN(InstrChain);
  // methods
  PUBLIC void __Init__(InstructionMsg* instr_msg, Stream* stream);
  PUBLIC void __Delete__();
  PUBLIC bool Done() const;

  // fields
  OBJECT_MSG_DEFINE_FLAT_MSG(InstructionStatusBuffer, status_buffer);
  OBJECT_MSG_DEFINE_PTR(Stream, stream); 
  OBJECT_MSG_DEFINE_PTR(const StreamType, stream_type);

  // links
  OBJECT_MSG_DEFINE_LIST_LINK(instr_chain_link);
  OBJECT_MSG_DEFINE_LIST_LINK(pending_chain_link);
  OBJECT_MSG_DEFINE_LIST_HEAD(Instruction, instruction_link, instruction_list);
  OBJECT_MSG_DEFINE_LIST_HEAD(CallbackMsg, callback_link, callback_list);
  OBJECT_MSG_DEFINE_SKIPLIST_HEAD(InstrChainEdge, src_instr_chain, in_edges);
  OBJECT_MSG_DEFINE_SKIPLIST_HEAD(InstrChainEdge, dst_instr_chain, out_edges);
OBJECT_MSG_END(InstrChain);
// clang-format on

}  // namespace vm
}  // namespace oneflow

#endif  // ONEFLOW_CORE_VM_VPU_INSTRUCTION_MSG_H_