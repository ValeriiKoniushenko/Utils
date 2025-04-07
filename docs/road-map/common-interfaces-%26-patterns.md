## Common interfaces & patterns

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

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
