# tag::configure[]
git submodule update --init --recursive
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
# end::configure[]

# tag::test[]
./build/bin/UtilsTests
# end::test[]
