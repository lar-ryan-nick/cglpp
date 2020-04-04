# <img alt="CGL++ Logo" src="res/img/cgl.png" height=30/> CGL++

A graphics library written in C++11.

## Authors
#### Ryan Wiener

## Running Project0

To create the executable for project0 run the command `make project0`. This will create the binary which is located in `samples/bin/project0`. To run the binary, pass the model file in as a command line argument (i.e. `samples/bin/project0 <path-to-model-file>`. When viewing the model the controls are much like a first person shooter. Use the wasd keys to move around and the mouse to rotate the viewing direction. The mouse will become locked and invisible when focused on the window created by this application; this is normal, expected behavior.

## Dependencies

The repo comes with source captures of all necessary dependencies in terms of code. However to compile you will need g++ with -std=c++11 support and cmake (maybe over version 3.something).

Here are the libraries that I used to make this project:
* [GLFW](https://www.glfw.org/) (3.3.2)
* [GLAD](https://glad.dav1d.de/) (OpenGl Core 3.3)
* [Assimp](https://www.assimp.org/index.php) (5.0.1)
* [Freetype](https://www.freetype.org/) (2.10.1)

## Special Thanks
#### [Learn OpenGL](https://www.freetype.org/)
#### [OGLdev](http://ogldev.atspace.co.uk/index.html)
