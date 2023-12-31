# octree-example

A simple application showcasing an Octree space partitioning algorithm. Rendering is done using the [Simplex](https://github.com/lampitosgames/Simplex_2016_Fall) rendering system. 

The application spawns entities into a global dimension and runs collision checks for each entity against every other entity to see if they overlap. The frame rate is calculated and displayed in the upper left GUI. It starts off lower because of the expensive checks that must be run against each entity every frame. As octree subdivisions are created, this creates new dimensions which allow for the number of necessary collision checks to be cut down dramatically since entities only test against their local dimension. This should result in a higher frame rate.
