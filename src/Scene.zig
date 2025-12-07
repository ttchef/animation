const std = @import("std");
const Tokenizer = @import("Tokenizer.zig");

current_actor: *Actor = undefined,
actors: std.StringHashMapUnmanaged(Actor) = .empty,

pub const Actor = union(Tag) {
    shape: Shape,

    pub const Tag = enum {
        shape,
    };

    pub const Shape = struct {
        pos: @Vector(2, f32),
        color: [8]u8, // hex #FFFFFF
    };
};

pub fn init(allocator: std.mem.Allocator) !@This() {
    _ = allocator;
    return .{};
}

pub fn deinit(_: @This(), allocator: std.mem.Allocator) void {
    _ = allocator;
}

pub fn construct(self: *@This(), tokenizer: *Tokenizer) !void {
    _ = self;
    while (tokenizer.next()) |kind| switch (kind) {
        .identifier => {
            if (std.meta.stringToEnum(Actor.Tag, tokenizer.lexeme())) |actor_tag| switch (actor_tag) {
                .shape => {
                    std.debug.print("Shape: {s}\n", .{tokenizer.lexeme()});
                },
            };
        },
        else => {},
    };
}
