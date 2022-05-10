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

#include "sw/redis-predictor/xgboost_model.h"
#include <fstream>

namespace sw::redis::predictor {

std::string XGBoostModel::dump() const {
    try {
        nlohmann::json conf;
        conf["learner"]["learner_model_param"]["base_score"] = std::to_string(_base_score);
        auto &trees = conf["learner"]["gradient_booster"]["model"]["trees"];
        for (const auto &tree : _trees) {
            trees.push_back(tree.dump());
        }

        return conf.dump();
    } catch (const std::exception &e) {
        throw Error(std::string("failed to dump model: ") + e.what());
    }

    throw Error("should not be here");
}

void XGBoostModel::load(const std::string &conf) {
    try {
        auto config = nlohmann::json::parse(conf);
        auto base_score = std::stof(
                config["learner"]["learner_model_param"]["base_score"].get<std::string>());
        const auto &trees_conf = config["learner"]["gradient_booster"]["model"]["trees"];
        std::vector<Tree> trees;
        for (const auto &ele : trees_conf) {
            Tree tree;
            tree.load(ele);
            trees.push_back(std::move(tree));
        }
        _trees.swap(trees);
        _base_score = base_score;
    } catch (const std::exception &e) {
        throw Error(std::string("failed to load xgboost model: ") + e.what());
    }
}

float XGBoostModel::predict(const std::unordered_map<int, float> &features) const {
    auto score = _base_score;
    for (const auto &tree : _trees) {
        score += tree.predict(features);
    }
    return score;
}

}
