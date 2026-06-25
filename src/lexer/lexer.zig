const std = @import("std");
const testing = std.testing;

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

    fn text(self: Token) []const u21 {
        return switch (self) {
            .double_quote => "\"",
            .at => "@",
            .colon => ",",
            .equals => "=",
            .identifier => |_text| _text,
            .lcurly => "{",
            .let => "let",
            .link => "link",
            .lsquare => "[",
            .newline => "\n",
            .path => |_text| _text,
            .rcurly => "}",
            .rsquare => "]",
            .semicolon => ";",
            .literal => |_text| _text,
            .whitespace => " ",
        };
    }
};

const standalone_tokens = &[_]Token{
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

pub fn tokenize(allocator: std.mem.Allocator, source: []const u8) ![]Token {
    if (source.len == 0)
        return &[_]Token{};
    const source_view = try std.unicode.Utf8View.init(source);
    var source_iterator = source_view.iterator();
    var tokens = std.ArrayList(Token).empty;
    var query = std.ArrayList(u21).empty;
    var results = std.ArrayList(Token).empty;
    while (true) {
        const codepoints = source_iterator.peek(1);
        if (codepoints.len == 0)
            break;
        try query.append(allocator, codepoints[0]);
        for (standalone_tokens) |token| {
            const text = token.text();
            if (!lookUp(source_iterator, text))
                continue;
            for (0..results.items.len) |index|
                if (!isPrefixed(results.items[index].text(), query.items))
                    results.orderedRemove(index);
            try results.append(allocator, token);
        }
        if (results.len > 0) {
            tokens.append(allocator, results[0]);
        } else {
            // an unknown token was found here. a diagnostic should be issued,
            // containing information about such token and its location in the
            // source.
        }
    }
    results.deinit(allocator);
    query.deinit(allocator);
    tokens.deinit(allocator);
    return tokens;
}

fn lookUp(iterator: std.unicode.Utf8Iterator, text: []const u21) bool {
    // because we are the ones passing 'text' into here, no check on its
    // contents (e.g., is it empty?) is done. we always assume that it's
    // sensical.
    if (iterator.peek(text.len) != text)
        return false;
    for (0..text.len) |_|
        iterator.nextCodepoint();
    return true;
}

fn isPrefixed(self: []u21, prefix: []u21) bool {
    if (self.ptr == prefix.ptr) return true;
    if (self.len < prefix.len) return false;
    for (0..prefix.len) |index|
        if (self[index] != prefix[index])
            return false;
}

test "returns empty slice for empty source" {
    const tokens = try tokenize(std.testing.allocator, "");
    try testing.expectEqual(tokens.len, 0);
}

test "tokenizes 'hello, world' source" {
    const tokens = try tokenize(std.testing.allocator,
        \\ link standard/io
        \\
        \\ let main _:@string[] {
        \\   let message = "Hello, world!";
        \\   print message;
        \\ }
    );
    try testing.expectEqualSlices(Token, &[_]Token{
        .link,
        .whitespace,
        .{ .path = "standard/io" },
        .newline,
        .newline,
        .let,
        .{ .identifier = "main" },
        .whitespace,
        .{ .identifier = "_" },
        .colon,
        .at,
        .{ .identifier = "string" },
        .lsquare,
        .rsquare,
        .whitespace,
        .lcurly,
        .newline,
        .whitespace,
        .whitespace,
        .let,
        .whitespace,
        .{ .identifier = "message" },
        .whitespace,
        .equals,
        .whitespace,
        .double_quote,
        .{ .literal = "Hello, world!" },
        .double_quote,
        .semicolon,
        .newline,
        .whitespace,
        .whitespace,
        .{ .identifier = "print" },
        .whitespace,
        .{ .identifier = "message" },
    }, tokens);
}
