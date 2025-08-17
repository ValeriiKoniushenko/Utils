## Delegates

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Intro

You’ve probably heard of Event-Oriented Programming. If so, you can include ```Core/Delegate.h```
and use it as you like.

If not, let’s break it down in a minute! For example, imagine YouTube and a video about ‘cute cats.’
If you like a channel’s content, you can subscribe. When a new video is published, you get a
notification. Delegates in code work in the same way.

Let’s look at an example below.

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

Of course, you shouldn't 'unsubscribe' manually. You can use RAII object for that: ```IDGuard```
```c++
Core::Delegate<void()> cuteCatsChannel;

Core::Delegate<void()>::IDGuard id = cuteCatsChannel.subscribeAndGetID(
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