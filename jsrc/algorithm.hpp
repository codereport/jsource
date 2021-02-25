
#include <tuple>
#include <algorithm>

namespace algo {

/**
 * @brief Variadic version of `std::mismatch`
 */
template <typename P, typename I, typename... Is>
auto zip_find(P pred, I f, I l, Is... fs) {
    while (f != l) {
        if (pred(*f, *fs...)) break;
        ++f, ((void)++fs,...);
    }
    return std::tuple{f, fs...};
}

template <typename P, typename I, typename... Is>
auto zip_found(P pred, I f, I l, Is... fs) {
    auto const t = zip_find(pred, f, l, fs...);
    return std::get<0>(t) != l;
}

template <typename I, typename I2>
auto is_mismatched(I f, I l, I2 f2) {
    return std::mismatch(f, l, f2).first != l;
}

}