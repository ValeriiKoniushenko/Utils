## Common concepts

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Intro

Quite simple thing: just wrapper for common practacies in your code which you can use.
Next concepts already implemented instead of you:

- ```Utils::IsArithmetic```
- ```Utils::IsFloating```
- ```Utils::IsIntegral```

### Requirements

#### CMake

Needed target for you is: ```Utils::Utils```
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Utils)```

#### C++ side

Include it to your file: ```#include "Utils/Concepts.h"```