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
    c.addIncludePath(b.path("libs/glad/include/"));
    c.addIncludePath(b.dependency("stb", .{}).path("."));

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

    exe.addIncludePath(b.path("include/"));
    exe.addIncludePath(b.path("libs/glad/include/"));
    exe.addIncludePath(b.dependency("stb", .{}).path("."));

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
