# Helper library for CQG Web API in C++

This is a simple build environment for compiling the protobuf stuff of CQG

Prerequisites:
- protobuf NOTE: v21.12 was used, newer versions seem to have issues
- CMake 3.25 or newer
- CQG's Web API ZIP file

## Project directory layout

**Step 1** Download CQG's `WebAPI.zip` file and unzip in this repository's root directory in a directory called WebAPI. The resultant directory structure should look like:
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
 
**Step 2** Compile
```
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/protobuf/install/dir
make
```
