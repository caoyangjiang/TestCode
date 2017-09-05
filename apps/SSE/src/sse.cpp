// Copyright 2017 Caoyang Jiang

#include <smmintrin.h>
#include <chrono>
#include <cstdint>
#include <iostream>

#define TYPE uint32_t
int main()
{
  int16_t* data;
  int16_t* data2;
  TYPE* p;
  __m128i reg1, reg2, reg3, regtmp;
  uint32_t sum = 0;
  data         = new int16_t[256];
  data2        = new int16_t[256];
  std::chrono::high_resolution_clock::time_point beg, end;

  for (size_t i = 0; i < 256; i++)
  {
    data[i]  = i;
    data2[i] = 2 * i;
  }

  reg1   = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
  regtmp = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + 8));
  reg1   = _mm_slli_epi16(reg1, 8);
  reg1   = _mm_add_epi16(reg1, regtmp);

  reg2   = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data2));
  regtmp = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data2 + 8));
  reg2   = _mm_slli_epi16(reg2, 8);
  reg2   = _mm_add_epi16(reg2, regtmp);

  reg3 = _mm_sad_epu8(reg1, reg2);
  reg3 = _mm_hadd_epi32(reg3, reg3);
  reg3 = _mm_hadd_epi32(reg3, reg3);

  p = reinterpret_cast<TYPE*>(&reg3);
  sum += *p;
  std::cout << "sum" << sum << std::endl;
  reg1   = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + 16));
  regtmp = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + 32));
  reg1   = _mm_slli_epi16(reg1, 8);
  reg1   = _mm_add_epi16(reg1, regtmp);

  reg2   = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data2 + 16));
  regtmp = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data2 + 32));
  reg2   = _mm_slli_epi16(reg2, 8);
  reg2   = _mm_add_epi16(reg2, regtmp);

  reg3 = _mm_sad_epu8(reg1, reg2);
  reg3 = _mm_hadd_epi32(reg3, reg3);
  reg3 = _mm_hadd_epi32(reg3, reg3);

  p = reinterpret_cast<TYPE*>(&reg3);
  sum += *p;

  for (size_t i = 0; i < 4; i++)
  {
    std::cout << static_cast<uint32_t>(*(p + i)) << std::endl;
  }

  std::cout << "Sum: " << sum << std::endl;
  delete[] data;
  delete[] data2;
  return 0;
}