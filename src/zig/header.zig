const std = @import("std");

pub const Error = i32;
pub const Tokenizer = @import("Tokenizer.zig");

pub export fn tokenizer_init(tokenizer: *Tokenizer, slice: [*:0]const u8) void {
    tokenizer.* = .{
        .slice = std.mem.span(slice),
    };
}

pub export fn tokenizer_next(tokenizer: *Tokenizer) Tokenizer.Token {
    return tokenizer.next() orelse .eof;
}

pub export fn tokenizer_current(tokenizer: *Tokenizer, buffer: [*:0]u8) Error {
    const len = tokenizer.end - tokenizer.start;
    if (std.mem.len(buffer) <= len) return 1;
    @memcpy(buffer[0..len], tokenizer.slice[tokenizer.start..tokenizer.end]);
    buffer[len] = 0;
    return 0;
}

test Tokenizer {
    var tokenizer: Tokenizer = undefined;
    const str: [*:0]const u8 = "Hello, world!";
    tokenizer_init(&tokenizer, str);
    while (true) {
        const token = tokenizer_next(&tokenizer);
        if (token == .eof) break;
        var current: [128:0]u8 = undefined;
        _ = tokenizer_current(&tokenizer, current[0..].ptr);
        std.debug.print("{t} {s}\n", .{ token, current[0..].ptr });
    }
}
