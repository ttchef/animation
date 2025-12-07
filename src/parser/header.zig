const source = @import("parser.zig");

pub export fn parse() void {
    source.parse();
}
