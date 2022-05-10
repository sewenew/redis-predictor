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

#include "sw/redis-predictor/predict_command.h"
#include "sw/redis-predictor/errors.h"
#include "sw/redis-predictor/redis_predictor.h"

namespace sw::redis::predictor {

int PredictCommand::run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const {
    try {
        assert(ctx != nullptr);

        auto args = _parse_args(argv, argc);

        auto key = api::open_key(ctx, args.key_name, api::KeyMode::READONLY);
        if (!api::key_exists(key.get(), RedisPredictor::instance().type())) {
            RedisModule_ReplyWithNull(ctx);
        } else {
            auto &model = _get_model(*key);
            auto score = model.predict(args.features);

            RedisModule_ReplyWithDouble(ctx, score);
        }

        return REDISMODULE_OK;
    } catch (const WrongArityError &err) {
        return RedisModule_WrongArity(ctx);
    } catch (const Error &err) {
        return api::reply_with_error(ctx, err);
    }

    return REDISMODULE_ERR;
}

Model& PredictCommand::_get_model(RedisModuleKey &key) const {
    auto *model = static_cast<Model *>(RedisModule_ModuleTypeGetValue(&key));
    if (model == nullptr) {
        throw Error("failed to model by key");
    }

    return *model;
}

PredictCommand::Args PredictCommand::_parse_args(RedisModuleString **argv, int argc) const {
    assert(argv != nullptr);

    if (argc < 4 || argc % 2 != 0) {
        throw WrongArityError();
    }

    Args args;
    args.key_name = argv[1];

    for (auto idx = 2; idx < argc; idx += 2) {
        try {
            auto fea_idx = static_cast<int>(api::to_long_long(argv[idx]));
            auto fea_value = static_cast<float>(api::to_double(argv[idx + 1]));
            args.features.emplace(fea_idx, fea_value);
        } catch (const std::exception &) {
            throw Error("invalid feature");
        }
    }

    return args;
}

}
