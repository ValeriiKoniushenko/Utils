## Atomic strings

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

> 1. [Intro](#intro)
> 2. [Quick example](#quick-example)
> 3. [Requirements](#requirements)

### Intro

Have you ever wondered why, sometimes, we compare strings character by character even when they are 
exactly the same? Imagine you can predict the behavior, and you know that string ```X``` will be identical 
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
the global buffer of all atomic strings ```class StringPool```. However, each string literal 
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

### Comparison Benchmarks

Comparing of '==' operations between: ```std::string``` & ```Core::StringAtom```

| Benchmark                        | Size   | Time (ns) | CPU (ns) | Iterations      |
|----------------------------------| ------ | --------- | -------- | --------------- |
| **std::string comparison**       | 2      | 1.83      | 1.83     | 383,319,634     |
|                                  | 8      | 1.83      | 1.83     | 383,239,124     |
|                                  | 64     | 2.04      | 2.04     | 343,161,424     |
|                                  | 512    | 4.76      | 4.76     | 146,580,865     |
|                                  | 2048   | 18.5      | 18.5     | 38,281,040      |
| **BigO / RMS**                   |        | 0.01 N    | 0.01 N   | 23%             |
| **StringAtom Static comparison** | 2      | 0.200     | 0.200    | 3,501,128,764   |
|                                  | 8      | 0.200     | 0.200    | 3,502,440,483   |
|                                  | 64     | 0.200     | 0.200    | 3,501,748,353   |
|                                  | 512    | 0.200     | 0.200    | 3,504,884,407   |
|                                  | 2048   | 0.200     | 0.200    | 3,504,967,204   |
| **BigO / RMS**                   |        | 0.20 (1)  | 0.20 (1) | 0%              |


### Pushing Back (Normal)

| Benchmark                   | Size | Time (ns) | CPU (ns) | Iterations  |
|-----------------------------| ---- | --------- | -------- | ----------- |
| **std::string PushingBack** | 2    | 2.28      | 2.28     | 307,189,879 |
|                             | 8    | 6.01      | 6.01     | 116,523,667 |
|                             | 64   | 74.4      | 74.4     | 9,415,181   |
|                             | 512  | 487       | 487      | 1,434,702   |
|                             | 2048 | 1855      | 1855     | 376,831     |
| **BigO / RMS**              |      | 0.91 N    | 0.91 N   | 3%          |
| **StringAtom PushingBack**  | 2    | 9.79      | 9.79     | 71,666,968  |
|                             | 8    | 12.3      | 12.3     | 56,693,206  |
|                             | 64   | 41.4      | 41.4     | 16,900,629  |
|                             | 512  | 312       | 312      | 2,238,073   |
|                             | 2048 | 1258      | 1258     | 556,665     |
| **BigO / RMS**              |      | 0.61 N    | 0.61 N   | 2%          |


### Pushing Back (Long String Mode)

| Benchmark                               | Size | Time (ns) | CPU (ns) | Iterations  |
|-----------------------------------------| ---- | --------- | -------- | ----------- |
| **std::string PushingBack\_LongString** | 2    | 2.49      | 2.49     | 281,601,512 |
|                                         | 8    | 25.2      | 25.2     | 27,813,785  |
|                                         | 64   | 102       | 102      | 6,862,563   |
|                                         | 512  | 608       | 608      | 1,153,361   |
|                                         | 2048 | 2343      | 2343     | 299,668     |
| **BigO / RMS**                          |      | 1.15 N    | 1.15 N   | 3%          |
| **StringAtom PushingBack\_LongString**  | 2    | 9.63      | 9.63     | 72,672,302  |
|                                         | 8    | 14.5      | 14.5     | 48,225,141  |
|                                         | 64   | 94.4      | 94.4     | 7,398,923   |
|                                         | 512  | 621       | 621      | 1,126,125   |
|                                         | 2048 | 2354      | 2354     | 297,275     |
| **BigO / RMS**                          |      | 1.15 N    | 1.15 N   | 3%          |


### Requirements

#### CMake

Needed target for you is: ```Utils::Core```
Just link it with your already existing target in your CMakeLists.txt:
```target_link_libraries(YourTarget PUBLIC Utils::Core)```

#### C++ side

- Main class: ```#include "Core/String.h"```
- Strings' helpers: ```#include "Core/StringHelper.h"```