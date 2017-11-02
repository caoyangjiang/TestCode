// Copyright 2016 Caoyang Jiang

#ifndef MODULES_TRANSFORM_INCLUDE_TESTCODE_TRANSFORM_DCT2D_H_
#define MODULES_TRANSFORM_INCLUDE_TESTCODE_TRANSFORM_DCT2D_H_

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
 * @brief   Simple NxN DCT forward and inverse transformation.
 */
class Dct2d : public Dct
{
 public:
  Dct2d();
  ~Dct2d() override;

  TESTCODE_TRANSFORM_DLL bool Forward(
      const std::vector<std::vector<double>>& datain,
      std::vector<std::vector<double>>& dataout) override;

  TESTCODE_TRANSFORM_DLL bool Inverse(
      const std::vector<std::vector<double>>& datain,
      std::vector<std::vector<double>>& dataout) override;

 private:
  const double PI = 3.1415926;
};

}  // namespace TestCode
#endif  // MODULES_TRANSFORM_INCLUDE_TESTCODE_TRANSFORM_DCT2D_H_
