const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const glfw = b.dependency("glfw", .{ .target = target, .optimize = optimize });

    const zig = b.addLibrary(.{
        .name = "zig",
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/zig/root.zig"),
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });

    const exe = b.addExecutable(.{
        .name = "animation",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
        .use_llvm = true,
    });

    exe.addCSourceFiles(.{
        .root = b.path("src/"),
        .files = &.{
            "main.c",
            "utils.c",
            "renderer/renderer.c",
            "renderer/shapes.c",
            "containers/darray.c",
        },
    });

    exe.addCSourceFile(.{ .file = b.path("libs/glad/src/glad.c") });

    exe.addIncludePath(b.path("include/"));
    exe.addIncludePath(b.path("libs/glad/include/"));
    exe.addIncludePath(glfw.path("include/"));

    exe.linkLibrary(zig);
    exe.linkLibrary(glfw.artifact("glfw"));

    b.installArtifact(exe);

    const run_step = b.step("run", "Run the app");
    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| run_cmd.addArgs(args);
}
