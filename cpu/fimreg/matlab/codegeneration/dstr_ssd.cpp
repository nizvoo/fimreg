/* =====================================
=== FIMREG - Fast Image Registration ===
========================================

Written by Roelof Berg, Berg Solutions (rberg@berg-solutions.de) with support from
Lars Koenig, Fraunhofer MEVIS (lars.koenig@mevis.fraunhofer.de) Jan Ruehaak, Fraunhofer
MEVIS (jan.ruehaak@mevis.fraunhofer.de).

THIS IS A LIMITED RESEARCH PROTOTYPE. Documentation: www.berg-solutions.de/fimreg.html

------------------------------------------------------------------------------

Copyright (c) 2014, Roelof Berg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

* Neither the name of the owner nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------*/

/*
 * dstr_ssd.cpp
 *
 * CODE GENERATED BY MATLAB CODER (THE HUMAN READABILITY IS THEREFORE LIMITED)
 *
 */

#include "stdafx.h"

/* Include files */
#include "rt_nonfinite.h"
#include "calcDSPLayout.h"
#include "diffimg.h"
#include "dstr_ssd.h"
#include "gaussnewton.h"
#include "gen_example_data.h"
#include "generatePyramidPC.h"
#include "get_current_time_in_sec.h"
#include "jacobian.h"
#include "jacobianOnTarget.h"
#include "myprintf.h"
#include "notiifyFinishedOnTarget.h"
#include "sendToTarget.h"
#include "ssd.h"
#include "ssdOnTarget.h"
#include "start_jacobianOnTarget.h"
#include "start_ssdOnTarget.h"
#include "transform.h"
#include "transmitImageData.h"
#include "waitUntilTargetReady.h"
#include "fimreg_emxutil.h"

/* Custom Source Code */
#include "pseudo_stdafx.h"             //precompiled header not possible because of include position of matlab

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
real32_T b_dstr_ssd(uint32_T DSPCount, const real32_T w[3], const uint32_T Level)
{
  real32_T SSD;
  emxArray_real32_T *SSDp;
  uint32_T aHeight;
  uint32_T aWidth;
  int32_T vlen;
  int32_T loop_ub;
  uint32_T i;
  uint32_T x;
  uint32_T y;
  emxInit_real32_T(&SSDp, 1);

  /* Global definition has only effect in matlab simulation (coder.extrinsic forbids global use here in matlab coder) */
  /* coder.extrinsic('global');  %this line unfortunately doesn't work */
  /* global GlobTParts GlobTPartsPtrs GlobRParts GlobRPartsPtrs GlobDSPResponsibilityBox GlobBoundBox GlobImgDimension; */
  /* Calc layout of DSP grid regarding the Reference image */
  calcDSPLayout(DSPCount, &aWidth, &aHeight);
  vlen = SSDp->size[0];
  SSDp->size[0] = (int32_T)DSPCount;
  emxEnsureCapacity((emxArray__common *)SSDp, vlen, (int32_T)sizeof(real32_T));
  loop_ub = (int32_T)DSPCount - 1;
  for (vlen = 0; vlen <= loop_ub; vlen++) {
    SSDp->data[vlen] = 0.0F;
  }

  /* Matlab coder compilation: Start calculation nonblocking on all DSPs in */
  /* parallel, then the results can be taken below in a blocking way. */
  i = 1U;
  for (x = 1U; x <= aWidth; x++) {
    for (y = 1U; y <= aHeight; y++) {
      start_ssdOnTarget(i, w, Level);
      i++;
    }
  }

  i = 1U;
  for (x = 1U; x <= aWidth; x++) {
    for (y = 1U; y <= aHeight; y++) {
      /* calculate distributed (happens in parallel on the DSPs, but serialized in matlab) */
      /* Matlab coder compilation: Fetch calculation result from DSP */
      /* (Blocking is ok here as the calculation was allready started */
      /* above in a nonblocking way and all DSPs spend equal time for */
      /* calculation and just fetching the data needs not much time.) */
      SSDp->data[(int32_T)i - 1] = ssdOnTarget(i);
      i++;
    }
  }

  /* Matlab coder compilation: Wait until all targets ready */
  /* block; ! */
  /* Sum up all partial results */
  if (SSDp->size[0] == 0) {
    SSD = 0.0F;
  } else {
    vlen = SSDp->size[0];
    SSD = SSDp->data[0];
    for (loop_ub = 2; loop_ub <= vlen; loop_ub++) {
      SSD += SSDp->data[loop_ub - 1];
    }
  }

  emxFree_real32_T(&SSDp);
  return SSD;
}

real32_T dstr_ssd(uint32_T DSPCount, const real32_T w[3], uint32_T Level)
{
  real32_T SSD;
  emxArray_real32_T *SSDp;
  uint32_T aHeight;
  uint32_T aWidth;
  int32_T vlen;
  int32_T loop_ub;
  uint32_T i;
  uint32_T x;
  uint32_T y;
  real32_T b_w[3];
  emxInit_real32_T(&SSDp, 1);

  /* Global definition has only effect in matlab simulation (coder.extrinsic forbids global use here in matlab coder) */
  /* coder.extrinsic('global');  %this line unfortunately doesn't work */
  /* global GlobTParts GlobTPartsPtrs GlobRParts GlobRPartsPtrs GlobDSPResponsibilityBox GlobBoundBox GlobImgDimension; */
  /* Calc layout of DSP grid regarding the Reference image */
  calcDSPLayout(DSPCount, &aWidth, &aHeight);
  vlen = SSDp->size[0];
  SSDp->size[0] = (int32_T)DSPCount;
  emxEnsureCapacity((emxArray__common *)SSDp, vlen, (int32_T)sizeof(real32_T));
  loop_ub = (int32_T)DSPCount - 1;
  for (vlen = 0; vlen <= loop_ub; vlen++) {
    SSDp->data[vlen] = 0.0F;
  }

  /* Matlab coder compilation: Start calculation nonblocking on all DSPs in */
  /* parallel, then the results can be taken below in a blocking way. */
  i = 1U;
  for (x = 1U; x <= aWidth; x++) {
    for (y = 1U; y <= aHeight; y++) {
      /* Call ssd.m (its generated c++ code) on the DSP target system */
      /* Matlab coder compilation: */
      for (vlen = 0; vlen < 3; vlen++) {
        b_w[vlen] = w[vlen];
      }

      matlab_c_start_ssdOnTarget(i, b_w, Level);
      i++;
    }
  }

  i = 1U;
  for (x = 1U; x <= aWidth; x++) {
    for (y = 1U; y <= aHeight; y++) {
      /* calculate distributed (happens in parallel on the DSPs, but serialized in matlab) */
      /* Matlab coder compilation: Fetch calculation result from DSP */
      /* (Blocking is ok here as the calculation was allready started */
      /* above in a nonblocking way and all DSPs spend equal time for */
      /* calculation and just fetching the data needs not much time.) */
      SSDp->data[(int32_T)i - 1] = ssdOnTarget(i);
      i++;
    }
  }

  /* Matlab coder compilation: Wait until all targets ready */
  /* block; ! */
  /* Sum up all partial results */
  if (SSDp->size[0] == 0) {
    SSD = 0.0F;
  } else {
    vlen = SSDp->size[0];
    SSD = SSDp->data[0];
    for (loop_ub = 2; loop_ub <= vlen; loop_ub++) {
      SSD += SSDp->data[loop_ub - 1];
    }
  }

  emxFree_real32_T(&SSDp);
  return SSD;
}

/* End of code generation (dstr_ssd.cpp) */
