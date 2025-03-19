# Utils

The utilitary library for common solutions in your code. It has some count of already implemented things as:
- Common concepts(from C++20)
- Common classes for working with a space coordinates
- Common functions to work with Math
- Common interfaces\patterns
- Enum-generator
- Atomic string for working with compile-time and dynamic strings
- Delegates
- Run-time asserts

___

# Getting started

## Requirements
Needed settings\programs on your system are:
- cmake 3.30 >=
- clang 18.1.8 >= | gcc 14.2.1 >=
- Python 3.13.2 >=

For better experience:
- setup ```core.autocrlf=true``` for git environment

## Installation

### Step #1: Repository preparation

#### Automatic

Automation pre-installation will check needed dependencies, its version and many others.
So, in general it's more safe way to prepare the cloned repo. But if you have some troubles
with this point, just go below to the 'Manual' part.

1. Clone this repository
2. In the terminal run the script './install.py' using your python3: ```python install.py```

#### Manual

If you have some trouble with automation pre-installation you can try to do main steps by
your hands.

1. Clone this repository
2. In the terminal run: ```git submodule update --init --force --remot```

### Step #2: Building
1. Open the terminal and make sure that your current path inside project root folder 
2. Create build dir & go into it: ```mkdir build; cd build```
3. Run cmake: ```cmake -DCMAKE_BUILD_TYPE="Debug" ../```
4. Build project using cmake: ```cmake --build .```

PS: to build Debug or Release version of the project set ```Debug``` or ```Release``` to ```-DCMAKE_BUILD_TYPE```.
E.g: ```-DCMAKE_BUILD_TYPE="Release"```

---

# Road map

## Common concepts

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

#### C++ includes

Include it to your file: ```#include "Utils/Concepts.h"```

___

## Working with space coordinates

### Intro

Of course, it has basic set of tools to work with space coordinates.

- ```Core::Rect<ArithmeticType>``` - use it to create you own Rectangle abstraction
- ```Core::GlobalPosition<Len, Type, Precision>``` - the same glm::vec2 or vec3 but better
- ```Core::Size<Dimension, ArithmeticType>``` - better than glm::vecX
- ```Core::LocalPosition<Len, Type, Precision>``` - the friend of the GlobalPosition, but for now - **not implemented**.

*More detailed view & examples you can find here: [link](docs/SpaceCoordinates.md)*

### Fast example

Working with ```Rect```.

```c++
#include "Core/Rect.h"

using namespace Core;
using namespace std;

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
using namespace std;

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

#### C++ includes

- For ```Rect```: ```#include "Core/Rect.h"```
- For ```GlobalPosition```: ```#include "Core/Position.h"```
- For ```Size```: ```#include "Core/Size.h"```

___

## Common functions to work with Math

### Intro

Usually functions for you, but in ```compile-time```!

- ```Math::Abs``` - for working with any arithmetical type
- ```Math::IsEqual``` - comparing of the float or double
- ```Math::IsZero``` - checking for 0.0 for types with floating point

### Fast example

```c++
#include "Core/Math.h"

using namespace Math;
using namespace std;

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
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ includes

Just include it to your file: ```#include "Core/Math.h"```

___

## Common interfaces & patterns

### Intro

Popular practice is to add a function 'swap' to your classes. Why we can't
bring it out to the interface? Yes, we did it!
Or working with our the ~~anti~~ best pattern: Singleton? We also have it.

### Fast example

```c++
#include "Core/CommonInterfaces.h"

struct MyType : public Core::ISwappable<MyType>
{
    void swap(MyType&) override { ... }
};
```

Working with singleton:

```c++
#include "Core/Singleton.h"

struct MyType : public Core::Singleton<MyType>
{
    // Yes, it's ready to use!
};
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ includes

- For singleton: ```#include "Core/Singleton.h"```
- For interfaces: ```#include "Core/CommonInterfaces.h"```

___

