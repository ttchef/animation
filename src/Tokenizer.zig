const std = @import("std");

file_path: []const u8,
slice: []const u8,
start: usize = 0,
end: usize = 0,
current_token: Token = .eof,

pub const Token = enum(u8) {
    eof = 0, // null
    invalid = 1,
    identifier = 2,
    integer_literal = 3,
    float_literal = 4,
    string_literal = 5,
    char_literal = 6,
    dot = '.',
    comma = ',',
    colon = ':',
    bang = '!',
    pipe = '|',
    equal = '=',
    semicolon = ';',
    ampersand = '&',
    question_mark = '?',
    percent = '%',
    plus = '+',
    minus = '-',
    asterisk = '*',
    slash = '/',
    l_paren = '(',
    r_paren = ')',
    l_brace = '{',
    r_brace = '}',
    l_bracket = '[',
    r_bracket = ']',
    l_angle_bracket = '<',
    r_angle_bracket = '>',
};

/// Returns null on EOF not .eof
pub fn next(self: *@This()) ?Token {
    const len = self.slice.len;

    self.start = self.end;
    while (self.start < len and std.ascii.isWhitespace(self.slice[self.start])) self.start += 1;

    if (self.start >= len) {
        self.end = self.start;
        return null;
    }

    self.end = self.start;
    const start = self.slice[self.start];

    if (std.ascii.isAlphabetic(start) or start == '_') {
        self.end += 1;
        while (self.end < len) {
            const ch = self.slice[self.end];
            if (!(std.ascii.isAlphanumeric(ch) or ch == '_')) break;
            self.end += 1;
        }
        return .identifier;
    }

    if (std.ascii.isDigit(start)) {
        self.end += 1;
        var float = false;

        while (self.end < len) {
            const ch = self.slice[self.end];
            if (ch == '.' and !float) {
                float = true;
                self.end += 1;
            } else if (std.ascii.isDigit(ch)) {
                self.end += 1;
            } else break;
        }

        return if (float) .float_literal else .integer_literal;
    }

    if (self.slice[self.start] == '"') {
        self.end = self.start + 1;

        while (self.end < len and self.slice[self.end] != '"') self.end += 1;
        if (self.end >= len) return .invalid;
        self.end += 1;
        return .string_literal;
    }

    self.end += 1;
    return @enumFromInt(self.slice[self.end - 1]);
}

/// Returns `true` if skipped a token, `false` if we are at the eof.
pub fn skip(self: *@This()) bool {
    return self.next() != null;
}

pub fn expect(self: *@This(), target: Token) !void {
    const found = self.next() orelse return error.ExpectedTokenFoundEof;
    if (found != target) {
        self.errorOut();
        var line_start: usize = self.start;
        while (self.start > 0 and self.slice[line_start] != '\n') line_start -= 1;
        line_start += 1;
        var line_end: usize = self.end;
        while (line_end < self.end and self.slice[line_end] != '\n') line_end += 1;
        var empty: [128]u8 = @splat(' ');
        empty[self.end - line_start - 1] = '^';
        std.log.err("Expected {t} found '{s}'\n{s}\n{s}", .{ target, self.lexeme(), self.slice[line_start..line_end], empty });
        return error.ExpectedToken;
    }
}

pub fn errorOut(self: @This()) void {
    var line_number: usize = 1;
    for (self.slice[0..self.end]) |char| {
        if (char == '\n') line_number += 1;
    }
    var line_start: usize = self.start;
    while (self.start > 0 and self.slice[line_start] != '\n') line_start -= 1;
    std.debug.print("{s}:{d}:{d}: ", .{ self.file_path, line_number, self.end - line_start - 1 });
}

pub fn lexeme(self: @This()) []const u8 {
    return if (self.slice[self.start] == '"') self.slice[self.start + 1 .. self.end - 1] else self.slice[self.start..self.end];
}
