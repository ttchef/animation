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
        pos: @Vector(3, f32) = @splat(0),
        color: [8]u8 = @splat('F'), // hex #FFFFFFFF
        easing: Easing = .lerp,

        pub const Easing = enum {
            lerp,
        };
    };
};

pub fn init(allocator: std.mem.Allocator) !@This() {
    _ = allocator;
    return .{};
}

pub fn deinit(self: @This(), allocator: std.mem.Allocator) void {
    self.actors.deinit(allocator);
}

pub fn construct(self: *@This(), allocator: std.mem.Allocator, tokenizer: *Tokenizer) !void {
    while (tokenizer.next()) |kind| switch (kind) {
        .identifier => {
            if (std.meta.stringToEnum(Actor.Tag, tokenizer.lexeme())) |actor_tag| switch (actor_tag) {
                .shape => {
                    const token = tokenizer.next() orelse return error.MissingIdentifier;
                    if (token != .identifier) return error.TokenMustBeIdentifier;

                    const shape_name = tokenizer.lexeme();
                    var shape: Actor.Shape = .{};
                    shape.pos = .{ 0, 0, 1 };

                    try self.actors.put(allocator, shape_name, .{ .shape = shape });
                    std.debug.print("Shape: {s}\n", .{tokenizer.lexeme()});
                },
            };
        },
        else => {},
    };
}
