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

#ifndef SEWENEW_REDISPREDICTOR_MODEL_H
#define SEWENEW_REDISPREDICTOR_MODEL_H

#include <memory>
#include <string>
#include <unordered_map>

namespace sw::redis::predictor {

class Model {
public:
    explicit Model(const std::string &type) : _type(type) {}

    virtual ~Model() = default;

    virtual std::string dump() const = 0;

    virtual void load(const std::string &model_file) = 0;

    virtual float predict(const std::unordered_map<int, float> &features) const = 0;

    const std::string& type() const {
        return _type;
    }

private:
    std::string _type;
};

using ModelUPtr = std::unique_ptr<Model>;

}

#endif // end SEWENEW_REDISPREDICTOR_MODEL_H
