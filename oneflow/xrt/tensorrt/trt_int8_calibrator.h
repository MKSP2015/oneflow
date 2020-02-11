#ifndef ONEFLOW_XRT_TENSORRT_TRT_INT8_CALIBRATOR_H_
#define ONEFLOW_XRT_TENSORRT_TRT_INT8_CALIBRATOR_H_

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "NvInfer.h"
#include "oneflow/xrt/parameter.h"
#include "oneflow/xrt/tensorrt/trt_unique_ptr.h"

namespace oneflow {
namespace xrt {

namespace tensorrt {

// Refered from tensorflow
class TRTInt8Calibrator : public nvinfer1::IInt8EntropyCalibrator2 {
 public:
  // Construct a calibrator for future calibration.
  TRTInt8Calibrator();

  // Construct a finalized calibrator where we don't need to run calibration any
  // more, as the calibration data is provided.
  TRTInt8Calibrator(const std::string& calibration_data);

  ~TRTInt8Calibrator() = default;

  int getBatchSize() const override;

  bool getBatch(void* bindings[], const char* names[],  // NOLINT
                int num_bindings) override;

  void setBatchSize(const int batch_size);

  // Feed calibration data to the calibrator, and return true if the data is
  // accepted. Return false if the calibrator has been terminated.
  bool setBatch(const std::vector<const Parameter*>& params);

  // Wait until the last batch is consumed by the calibrator and set done.
  void waitAndSetDone();

  // Notify that calibration is done and future batches provided by setBatch()
  // will be ignored.
  void setDone();

  // If not null, calibration is skipped.
  const void* readCalibrationCache(std::size_t& length) override;

  void writeCalibrationCache(const void* ptr, std::size_t length) override;

  const std::string& getCalibrationTableAsString() {  // NOLINT
    return calibration_table_;
  }

 private:
  int batch_size_;

  // mutex for condition_variable
  std::mutex cond_mtx_;

  // condition variable to implement producer-consumer queue for calibration
  std::condition_variable cond_;

  // Is calibration finished?
  bool done_;

  // Map to keep tensorrt input buffers and sizes keyed with buffer names
  std::unordered_map<std::string, std::pair<void*, size_t>> dev_buffers_;

  bool calib_running_;
  bool batch_is_set_;

  std::string calibration_table_;
};

struct TRTInt8CalibratorResource {
 public:
  static TRTInt8CalibratorResource* LookupOrCreate(const std::string& name);
  // Individual mutex
  std::mutex mutex_;

  std::shared_ptr<TRTInt8Calibrator> calibrator_;
  std::shared_ptr<std::thread> thread_;

  nv::unique_ptr<nvinfer1::ICudaEngine> engine_;
};

}  // namespace tensorrt

}  // namespace xrt
}  // namespace oneflow

#endif  // ONEFLOW_XRT_TENSORRT_TRT_INT8_CALIBRATOR_H_
