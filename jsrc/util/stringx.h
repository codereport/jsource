
#pragma once

#include <string>

namespace util{
    auto remove_all_occurrences(std::string &str, std::string_view sub_str) -> void;
}
