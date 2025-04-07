## Working with space coordinates

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Intro

Of course, it has basic set of tools to work with space coordinates.

- ```Core::Rect<ArithmeticType>``` - use it to create you own Rectangle abstraction
- ```Core::GlobalPosition<Len, Type, Precision>``` - the same glm::vec2 or vec3 but better
- ```Core::Size<Dimension, ArithmeticType>``` - better than glm::vecX
- ```Core::LocalPosition<Len, Type, Precision>``` - the friend of the GlobalPosition, but for now - **not implemented**.

*More detailed view & examples you can find here: [link](docs/SpaceCoordinates.md)*

### Quick example

Working with ```Rect```.

```c++
#include "Core/Rect.h"

using namespace Core;
using std::cout;
using std::endl;

FRect rect = { 0.f, 10.f, 10.f, 0.f };

FRect::GlobalPositionT innerPoint = { 3.f, 3.f };
if (rect.isContain(innerPoint))
    cout << "The rect contains this point"

auto corner = rect.getLeftTop();
cout << "LT: " << corner.x << ":" << corner.y;

// Output:
// > The rect contains this point
// > LT: 0:10
```

Working with ```Size```.

```c++
#include "Core/Size.h"

using namespace Core;
using std::cout;
using std::endl;

ISize2 a = { 5, 10 }; // ISize2 == int size 2D
ISize2 b = { 2, 4 };
cout << "Area 'a' = " << a.area() << endl;
cout << "Area 'b' = " << b.area() << endl;

if (a > b)
    cout << "Hello";

// Output:
// > Area 'a' = 50
// > Area 'b' = 8
// > Hello
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

- For ```Rect```: ```#include "Core/Rect.h"```
- For ```GlobalPosition```: ```#include "Core/Position.h"```
- For ```Size```: ```#include "Core/Size.h"```