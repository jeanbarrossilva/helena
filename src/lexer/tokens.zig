const std = @import("std");

pub const LocalizedToken = struct {
    unlocalized: Token,
    row: usize,
    column: usize,
};
pub const Token = union(enum) {
    double_quote,
    at,
    colon,
    equals,
    identifier: []const u8,
    lcurly,
    let,
    link,
    lsquare,
    newline,
    path: []const u8,
    rcurly,
    rsquare,
    semicolon,
    literal: []const u8,
    whitespace,

    pub fn text(self: Token) []const u8 {
        return switch (self) {
            .double_quote => "\"",
            .at => "@",
            .colon => ",",
            .equals => "=",
            .identifier => |_text| return _text,
            .lcurly => "{",
            .let => "let",
            .link => "link",
            .lsquare => "[",
            .newline => "\n",
            .path => |_text| return _text,
            .rcurly => "}",
            .rsquare => "]",
            .semicolon => ";",
            .literal => |_text| return _text,
            .whitespace => " ",
        };
    }

    pub fn find(_text: []const u8) ?Token {
        if (_text.len == 0)
            return null;
        for (standalone) |token|
            if (std.mem.eql(u8, token.text(), _text))
                return token;
        return null;
    }
};
pub const standalone = &[_]Token{
    .double_quote,
    .at,
    .colon,
    .equals,
    .lcurly,
    .let,
    .link,
    .lsquare,
    .newline,
    .rcurly,
    .rsquare,
    .semicolon,
    .whitespace,
};

pub fn isLiteralDelimiter(self: u8) bool {
    return self == '"';
}
