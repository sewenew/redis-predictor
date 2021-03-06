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

#ifndef SEWENEW_REDISPREDICTOR_MODULE_API_H
#define SEWENEW_REDISPREDICTOR_MODULE_API_H

#ifdef __cplusplus

extern "C" {

#endif

#include "sw/redis-predictor/redismodule.h"

#ifdef __cplusplus

}

#endif

#include <memory>
#include <string>
#include <string_view>
#include "sw/redis-predictor/errors.h"

namespace sw::redis::predictor::api {

template <typename ...Args>
void warning(RedisModuleCtx *ctx, const char *fmt, Args &&...args) {
    RedisModule_Log(ctx, "warning", fmt, std::forward<Args>(args)...);
}

template <typename ...Args>
void notice(RedisModuleCtx *ctx, const char *fmt, Args &&...args) {
    RedisModule_Log(ctx, "notice", fmt, std::forward<Args>(args)...);
}

template <typename ...Args>
void debug(RedisModuleCtx *ctx, const char *fmt, Args &&...args) {
    RedisModule_Log(ctx, "debug", fmt, std::forward<Args>(args)...);
}

template <typename ...Args>
void verbose(RedisModuleCtx *ctx, const char *fmt, Args &&...args) {
    RedisModule_Log(ctx, "verbose", fmt, std::forward<Args>(args)...);
}

struct RedisKeyCloser {
    void operator()(RedisModuleKey *key) const {
        RedisModule_CloseKey(key);
    }
};

using RedisKey = std::unique_ptr<RedisModuleKey, RedisKeyCloser>;

enum class KeyMode {
    READONLY,
    WRITEONLY,
    READWRITE
};

RedisKey open_key(RedisModuleCtx *ctx, RedisModuleString *name, KeyMode mode);

// If key doesn't exist return false.
// If key type is NOT *key_type*, throw WrongTypeError.
// Otherwise, return true.
bool key_exists(RedisModuleKey *key, RedisModuleType *key_type);

int reply_with_error(RedisModuleCtx *ctx, const Error &err);

std::string_view to_string_view(RedisModuleString *str);

std::string to_string(RedisModuleString *str);

long long to_long_long(RedisModuleString *str);

double to_double(RedisModuleString *str);

}

#endif // end SEWENEW_REDISPREDICTOR_MODULE_API_H
