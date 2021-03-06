/* file: batch_normalization_layer_forward_types.h */
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
//  Implementation of the forward batch normalization layer.
//--
*/

#ifndef __BATCH_NORMALIZATION_LAYER_FORWARD_TYPES_H__
#define __BATCH_NORMALIZATION_LAYER_FORWARD_TYPES_H__

#include "algorithms/algorithm.h"
#include "data_management/data/tensor.h"
#include "data_management/data/homogen_tensor.h"
#include "services/daal_defines.h"
#include "algorithms/neural_networks/layers/layer_forward_types.h"
#include "algorithms/neural_networks/layers/batch_normalization/batch_normalization_layer_types.h"

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace batch_normalization
{
/**
 * \brief Contains classes for forward batch normalization layer
 */
namespace forward
{
/**
 * Available identifiers of input objects for the forward batch normalization layer
 */
enum InputLayerDataId
{
    populationMean     = 3, /*!< 1-dimensional tensor of size \f$n_k\f$ that stores population mean computed on the previous stage */
    populationVariance = 4  /*!< 1-dimensional tensor of size \f$n_k\f$ that stores population variance computed on the previous stage */
};

/**
 * \brief Contains version 1.0 of Intel(R) Data Analytics Acceleration Library (Intel(R) DAAL) interface.
 */
namespace interface1
{
/**
 * \brief %Input objects for the forward batch normalization layer.
 */
class Input : public layers::forward::Input
{
public:
    /** Default constructor */
    Input() : layers::forward::Input(5) {}

    virtual ~Input() {}

    /**
     * Returns an input object for the forward batch normalization layer
     */
    using layers::forward::Input::get;
    /**
    * Sets an input object for the forward batch normalization layer
    */
    using layers::forward::Input::set;


    /**
     * Allocates memory to store input objects of forward batch normalization layer
     * \param[in] parameter %Parameter of forward batch normalization layer
     * \param[in] method    Computation method for the layer
     */
    template <typename algorithmFPType>
    void allocate(const daal::algorithms::Parameter *parameter, const int method)
    {
        const Parameter *param =  static_cast<const Parameter *>(parameter);

        if (!get(layers::forward::weights))
        {
            set(layers::forward::weights, services::SharedPtr<data_management::Tensor>(
                    new data_management::HomogenTensor<float>(getWeightsSizes(param), data_management::Tensor::doAllocate, 0.5f)) );
        }

        if (!get(layers::forward::biases))
        {
            set(layers::forward::biases, services::SharedPtr<data_management::Tensor>(
                    new data_management::HomogenTensor<float>(getBiasesSizes(param), data_management::Tensor::doAllocate, 0.0f)) );
        }
    }

    /**
     * Returns dimensions of weights tensor
     * \return Dimensions of weights tensor
     */
    virtual const services::Collection<size_t> getWeightsSizes(const layers::Parameter *parameter) const DAAL_C11_OVERRIDE

    {
        const Parameter *algParameter =  static_cast<const Parameter *>(parameter);
        const services::Collection<size_t> &dims = get(layers::forward::data)->getDimensions();
        services::Collection<size_t> wDims(1);
        wDims[0] = dims[algParameter->dimension];
        return wDims;
    }

    /**
     * Returns dimensions of biases tensor
     * \return Dimensions of biases tensor
     */
    virtual const services::Collection<size_t> getBiasesSizes(const layers::Parameter *parameter) const DAAL_C11_OVERRIDE
    {
        return getWeightsSizes(parameter);
    }

    /**
     * Returns an input object for forward batch normalization layer
     * \param[in] id    Identifier of the input object
     * \return          %Input object that corresponds to the given identifier
     */
    services::SharedPtr<data_management::Tensor> get(InputLayerDataId id) const
    {
        return services::staticPointerCast<data_management::Tensor, data_management::SerializationIface>(Argument::get(id));
    }

    /**
     * Sets input for the forward batch normalization layer
     * \param[in] id    Identifier of the input object
     * \param[in] ptr   Input object to set
     */
    void set(InputLayerDataId id, const services::SharedPtr<data_management::Tensor> &ptr)
    {
        Argument::set(id, ptr);
    }

    /**
     * Checks input object of the forward batch normalization layer
     * \param[in] parameter %Parameter of layer
     * \param[in] method    Computation method of the layer
     */
    void check(const daal::algorithms::Parameter *parameter, int method) const DAAL_C11_OVERRIDE
    {
        const Parameter *algParameter = static_cast<const Parameter *>(parameter);
        size_t dimension = algParameter->dimension;
        services::SharedPtr<data_management::Tensor> dataTensor = get(layers::forward::data);

        services::SharedPtr<services::Error> error = checkTensor(dataTensor, "data in Input");
        if (error) { this->_errors->add(error); return; }

        size_t dimensionSize = dataTensor->getDimensionSize(dimension);
        services::Collection<size_t> weightDims(1);
        weightDims[0] = dimensionSize;

        if (get(layers::forward::weights))
        {
            error = checkTensor(get(layers::forward::weights), "weights in Input", &weightDims);
            if (error) { this->_errors->add(error); return; }
        }

        if (get(layers::forward::biases))
        {
            error = checkTensor(get(layers::forward::biases), "biases in Input", &weightDims);
            if (error) { this->_errors->add(error); return; }
        }

        error = checkTensor(get(populationMean), "populationMean in Input", &weightDims);
        if (error) { this->_errors->add(error); return; }

        error = checkTensor(get(populationVariance), "populationVariance in Input", &weightDims);
        if (error) { this->_errors->add(error); return; }
    }
};

/**
 * \brief Provides methods to access the result obtained with the compute() method
 *        of the forward batch normalization layer
 */
class Result : public layers::forward::Result
{
public:
    /** Default Constructor */
    Result() {}
    virtual ~Result() {}

    using layers::forward::Result::get;
    using layers::forward::Result::set;

    /**
     * Returns dimensions of value tensor
     * \return Dimensions of value tensor
     */
    virtual const services::Collection<size_t> getValueSize(const services::Collection<size_t> &inputSize,
                                                            const daal::algorithms::Parameter *par, const int method) const DAAL_C11_OVERRIDE
    {
        return inputSize;
    }

    /**
     * Allocates memory to store the result of the forward batch normalization layer
     * \param[in] input Pointer to an object containing the input data
     * \param[in] parameter %Parameter of the forward batch normalization layer
     * \param[in] method Computation method for the layer
     */
    template <typename algorithmFPType>
    void allocate(const daal::algorithms::Input *input, const daal::algorithms::Parameter *parameter, const int method)
    {
        const Input *in = static_cast<const Input *>(input);

        Argument::set(layers::forward::value, services::SharedPtr<data_management::SerializationIface>(
                          new data_management::HomogenTensor<algorithmFPType>(in->get(layers::forward::data)->getDimensions(),
                                                                              data_management::Tensor::doAllocate)));
        Argument::set(layers::forward::resultForBackward, services::SharedPtr<data_management::SerializationIface>(
                          new LayerData()));

        allocateLayerData<algorithmFPType>(input, parameter, method);
    }

    /**
      * Allocates memory for storing final results of the batch normalization layer
      * \param[in] input     Pointer to the structure with result objects
      * \param[in] parameter Pointer to the structure of layer parameters
      * \param[in] method    Computation method
      */
    template <typename algorithmFPType>
    void allocateLayerData(const daal::algorithms::Input *input, const daal::algorithms::Parameter *parameter, const int method)
    {
        const Input *in = static_cast<const Input *>(input);
        const Parameter *algParameter = static_cast<const Parameter *>(parameter);

        size_t dimension = algParameter->dimension;
        size_t dimensionSize = in->get(layers::forward::data)->getDimensionSize(dimension);
        services::Collection<size_t> auxDims(1);
        auxDims[0] = dimensionSize;
        set(auxData,    in->get(layers::forward::data));
        set(auxWeights, in->get(layers::forward::weights));
        set(auxMean, services::SharedPtr<data_management::Tensor>(
                new data_management::HomogenTensor<algorithmFPType>(auxDims, data_management::Tensor::doAllocate)));
        set(auxStandardDeviation, services::SharedPtr<data_management::Tensor>(
                new data_management::HomogenTensor<algorithmFPType>(auxDims, data_management::Tensor::doAllocate)));
        set(auxPopulationMean, services::SharedPtr<data_management::Tensor>(
                new data_management::HomogenTensor<algorithmFPType>(auxDims, data_management::Tensor::doAllocate)));
        set(auxPopulationVariance, services::SharedPtr<data_management::Tensor>(
                new data_management::HomogenTensor<algorithmFPType>(auxDims, data_management::Tensor::doAllocate)));
    }

    /**
     * Returns the result of the forward batch normalization layer
     * \param[in] id    Identifier of the result
     * \return          Result that corresponds to the given identifier
     */
    services::SharedPtr<data_management::Tensor> get(LayerDataId id) const
    {
        services::SharedPtr<layers::LayerData> layerData = get(layers::forward::resultForBackward);
        return services::staticPointerCast<data_management::Tensor, data_management::SerializationIface>((*layerData)[id]);
    }

    /**
     * Sets the result of the forward batch normalization layer
     * \param[in] id    Identifier of the result
     * \param[in] ptr   Result
     */
    void set(LayerDataId id, const services::SharedPtr<data_management::Tensor> &ptr)
    {
        services::SharedPtr<layers::LayerData> layerData = get(layers::forward::resultForBackward);
        (*layerData)[id] = ptr;
    }

    /**
     * Checks the result of the forward batch normalization layer
     * \param[in] input     %Input of the layer
     * \param[in] parameter %Parameter of the layer
     * \param[in] method    Computation method of the layer
     */
    void check(const daal::algorithms::Input *input, const daal::algorithms::Parameter *parameter, int method) const DAAL_C11_OVERRIDE
    {
        const Input *algInput = static_cast<const Input *>(input);
        const Parameter *algParameter = static_cast<const Parameter *>(parameter);
        size_t dimension = algParameter->dimension;

        services::SharedPtr<data_management::Tensor> dataTensor = algInput->get(layers::forward::data);
        const services::Collection<size_t> &dataDims = dataTensor->getDimensions();

        services::SharedPtr<data_management::Tensor> valueTensor = get(layers::forward::value);
        services::SharedPtr<services::Error> error = checkTensor(valueTensor, "value", &dataDims);
        if (error) { this->_errors->add(error); return; }

        size_t dimensionSize = valueTensor->getDimensionSize(dimension);
        services::Collection<size_t> auxDims(1);
        auxDims[0] = dimensionSize;

        services::SharedPtr<LayerData> layerData = get(layers::forward::resultForBackward);
        if (!layerData) { this->_errors->add(services::ErrorNullLayerData); return; }

        if (layerData->size() != 6)
            if (!layerData) { this->_errors->add(services::ErrorIncorrectSizeOfLayerData); return; }

        error = checkTensor(get(auxData), "auxData in Result", &dataDims);
        if (error) { this->_errors->add(error); return; }

        error = checkTensor(get(auxWeights), "auxWeights in Result", &auxDims);
        if (error) { this->_errors->add(error); return; }

        error = checkTensor(get(auxMean), "auxMean in Result", &auxDims);
        if (error) { this->_errors->add(error); return; }

        error = checkTensor(get(auxStandardDeviation), "auxStandardDeviation in Result", &auxDims);
        if (error) { this->_errors->add(error); return; }

        error = checkTensor(get(auxPopulationMean), "auxPopulationMean in Result", &auxDims);
        if (error) { this->_errors->add(error); return; }

        error = checkTensor(get(auxPopulationVariance), "auxPopulationVariance in Result", &auxDims);
        if (error) { this->_errors->add(error); return; }
    }

    /**
     * Returns the serialization tag of the forward batch normalization layer result
     * \return     Serialization tag of the forward batch normalization layer result
     */
    int getSerializationTag() { return SERIALIZATION_NEURAL_NETWORKS_LAYERS_BATCH_NORMALIZATION_FORWARD_RESULT_ID; }

    /**
     * Serializes the object
     * \param[in]  arch  Storage for the serialized object or data structure
     */
    void serializeImpl(data_management::InputDataArchive  *arch) DAAL_C11_OVERRIDE
    {serialImpl<data_management::InputDataArchive, false>(arch);}

    /**
     * Deserializes the object
     * \param[in]  arch  Storage for the deserialized object or data structure
     */
    void deserializeImpl(data_management::OutputDataArchive *arch) DAAL_C11_OVERRIDE
    {serialImpl<data_management::OutputDataArchive, true>(arch);}

protected:
    /** \private */
    template<typename Archive, bool onDeserialize>
    void serialImpl(Archive *arch)
    {
        daal::algorithms::Result::serialImpl<Archive, onDeserialize>(arch);
    }
};
} // namespace interface1
using interface1::Input;
using interface1::Result;

} // namespace forward
} // namespace batch_normalization
} // namespace layers
} // namespace neural_networks
} // namespace algorithm
} // namespace daal

#endif
