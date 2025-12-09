const std = @import("std");

slice: [:0]const u8,
start: usize = 0,
end: usize = 0,

pub const Token = enum(u8) {
    // eof = null
    invalid,
    identifier,
    string_literal,
    char_literal,
    int,
    float,
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
    tilde = '~',
};

pub fn peek(self: @This()) ?Token {
    var tmp: @This() = self;
    return tmp.next();
}

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

        return if (float) .float else .int;
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

pub fn lexeme(self: @This()) []const u8 {
    return if (self.slice[self.start] == '"') self.slice[self.start + 1 .. self.end - 1] else self.slice[self.start..self.end];
}

pub fn printInfo(self: @This(), file_name: []const u8) void {
    var line_number: usize = 1;

    for (self.slice[0..self.end]) |c| {
        if (c == '\n') line_number += 1;
    }

    var line_start: usize = self.start;
    while (line_start > 0 and self.slice[line_start - 1] != '\n') line_start -= 1;

    const col = self.start - line_start + 1;
    std.debug.print("{s}:{d}:{d}: ", .{ file_name, line_number, col });
}
