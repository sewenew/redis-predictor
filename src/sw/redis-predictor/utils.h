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

#ifndef SEWENEW_REDISPREDICTOR_UTILS_H
#define SEWENEW_REDISPREDICTOR_UTILS_H

#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include "sw/redis-predictor/module_api.h"

namespace sw::redis::predictor::util {

bool str_case_equal(const std::string_view &s1, const std::string_view &s2);

std::string load_file(const std::string &uri);

}

#endif // end SEWENEW_REDISPREDICTOR_UTILS_H