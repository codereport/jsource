#include <algorithm>
#include <array>
#include <tuple>

namespace algo {

/**
 * @brief Variadic version of `std::equal_to`
 */
static auto constexpr equal_to = [](auto const &first, auto const &...values) -> bool {
    static_assert(sizeof...(values) != 0);
    return ((first == values) && ...);
};

/**
 * @brief Variadic version of `std::not_equal_to`
 */
static auto constexpr not_equal_to = [](auto const &first, auto const &...values) -> bool {
    return !equal_to(first, values...);
};

/**
 * @brief Variadic version of `std::mismatch`
 */
template <typename P, typename I, typename... Is>
[[nodiscard]] static constexpr auto
zip_find(P const &pred, I f, I const l, Is... fs) {
    static_assert(sizeof...(Is) != 0);
    while (f != l) {
        if (pred(*f, *fs...)) break;
        ++f, ((void)++fs, ...);
    }
    return std::tuple{f, fs...};
}

/**
 * @brief Predicate version of `algo::zip_found`
 */
template <typename P, typename I, typename... Is>
[[nodiscard]] static constexpr auto
zip_found(P const &pred, I f, I const l, Is... fs) -> bool {
    auto const t = zip_find(pred, f, l, fs...);
    return std::get<0>(t) != l;
}


/**
 * @brief Variadic, Predicate version of `std::mismatch`
 */
template <typename I, typename... Is>
[[nodiscard]] static constexpr auto
is_mismatched(I f, I const l, Is... fs) -> bool {
    return zip_found(not_equal_to, f, l, fs...);
}

// // This 2 arg function takes priority so will not work when called from constexpr functions. This isn't an issue in
// cpp20.
// /**
//  * @brief Predicate version of `std::equal`
//  * @note std::equal isn't constexpr till cpp20
//  */
// template <typename I, typename I2>
// [[nodiscard]] static auto
// is_equal(I f, I const l, I2 f2) -> bool {
//     return std::equal(f, l, f2);
// }

/**
 * @brief Variadic, Predicate version of `std::equal`
 */
template <typename I, typename... Is>
[[nodiscard]] static constexpr auto
is_equal(I f, I const l, Is... fs) -> bool {
    return !is_mismatched(f, l, fs...);
}

namespace ranges {

/**
 * @brief Variadic version of `std::mismatch`, for ranges.
 * @note This can be the same name as zip_find if we use concepts.
 */
template <typename P, typename R, typename... Rs>
[[nodiscard]] static constexpr auto
zip_find(P const &pred, R const &r, Rs const &...rs) {
    return algo::zip_find(pred, std::cbegin(r), std::cend(r), std::cbegin(rs)...);
}

/**
 * @brief Variadic version of `algo::zip_found`, for ranges.
 * @note This can be the same name as zip_found if we use concepts.
 */
template <typename P, typename R, typename... Rs>
[[nodiscard]] static constexpr auto
zip_found(P const &pred, R const &r, Rs const &...rs) -> bool {
    return algo::zip_found(pred, std::cbegin(r), std::cend(r), std::cbegin(rs)...);
}

/**
 * @brief Variadic, Predicate version of `std::mismatch`, for ranges;
 */
template <typename R, typename... Rs>
[[nodiscard]] static constexpr auto
is_mismatched(R const &r, Rs const &...rs) -> bool {
    return not_equal_to(std::size(r), std::size(rs)...) ||
           algo::is_mismatched(std::cbegin(r), std::cend(r), std::cbegin(rs)...);
}

/**
 * @brief Variadic, Predicate version of `std::equal`, for ranges;
 */
template <typename... Rs>
[[nodiscard]] static constexpr auto
is_equal(Rs const &...rs) -> bool {
    return !is_mismatched(rs...);
}

// static_assert tests. With constexpr we can start checking to make sure our functions work at compile time.
static_assert(!equal_to(1, 2, 3, 4));
static_assert(equal_to(1, 1, 1, 1));
static_assert(not_equal_to(1, 2, 3, 4));
static_assert(!not_equal_to(1, 1, 1, 1));
static_assert(
  !ranges::zip_found(not_equal_to, std::array{0, 1, 2, 3, 4}, std::array{0, 1, 2, 3, 4}, std::array{0, 1, 2, 3, 4}));
static_assert(
  ranges::zip_found(not_equal_to, std::array{0, 1, 2, 5, 4}, std::array{0, 1, 2, 3, 4}, std::array{0, 1, 2, 3, 6}));
static_assert(is_mismatched(std::array{0, 1, 2, 5, 4}, std::array{0, 1, 2, 3, 4}, std::array{0, 1, 2, 3, 6}));
static_assert(is_mismatched(std::array{0, 1, 2, 5, 4}, std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3, 6}));
static_assert(!is_mismatched(std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}));
static_assert(is_equal(std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}));
static_assert(ranges::is_equal(std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}));
}  // namespace ranges
}  // namespace algo
