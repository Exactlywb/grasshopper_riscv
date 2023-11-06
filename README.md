# Grasshopper vectorization using RISC-V64
Implementation of a project on information security course

## Directory hierarchy

* lib -- common libraries 
* doc -- documentation, articles, presentation [Pavel Philippenko](https://github.com/pavel-collab)
* tools/simple -- sources for simple version of grasshopper [Daniil Frolov](https://github.com/Exactlywb), [Egor Dolgodvorov](https://github.com/Krym4s)
* tools/vectorized -- sources for vectorized version of grasshopper [Vladislav Belov](https://github.com/kolobabka)
* tools/main -- common driver

# Contributing

Grasshopper project has `.clang-format` file. Since we still don't have CI, don't forget format your code 'by hands' 

```
git add <changed/added/deleted files>
git-clang-format origin/main
``` 

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

#### Usage

```
  cd build/bin
  ./simple <file_mode> <encryption_mode> <input_file> <key file> <init_vector_file>
```
- <file_mode>: 't' is for text input files, 'b' is fo bitmap image.
- <encryption_node>: One of 'ECB', 'CBC', 'PCBC', 'CFB', 'OFB'.
- <input_file>: text file or bitmap image.
- <key_file>: file with key, that was generated for this grasshopper session
- <init_vector_file>: file with init_vector, that was generated for this grasshopper session, only first block of init vector key is used.
- [output_img_cipher, output_img_orig]: pair of files for output of ciphered and original image.
