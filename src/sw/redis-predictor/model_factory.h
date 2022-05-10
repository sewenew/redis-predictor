/**************************************************************************
   Copyright (c) 2022 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#ifndef SEWENEW_REDISPREDICTOR_MODEL_FACTORY_H
#define SEWENEW_REDISPREDICTOR_MODEL_FACTORY_H

#include <string>
#include <unordered_map>
#include "sw/redis-predictor/model.h"

namespace sw::redis::predictor {

class ModelCreator {
public:
    virtual ~ModelCreator() = default;

    virtual ModelUPtr create() const = 0;
};

using ModelCreatorUPtr = std::unique_ptr<ModelCreator>;

template <typename M,
         typename std::enable_if<std::is_base_of_v<Model, M>, int>::type = 0>
class ModelCreatorTpl : public ModelCreator {
public:
    virtual ModelUPtr create() const override {
        return std::make_unique<M>();
    }
};

class ModelFactory {
public:
    ModelFactory();

    ModelFactory(const ModelFactory &) = delete;
    ModelFactory& operator=(const ModelFactory &) = delete;

    ModelFactory(ModelFactory &&) = delete;
    ModelFactory& operator=(ModelFactory &&) = delete;

    ~ModelFactory() = default;

    ModelUPtr create(const std::string &type);

    ModelUPtr create(const std::string &type, const std::string &conf);

private:
    std::unordered_map<std::string, ModelCreatorUPtr> _creators;
};

}

#endif // end SEWENEW_REDISPREDICTOR_MODEL_FACTORY_H
