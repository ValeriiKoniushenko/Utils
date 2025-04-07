## Installation

> - [🏠 Go Home](../index.md)
> - [👈 Go Parent Page](index.md)

### Step #1: Repository preparation

#### Automatic

Automation pre-installation will check needed dependencies, its version and many others.
So, in general it's more safe way to prepare the cloned repo. But if you have some troubles
with this point, just go below to the 'Manual' part.

1. Clone this repository
2. In the terminal run the script './install.py' using your python3: ```python install.py```

#### Manual

If you have some trouble with automation pre-installation you can try to do main steps by
your hands.

1. Clone this repository
2. In the terminal run: ```git submodule update --init --force --remot```

### Step #2: Building
1. Open the terminal and make sure that your current path inside project root folder
2. Create build dir & go into it: ```mkdir build; cd build```
3. Run cmake: ```cmake -DCMAKE_BUILD_TYPE="Debug" ../```
4. Build project using cmake: ```cmake --build .```

PS: to build Debug or Release version of the project set ```Debug``` or ```Release``` to ```-DCMAKE_BUILD_TYPE```.
E.g: ```-DCMAKE_BUILD_TYPE="Release"```