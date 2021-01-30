load("@//:cmake.bzl", "cmake_cc_library")

package(
	default_visibility = ["//visibility:public"],
)

filegroup(name = "files", srcs = glob(["**"]), visibility = ["//visibility:private"])

cmake_cc_library(
	name = "glfw",
	src = ":files",
	hdr_dirs_out = ["include"],
	libs_out = ["code/libglfw.a"],
)

