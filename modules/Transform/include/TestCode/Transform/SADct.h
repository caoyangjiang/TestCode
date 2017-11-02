// Copyright 2016 Caoyang Jiang

#ifndef MODULES_TRANSFORM_INCLUDE_TESTCODE_TRANSFORM_SADCT_H_
#define MODULES_TRANSFORM_INCLUDE_TESTCODE_TRANSFORM_SADCT_H_

TESTCODE_WINDOWS_DISABLE_ALL_WARNING
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <cmath>
#include <memory>
#include <vector>
TESTCODE_WINDOWS_DISABLE_ALL_WARNING

#include "TestCode/Transform/Dct.h"

namespace TestCode
{
/**
 * @brief  S.Thomas & M.Bela, "Shape-Adaptive DCT for Generic Coding of Video"
 */
class SADct : public Dct
{
 public:
  TESTCODE_TRANSFORM_DLL SADct();
  TESTCODE_TRANSFORM_DLL ~SADct() override;

  TESTCODE_TRANSFORM_DLL bool Forward(
      const std::vector<std::vector<double>>& datain,
      std::vector<std::vector<double>>& dataout) override;
  TESTCODE_TRANSFORM_DLL bool Inverse(
      const std::vector<std::vector<double>>& datain,
      std::vector<std::vector<double>>& dataout) override;

  TESTCODE_TRANSFORM_DLL bool SetContour(
      const std::vector<std::vector<bool>>& contour);

 private:
  std::vector<std::vector<bool>> contour_;
  std::vector<Eigen::MatrixXd> kernels_;  // Up to 64x64 transformation
  const uint32_t MAXDIM = 16;             // Maximum dimension in any direction
  const double PI       = 3.14159265;
};

}  // namespace TestCode
#endif  // MODULES_TRANSFORM_INCLUDE_TESTCODE_TRANSFORM_SADCT_H_
