load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("cmake_macro.bzl", "cmake_library")

package(
	default_visibility = ["//visibility:public"]
)

filegroup(
	name ="glfw_test",
	srcs = ["@glfw//:glfw_test"],
)

cc_library(
	name = "stb_image",
	hdrs = ["@stb_image//file"],
)

cmake_external(
	name = "assimp",
	cache_entries = {
		"ASSIMP_BUILD_ASSIMP_TOOLS": "OFF",
		"ASSIMP_BUILD_TESTS": "OFF",
	},
	lib_name = "libassimp",
	lib_source = "@assimp//:all",
	make_commands = ["make -j4", "make install"],
	static_libraries = ["libIrrXML.a"],
	shared_libraries = ["libassimp.so", "libassimp.so.5", "libassimp.so.5.0.0"],
)

cmake_external(
	name = "glfw",
	cache_entries = {
	},
	lib_source = "@glfw//:all",
	static_libraries = ["libglfw3.a"],
	linkopts = ["-lpng", "-lX11", "-lz", "-lpthread", "-ldl"],
)

cmake_external(
	name = "freetype",
	cache_entries = {
	},
	lib_source = "@freetype//:all",
	out_include_dir = "include/freetype2",
	static_libraries = ["libfreetype.a"],
)

cc_library(
	name = "cglpp",
	hdrs = glob(["include/*.h"]),
	srcs = glob(["src/*.cpp"]),
	deps = [
		"@glfw//:glfw",
		#":glfw",
		"@glad//:glad",
		"@glm//:glm",
		#"@assimp//:assimp",
		":assimp",
		":freetype",
		":stb_image",
	],
	# TODO: replace <> includes with "" unless they are system includes (i.e. <string>, "glad/glad.h").
	includes = ["external/glfw/include", "external/glm", "external/glad/include", "external/stb_image/file"],
	strip_include_prefix = "include",
	data = glob(["res/**/*"]),
)

cc_library(
	name = "sample",
	hdrs = glob(["samples/include/*.h"]),
	srcs = glob(["samples/src/*.cpp"]),
	strip_include_prefix = "samples/include",
	deps = [":cglpp"],
)	

cc_binary(
	name = "application_test",
	srcs = ["samples/main/applicationTest.cpp"],
	deps = [":sample", ":cglpp"],
)

cc_binary(
  name = "vulkan_test",
  srcs = ["samples/main/VulkanTest.cpp"],
  deps = ["@glfw//:glfw", "@vulkan_sdk//:vulkan"],
  #includes = ["external/glfw/include"],
)
