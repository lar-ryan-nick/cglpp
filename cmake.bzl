def _cmake_cc_library_impl(ctx):
  temp_dir = ctx.actions.declare_directory("build")

  ctx.actions.run(
    inputs = ctx.files.src,
    executable = "cmake",
    arguments = ["-DINSTALL_PREFIX=" + temp_dir.dirname,
                 "-S external/glfw", "-B external/glfw/build"],
    progress_message = "running cmake",
    outputs = [temp_dir],
  )

  ctx.actions.run(
    inputs = ctx.files.src,
    executable = "make",
    arguments = ["-j2"],
    progress_message = "Compiling external dependency",
  )

  ctx.actions.run(
    inputs = ctx.files.src,
    executable = "make",
    arguments = ["install"],
    progress_message = "Collecting files from compiled dependency",
  )

  cc_toolchain_info = cc_common.ccToolchainInfo.cc_toolchain_info
  feature_configuration = cc_common.configure_features(ctx, cc_toolchain_info)

  # TODO: learn how to use cc_common better.
  includes = depset([ctx.actions.declare_directory(temp_dir + hdr_dir) for hdr_dir in ctx.attr.hdr_dirs_out])
  libraries = [cc_common.create_library_to_link(ctx.actions, feature_configuration, cc_toolchain_info, static_library = [ctx.actions.declare_file(temp_dir + lib) for lib in ctx.attr.libs_out])]

  return CcInfo(compilation_context = cc_common.create_compilation_context(headers = includes),
                linking_context = cc_common.create_linking_contxt(linker_inputs = depset(cc_common.create_linker_input(owner = ctx.label, libraries = libraries))))


cmake_cc_library = rule(
  implementation = _cmake_cc_library_impl,
  attrs = {
    "src": attr.label(mandatory = True, allow_files = True),
    "hdr_dirs_out": attr.string_list(mandatory = True),
    "libs_out": attr.string_list(mandatory = True),
  },
)
