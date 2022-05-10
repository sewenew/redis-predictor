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

#include "sw/redis-predictor/create_command.h"
#include <fstream>
#include "sw/redis-predictor/errors.h"
#include "sw/redis-predictor/redis_predictor.h"
#include "sw/redis-predictor/utils.h"

namespace sw::redis::predictor {

int CreateCommand::run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const {
    try {
        auto res = _run(ctx, argv, argc);

        RedisModule_ReplyWithLongLong(ctx, res);

        RedisModule_ReplicateVerbatim(ctx);

        return REDISMODULE_OK;
    } catch (const WrongArityError &err) {
        return RedisModule_WrongArity(ctx);
    } catch (const Error &err) {
        return api::reply_with_error(ctx, err);
    }

    return REDISMODULE_ERR;
}

long long CreateCommand::_run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const {
    assert(ctx != nullptr);

    auto args = _parse_args(argv, argc);

    auto key = api::open_key(ctx, args.key_name, api::KeyMode::WRITEONLY);
    assert(key);

    if (api::key_exists(key.get(), RedisPredictor::instance().type()) &&
            args.create_if_not_exist) {
        return 0;
    }

    _create_model(*key, args.framework, args.model_file);

    return 1;
}

CreateCommand::Args CreateCommand::_parse_args(RedisModuleString **argv, int argc) const {
    assert(argv != nullptr);

    if (argc < 3) {
        throw WrongArityError();
    }

    Args args;
    args.key_name = argv[1];

    auto pos = _parse_opts(argv, argc, args);
    if (pos + 2 != argc) {
        throw WrongArityError();
    }

    args.framework = api::to_string(argv[pos]);
    args.model_file = api::to_string_view(argv[pos + 1]);

    return args;
}

int CreateCommand::_parse_opts(RedisModuleString **argv, int argc, Args &args) const {
    auto idx = 2;
    while (idx < argc) {
        auto opt = api::to_string_view(argv[idx]);

        if (util::str_case_equal(opt, "--NX")) {
            args.create_if_not_exist = true;
        } else {
            // Finish parsing options.
            break;
        }

        ++idx;
    }

    return idx;
}

void CreateCommand::_create_model(RedisModuleKey &key,
        const std::string &framework,
        const std::string &model_file) const {
    auto conf = util::load_file(model_file);
    auto &predictor = RedisPredictor::instance();
    auto &factory = predictor.model_factory();
    auto model = factory.create(framework, conf);

    if (RedisModule_ModuleTypeSetValue(&key, predictor.type(), model.get()) != REDISMODULE_OK) {
        throw Error("failed to create model");
    }

    model.release();
}

}