## Enum-generator

### Intro

Do you want to convert an enum's constant to string or vise versa? Or you want to
know home many constants in your enum? I think it's not a problem. Use compile-time
wrapper for it without loosing of optimization privelegious.

### Fast example

```c++
#include "Core/Enum.h"

using namespace std;

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

## Atomic strings
This file will be filled up in the future. Just track the latest version of this library.
But now, you can use this class to get better perfomance experience working with compile-time strings; but, also, you can work and with dynamic strings.
So about a class: you can find this class by the next path: ```sources/Core/String.h```
Main class is ```Core::BaseClass<CharType>```. But to use it easier just use aliases ```Core::StringAtom``` for working with one-byte charset.
I can say a lot about this class but want to highlight main concepts:
1. Try to use this class only with compile time strings. It was created mainly for these purposes. For example to increase equality comparision between two compile-time strings.
2. But if you want to put your own dynamic string you also can do it. It has the same methods like std::string and more.
Let's go to the examples:
```c++
#include "Core/Strings.h"
#include <iostream>

int main()
{
    using namespace std;
    Core::StringAtom name = "Andrew"_atom; // At this moment you put a compile-time(static) string to the internal string-pool. It's absolutely static string.
    cout << name.Data() << endl; // => Andrew
    cout << (name == "Andrew") << endl; // => true; now a compiler compare not two strings, but their addresses.

    // From this code's line you will work with a variable 'name' as with dynamic-string.
    name.PushBack("!"); // But it's not a problem to make from static string -> dynamic. Don't worry about it, just use methods as you want.
    cout << (name == "Andrew!") << endl; // => true; now a compiler compare JUST two strings. You'll get small deoptimization, but it's okay. As with std::string.
    cout << name.Data() << endl; // => Andrew!

    name.Trim("!");
    cout << name.Data() << endl; // => Andrew

    if (stdname.Match("\\w+\\!"))
        cout << "Matched" << endl;
    else
        cout << "NOT matched" << endl;
    // => Matched

    return 0;
}
```

Just want to highlight again: it has absolutely full functionality of the std::string, and also has new features.
To check it all you can look at the header file 'String.h', or make the same with examples in the 'test' directory: ```tests/StringTest.cpp```

## Delegates
It's quite common practice to extend your program; to use Event Orienting Programming. And delegates can help you to make more comfortable.
Let's look into the code:
```c++
#include "Core/Delegate.h"
#include <iostream>

int main()
{
    using namespace std;

    // At this line was created a delegate. So, it's just listener of some custom events and you can trigger subscribed function from any part of you block-scope.
    Core::Delegate<void()> delegate;

    bool wasInvoked = false;

    // Subscribing to the delegate. So, after function 'Trigger' your lambda will be invoked.
    auto id = delegate.Subscribe(
        [&]()
        {
            wasInvoked = true;
        });

    delegate.Trigger(); // Invoking of your lambda above.

    cout << (wasInvoked) << endl; // => true

    return 0;
}
```

If you want to get more complex cases & functionality just look at the test: ```tests/DelegateTest.cpp```

## Asserts
Sometimes you don't want to core user's programm especially in the enterprise\release version but want to say smth. you can use Assert function.
It's easy, let's do it:
```c++
#include "Core/Assert.h"
#include <iostream>

int main()
{
    using namespace std;

    constexpr const float tax = 0.05f;
    float bruttoSalary = 0;
    cout << "Brutto salary: ";
    cin >> bruttoSalary;

    Assert(yourMoney < 0.f, "Money can't be less then zero"); // At this moment you can see potentially OS-window with a described error.
    if (yourMoney >= 0.f)
    {
        cout << "Netto salary: " << bruttoSalary * (1.f - tax) << endl;
    }

    return 0;
}
```

## 📞 Feedback & Contacts

You can send me e-mail: Valerii.Koniushenko@gmail.com or try to find me in telegram: @markmoran24