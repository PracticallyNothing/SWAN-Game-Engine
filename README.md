# :warning: WARNING :warning:
///////////////////////////////////////////////////////////////////////////////////////////  

SWAN is still in a *very* early stage of development.  
It lacks many of the core elements needed to make it a game engine.  
(Physics, support for skinned meshes, etc.)  
If you decide to use this project, be ready for lots of missing features,  
low framerates and weird, nonsensical behaviour.  

///////////////////////////////////////////////////////////////////////////////////////////  

-----------------------------------

## SWAN: Something we all need
---------------------------
**SWAN** aspires to be a game engine that enables developers to create simple games *quickly* and *efficiently*.

SWAN is programmed in `C++`.

##### It currently features:
- A basic OpenGL renderer
- A primitive GUI system
- A Wavefront OBJ mesh importer

The project relies on several other libraries:

| Library   | Purpose                                               | Link                                                       |
| :-------: | ----------------------------------------------------- | ---------------------------------------------------------: |
| SDL2      | Input, opening a window and initializing a GL context | [SDL homepage](https://www.libsdl.org/index.php)           |
| GLAD      | Loading GL functions                                  | [GLAD Github page](https://github.com/Dav1dde/glad)        |
| GLM       | Maths (*matrix arithmetic*)                           | [GLM homepage](https://glm.g-truc.net/0.9.8/index.html)    |
| STB Image | Image loading                                         | [STB Github page](https://github.com/nothings/stb)         |
| cpptoml   | [TOML](https://github.com/toml-lang/toml) reading     | [cpptoml Github page](https://github.com/skystrife/cpptoml)|

Future plans:
- Scripting with Lua
- Support for earlier versions of OpenGL (GL 2.0 and 1.0)
- A basic physics system

-----
## Compilation instructions
To compile SWAN and its demos, you will need a few tools:
- CMake
- A compiler with support for the C++14 standard

After you have obtained said tools, do the following:

1. Create a "build" directory so as to keep the main directory clean  
`mkdir build` (works in both Bash and Windows CMD)

2. Go into that directory  
`cd build`

3. Run CMake with the main directory as the argument  
`cmake ../`

4. Run whatever commands are necessary to build the project  
e.g. `make` for Linux or `mingw32-make.exe` for MinGW on Windows
