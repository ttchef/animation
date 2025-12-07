const std = @import("std");

pub const Error = i32;
pub const Tokanizer = @import("Tokanizer.zig");

pub export fn tokanizer_init(tokanizer: *Tokanizer, slice: [*:0]const u8) void {
    tokanizer.* = .{
        .slice = std.mem.span(slice),
    };
}

pub export fn tokanizer_next(tokanizer: *Tokanizer) Tokanizer.Token {
    return tokanizer.next() orelse .eof;
}

pub export fn tokanizer_current(tokanizer: *Tokanizer, buffer: [*:0]u8) Error {
    const len = tokanizer.end - tokanizer.start;
    if (std.mem.len(buffer) <= len) return 1;
    @memcpy(buffer[0..len], tokanizer.slice[tokanizer.start..tokanizer.end]);
    buffer[len] = 0;
    return 0;
}

test Tokanizer {
    var tokanizer: Tokanizer = undefined;
    const str: [*:0]const u8 = "Hello, world!";
    tokanizer_init(&tokanizer, str);
    while (true) {
        const token = tokanizer_next(&tokanizer);
        if (token == .eof) break;
        var current: [128:0]u8 = undefined;
        _ = tokanizer_current(&tokanizer, current[0..].ptr);
        std.debug.print("{t} {s}\n", .{ token, current[0..].ptr });
    }
}
