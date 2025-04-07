## Atomic strings

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

> 1. [Intro](#intro)
> 2. [Quick example](#quick-example)
> 3. [Requirements](#requirements)

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

### Technical description

Techinally, it is working only with addresses. So, when you are creating your atomic string - you will 
register it in the global buffer of all atomic strings: ```class _StringPool```. But registration of the
string will be procceded only once per one string-literal. So:
```c++
auto str1 = "Hello"_atom;
auto str2 = "Hello"_atom;

// the same addresses of "Hello"
cout << (str1.data() == str2.data() ? "EQUAL" : "NO" ); // > EQUAL
```

So, in such case as above your strings will be only like a viewer of the real string literal "Hello".

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

### Benchmark results

| Benchmark                                  | Time (ns) | CPU (ns) | Iterations     |
|-------------------------------------------|-----------|----------|----------------|
| BM_StdStringComparison/2                  | 1.80      | 1.80     | 385,146,536    |
| BM_StdStringComparison/8                  | 1.80      | 1.80     | 389,687,445    |
| BM_StdStringComparison/64                 | 2.00      | 2.00     | 349,641,324    |
| BM_StdStringComparison/512                | 4.04      | 4.03     | 173,257,057    |
| BM_StdStringComparison/2048               | 15.0      | 14.9     | 47,864,830     |
| BM_StdStringComparison_BigO               | 0.01 N    | 0.01 N   |                |
| BM_StdStringComparison_RMS                | 27 %      | 27 %     |                |
| BM_StringAtom_Static_Comparison/2         | 0.400     | 0.399    | 1,759,065,109  |
| BM_StringAtom_Static_Comparison/8         | 0.399     | 0.398    | 1,751,357,249  |
| BM_StringAtom_Static_Comparison/64        | 0.399     | 0.398    | 1,759,438,596  |
| BM_StringAtom_Static_Comparison/512       | 0.399     | 0.398    | 1,754,651,687  |
| BM_StringAtom_Static_Comparison/2048      | 0.399     | 0.398    | 1,757,571,927  |
| BM_StringAtom_Static_Comparison_BigO      | 0.40 (1)  | 0.40 (1) |                |
| BM_StringAtom_Static_Comparison_RMS       | 0 %       | 0 %      |                |

### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your alredy existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

- Main class: ```#include "Core/String.h"```
- Strings' helpers: ```#include "Core/StringHelper.h"```