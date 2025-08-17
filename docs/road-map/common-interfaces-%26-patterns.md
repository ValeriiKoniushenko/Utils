## Common interfaces & patterns

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

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
