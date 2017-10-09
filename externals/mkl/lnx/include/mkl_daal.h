/*******************************************************************************
* Copyright 2014-2016 Intel Corporation
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
*******************************************************************************/

#ifndef MKL_DAL_H
#define MKL_DAL_H

//felix
// MKL definitions for OpenBLAS
#include <stddef.h>
#include "mkl_dnn_types.h"
#include "cblas.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(MKL_INT)
#if defined(_WIN64) || defined(__x86_64__)
#define MKL_INT __int64
#else
#define MKL_INT __int32
#endif
#endif

#ifndef MKL_Complex8
typedef
struct _MKL_Complex8 {
    float real;
    float imag;
} MKL_Complex8;
#endif

#ifndef MKL_Complex16
typedef
struct _MKL_Complex16 {
    double real;
    double imag;
} MKL_Complex16;
#endif

typedef void *             _MKL_DSS_HANDLE_t;

enum PARDISO_ENV_PARAM {
       PARDISO_OOC_FILE_NAME = 1
};

typedef int             IppStatus;
typedef unsigned char   Ipp8u;
typedef unsigned short  Ipp16u;
typedef unsigned int    Ipp32u;
typedef signed short    Ipp16s;
typedef signed int      Ipp32s;
typedef float           Ipp32f;
typedef double          Ipp64f;

#if defined(__cplusplus)
}
#endif

#endif /*MKL_DAL_H*/
