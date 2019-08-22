#ifndef ONEFLOW_CORE_OPERATOR_REDUCE_SPLIT_OP_H_
#define ONEFLOW_CORE_OPERATOR_REDUCE_SPLIT_OP_H_

#include "oneflow/core/operator/operator.h"
#include "oneflow/core/graph/logical_node.h"

namespace oneflow {

class ReduceSplitOp final : public Operator {
 public:
  OF_DISALLOW_COPY_AND_MOVE(ReduceSplitOp);
  ReduceSplitOp() = default;
  ~ReduceSplitOp() = default;

  void InitFromOpConf() override;
  const PbMessage& GetCustomizedConf() const override;
  LogicalNode* NewProperLogicalNode() const override { return new ReduceSplitLogicalNode; }
  void InferBlobDescs(std::function<BlobDesc*(const std::string&)> GetBlobDesc4BnInOp,
                      const ParallelContext* parallel_ctx) const override;

 private:
  void InferHasBatchDim(std::function<bool*(const std::string&)> HasBatchDim4BnInOp) const override;
  void GetSbpSignatures(SbpSignatureList* sbp_sig_list) const override {}
  void VirtualGenKernelConf(std::function<const BlobDesc*(const std::string&)> GetBlobDesc4BnInOp,
                            const ParallelContext*, KernelConf*) const override;
};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_OPERATOR_REDUCE_SPLIT_OP_H_
