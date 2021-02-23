## Refactorings

### Core Guidelines Used
* [Con.4: Use const to define objects with values that do not change after construction](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#con4-use-const-to-define-objects-with-values-that-do-not-change-after-construction)
* [ES.21: Don’t introduce a variable (or constant) before you need to use it](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines.html#es21-dont-introduce-a-variable-or-constant-before-you-need-to-use-it)
* [ES.22: Don’t declare a variable until you have a value to initialize it with](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es22-dont-declare-a-variable-until-you-have-a-value-to-initialize-it-with)
* [ES.30: Don’t use macros for program text manipulation](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es30-dont-use-macros-for-program-text-manipulation)
* [ES.49: If you must use a cast, use a named cast](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es49-if-you-must-use-a-cast-use-a-named-cast)
* [E.12: Use noexcept when exiting a function because of a throw is impossible or unacceptable](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#e12-use-noexcept-when-exiting-a-function-because-of-a-throw-is-impossible-or-unacceptable)

| Rule  |   Core Guideline   | Count |
| :---: | :----------------: | :---: |
| CON.4 |    Use `const`     |   4   |
| ES.21 |    Reduce scope    |   3   |
| ES.22 |  Decl-Init Split   |   5   |
| ES.30 |   No Text Macros   |   6   |
| ES.49 | Use explicit casts |   4   |
| E.12  |   Use `noexcept`   |   3   |

### Refactoring Translations

|         From          |                         To                          |
| :-------------------: | :-------------------------------------------------: |
|          `A`          |                       `array`                       |
|       `SETIC` 1       |                   `item_count()`                    |
|       `REPSGN`        |                 `replicate_sign` 2                  |
|         `AV`          |                 `pointer_to_values`                 |
|         `IAV`         |                 `pointer_to_values`                 |
|    `IAV(w)[i] = k`    |               `set_value_at(w, i, k)`               |
|       `k & ~1`        |                  `!zero_or_one(k)`                  |
| `GAT0(z, TYPE, x, y)` |   `z = make_array<TYPE, copy_shape_0>(jt, x, y)`    |
|       `GA(...)`       |                    `make_array`                     |
|         `sc`          |                `make_scalar_integer`                |
|      `DO(m, s)`       | `for (int i = 0; i < m; ++i) m` OR `std::algorithm` |

1. There could be exceptions where this doesn't work
2. Will probably be renamed
