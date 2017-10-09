/* file: service_rng_mkl.h */
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

/*
//++
//  Declaration of math service functions
//--
*/

#ifndef __SERVICE_RNG_MKL_H__
#define __SERVICE_RNG_MKL_H__

#include "vmlvsl.h"
#include "service_defines.h"
#include "vmlvsl.h"
#include <random>
#include <functional>

namespace daal
{
namespace internal
{
namespace mkl
{

template<typename baseType, CpuType cpu>
struct MklIntRng {};

/*
// Double precision functions definition
*/

using namespace std;

template<CpuType cpu>
struct MklIntRng<int, cpu>
{
    typedef size_t SizeType;

    MklIntRng(int seed):
        BRNG((1<<20)*8), /* MT19937 */
        SEED(seed),
        METHOD(0), /* VSL_RNG_METHOD_BERNOULLI_ICDF */
        stream(0),
        errcode(0)
    {
        rand = mt19937(seed);
    }

    ~MklIntRng()
    {
    }

    void uniform(size_t n, int a, int b, int* r)
    {
        for (size_t i = 0; i < n; i++) {
            r[i] = std::uniform_int_distribution<int>{a, b}(rand);
        }
    }

    void bernoulli(size_t n, int* r, double p)
    {
        for (size_t i = 0; i < n; i++) {
            r[i] = std::bind(std::bernoulli_distribution(p), rand);
        }
    }

private:
   const int BRNG;
   const unsigned int SEED;
   const int METHOD;
   void* stream;
   int errcode;
   mt19937 rand;
};

}
}
}

#endif
