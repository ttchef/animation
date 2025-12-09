const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const c = b.addTranslateC(.{
        .root_source_file = b.addWriteFiles().add("c.h",
            \\#include <renderer/renderer.h>
            \\#include <renderer/shapes.h>
        ),
        .target = target,
        .optimize = optimize,
    });

    c.addIncludePath(b.path("include/"));
    c.addIncludePath(b.path("libs/"));
    c.addIncludePath(b.path("libs/glad/include/"));
    c.addIncludePath(b.dependency("stb", .{}).path("."));
    c.addIncludePath(b.dependency("handmade_math", .{}).path("."));

    const yes = b.dependency("yes", .{ .target = target, .optimize = optimize }).module("yes");

    const exe = b.addExecutable(.{
        .name = "animation",
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/main.zig"),
            .target = target,
            .optimize = optimize,
            .imports = &.{
                .{ .name = "c", .module = c.createModule() },
                .{ .name = "yes", .module = yes },
            },
            .link_libc = true,
        }),
    });

    for (c.include_dirs.items) |include_dir| exe.root_module.include_dirs.append(b.allocator, include_dir) catch @panic("OOM");

    exe.addCSourceFiles(.{
        .root = b.path("src/"),
        .files = &.{
            "renderer/renderer.c",
            "renderer/shapes.c",
            "containers/darray.c",
        },
    });

    exe.addCSourceFile(.{ .file = b.path("libs/glad/src/glad.c") });

    b.installArtifact(exe);

    const run_step = b.step("run", "Run the app");
    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);
    run_cmd.step.dependOn(b.getInstallStep());
    run_cmd.addArgs(b.args orelse &.{"lang.lua"});
}
