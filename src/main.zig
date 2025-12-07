const std = @import("std");
const yes = @import("yes");
const Tokenizer = @import("Tokenizer.zig");
const Scene = @import("Scene.zig");

pub fn main() !void {
    var gpa: std.heap.DebugAllocator(.{}) = .init;
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var threaded: std.Io.Threaded = .init(allocator);
    defer threaded.deinit();
    const io: std.Io = threaded.io();

    var args = std.process.args();
    _ = args.skip();
    const source_file_name: [:0]const u8 = try while (args.next()) |arg| {
        std.Io.Dir.cwd().access(io, arg, .{}) catch continue;
        break arg;
    } else error.NoSourceFileSpecified;

    const source_file: std.Io.File = try std.Io.Dir.cwd().openFile(io, source_file_name, .{});
    defer source_file.close(io);

    var source_file_slice = try allocator.alloc(u8, @intCast((try source_file.stat(io)).size));
    defer allocator.free(source_file_slice);
    var reader = source_file.reader(io, source_file_slice);
    source_file_slice = try reader.interface.take(source_file_slice.len);

    var tokenizer: Tokenizer = .{ .slice = source_file_slice };

    var scene: Scene = try .init(allocator);
    defer scene.deinit(allocator);
    try scene.construct(&tokenizer);

    var window: yes.Window = try .open(yes.Window.Config{
        .title = "Animationzzz",
        .api = .{ .opengl = .{} },
    });
    defer window.close();
    try yes.opengl.makeCurrent(window);

    main_loop: while (true) {
        while (try window.poll()) |event| switch (event) {
            .close => break :main_loop,
            else => {},
        };
    }
}
