## Asserts

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

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

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

Just include: ```#include "Core/Assert.h"```