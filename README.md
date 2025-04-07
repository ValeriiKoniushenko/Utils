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

# Table of contents

[📑 Click me to open full doc](docs/index.md)

> 1. **[Getting started](#getting-started)**
>     1. [Build Requirements](#build-requirements)
>     2. [Installation](#installation)
> 2. **[Road map](#road-map)**
>     1. [Common concepts](#common-concepts)
>     2. [Space coordinates](#working-with-space-coordinates)
>     3. [Math](#common-functions-to-work-with-math)
>     4. [Interfaces & patterns](#common-interfaces-%26-patterns)
>     5. [Enum-generator](#enum-generator)
>     6. [Atomic Strings](#atomic-strings)
>     7. [Delegates](#delegates)
>     8. [Asserts](#asserts)
> 3. **[Feedback & Contacts](#feedback-%26-contacts)**

# Getting started

## Build Requirements
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

#### C++ side

Include it to your file: ```#include "Utils/Concepts.h"```

[☝️ Go Top](#table-of-contents)

## Working with space coordinates

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

[☝️ Go Top](#table-of-contents)

## Common functions to work with Math

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
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include it to your file: ```#include "Core/Math.h"```

[☝️ Go Top](#table-of-contents)

## Common interfaces & patterns

### Intro

Popular practice is to add a function 'swap' to your classes. Why we can't
bring it out to the interface? Yes, we did it!
Or working with our the ~~anti~~ best pattern: Singleton? We also have it.

### Quick example

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

#### C++ side

- For singleton: ```#include "Core/Singleton.h"```
- For interfaces: ```#include "Core/CommonInterfaces.h"```

[☝️ Go Top](#table-of-contents)

## Enum-generator

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

[☝️ Go Top](#table-of-contents)

## Atomic strings

### Intro

Have you ever wondered why sometimes we have absolutley the same strings, but we still compare it character
to character? So, let's imagine that you can predict some behavior and you konw - next string X will be the same
as next string Y. Let's not compair its characters, let's just compare their addresses!

```c++
std::string A = "Hello";
std::string B = "Hello";

// Comparing character to character, but strings are the same - we know it!
if (A == B){} 

// Comparing addresses, because it's the same literals
if ("Hello" == "Hello"){}
```

So, the implemetation of the new string brings you up second solution of the code example above! But it will
be processed automatically.

*More detailed view & examples you can find here: [link](docs/Strings.md)*

### Quick example
```c++
#include "Core/Strings.h"

using std::cout;
using std::endl;
using namespace Core;

// At this moment you put a compile-time(static) string 
// to the internal string-pool. It's absolutely static string.    
StringAtom name = "Andrew"_atom;    
cout << name.data() << endl; // => Andrew

// Now an executed code is comparing not two strings, 
// but its addresses.
cout << (name == "Andrew") << endl; // => true 

// But it's not a problem to make from static string -> dynamic. 
// Don't worry about it, just use methods as you want.
// From this code's line you will work with a variable 
// 'name' as with dynamic-string.
name.pushBack("!");

// Now, an execution code is compare two strings(chars to chars). 
// You'll get small de-optimization, but it's okay. 
// Logically 'name' like std::string.
cout << (name == "Andrew!") << endl; // => true
cout << name.data() << endl; // => Andrew!

name.trim("!");
cout << name.data() << endl; // => Andrew

if (name.regexMatch("\\w+\\!"))
    cout << "Matched" << endl;
else
    cout << "NOT matched" << endl;
// => Matched
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

- Main class: ```#include "Core/String.h"```
- Strings' helpers: ```#include "Core/StringHelper.h"```

[☝️ Go Top](#table-of-contents)

## Delegates

### Intro

I think you heard before about Event Oriented Programming. So, if yes - you can include Core/Delegate.h
and use as you want.

If no - let's understand it in one minute!
For example, let's take YouTube and video about 'cute cats'. If you really like a video's content
you can subscribe to the channel. And when new video will be published you'll get notification about
that. So, Delegates in the code - are the same.

Let's look on the example below.

*More detailed view & examples you can find here: [link](docs/Delegate.md)*

### Quick example
```c++
#include "Core/Delegate.h"

using std::cout;
using std::endl;

// At this line a delegate was created. So, it's just listener 
// of some events and you can trigger subscribed function from 
// any part of you block-scope.
Core::Delegate<void()> cuteCatsChannel;

// Subscribing to the delegate. So, after function 'trigger' 
// your lambda will be invoked.
auto id = cuteCatsChannel.subscribeAndGetID(
    [&]()
    {
        cout << "Wow, new video!" << endl; 
    });

// ==== Channel's code ====
delegate.trigger();
// ========================

// After 'trigger' above, you'll get a message to a console:
// > Wow, new video!

// And, let's imagine that 'cute cats' channel is boring for 
// you after. What are we doing when you met boring channel? 
// Yes - put a button 'unsubscribe'! In the code we'll do the same:
delegate.unsubscribe(id);
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include: ```#include "Core/Delegate.h"```

[☝️ Go Top](#table-of-contents)

## Asserts

### Intro

Sometimes you don't want to interrupt user's programm especially in
the enterprise\release version but want to say something to developer
of just for debug. For these purposes you can use Assert functionality.

It has two main way to work with it:

- ```Assert([condition], [message])``` - thow an assert with/without a message
- ```Verify([condition], [message])``` - the same, but will return true\false

Main benefits of it are:

- Printing all messages to ```std::cerr```
- Printing of the callstack\backstrace of an assert (if a compiler supports it)

That's ease, let's look in the code.

### Quick example

```c++
#include "Core/Assert.h"

using std::cout;
using std::endl;

if (1 < 2)
{
    // Without params - just throw it.
    Assert(); // Catch the Assert!
}

// The same, but with condition. 
// If condition is false - assert will be thrown
Assert(1 < 2);

// Same, but with a message
Assert(1 < 2, "Some message here");

// Question: 1 < 2? Yes! It's true. 
// So, you will go to the if's body 
if (Verify(1 < 2, "I'm in 'if' statement") {}
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include: ```#include "Core/Assert.h"```

[☝️ Go Top](#table-of-contents)

# Feedback & Contacts

If you have some questions or propositions - contact me:

- e-mail: Valerii.Koniushenko@gmail.com
- Telegram: @markmoran24