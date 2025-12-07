const std = @import("std");

comptime {
    _ = @import("header.zig");
}

pub fn parse() void {
    _ = std.c.printf("Hello, world!\n");

    _ = @Int(.signed, 10);
}
