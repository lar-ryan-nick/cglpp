# CPPGL
### Ryan Wiener
## Downloading External Libraries
### GLFW
[GLFW download link]("http://www.glfw.org/download.html")
[GLFW download instructions link](http://www.glfw.org/docs/latest/compile.html)
### GLAD
[GLAD download link](http://glad.dav1d.de)
Go to the web service, make sure the language is set to C++ and in the API section, select an OpenGL version of at least 3.3 (which is what we'll be using for these tutorials; higher versions are fine as well). Also make sure the profile is set to Core and that the Generate a loader option is ticked. Ignore the extensions (for now) and click Generate to produce the resulting library files. Add the glad.c file to the project src directory or make it a lib file called libglad and add it to the lib file.
