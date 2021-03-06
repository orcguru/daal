/* file: low_order_moments_batch.h */
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
//  Implementation of LowOrderMoments Batch algorithm interface.
//--
*/

#ifndef __LOW_ORDER_MOMENTS_BATCH_H__
#define __LOW_ORDER_MOMENTS_BATCH_H__

#include "algorithms/algorithm.h"
#include "data_management/data/numeric_table.h"
#include "services/daal_defines.h"
#include "algorithms/moments/low_order_moments_types.h"

namespace daal
{
namespace algorithms
{
namespace low_order_moments
{

namespace interface1
{
/**
* <a name="DAAL-CLASS-ALGORITHMS__LOW_ORDER_MOMENTS__BATCHCONTAINERIFACE"></a>
* \brief Class that specifies interfaces of implementations of the low order moments algorithm.
*/
class BatchContainerIface : public daal::algorithms::AnalysisContainerIface<batch>
{
public:
    BatchContainerIface() {};
    virtual ~BatchContainerIface() {}

    /**
     * Runs the implementation of the correlation or variance-covariance matrix algorithm in the batch processing mode
     */
    virtual void compute() = 0;
};
/**
 * <a name="DAAL-CLASS-ALGORITHMS__LOW_ORDER_MOMENTS__BATCHCONTAINER"></a>
 * \brief Provides methods to run implementations of the low order moments algorithm.
 *        This class is associated with daal::algorithms::low_order_moments::Batch class

 *
 * \tparam method           Computation method of the algorithm, \ref daal::algorithms::low_order_moments::Method
 * \tparam algorithmFPType  Data type to use in intermediate computations of the low order moments, double or float
 */
template<typename algorithmFPType, Method method, CpuType cpu>
class DAAL_EXPORT BatchContainer : public BatchContainerIface
{
public:
    BatchContainer(daal::services::Environment::env *daalEnv);
    virtual ~BatchContainer();
    /**
     * Runs the implementation of the low order moments algorithm in the batch processing mode
     */
    virtual void compute();
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__LOW_ORDER_MOMENTS__BATCHIFACE"></a>
 * \brief Abstract class that specifies interface of the algorithms
 *        for computing moments of low order in the batch processing mode
 */
class DAAL_EXPORT BatchIface : public daal::algorithms::Analysis<batch>
{
public:
    /** Default constructor */
    BatchIface()
    {
        initialize();
    };

    /**
     * Constructs an algorithm for moments of low order computation
     * by copying input objects and parameters of another algorithm for moments of low order computation
     * \param[in] other An algorithm to be used as the source to initialize the input objects
     *                  and parameters of the algorithm
     */
    BatchIface(const BatchIface &other)
    {
        initialize();
        input.set(data, other.input.get(data));
    }

    /**
     * Returns the structure that contains moments of low order
     * \return Structure that contains the computed matrix
     */
    services::SharedPtr<Result> getResult()
    {
        return _result;
    };

    /**
     * Registers user-allocated memory to store results of computation of moments of low order
     * \param[in] result    Structure to store the results
     */
    virtual void setResult(const services::SharedPtr<Result> &result)
    {
        _result = result;
        _res = _result.get();
    }

    /**
     * Returns a pointer to the newly allocated algorithm for moments of low order computation
     * with a copy of input objects and parameters of this algorithm for moments of low order
     * \return Pointer to the newly allocated algorithm
     */
    services::SharedPtr<BatchIface> clone() const
    {
        return services::SharedPtr<BatchIface>(cloneImpl());
    }

    virtual ~BatchIface() {}

    Input input;                    /*!< %Input data structure */

protected:
    services::SharedPtr<Result> _result;

    void initialize()
    {
        _result = services::SharedPtr<Result>(new Result());
        _in  = &input;
        _par = NULL;
    }
    virtual BatchIface *cloneImpl() const DAAL_C11_OVERRIDE = 0;
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__LOW_ORDER_MOMENTS__BATCH"></a>
 * \brief Computes moments of low order in the batch processing mode.
 *
 * \tparam method           Computation method of the algorithm, \ref daal::algorithms::low_order_moments::Method
 * \tparam algorithmFPType  Data type to use in intermediate computations of the low order moments, double or float
 *
 * \par Enumerations
 *      - \ref Method   Computation methods for moments of low order
 *      - \ref InputId  Identifiers of input objetcs for the low order moments algorithm
 *      - \ref ResultId Identifiers of results of the low order moments algorithm
 *
 * \par References
 *      - <a href="DAAL-REF-LOW_ORDER_MOMENTS-ALGORITHM">Low order moments algorithm description and usage models</a>
 *      - Input class
 *      - Result class
 */
template<typename algorithmFPType = double, Method method = defaultDense>
class DAAL_EXPORT Batch : public BatchIface
{
public:
    /** Default constructor */
    Batch()
    {
        initialize();
    }

    /**
     * Constructs an algorithm that computes moments of low order by copying input objects
     * of another algorithm that computes moments of low order
     * \param[in] other An algorithm to be used as the source to initialize the input objects
     *                  and parameters of the algorithm
     */
    Batch(const Batch<algorithmFPType, method> &other) : BatchIface(other)
    {
        initialize();

    }

    virtual ~Batch() {}

    /**
    * Returns method of the algorithm
    * \return Method of the algorithm
    */
    virtual int getMethod() const DAAL_C11_OVERRIDE { return(int)method; }

    /**
     * Returns a pointer to the newly allocated algorithm that computes moments of low order
     * with a copy of input objects of this algorithm
     * \return Pointer to the newly allocated algorithm
     */
    services::SharedPtr<Batch<algorithmFPType, method> > clone() const
    {
        return services::SharedPtr<Batch<algorithmFPType, method> >(cloneImpl());
    }

protected:
    virtual Batch<algorithmFPType, method> *cloneImpl() const DAAL_C11_OVERRIDE
    {
        return new Batch<algorithmFPType, method>(*this);
    }

    virtual void allocateResult() DAAL_C11_OVERRIDE
    {
        _result->allocate<algorithmFPType>(&input, 0, 0);
        _res = _result.get();
    }

    void initialize()
    {
        this->_ac = new __DAAL_ALGORITHM_CONTAINER(batch, BatchContainer, algorithmFPType, method)(&_env);
    }
};
} // namespace interface1
using interface1::BatchContainerIface;
using interface1::BatchContainer;
using interface1::BatchIface;
using interface1::Batch;

} // namespace daal::algorithms::low_order_moments
}
} // namespace daal
#endif
