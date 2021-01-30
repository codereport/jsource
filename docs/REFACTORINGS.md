### Refactorings

#### Core Guidelines Used
[ES.21](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines.html#es21-dont-introduce-a-variable-or-constant-before-you-need-to-use-it)
[ES.22](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es22-dont-declare-a-variable-until-you-have-a-value-to-initialize-it-with)
[CON.4](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#con4-use-const-to-define-objects-with-values-that-do-not-change-after-construction)

|Rule|Core Guideline|Count|
|:-:|:-:|:-:|
|Decl-Init Split|ES.22|1|
|Use `const`|CON.4|1|
|Reduce scope|ES.21|1|