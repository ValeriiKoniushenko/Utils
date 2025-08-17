## Atomic strings

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

> 1. [Intro](#intro)
> 2. [Quick example](#quick-example)
> 3. [Requirements](#requirements)

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

So, implementing the new string gives you the second solution from the code example above - but 
it will be handled automatically.

### Technical description

Technically, it works only with addresses. When you create an atomic string, it is registered in 
the global buffer of all atomic strings ```class _StringPool```. However, each string literal 
is registered only once. So:

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

// At this point, you are adding a compile-time (static) string
// to the internal string pool. This is a fully static string.
StringAtom name = "Andrew"_atom;    
cout << name.data() << endl; // => Andrew

// Now the executed code compares addresses instead of the strings themselves.
cout << (name == "Andrew") << endl; // => true 

// Converting a static string to a dynamic string is not a problem. 
// Don’t worry - just use the methods as needed.
// From this line onward, you can work with the variable 'name' 
// as a dynamic string.
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