load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

http_archive(
   name = "rules_foreign_cc",
   sha256 = "c2cdcf55ffaf49366725639e45dedd449b8c3fe22b54e31625eb80ce3a240f1e",
   strip_prefix = "rules_foreign_cc-0.1.0",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.1.0.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

http_archive(
	name = "glfw",
	urls = ["https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip", "https://github.com/glfw/glfw/archive/3.3.2.zip", "https://github.com/glfw/glfw/archive/3.3.2.tar.gz"],
	sha256 = "08a33a512f29d7dbf78eab39bd7858576adcc95228c9efe8e4bc5f0f3261efc7",
	build_file = "@//:glfw.BUILD",
	strip_prefix = "glfw-3.3.2",
)

http_archive(
	name = "assimp",
	urls = ["https://github.com/assimp/assimp/archive/v5.0.1.zip", "https://github.com/assimp/assimp/archive/v5.0.1.tar.gz"],
	sha256 = "d10542c95e3e05dece4d97bb273eba2dfeeedb37a78fb3417fd4d5e94d879192",
	#build_file = "@//:assimp.BUILD",
	strip_prefix = "assimp-5.0.1",
	build_file_content = all_content,
)

http_archive(
	name = "freetype",
	urls = ["https://download.savannah.gnu.org/releases/freetype/freetype-2.10.4.tar.gz", "https://download.savannah.gnu.org/releases/freetype/freetype-2.10.4.tar.xz"],
	strip_prefix = "freetype-2.10.4",
	build_file_content = all_content,
	sha256 = "5eab795ebb23ac77001cfb68b7d4d50b5d6c7469247b0b01b2c953269f658dac"
)

http_archive(
	name = "glm",
	urls = ["https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip", "https://github.com/g-truc/glm/archive/0.9.9.8.zip", "https://github.com/g-truc/glm/archive/0.9.9.8.tar.gz"],
	strip_prefix = "glm",
	build_file = "@//:glm.BUILD",
	sha256 = "37e2a3d62ea3322e43593c34bae29f57e3e251ea89f4067506c94043769ade4c"
)

http_file(
	name = "stb_image",
	urls = ["https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"],
	sha256 = "8e5b0d717dfc8a834c97ef202d20e78d083d009586e1731c985817d0155d568c",
	downloaded_file_path = "stb/stb_image.h",
)

new_local_repository(
	name = "glad",
	path = "third_party/glad",
	build_file = "glad.BUILD",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_github_zaucy_rules_7zip",
    strip_prefix = "rules_7zip-e95ba876db445cf2c925c02c4bc18ed37a503fd8",
    url = "https://github.com/zaucy/rules_7zip/archive/e95ba876db445cf2c925c02c4bc18ed37a503fd8.zip",
    sha256 = "b66e1c712577b0c029d4c94228dba9c8aacdcdeb88c3b1eeeffd00247ba5a856",
)

load("@com_github_zaucy_rules_7zip//:setup.bzl", "setup_7zip")
setup_7zip()

http_archive(
    name = "com_github_zaucy_rules_vulkan",
    strip_prefix = "rules_vulkan-56fcc35f4def06de53ba36b2c5bd3ff20fcb43cf",
    url = "https://github.com/zaucy/rules_vulkan/archive/56fcc35f4def06de53ba36b2c5bd3ff20fcb43cf.zip",
    sha256 = "bc4b3aa29e30f11144fbc3254b86bb1fb735e01fbe6dfce5857fcb6fdf5b6952",
)

load("@com_github_zaucy_rules_vulkan//:repo.bzl", "vulkan_repos")
vulkan_repos()

