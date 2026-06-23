# Utils

[![Jenkins](https://img.shields.io/badge/jenkins-%232C5263.svg?style=for-the-badge&logo=jenkins&logoColor=white)](https://jenkins.vakon.space/job/Utils/)

**Builds**:
- [![MSVC Debug](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FWinBuild_MSVC_Debug%2F&label=MSVC%20Debug)](https://jenkins.vakon.space/job/Utils/job/WinBuild_MSVC_Debug/) [![MSVC Release](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FWinBuild_MSVC_Release%2F&label=MSVC%20Release)](https://jenkins.vakon.space/job/Utils/job/WinBuild_MSVC_Release/)
- [![GCC Debug](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FLinuxBuild_GCC_Debug%2F&label=GCC%20Debug)](https://jenkins.vakon.space/job/Utils/job/LinuxBuild_GCC_Debug/) [![GCC Release](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FLinuxBuild_GCC_Release%2F&label=GCC%20Release)](https://jenkins.vakon.space/job/Utils/job/LinuxBuild_GCC_Release/)
- [![Clang Debug](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FLinuxBuild_Clang_Debug%2F&label=Clang%20Debug)](https://jenkins.vakon.space/job/Utils/job/LinuxBuild_Clang_Debug/) [![Clang Release](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FLinuxBuild_Clang_Release%2F&label=Clang%20Release)](https://jenkins.vakon.space/job/Utils/job/LinuxBuild_Clang_Release/)

**Unit Tests**:
- [![MSVC Debug](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FWinUnitTests_MSVC_Debug%2F&label=MSVC%20Debug)](https://jenkins.vakon.space/job/Utils/job/WinUnitTests_MSVC_Debug/) [![MSVC Release](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FWinUnitTests_MSVC_Release%2F&label=MSVC%20Release)](https://jenkins.vakon.space/job/Utils/job/WinUnitTests_MSVC_Release/)
- [![GCC Debug](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FLinuxUnitTests_GCC_Debug%2F&label=GCC%20Debug)](https://jenkins.vakon.space/job/Utils/job/LinuxUnitTests_GCC_Debug/) [![GCC Release](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FLinuxUnitTests_GCC_Release%2F&label=GCC%20Release)](https://jenkins.vakon.space/job/Utils/job/LinuxUnitTests_GCC_Release/)
- [![Clang Debug](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FLinuxUnitTests_Clang_Debug%2F&label=Clang%20Debug)](https://jenkins.vakon.space/job/Utils/job/LinuxUnitTests_Clang_Debug/) [![Clang Release](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FLinuxUnitTests_Clang_Release%2F&label=Clang%20Release)](https://jenkins.vakon.space/job/Utils/job/LinuxUnitTests_Clang_Release/)

[![Clang Debug](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.vakon.space%2Fjob%2FUtils%2Fjob%2FValgrind%2F&label=Valgrind)](https://jenkins.vakon.space/job/Utils/job/Valgrind/)

---

## Intro

A utility library providing common solutions for your code. It already includes several implemented features, such as:
- Common concepts(from C++20)
- Common classes for working with space coordinates
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

For better experience:
- setup ```core.autocrlf=true``` for git environment

## Installation

**Quick start is:**
```sh
mkdir build && cd build && cmake ../ && cmake --build .
```

## Blazing fast build with ccache

# ccache Setup

ccache is a compiler cache that dramatically speeds up recompilation by caching previous compilation results. This
project supports ccache on both Linux and Windows.

---

## Linux

### Install

**Arch Linux:**

```bash
sudo pacman -S ccache
```

**Ubuntu/Debian:**

```bash
sudo apt install ccache
```

### Configure

Create `~/.config/ccache/ccache.conf`:

```ini
hash_dir = false
sloppiness = include_file_mtime,include_file_ctime,time_macros,locale
depend_mode = true
compression = true
max_size = 20G
```

---

## Windows

ccache supports MSVC since version 4.3.

### Install

**winget:**

```powershell
winget install ccache
```

**Or download manually** from [ccache releases](https://github.com/ccache/ccache/releases) and add the directory to your
`PATH`.

Verify:

```powershell
ccache --version
```

### Configure

Create `%APPDATA%\ccache\ccache.conf` (e.g. `C:\Users\<you>\AppData\Roaming\ccache\ccache.conf`):

```ini
hash_dir = false
sloppiness = include_file_mtime,include_file_ctime,time_macros,locale
compression = true
max_size = 20G
```

> `depend_mode` is omitted on Windows — it requires GCC/Clang-style `.d` files and does not work with MSVC.

### MSVC: required CMake flag

MSVC by default writes debug info into `.pdb` files with absolute paths, which causes cache misses. The project already
handles this — `/Zi` is replaced with `/Z7` (embedded debug info) automatically when ccache is detected.

### Environment variable (PowerShell)

For MSVC builds, set this in your session or CI environment:

```powershell
$env:CCACHE_CPP2 = "true"
```

Or add it permanently via System Properties → Environment Variables.

### Enable in CMake

Same as Linux — ccache is detected automatically from `PATH`.

---

## CMake integration details

The project detects ccache at configure time and sets it as the compiler launcher:

```cmake
find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif ()
```

This works for Clang, GCC, and MSVC without any manual configuration.

---

## Troubleshooting

**Stats not changing after build** — ccache is not on `PATH` or the launcher is not set. Confirm with:

```bash
cmake --build build/ -v 2>&1 | head -5
# Should show: ccache /usr/bin/g++ ...
```

**0% hits after second build** — check `hash_dir`. It must be `false`. Verify with:

```bash
ccache --show-config | grep hash_dir
```

**Uncacheable calls** — expected for linker invocations. Also check for C++20 module scanning flags (`-fmodules-ts`). If
present and you do not use C++20 modules, disable scanning:

```cmake
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
```

**Diagnosing misses** — enable the log temporarily:

```bash
# Add to ccache.conf:
log_file = /tmp/ccache.log

# Then build and inspect:
grep "Result: " /tmp/ccache.log | sort | uniq -c | sort -rn
```

Remove `log_file` from the config once done.

### Step #1: Repository preparation

In general, it’s very easy. Everything that you need to do is clone this repository and run CMake. 
CMake will generate all the necessary files and fetch any required dependencies for you.

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

[☝️ Go Top](#table-of-contents)

## Working with space coordinates

### Intro

Of course, it has basic set of tools to work with space coordinates.

- ```Core::Rect<ArithmeticType>``` - use it to create you own Rectangle abstraction
- ```Core::GlobalPosition<Len, Type, Precision>``` - the same glm::vec2 or vec3 but better
- ```Core::Size<Dimension, ArithmeticType>``` - better than glm::vecX
- ```Core::RelativePosition<Len, Type, Precision>``` - the friend of the GlobalPosition, but for now - **not implemented**.

*More detailed view & examples you can find here: [link](docs/SpaceCoordinates.md)*

### Quick example

Working with ```Rect```.

```c++
#include "Core/Rect.h"

using namespace Core;
using std::cout;
using std::endl;

FRect rect = FRect{ 0.f, 10.f, 10.f, 0.f };

auto innerPoint = FRect::GlobalPositionT{ 3.f, 3.f };
// or simpler: auto innerPoint = GlobalPosition2F{ 3.f, 3.f };

if (rect.isContain(innerPoint))
    cout << "The rect contains this point";

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

ISize2 a = ISize2{ 5, 10 }; // ISize2 == int size 2D
ISize2 b = ISize2{ 2, 4 };
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
Just link it with your already existing target in your CMakeLists.txt:
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
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include it to your file: ```#include "Core/Math.h"```

[☝️ Go Top](#table-of-contents)

## Common interfaces & patterns

### Intro

It’s common practice to add a ```swap``` function to your classes. Why not move it to the interface? We did!
Or working with our favorite (or ~~anti~~ best) pattern: Singleton? We’ve got that too.

### Quick example

```c++
#include "Core/CommonInterfaces.h"

struct MyType : public Core::ISwappable<MyType>
{
    void swap(MyType&) override { ... }
};
```

Working with singleton:

Yes, you can use 'simple' version of it ```Singleton```. But better to use ```StrictSingleton``` - 
it will provide more protection to your 'god-object'. Also, to restrict manual creation of your 
god object - you must restrict several things with your class. In general, you don't have to worry
and just put ```SINGLETONS_FRIEND(your_class)``` to the class body.
```c++
#include "Core/Singleton.h"

// Unsafe & deprecated version. Can be accidentally copied.
struct UnsafeGod : public Core::Singleton<UnsafeGod>
{
    // Yes, it's ready to use!
};

// Safer version of  it. Now, you can't just copy\move it to some another object.
struct SaferGod : public Core::StrictSingleton<SaferGod>
{
};

// Safest version!
struct SafestGod : public Core::StrictSingleton<SafestGod>
{
    SINGLETONS_FRIEND(SafestGod)
public:
    // ...
};
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

- For singleton: ```#include "Core/Singleton.h"```
- For interfaces: ```#include "Core/CommonInterfaces.h"```

[☝️ Go Top](#table-of-contents)

## Enum-generator

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

[☝️ Go Top](#table-of-contents)

## Atomic strings

### Intro

Have you ever wondered why, sometimes, we compare strings character by character even when they are
exactly the same? Imagine you can predict the behavior and you know that string ```X``` will be identical
to string ```Y```. Instead of comparing their characters, you can just compare their addresses!

```c++
std::string A = "Hello";
std::string B = "Hello";

// Comparing character to character, but strings are the same - we know it!
if (A == B){} 

// Comparing addresses, because it's the same literals
if ("Hello" == "Hello"){}
```

So, the implementation of the new strings will bring you up second solution for code example above! 
But it will be processed automatically.

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
name.push_back("!");

// Now, an execution code is compare two strings(chars to chars). 
// You'll get small de-optimization, but it's okay. 
// Logically 'name' like std::string.
cout << (name == "Andrew!") << endl; // => true
cout << name.data() << endl; // => Andrew!

name.trim('!');
cout << name.data() << endl; // => Andrew

if (name.regexMatch("\\w+\\!"))
    cout << "Matched" << endl;
else
    cout << "NOT matched" << endl;
// => Matched
```

### Benchmark results

Comparing of '==' operations between: ```std::string``` & ```Core::StringAtom```

| Benchmark                                  | Time (ns) | CPU (ns) | Iterations    |
|-------------------------------------------|-----------|----------|---------------|
| BM_StdStringComparison/2                  | 1.83      | 1.83     | 379,678,000   |
| BM_StdStringComparison/8                  | 1.83      | 1.83     | 382,830,982   |
| BM_StdStringComparison/64                 | 2.04      | 2.04     | 344,510,558   |
| BM_StdStringComparison/512                | 4.25      | 4.25     | 165,402,730   |
| BM_StdStringComparison/2048               | 19.0      | 19.0     | 36,727,706    |
| BM_StdStringComparison_BigO               | 0.01 N    | 0.01 N   |               |
| BM_StdStringComparison_RMS                | 23 %      | 23 %     |               |
| BM_StringAtom_Static_Comparison/2         | 0.201     | 0.201    | 3'461'975'770 |
| BM_StringAtom_Static_Comparison/8         | 0.201     | 0.201    | 3'477'984'307 |
| BM_StringAtom_Static_Comparison/64        | 0.201     | 0.201    | 3'458'955'857 |
| BM_StringAtom_Static_Comparison/512       | 0.201     | 0.201    | 3'474'828'775 |
| BM_StringAtom_Static_Comparison/2048      | 0.201     | 0.201    | 3'462'340'161 |
| BM_StringAtom_Static_Comparison_BigO      | 0.20 (1)  | 0.20 (1) |               |
| BM_StringAtom_Static_Comparison_RMS       | 0 %       | 0 %      |               |


### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

- Main class: ```#include "Core/String.h"```
- Strings' helpers: ```#include "Core/StringHelper.h"```

[☝️ Go Top](#table-of-contents)

## Delegates

### Intro

You’ve probably heard of Event-Oriented Programming. If so, you can include ```Core/Delegate.h``` 
and use it as you like.

If not, let’s break it down in a minute! For example, imagine YouTube and a video about ‘cute cats.’ 
If you like a channel’s content, you can subscribe. When a new video is published, you get a 
notification. Delegates in code work in the same way.

Let’s look at an example below.

*More detailed view & examples you can find here: [link](docs/Delegate.md)*

### Quick example
```c++
#include "Core/Delegate.h"

using std::cout;
using std::endl;

// At this line a delegate was created. So, it's just listener 
// of some events, and you can trigger subscribed function from 
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

Of course, you shouldn't 'unsubscribe' manually. You can use RAII object for that: ```DelegateSubscriber```
```c++
Core::Delegate<void()> cuteCatsChannel;

DelegateSubscriber id = cuteCatsChannel.subscribeAndGetID(
    [&]()
    {
        cout << "Wow, new video!" << endl; 
    });

delegate.trigger();

// 'id' unsubscribe automatically
```


### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include: ```#include "Core/Delegate.h"```

[☝️ Go Top](#table-of-contents)

## Asserts

### Intro

Sometimes you don’t want to interrupt the user’s program - especially in enterprise or release 
versions - but you still want to notify the developer or use it for debugging. For these cases, 
you can use the Assert functionality.

It has two main ways to work with it:

- ```Assert([condition], [message])``` - thow an assert with/without a message
- ```Verify([condition], [message])``` - the same, but will return true\false

Main benefits of it are:

- Printing all messages to ```std::cerr```
- Printing of the callstack\backtrace of an assert (if a compiler supports it)

That's ease, let's look in the code.

### Quick example

```c++
#include "Core/Assert.h"

using std::cout;
using std::endl;

if (1 < 2)
{
    // Without params - just throw it.
    Assert(false); // Catch the Assert!
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

Also, if you want to force stop using of ```std::stacktrace``` you can add '#define'
```DONT_USE_CPP_LIB_STACKTRACE``` - before including of ```"Core/Assert.h"```:
```c++
#define DONT_USE_CPP_LIB_STACKTRACE
#include "Core/Assert.h"

...
```

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include: ```#include "Core/Assert.h"```

[☝️ Go Top](#table-of-contents)

# Feedback & Contacts

If you have some questions or propositions - contact me:

- e-mail: <a href="mailto:Valerii.Koniushenko@gmail.com">Valerii.Koniushenko@gmail.com</a>
- Telegram: [@valeriikoniushenko](https://t.me/valeriikoniushenko)
