## Installation

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

**Blazing fast answer is:**
```sh
mkdir build && cd build && cmake ../ && cmake --build .
```

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