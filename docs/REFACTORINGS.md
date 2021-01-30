## Refactorings

### Core Guidelines Used
* [ES.21](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines.html#es21-dont-introduce-a-variable-or-constant-before-you-need-to-use-it)
* [ES.22](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es22-dont-declare-a-variable-until-you-have-a-value-to-initialize-it-with)
* [ES.30](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es30-dont-use-macros-for-program-text-manipulation)
* [CON.4](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#con4-use-const-to-define-objects-with-values-that-do-not-change-after-construction)

// TODO see if there is a rule about avoiding mutation when possible or being more declarative
//      or simplifying?? to capture what is happening here:
```cpp
array z = refactorme_num(k);
z       = !zero_or_one(k) ? z : zeroionei(k);
return z;

// to 
    
return !zero_or_one(k) ? refactorme_num(k) : zeroionei(k);
```

|Rule|Core Guideline|Count|
|:-:|:-:|:-:|
|Decl-Init Split|ES.22|1|
|Use `const`|CON.4|1|
|Reduce scope|ES.21|2|
|No Text Macros|ES.30|4|

### Refactoring Translations

|From|To|
|:-:|:-:|
|`A`|`array`|
|`SETIC`*|`item_count()`|
|`REPSGN`|`replicate_sign`|
|`AV`|`pointer_to_ravel`|
|`IAV`|`pointer_to_ravel`|

* There could be exceptions where this doesn't work