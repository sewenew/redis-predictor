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

#ifndef SEWENEW_REDISPREDICTOR_REGRESSION_TREE_HPP
#define SEWENEW_REDISPREDICTOR_REGRESSION_TREE_HPP

#include "sw/redis-predictor/errors.h"

namespace sw::redis::predictor {

template <typename FeaIndex, typename FeaValue>
void RegressionTree<FeaIndex, FeaValue>::load(const nlohmann::json &conf) {
    auto size = static_cast<std::size_t>(
            std::stol(conf.at("tree_param").at("num_nodes").get<std::string>()));
    if (size == 0) {
        throw Error("Invalid tree");
    }
    std::vector<TreeNode> tree;
    tree.resize(size);
    const auto &left_children = conf.at("left_children");
    const auto &right_children = conf.at("right_children");
    const auto &default_left = conf.at("default_left");
    const auto &split_conditions = conf.at("split_conditions");
    const auto &split_indices = conf.at("split_indices");
    if (left_children.size() != size || right_children.size() != size ||
            default_left.size() != size || split_conditions.size() != size ||
            split_indices.size() != size) {
        throw Error("Invalid tree");
    }
    for (auto idx = 0U; idx < size; ++idx) {
        TreeNode node;
        node.feature = split_indices.at(idx).get<FeaIndex>();
        node.value = split_conditions.at(idx).get<FeaValue>();
        node.default_left = default_left.at(idx).get<int>() == 1 ? true : false;
        node.left = left_children.at(idx).get<int>();
        node.right = right_children.at(idx).get<int>();
        tree[idx] = std::move(node);
    }
    _tree.swap(tree);
}

template <typename FeaIndex, typename FeaValue>
FeaValue RegressionTree<FeaIndex, FeaValue>::predict(
        const std::unordered_map<FeaIndex, FeaValue> &features) const {
    assert(_tree.size() > 0);
    const auto *node = &(_tree.front());
    while (!_is_leaf(*node)) {
        auto iter = features.find(node->feature);
        auto child = 0;
        if (iter != features.end()) {
            if (iter->second < node->value) {
                child = node->left;
            } else {
                child = node->right;
            }
        } else {
            // Missing feature
            child = node->default_left ? node->left : node->right;
        }

        assert(static_cast<std::size_t>(child) < _tree.size());

        node = &_tree[child];
    }

    assert(node != nullptr);

    return node->value;
}

template <typename FeaIndex, typename FeaValue>
nlohmann::json RegressionTree<FeaIndex, FeaValue>::dump() const {
    nlohmann::json conf;
    conf["tree_param"]["num_nodes"] = std::to_string(_tree.size());
    std::vector<int> left_children;
    left_children.resize(_tree.size());
    std::vector<int> right_children;
    right_children.resize(_tree.size());
    std::vector<int> default_left;
    default_left.resize(_tree.size());
    std::vector<int> split_indices;
    split_indices.resize(_tree.size());
    std::vector<float> split_conditions;
    split_conditions.resize(_tree.size());
    for (auto idx = 0U; idx < _tree.size(); ++idx) {
        auto &node = _tree[idx];
        split_indices[idx] = node.feature;
        split_conditions[idx] = node.value;
        default_left[idx] = node.default_left ? 1 : 0;
        left_children[idx] = node.left;
        right_children[idx] = node.right;
    }
    conf["left_children"] = left_children;
    conf["right_children"] = right_children;
    conf["default_left"] = default_left;
    conf["split_conditions"] = split_conditions;
    conf["split_indices"] = split_indices;

    return conf;
}

}

#endif // end SEWENEW_REDISPREDICTOR_REGRESSION_TREE_HPP
