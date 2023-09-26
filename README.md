# Intersections of triangles in 3D
### About
this program calculates the numbers of triangles that intersect with each other
## Requirements
**cmake** version must be 3.15 or higher
## How to build
```bash
git clone git@github.com:VictorBerbenets/Triangles-3D.git
cd Triangles/
git submodule init
git submodule update
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release ..
cd build/
cmake --build .
```
After that you can run triangles program:

```bash
./triangles
```
The program will be waiting for input data from stdin in such way:  
**number** coord1 coord2 ...  
**number** - number of triangles  
coord1 coord2 ... - coordinates of points that form triangles  
At the end, the program prints number of intersected triangles to stdout.  
For example:  
2   1 0 0  0 0 0  -1 0 0  3 4 5  -3 -4 -5  3 -4 5  
Two triangles are introduced here. The first consists of points formed by the first nine numbers, and the second by the next nine.  
## How to run tests:
### You can run unit tests:
```bash
./tests/unit
```
### Or you can run end2end tests:
```bash
bash ../tests/end2end/test_runner.sh <tests_number> <triangles_number> <min_box> <max_box>
```
#### recommended test parameters: 
**tests_number** <= 10; **triangles_number** <= 100; **min_box** and **max_box** are optional parameters.  
After running you can see generated directory: Triangles/tests/end2end/resources. There will be the tests themselves and the answers to them.
