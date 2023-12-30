#pragma once

#include "Lake/Common.h"

#include <tuple>

namespace lake {

    struct PairHash {
        template <typename T1, typename T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };

    struct PairEqual {
        template <typename T1, typename T2>
        bool operator()(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };

} // namespace lake
