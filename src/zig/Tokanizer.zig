const std = @import("std");

pub fn parse() void {
    _ = std.c.printf("Hello, world!\n");

    _ = @Int(.signed, 10);
}
