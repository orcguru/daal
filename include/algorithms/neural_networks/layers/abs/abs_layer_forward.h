/* file: abs_layer_forward.h */
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
//  Implementation of the absolute value (abs) layer interface
//--
*/

#ifndef __NEURAL_NETWORK_ABS_LAYER_FORWARD_H__
#define __NEURAL_NETWORK_ABS_LAYER_FORWARD_H__

#include "algorithms/algorithm.h"
#include "data_management/data/tensor.h"
#include "services/daal_defines.h"
#include "algorithms/neural_networks/layers/layer.h"
#include "algorithms/neural_networks/layers/abs/abs_layer_types.h"
#include "algorithms/neural_networks/layers/abs/abs_layer_forward_types.h"

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace abs
{
namespace forward
{
namespace interface1
{
/**
* \brief Provides methods to run implementations of the of the forward abs layer
*        This class is associated with the daal::algorithms::neural_networks::layers::abs::forward::Batch class
*        and supports the method of forward abs layer computation in the batch processing mode
*
* \tparam algorithmFPType  Data type to use in intermediate computations of forward abs layer, double or float
* \tparam method           Computation method of the layer, \ref daal::algorithms::neural_networks::layers::abs::Method
* \tparam cpu              Version of the cpu-specific implementation of the layer, \ref daal::CpuType
*/
template<typename algorithmFPType, Method method, CpuType cpu>
class DAAL_EXPORT BatchContainer : public AnalysisContainerIface<batch>
{
public:
    /**
    * Constructs the container for the forward abs layer with the specified environment
    * \param[in] daalEnv   Environment object
    */
    BatchContainer(daal::services::Environment::env *daalEnv);
    ~BatchContainer();

    void compute();
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__NEURAL_NETWORKS__LAYERS__ABS__FORWARD__BATCH"></a>
 * \brief Computes the results of the forward abs layer in the batch processing mode
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations of the forward abs layer, double or float
 * \tparam method           Forward abs layer method, \ref Method
 *
 * \par Enumerations
 *      - \ref Method                     Computation methods for the forward abs layer
 *      - \ref forward::InputId           Identifiers of input objects for the forward abs layer
 *      - \ref forward::ResultId          Identifiers of result objects for the forward abs layer
 *      - \ref LayerDataId                Identifiers of extra results computed by the forward abs layer
 *
 * \par References
 *      - <a href="DAAL-REF-ABSFORWARD-ALGORITHM">Forward abs layer description and usage models</a>
 *      - \ref backward::interface1::Batch "backward::Batch" class
 */
template<typename algorithmFPType = float, Method method = defaultDense>
class Batch : public layers::forward::LayerIface
{
public:
    Input input;         /*!< %Input objects of the layer */

    /** Default constructor */
    Batch()
    {
        initialize();
    };

    /**
     * Constructs a forward abs layer by copying input objects
     * and parameters of another forward abs layer in the batch processing mode
     * \param[in] other Algorithm to use as the source to initialize the input objects
     *                  and parameters of the algorithm
     */
    Batch(const Batch<algorithmFPType, method> &other)
    {
        initialize();
        input.set(layers::forward::data, other.input.get(layers::forward::data));
        input.set(layers::forward::weights, other.input.get(layers::forward::weights));
        input.set(layers::forward::biases, other.input.get(layers::forward::biases));
    }

    /**
    * Returns the method of the algorithm
    * \return Method of the algorithm
    */
    virtual int getMethod() const DAAL_C11_OVERRIDE { return(int) method; }

    /**
     * Returns the structure that contains input objects of the abs forward layer
     * \return Structure that contains input objects of the abs forward layer
     */
    virtual Input *getLayerInput() DAAL_C11_OVERRIDE { return &input; }

    /**
     * Returns the structure that contains parameters of the forward abs layer
     * \return Structure that contains parameters of the forward abs layer
     */
    virtual Parameter *getLayerParameter() { return NULL; };

    /**
     * Returns the structure that contains result of the forward abs layer
     * \return Structure that contains result of the forward abs layer
     */
    services::SharedPtr<layers::forward::Result> getLayerResult()
    {
        return _result;
    }

    /**
     * Returns the structure that contains result of the forward abs layer
     * \return Structure that contains result of the forward abs layer
     */
    services::SharedPtr<Result> getResult()
    {
        return _result;
    }

    /**
     * Registers user-allocated memory to store result of the forward abs layer
     * \param[in] result  Structure to store result of the forward abs layer
     */
    void setResult(services::SharedPtr<Result> result)
    {
        _result = result;
        _res = _result.get();
    }

    /**
     * Returns a pointer to the newly allocated forward abs layer
     * with a copy of input objects and parameters of this forward abs layer
     * in the batch processing mode
     * \return Pointer to the newly allocated algorithm
     */
    services::SharedPtr<Batch<algorithmFPType, method> > clone() const
    {
        return services::SharedPtr<Batch<algorithmFPType, method> >(cloneImpl());
    }


    /**
    * Allocates memory to store the result of the forward abs layer
    */
    virtual void allocateResult()
    {
        this->_result->template allocate<algorithmFPType>(&(this->input), NULL, (int) method);
        this->_res = this->_result.get();
    }

    /**
    * Allocates memory to store the result of the forward abs layer for the backward abs layer
    */
    virtual void allocateLayerData() DAAL_C11_OVERRIDE
    {
        this->_result->template allocateLayerData<algorithmFPType>(&(this->input), NULL, (int) method);
        this->_res = this->_result.get();
    }

protected:
    virtual Batch<algorithmFPType, method> *cloneImpl() const DAAL_C11_OVERRIDE
    {
        return new Batch<algorithmFPType, method>(*this);
    }

    void initialize()
    {
        Analysis<batch>::_ac = new __DAAL_ALGORITHM_CONTAINER(batch, BatchContainer, algorithmFPType, method)(&_env);
        _in = &input;
        _result = services::SharedPtr<Result>(new Result());
    }

private:
    services::SharedPtr<Result> _result;
};
} // namespace interface1
using interface1::BatchContainer;
using interface1::Batch;
} // namespace forward
} // namespace abs
} // namespace layers
} // namespace neural_networks
} // namespace algorithms
} // namespace daal
#endif
