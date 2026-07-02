const std = @import("std");

pub const Token = union(enum) {
    asterisk,
    at,
    colon,
    comma,
    dot,
    equals,
    exclamation,
    identifier: []const u8,
    left_curly_brace,
    left_parenthesis,
    left_square_bracket,
    let,
    link,
    literal: []const u8,
    newline,
    number: struct {
        text: []const u8,
        is_integer: bool,
    },
    right_curly_brace,
    right_parenthesis,
    right_square_bracket,
    semicolon,
    tab,
    whitespace,

    pub const literal_delimiter_len = 1;

    pub fn format(
        self: Token,
        comptime _: []const u8,
        _: std.fmt.FormatOptions,
        writer: anytype,
    ) !void {
        return switch (self) {
            .identifier => |t| writer.print(
                ".identifier = {}",
                .{std.fmt.fmtSliceEscapeUpper(t)},
            ),
            .literal => |t| writer.print(
                ".literal = \"{}\"",
                .{std.fmt.fmtSliceEscapeUpper(t)},
            ),
            .number => |t| writer.print(".number = {d}", .{
                if (t.is_integer)
                    std.fmt.parseInt(isize, t.text, 10)
                else _: {
                    const decimal_len = t.text.len - std.mem.indexOfScalar(
                        u8,
                        &t.text,
                        '.',
                    );
                    break :_ if (decimal_len <= 7)
                        std.fmt.parseFloat(f32, t.text)
                    else if (decimal_len <= 16)
                        std.fmt.parseFloat(f64, t.text)
                    else
                        std.fmt.parseFloat(f128, t.text);
                },
            }),
            else => writer.writeAll(@tagName(self)),
        };
    }

    pub fn separator(text: u8) ?Token {
        return switch (text) {
            '\n' => .newline,
            '\t' => .tab,
            ' ' => .whitespace,
            else => null,
        };
    }

    pub fn word(text: []const u8) ?Token {
        return if (text.len == 0)
            null
        else if (findNumber(text)) |num|
            num
        else if (staticWord(text)) |w|
            w
        else
            .{ .identifier = text };
    }

    pub fn isLiteralDelimiter(self: u8) bool {
        return self == '"';
    }

    fn findNumber(text: []const u8) ?Token {
        var is_integer = true;
        for (text, 0..) |char, index| {
            if (index > 0 and char == '.') {
                if (is_integer) {
                    is_integer = false;
                } else {
                    break;
                }
            } else if (!std.ascii.isDigit(char))
                break;
            if (index == text.len - 1)
                return .{ .number = .{ .text = text, .is_integer = is_integer } };
        }
        return null;
    }

    fn staticWord(text: []const u8) ?Token {
        const character = if (text.len == 1) text[0] else null;
        return if (character) |char| {
            return switch (char) {
                '*' => .asterisk,
                '@' => .at,
                ':' => .colon,
                ',' => .comma,
                '.' => .dot,
                '=' => .equals,
                '!' => .exclamation,
                '{' => .left_curly_brace,
                '(' => .left_parenthesis,
                '[' => .left_square_bracket,
                '}' => .right_curly_brace,
                ')' => .right_parenthesis,
                ']' => .right_square_bracket,
                ';' => .semicolon,
                '\t' => .tab,
                ' ' => .whitespace,
                else => null,
            };
        } else if (std.mem.eql(u8, "let", text))
            .let
        else if (std.mem.eql(u8, "link", text))
            .link
        else
            null;
    }

    pub fn isLineDelimiter(text: u8) bool {
        return text == '\n';
    }
};

test "word" {
    try std.testing.expectEqualDeep(
        Token{ .identifier = "helena" },
        Token.word("helena"),
    );
    try std.testing.expectEqualDeep(
        Token{ .number = .{ .text = "2003", .is_integer = true } },
        Token.word("2003"),
    );
    try std.testing.expectEqualDeep(
        Token{ .number = .{ .text = "3.14", .is_integer = false } },
        Token.word("3.14"),
    );
}

test "staticWord" {
    try std.testing.expectEqualDeep(null, Token.staticWord(""));
    try std.testing.expectEqualDeep(.asterisk, Token.staticWord("*"));
    try std.testing.expectEqualDeep(.at, Token.staticWord("@"));
    try std.testing.expectEqualDeep(.colon, Token.staticWord(":"));
    try std.testing.expectEqualDeep(.comma, Token.staticWord(","));
    try std.testing.expectEqualDeep(.dot, Token.staticWord("."));
    try std.testing.expectEqualDeep(.equals, Token.staticWord("="));
    try std.testing.expectEqualDeep(
        .exclamation,
        Token.staticWord("!"),
    );
    try std.testing.expectEqualDeep(.left_curly_brace, Token.staticWord("{"));
    try std.testing.expectEqualDeep(.left_parenthesis, Token.staticWord("("));
    try std.testing.expectEqualDeep(.left_square_bracket, Token.staticWord("["));
    try std.testing.expectEqualDeep(.let, Token.staticWord("let"));
    try std.testing.expectEqualDeep(.link, Token.staticWord("link"));
    try std.testing.expectEqualDeep(.right_curly_brace, Token.staticWord("}"));
    try std.testing.expectEqualDeep(.right_parenthesis, Token.staticWord(")"));
    try std.testing.expectEqualDeep(.right_square_bracket, Token.staticWord("]"));
    try std.testing.expectEqualDeep(.semicolon, Token.staticWord(";"));
    for ([_]usize{0} ++ std.ascii.lowercase) |character|
        try std.testing.expectEqual(null, Token.staticWord(
            &[_]u8{@intCast(character)},
        ));
}

test "separator" {
    try std.testing.expectEqualDeep(.newline, Token.separator('\n'));
    try std.testing.expectEqualDeep(.tab, Token.separator('\t'));
    try std.testing.expectEqualDeep(.whitespace, Token.separator(' '));
    for ([_]usize{0} ++ std.ascii.lowercase) |character|
        try std.testing.expectEqual(
            null,
            Token.separator(@intCast(character)),
        );
}

test "isLiteralDelimiter" {
    try std.testing.expect(Token.isLiteralDelimiter('"'));
    for (std.ascii.lowercase) |character|
        try std.testing.expect(!Token.isLiteralDelimiter(character));
}
