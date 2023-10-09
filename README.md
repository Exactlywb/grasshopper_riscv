# Grasshopper vectorization using RISC-V64
Implementation of a project on information security course

## Directory hierarchy

* lib -- common libraries 
* doc -- documentation, articles, presentation [Pavel Philippenko](https://github.com/pavel-collab)
* tools/simple -- sources for simple version of grasshopper [Daniil Frolov](https://github.com/Exactlywb), [Egor Dolgodvorov](https://github.com/Krym4s)
* tools/vectorized -- sources for vectorized version of grasshopper [Vladislav Belov](https://github.com/kolobabka)


## How to build

### Setting the environment
```
export BUILD_PATH= # path to build directory
export BUILD_TYPE= # Release or Debug
``` 

### Building
```
$ cmake -B $BUILD_PATH -DCMAKE_BUILD_TYPE=$BUILD_TYPE
$ cmake --build build
```

Ready to use. Check the `$BUILD_PATH/bin` folder with executable files 

**simple** -- a non-vectorized version 

**vectorized** -- a vectorized version with (according to RVV spec 1.0)