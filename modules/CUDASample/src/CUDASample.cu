// Copyright 2015 Jason Juang

#include "Hvr/CUDASample/CUDASample.cuh"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include "opencv2/opencv.hpp"
HVR_WINDOWS_ENABLE_ALL_WARNING

#include "Hvr/CUDASample/CUDAConfig.h"

__global__ static void CUDAKernelSetImageToWhite(int *data)
{
  const int x  = blockIdx.x * blockDim.x + threadIdx.x;
  const int y  = blockIdx.y * blockDim.y + threadIdx.y;
  const int mx = gridDim.x * blockDim.x;

  data[y * mx + x] = 255;
}

namespace hvr
{
CUDASample::CUDASample()
{
}

CUDASample::~CUDASample()
{
}

void CUDASample::SetImageToWhite(cv::Mat &img) const
{
  if (img.empty()) return;

  const int h = img.rows;
  const int w = img.cols;

  CUDAConfig cudaconfig;
  cudaconfig.h_a = cudaconfig.align(h, cudaconfig.blk_h);
  cudaconfig.w_a = cudaconfig.align(w, cudaconfig.blk_w);

  const int h_a = cudaconfig.h_a;
  const int w_a = cudaconfig.w_a;

  int *data;
  cudaMallocManaged(reinterpret_cast<void **>(&data),
                    cudaconfig.h_a * cudaconfig.w_a * sizeof(int),
                    cudaMemAttachGlobal);

  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++)
    {
      data[i * w_a + j] = img.at<uchar>(i, j);
    }

  dim3 blks((w_a >> cudaconfig.shift_w), (h_a >> cudaconfig.shift_h));
  dim3 threads(cudaconfig.blk_w, cudaconfig.blk_h);

  CUDAKernelSetImageToWhite<<<blks, threads>>>(data);

  cudaDeviceSynchronize();

  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++)
    {
      img.at<uchar>(i, j) = data[i * w_a + j];
    }

  cudaFree(data);
}

}  // namespace hvr
