const std = @import("std");

slice: []const u8,
start: usize = 0,
end: usize = 0,

pub const Token = enum(u8) {
    eof = 0, // null
    invalid = 1,
    identifier = 2,
    number_literal = 3,
    string_literal = 4,
    char_literal = 5,
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
        var seen_dot = false;

        while (self.end < len) {
            const ch = self.slice[self.end];
            if (ch == '.' and !seen_dot) {
                seen_dot = true;
                self.end += 1;
            } else if (std.ascii.isDigit(ch)) {
                self.end += 1;
            } else break;
        }

        return .number_literal;
    }

    if (start == '"') {
        while (self.end < len and self.slice[self.end] != '"') self.end += 1;
        return .string_literal;
    }

    self.end += 1;
    return @enumFromInt(self.slice[self.end - 1]);
}

pub fn skip(self: *@This()) bool {
    return self.next() != null;
}

pub fn lexeme(self: @This()) []const u8 {
    return self.slice[self.start..self.end];
}
