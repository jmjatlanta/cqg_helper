# Helper library for CQG Web API in C++

This is a simple build environment for compiling the protobuf stuff of CQG

Prerequisites:
- protobuf
- CMake
- CQG's Web API ZIP file

## Project directory layout

Step 1 Download CQG's `WebAPI.zip` file and unzip in this repository's root directory in a directory called WebAPI. The resultant directory structure should look like:
```
cqg_helper
  |
   - WebAPI
      |
       - WebAPI
       - common
   - src
   - test
```
 
Step 2 Compile
```
cqg_helper> mkdir build
cqg_helper> cd build
cqg_helper/build> cmake .. -DCMAKE_PREFIX_PATH=/path/to/protobuf/install/dir
cqg_helper/build> make
```
