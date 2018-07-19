/*
 * e_expf.c - single-precision exp function
 *
 * Copyright (c) 2009-2015, Arm Limited.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Algorithm was once taken from Cody & Waite, but has been munged
 * out of all recognition by SGT.
 */

#include <math.h>
#include <errno.h>
#include "math_private.h"

float
expf(float X)
{
  int N; float XN, g, Rg, Result;
  unsigned ix = fai(X), edgecaseflag = 0;

  /*
   * Handle infinities, NaNs and big numbers.
   */
  if (__builtin_expect((ix << 1) - 0x67000000 > 0x85500000 - 0x67000000, 0)) {
    if (!(0x7f800000 & ~ix)) {
      if (ix == 0xff800000)
        return 0.0f;
      else
        return FLOAT_INFNAN(X);/* do the right thing with both kinds of NaN and with +inf */
    } else if ((ix << 1) < 0x67000000) {
      return 1.0f;               /* magnitude so small the answer can't be distinguished from 1 */
    } else if ((ix << 1) > 0x85a00000) {
      __set_errno(ERANGE);
      if (ix & 0x80000000) {
        return FLOAT_UNDERFLOW;
      } else {
        return FLOAT_OVERFLOW;
      }
    } else {
      edgecaseflag = 1;
    }
  }

  /*
   * Split the input into an integer multiple of log(2)/4, and a
   * fractional part.
   */
  XN = X * (4.0f*1.4426950408889634074f);
#ifdef __TARGET_FPU_SOFTVFP
  XN = _frnd(XN);
  N = (int)XN;
#else
  N = (int)(XN + (ix & 0x80000000 ? -0.5f : 0.5f));
  XN = N;
#endif
  g = (X - XN * 0x1.62ep-3F) - XN * 0x1.0bfbe8p-17F;  /* prec-and-a-half representation of log(2)/4 */

  /*
   * Now we compute exp(X) in, conceptually, three parts:
   *  - a pure power of two which we get from N>>2
   *  - exp(g) for g in [-log(2)/8,+log(2)/8], which we compute
   *    using a Remez-generated polynomial approximation
   *  - exp(k*log(2)/4) (aka 2^(k/4)) for k in [0..3], which we
   *    get from a lookup table in precision-and-a-half and
   *    multiply by g.
   *
   * We gain a bit of extra precision by the fact that actually
   * our polynomial approximation gives us exp(g)-1, and we add
   * the 1 back on by tweaking the prec-and-a-half multiplication
   * step.
   *
   * Coefficients generated by the command

./auxiliary/remez.jl --variable=g --suffix=f -- '-log(BigFloat(2))/8' '+log(BigFloat(2))/8' 3 0 '(expm1(x))/x'

  */
  Rg = g * (
            9.999999412829185331953781321128516523408059996430919985217971370689774264850229e-01f+g*(4.999999608551332693833317084753864837160947932961832943901913087652889900683833e-01f+g*(1.667292360203016574303631953046104769969440903672618034272397630620346717392378e-01f+g*(4.168230895653321517750133783431970715648192153539929404872173693978116154823859e-02f)))
            );

  /*
   * Do the table lookup and combine it with Rg, to get our final
   * answer apart from the exponent.
   */
  {
    static const float twotokover4top[4] = { 0x1p+0F, 0x1.306p+0F, 0x1.6ap+0F, 0x1.ae8p+0F };
    static const float twotokover4bot[4] = { 0x0p+0F, 0x1.fc1464p-13F, 0x1.3cccfep-13F, 0x1.3f32b6p-13F };
    static const float twotokover4all[4] = { 0x1p+0F, 0x1.306fep+0F, 0x1.6a09e6p+0F, 0x1.ae89fap+0F };
    int index = (N & 3);
    Rg = twotokover4top[index] + (twotokover4bot[index] + twotokover4all[index]*Rg);
    N >>= 2;
  }

  /*
   * Combine the output exponent and mantissa, and return.
   */
  if (__builtin_expect(edgecaseflag, 0)) {
    Result = fhex(((N/2) << 23) + 0x3f800000);
    Result *= Rg;
    Result *= fhex(((N-N/2) << 23) + 0x3f800000);
    /*
     * Step not mentioned in C&W: set errno reliably.
     */
    if (fai(Result) == 0)
      return MATHERR_EXPF_UFL(Result);
    if (fai(Result) == 0x7f800000)
      return MATHERR_EXPF_OFL(Result);
    return FLOAT_CHECKDENORM(Result);
  } else {
    Result = fhex(N * 8388608.0f + (float)0x3f800000);
    Result *= Rg;
  }

  return Result;
}
