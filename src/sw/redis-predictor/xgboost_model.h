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

#ifndef SEWENEW_REDISPREDICTOR_XGBOOST_MODEL_H
#define SEWENEW_REDISPREDICTOR_XGBOOST_MODEL_H

#include "sw/redis-predictor/model.h"
#include "sw/redis-predictor/regression_tree.h"

namespace sw::redis::predictor {

class XGBoostModel : public Model {
public:
    XGBoostModel() : Model("xgboost") {}

    virtual std::string dump() const override;

    virtual void load(const std::string &model_file) override;

    virtual float predict(const std::unordered_map<int, float> &features) const override;

private:
    using Tree = RegressionTree<>;
    std::vector<Tree> _trees;

    float _base_score = 0.0;
};

}

#endif // end SEWENEW_REDISPREDICTOR_XGBOOST_MODEL_H
