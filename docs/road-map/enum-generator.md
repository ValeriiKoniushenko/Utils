## Enum-generator

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Intro

Do you want to convert an enum's constant to string or vise versa? Or you want to
know home many constants in your enum? I think it's not a problem. Use compile-time
wrapper for it without loosing of optimization privelegious.

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
cout << (color == Color::fromStr("Blue")) << endl;
cout << (Color::Green == Color::fromStr("Green")) << endl;
cout << (1 == Color::fromStr("Green").cast()) << endl;

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
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include: ```#include "Core/Enum.h"```