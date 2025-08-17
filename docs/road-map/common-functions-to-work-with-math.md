## Common functions to work with Math

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Intro

Usually functions for you, but in ```compile-time```!

- ```Math::Abs``` - for working with any arithmetical type
- ```Math::IsEqual``` - comparing of the float or double
- ```Math::IsZero``` - checking for 0.0 for types with floating point

### Quick example

```c++
#include "Core/Math.h"

using namespace Math;
using std::cout;
using std::endl;

if (IsZero(0.0001f))
    cout << "YES #1" << endl;
else
    cout << "NO #1" << endl;


if (IsZero<float, 0.01f>(0.0001f))
    cout << "YES #2" << endl;
else
    cout << "NO #2" << endl;
    
// Output:
// > YES #1
// > NO #2
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include it to your file: ```#include "Core/Math.h"```