## Enum-generator

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Intro

Do you want to convert an enum constant to a string or vice versa?
Or do you want to know how many constants are in your enum? That’s easy. Use a compile-time wrapper
for this without losing any optimization benefits.

However, there’s a small but important caveat due to C++ compiler and standard rules: if you try
to assign a non-numeric value to an enum constant, you won’t be able to cast it to a string - or
from a string - in the future.

### Quick example

```c++
#include "Core/Enum.h"

using std::cout;
using std::endl;

CreateEnum(
    Color, int, 
    Red, 
    Green, 
    Blue
);

Color color = Color::Red;
cout << color.toStr() << endl;
cout << color.cast() << endl;

color = Color::Blue;
cout << color.toStr() << endl;
cout << color.cast() << endl;
cout << color.cast() << endl;
cout << (color.cast() == Color::fromStr("Blue")) << endl;
cout << (Color::Green == Color::fromStr("Green")) << endl;
cout << (1 == Color::fromStr("Green")) << endl;

// Output:
// > Red
// > 0
// > Blue
// > 2
// > 2
// > true
// > true
// > true
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include: ```#include "Core/Enum.h"```