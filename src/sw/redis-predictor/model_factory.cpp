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

#include "sw/redis-predictor/model_factory.h"
#include "sw/redis-predictor/errors.h"
#include "sw/redis-predictor/xgboost_model.h"

namespace sw::redis::predictor {

ModelFactory::ModelFactory() {
    _creators.emplace("xgboost", std::make_unique<ModelCreatorTpl<XGBoostModel>>());
}

ModelUPtr ModelFactory::create(const std::string &type) {
    auto iter = _creators.find(type);
    if (iter == _creators.end()) {
        throw Error("unknown model: " +  type);
    }

    return iter->second->create();
}

ModelUPtr ModelFactory::create(const std::string &type, const std::string &conf) {
    auto m = create(type);

    m->load(conf);

    return m;
}

}
