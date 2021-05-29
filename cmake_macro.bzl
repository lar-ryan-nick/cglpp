def cmake_library(name, srcs, cmake_file, visibility = None):
  native.genrule(
    name = name,
    srcs = srcs + ["cmake_file"],
    outs = ["external/glfw/install/libglfw3.a"],
    cmd = "cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=external/glfw/bin S external/glfw -B external/glfw/build && make -j 2 -C external/glfw/build && make -C external/glfw/build install",
  )
