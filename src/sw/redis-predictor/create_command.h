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

#ifndef SEWENEW_REDISPREDICTOR_CREATE_COMMAND_H
#define SEWENEW_REDISPREDICTOR_CREATE_COMMAND_H

#include <string_view>
#include "sw/redis-predictor/module_api.h"

namespace sw::redis::predictor {

// command: PREDICTOR.CREATE key --NX framwork model-file
// return:  Integer reply: 1 if create successfully. 0, otherwise, e.g. option --NX
//          has been set, while key already exists.
// error:   If the type doesn't match, or failed to create model,
//          return an error reply.
class CreateCommand {
public:
    int run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const;

private:
    struct Args {
        RedisModuleString *key_name;

        bool create_if_not_exist = false;

        std::string framework;

        std::string model_file;
    };

    long long _run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const;

    Args _parse_args(RedisModuleString **argv, int argc) const;

    // Return the position of the first non-option argument.
    int _parse_opts(RedisModuleString **argv, int argc, Args &args) const;

    void _create_model(RedisModuleKey &key,
            const std::string &framework,
            const std::string &model_file) const;
};

}

#endif // end SEWENEW_REDISPREDICTOR_CREATE_COMMAND_H
