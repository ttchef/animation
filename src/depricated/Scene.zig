const std = @import("std");
const Tokenizer = @import("Tokenizer.zig");

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
            linear,
            lerp,
            bezier,
        };
    };
};

pub fn init(allocator: std.mem.Allocator) !@This() {
    _ = allocator;
    return .{};
}

pub fn deinit(self: *@This(), allocator: std.mem.Allocator) void {
    self.actors.deinit(allocator);
}

pub fn construct(self: *@This(), allocator: std.mem.Allocator, tokenizer: *Tokenizer) !void {
    while (tokenizer.next()) |kind| switch (kind) {
        .identifier => {
            if (std.meta.stringToEnum(Actor.Tag, tokenizer.lexeme())) |actor_tag| switch (actor_tag) {
                .shape => {
                    try tokenizer.expect(.identifier);
                    const shape_name = tokenizer.lexeme();
                    const shape = try constructShape(tokenizer);

                    try self.actors.put(allocator, shape_name, .{ .shape = shape });
                },
            };
        },
        else => {},
    };

    var it = self.actors.iterator();
    while (it.next()) |actor| {
        std.debug.print("{any}\n", .{actor.value_ptr.shape});
    }
}

pub fn constructShape(tokenizer: *Tokenizer) !Actor.Shape {
    var shape: Actor.Shape = .{};

    try tokenizer.expect(.l_brace);

    while (true) {
        const backup = tokenizer.*;
        {
            const token = tokenizer.next() orelse return error.NoEnd;
            if (token == .r_brace) break;
            if (token != .identifier) return error.ExpectedIdentifier;
        }

        const field = std.meta.stringToEnum(std.meta.FieldEnum(Actor.Shape), tokenizer.lexeme()) orelse {
            if (std.mem.eql(u8, tokenizer.lexeme(), "action")) return shape;
            std.log.err("Invalid field {s}", .{tokenizer.lexeme()});
            return error.InvalidField;
        };

        tokenizer.expect(.equal) catch {
            tokenizer.* = backup;
            break;
        };

        switch (field) {
            .pos => {
                try tokenizer.expect(.l_paren);
                while (tokenizer.next()) |token| {
                    if (token == .r_paren) break;
                }
                shape.pos = @splat(10);
            },
            .color => {
                try tokenizer.expect(.string_literal);
                if (tokenizer.lexeme().len != 8) return error.InvalidHex;
                @memcpy(shape.color[0..], tokenizer.lexeme()[0..8]);
            },
            .easing => {
                try tokenizer.expect(.dot);
                try tokenizer.expect(.identifier);
                shape.easing = std.meta.stringToEnum(Actor.Shape.Easing, tokenizer.lexeme()) orelse return error.InvalidEasingFunction;
            },
        }
    }

    return shape;
}
