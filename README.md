# SWrasterizer
Software Rasterization project for online CS50 course

#### Description:
This project is a software rasterizer written in C++.
It contains vertex interpolation, flat shading, smooth shading, normal mapping, albedo texture mapping,
diffuse light, ambient light, specular light. Handmade linear algebra class, triangle rasterization using
barycentric coordinates, + much more. The project is a single threaded application that runs on the CPU.
Everything in this project (besides the image parser) was written from scratch. No external libraries
to aid in the rendering process were used.

#### Vector Class
Four main structs:
`Vector4`, `Vector3`, `Vector2`, `Mat3`

Vector4 is a struct that holds 4 floats `x, y, z, w`
Vector3 is a struct that holds 3 floats `x, y, z`
Vector2 is a struct that holds 2 floats `x, y`
Mat3 represents a 3x3 matrix and contains 3 `Vector3`s

#### How to Use
Within the main function of the win32_plat.cpp file you can load objects, textures and specify rendering options.
Creating a new render object should be the first TODO to initiate a project. When initiating this new render object,
a depth buffer `[render object].db` and an alpha buffer `[render object].ab` should be filled using:
```c++
g.db = vector<vector<float>>(pixel_info.w, vector<float>(pixel_info.h, far_plane));
g.ab = vector<vector<float>>(pixel_info.w, vector<float>(pixel_info.h, 1));
```
When specifying a render mode, using `[render object].mode` The following values correspond to the following modes:

'f' for no light interpolation, flat shading
's' for light interpolation (smooth shading)
'v' to only render verticies
'w' for wireframe only
'n' for normal buffer
'd' for depth buffer
'a' for alpha buffer

Ex:
`[render object].mode = 'f'`

The final step to initiating a render object is to initiate the background colour using:
`[render object].bgc = Vector4(R, G, B, A)`

To render objects:
The `parse_obj` function used like so:
```parse_obj("path", (bool) generate_vertex_normals);```
This function parses .obj files and stores them to be rendered.

The `load_texture` function used like so:
```load_texture("path", mesh_id, texture_type);```
This function parses texture files and stores them with a pre-existing mesh

The `addObject` function used like so:
```addObject(id, loaded_index, scale, Vector3 position, Vector3 angles, Vector4 color);```
This function adds an Object to be rendered.

#### In Depth:
In order to apply rotations and calculate lighting, I had to learn linear algebra. This was complicated
but eventually led me to where I am now. I used rotation matricies for the rotation and lighting calculations
found online for the specifics. The obj file parsing was helped through a couple tutorials that taught me
how to read and write to files in c++.

##### FYI:
I began this project with no prior C++ experience so the code is quite messy.
I learnt how to create a window and draw graphics through a youtube tutorial from
Dan Zaidan.

##### Controls:

WASD SPACE and SHIFT control camera movement.
The arrow keys control camera rotation.

POIJKL Control the lighting rotation and movement.
TGFHYU control the rotation of objects in the scene.
