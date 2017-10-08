<center>SWAN: Something we all need</center>
===========================
---------------------------
**SWAN** is a game engine written in `C++`, designed to be able to do simple games *quickly* and *efficiently*.

##### It features:
- A basic OpenGL renderer
- A primitive GUI system
- A Wavefront OBJ mesh importer

The project relies on several other libraries:

| Library   | Purpose                                               | Link                                                    |
| :-------: | ----------------------------------------------------- | ------------------------------------------------------: |
| SDL2      | Input, opening a window and initializing a GL context | [SDL homepage](https://www.libsdl.org/index.php)        |
| GLAD      | Loading GL functions                                  | [GLAD Github page](https://github.com/Dav1dde/glad)     |
| GLM       | Maths (*matrix arithmetic*)                           | [GLM homepage](https://glm.g-truc.net/0.9.8/index.html) |

Future plans:
- Scripting with Lua
- Seamless support for earlier versions of OpenGL (GL 2.0 and 1.0)
- A basic physics system
