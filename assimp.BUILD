load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

package(
	default_visibility = ["//visibility:public"]
)

cmake_external(
	name = "assimp",
	cache_entries = {
		ASSIMP_BUILD_ASSIMP_TOOLS: "OFF",
		ASSIMP_BUILD_TESTS: "OFF",
	},
	lib_source = "//:all",
	make_commands = ["make"],
	out_lib_dir = "code",
)
