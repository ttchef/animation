const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

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
    });

    exe.addCSourceFiles(.{
        .root = b.path("src/"),
        .files = &.{
            "main.c",
        },
    });

    exe.addIncludePath(b.path("include/"));

    exe.linkLibrary(zig);

    b.installArtifact(exe);

    const run_step = b.step("run", "Run the app");
    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| run_cmd.addArgs(args);
}
