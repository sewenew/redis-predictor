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

#ifndef SEWENEW_REDISPREDICTOR_REGRESSION_TREE_H
#define SEWENEW_REDISPREDICTOR_REGRESSION_TREE_H

#include <cassert>
#include <vector>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>

namespace sw::redis::predictor {

template <typename FeaIndex, typename FeaValue>
struct Node {
    FeaIndex feature;
    FeaValue value;
    bool default_left;
    int left;
    int right;
};

template <typename FeaIndex = int, typename FeaValue = float>
class RegressionTree {
public:
    RegressionTree() = default;

    void load(const nlohmann::json &conf);

    FeaValue predict(const std::unordered_map<FeaIndex, FeaValue> &features) const;

    nlohmann::json dump() const;

private:
    using TreeNode = Node<FeaIndex, FeaValue>;

    bool _is_leaf(const TreeNode &node) const {
        return node.left == -1;
    }

    std::vector<TreeNode> _tree;
};

}

#include "sw/redis-predictor/regression_tree.hpp"

#endif // end SEWENEW_REDISPREDICTOR_REGRESSION_TREE_H
