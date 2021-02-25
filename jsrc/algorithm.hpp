#include <algorithm>
#include <array>
#include <tuple>

namespace algo {

/**
 * @brief Variadic version of `std::equal_to`
 */
static auto constexpr is_equal_to = [](const auto &first, const auto &...values) -> bool {
    static_assert(sizeof...(values) != 0);
    return ((first == values) && ...);
};

/**
 * @brief Variadic version of `std::not_equal_to`
 */
static auto constexpr is_not_equal_to = [](const auto &first, const auto &...values) -> bool {
    return !is_equal_to(first, values...);
};

/**
 * @brief Variadic version of `std::mismatch`
 */
template <typename P, typename I, typename... Is>
static auto constexpr zip_find(const P &pred, I f, I l, Is... fs) {
    while (f != l) {
        if (pred(*f, *fs...)) break;
        ++f, ((void)++fs, ...);
    }
    return std::tuple{f, fs...};
}

/**
 * @brief Variadic version of `std::mismatch`, for ranges.
 * @note This can be the same name as zip_find if we use concepts.
 */
template <typename P, typename R, typename... Rs>
static auto constexpr zip_find_r(const P &pred, R &r, Rs &...rs) {
    return zip_find(pred, std::begin(r), std::end(r), std::begin(rs)...);
}

/**
 * @brief Predicate version of `algo::zip_found`
 */
template <typename P, typename I, typename... Is>
static auto constexpr zip_found(const P &pred, I f, I l, Is... fs) -> bool {
    auto const t = zip_find(pred, f, l, fs...);
    return std::get<0>(t) != l;
}

/**
 * @brief Variadic version of `algo::zip_found`, for ranges.
 * @note This can be the same name as zip_found if we use concepts.
 */
template <typename P, typename R, typename... Rs>
static auto constexpr zip_found_r(const P &pred, R &r, Rs &...rs) -> bool {
    return zip_found(pred, std::begin(r), std::end(r), std::begin(rs)...);
}

/**
 * @brief Predicate version of `std::mismatch`
 * @note std::mismatch isn't constexpr till cpp20
 */
template <typename I, typename I2>
static auto
is_mismatched(I f, I l, I2 f2) -> bool {
    return std::mismatch(f, l, f2).first != l;
}

/**
 * @brief Variadic, Predicate version of `std::mismatch`
 */
template <typename I, typename... Is>
static constexpr auto
is_mismatched_v(I f, I l, Is... fs) -> bool {
    return zip_found(is_not_equal_to, f, l, fs...);
}

/**
 * @brief Variadic, Predicate version of `std::mismatch`, for ranges;
 */
template <typename R, typename... Rs>
static constexpr auto
is_mismatched_r(R &r, Rs &...rs) -> bool {
    return is_not_equal_to(std::size(r), std::size(rs)...) ||
           is_mismatched_v(std::begin(r), std::end(r), std::begin(rs)...);
}

/**
 * @brief Variadic, Predicate version of `std::equal`
 */
template <typename I, typename... Is>
static constexpr auto
is_equal(I f, I l, Is... fs) -> bool {
    return !is_mismatched_v(f, l, fs...);
}

/**
 * @brief Variadic, Predicate version of `std::equal`, for ranges;
 */
template <typename... Rs>
static constexpr auto
is_equal_r(Rs &...rs) -> bool {
    return !is_mismatched_r(rs...);
}

// static_assert tests. With constexpr we can start checking to make sure our functions work at compile time.
static_assert(!is_equal_to(1, 2, 3, 4));
static_assert(is_equal_to(1, 1, 1, 1));
static_assert(is_not_equal_to(1, 2, 3, 4));
static_assert(!is_not_equal_to(1, 1, 1, 1));
static_assert(
  !zip_found_r(is_not_equal_to, std::array{0, 1, 2, 3, 4}, std::array{0, 1, 2, 3, 4}, std::array{0, 1, 2, 3, 4}));
static_assert(
  zip_found_r(is_not_equal_to, std::array{0, 1, 2, 5, 4}, std::array{0, 1, 2, 3, 4}, std::array{0, 1, 2, 3, 6}));
static_assert(is_mismatched_r(std::array{0, 1, 2, 5, 4}, std::array{0, 1, 2, 3, 4}, std::array{0, 1, 2, 3, 6}));
static_assert(is_mismatched_r(std::array{0, 1, 2, 5, 4}, std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3, 6}));
static_assert(!is_mismatched_r(std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}));
static_assert(is_equal_r(std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}, std::array{0, 1, 2, 3}));

}  // namespace algo
