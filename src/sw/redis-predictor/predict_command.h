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

#ifndef SEWENEW_REDISPREDICTOR_PREDICT_COMMAND_H
#define SEWENEW_REDISPREDICTOR_PREDICT_COMMAND_H

#include <unordered_map>
#include "sw/redis-predictor/module_api.h"
#include "sw/redis-predictor/model.h"

namespace sw::redis::predictor {

// command: PREDICTOR.PREDICT key fea-idx fea-value [fea-idx, fea-value, ...]
// return:  If model does not exist, return nil reply. Otherwise, return the
//          predict result, i.e. a double reply
// error:   If the type doesn't match, or failed to predict, return an error reply.
class PredictCommand {
public:
    int run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const;

private:
    struct Args {
        RedisModuleString *key_name;

        std::unordered_map<int, float> features;
    };

    Model& _get_model(RedisModuleKey &key) const;

    Args _parse_args(RedisModuleString **argv, int argc) const;
};

}

#endif // end SEWENEW_REDISPREDICTOR_PREDICT_COMMAND_H
