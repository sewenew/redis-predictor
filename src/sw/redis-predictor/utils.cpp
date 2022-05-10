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

#include "sw/redis-predictor/utils.h"
#include <algorithm>
#include <fstream>
#include <cctype>

namespace sw::redis::predictor::util {

bool str_case_equal(const std::string_view &s1, const std::string_view &s2) {
    if (s1.size() != s2.size()) {
        return false;
    }

    return std::equal(s1.data(), s1.data() + s1.size(),
            s2.data(), s2.data() + s2.size(),
            [](unsigned char lhs, unsigned char rhs) {
                return std::tolower(lhs) == std::tolower(rhs);
            });
}

std::string load_file(const std::string &uri) {
    const std::string FILE_PREFIX("file://");
    if (uri.size() > FILE_PREFIX.size() &&
            std::string_view(uri.data(), FILE_PREFIX.size()) == FILE_PREFIX) {
        std::ifstream file(uri.substr(FILE_PREFIX.size()));
        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        std::string buffer;
        buffer.resize(size);
        file.seekg(0);
        file.read(buffer.data(), size);

        return buffer;
    }

    throw Error("unknown uri scheme: " + uri);
}

}
