/*
 * Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * GNU Lesser General Public License v2.1 or any later version.
 */

#ifndef BLOCKFACTORY_CODER_GENERATEDCODEWRAPPER_H
#define BLOCKFACTORY_CODER_GENERATEDCODEWRAPPER_H

#ifndef MODEL
#error "MODEL option not specified"
#endif

#include <iostream>
#include <memory>
#include <string>

namespace blockfactory {
    namespace coder {
        template <typename T>
        class GeneratedCodeWrapper;
    } // namespace coder
} // namespace blockfactory

template <typename T>
class blockfactory::coder::GeneratedCodeWrapper
{
private:
    std::unique_ptr<T> m_model;
    std::string m_modelName;
    unsigned m_numSampleTimes;

    bool modelFailed() const;

public:
    GeneratedCodeWrapper(const std::string& modelName = {}, const unsigned& numSampleTimes = 0);
    ~GeneratedCodeWrapper() = default;

    bool initialize();
    bool step();
    bool terminate();

    //    double* getOutput(const unsigned& index) const;

    std::string getErrors() const;
    //    std::string getWarnings() const;
};

template <typename T>
bool blockfactory::coder::GeneratedCodeWrapper<T>::modelFailed() const
{
    if (m_model) {
        if (m_model->getRTM()) {
            if (!m_model->getRTM()->errorStatus)
                return false;
        }
    }
    return true;
}

template <typename T>
blockfactory::coder::GeneratedCodeWrapper<T>::GeneratedCodeWrapper(const std::string& modelName,
                                                                   const unsigned& numSampleTimes)
    : m_modelName(modelName)
    , m_numSampleTimes(numSampleTimes)
{}

template <typename T>
bool blockfactory::coder::GeneratedCodeWrapper<T>::initialize()
{
    if (m_model) {
        m_model.reset();
    }

    m_model = std::make_unique<T>();
    m_model->initialize();

    if (modelFailed()) {
        return false;
    }

    return true;
}

template <typename T>
bool blockfactory::coder::GeneratedCodeWrapper<T>::step()
{
    if (!m_model) {
        return false;
    }

    m_model->step();

    if (modelFailed()) {
        return false;
    }

    return true;
}

template <typename T>
bool blockfactory::coder::GeneratedCodeWrapper<T>::terminate()
{
    if (!m_model) {
        return false;
    }

    m_model->terminate();

    if (modelFailed()) {
        return false;
    }

    return true;
}

template <typename T>
std::string blockfactory::coder::GeneratedCodeWrapper<T>::getErrors() const
{
    if (!m_model) {
        return {};
    }

    if (modelFailed()) {
        return {m_model->getRTM()->errorStatus};
    }

    return {};
}

#endif // BLOCKFACTORY_CODER_GENERATEDCODEWRAPPER_H
