## Common concepts

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Intro

A simple wrapper for common practices in your code that you can use.
The following concepts are already implemented for you:

- ```Utils::IsArithmetic```
- ```Utils::IsFloating```
- ```Utils::IsIntegral```

### Requirements

#### CMake

Needed target for you is: ```Utils::Utils```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Utils)```

#### C++ side

Include it to your file: ```#include "Utils/Concepts.h"```