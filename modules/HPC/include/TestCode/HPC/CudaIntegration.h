// Copyright 2016 Caoyang Jiang

#ifndef MODULES_HPC_INCLUDE_TESTCODE_HPC_CUDAINTEGRATION_H_
#define MODULES_HPC_INCLUDE_TESTCODE_HPC_CUDAINTEGRATION_H_

#include <vector>

namespace TestCode
{
class CudaIntegration
{
 public:
  CudaIntegration();
  ~CudaIntegration();

  /**
   * @brief      Sets the color conversion width.
   *
   * @param[in]  width  The width
   */

  bool StartIntegration(double startx,
                        double endx,
                        size_t tcount,
                        std::vector<double>& area);
};

}  // namespace TestCode

#endif  // MODULES_HPC_INCLUDE_TESTCODE_HPC_CUDAINTEGRATION_H_
