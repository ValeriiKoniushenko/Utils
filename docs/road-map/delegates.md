## Delegates

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Intro

I think you heard before about Event Oriented Programming. So, if yes - you can include Core/Delegate.h
and use as you want.

If no - let's understand it in one minute!
For example, let's take YouTube and video about 'cute cats'. If you really like a video's content
you can subscribe to the channel. And when new video will be published you'll get notification about
that. So, Delegates in the code - are the same.

Let's look on the example below.

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