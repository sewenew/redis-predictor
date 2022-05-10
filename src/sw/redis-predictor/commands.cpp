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

#include "sw/redis-predictor/commands.h"
#include "sw/redis-predictor/errors.h"
#include "sw/redis-predictor/create_command.h"
#include "sw/redis-predictor/predict_command.h"

namespace sw::redis::predictor::cmd {

void create_commands(RedisModuleCtx *ctx) {
    if (RedisModule_CreateCommand(ctx,
                "PREDICTOR.CREATE",
                [](RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
                    CreateCommand cmd;
                    return cmd.run(ctx, argv, argc);
                },
                "write deny-oom",
                1,
                1,
                1) == REDISMODULE_ERR) {
        throw Error("fail to create PREDICTOR.CREATE command");
    }

    if (RedisModule_CreateCommand(ctx,
                "PREDICTOR.PREDICT",
                [](RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
                    PredictCommand cmd;
                    return cmd.run(ctx, argv, argc);
                },
                "readonly",
                1,
                1,
                1) == REDISMODULE_ERR) {
        throw Error("failed to create PREDICTOR.PREDICT command");
    }
}

}
