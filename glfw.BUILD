package(
	default_visibility = ["//visibility:public"],
)

COMMON = glob(["context*", "init*", "input*", "internal*", "mappings*", "monitor*", "vulkan*", "window*", "xkb*"])

COCOA_SPECIFIC = glob(["src/cocoa*"])

WINDOWS_SPECIFIC = glob(["win32*", "wgl*"])

EGL_SPECIFIC = glob(["egl*"])

LINUX_SPECIFIC = glob(["linux*", "posix*", "glx*"])

X11_SPECIFIC = glob(["x11*"]) + LINUX_SPECIFIC

cc_library(
	name = "glfw",
	srcs = COMMON + select({
		"@platforms//os:windows": WINDOWS_SPECIFIC,
		"@platforms//os:linux": X11_SPECIFIC,
	}),
	hdrs = glob(["include/GLFW/*.h"]),
	local_defines = select({
		"@platforms//os:windows": ["_GLFW_WIN32"],
		"@platforms//os:linux": ["_GLFW_X11"],
	}) + ["_GLFW_BUILD_DLL"],
	linkstatic = False,
	#strip_include_prefix = "include",
)
