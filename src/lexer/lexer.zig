const std = @import("std");
const testing = std.testing;
const utf8 = @import("utf8.zig");

const tokens = @import("tokens.zig");
const LocalizedToken = tokens.LocalizedToken;
const Token = tokens.Token;

pub const ParsedSource = struct {
    tokens: []const LocalizedToken,
    diagnostics: []const Diagnostic,
};
pub const Diagnostic = struct {
    text: []const u8,
    row: usize,
    column: usize,
    message: []const u8,
};

const empty_parsed_source = ParsedSource{
    .tokens = &.{},
    .diagnostics = &.{},
};

pub fn tokenize(allocator: std.mem.Allocator, source: []const u8) !ParsedSource {
    if (source.len == 0)
        return empty_parsed_source;
    var index: usize = 0;
    var row: usize = 0;
    var column: usize = 0;
    var head: ?usize = null;
    var _tokens = std.ArrayList(LocalizedToken).empty;
    var diagnostics = std.ArrayList(Diagnostic).empty;
    var is_in_literal = false;
    while (true) {
        const character = if (index < source.len) source[index] else 0;
        if (is_in_literal) {
            if (tokens.isLiteralDelimiter(character)) {
                if (head) |_head| {
                    try _tokens.append(allocator, .{
                        .unlocalized = .{
                            .literal = source[(_head + 1)..index],
                        },
                        .row = row,
                        .column = column,
                    });
                    head = null;
                    is_in_literal = false;
                }
            }
        } else if (tokens.isLiteralDelimiter(character)) {
            head = index;
            is_in_literal = true;
        } else if (character == ' ' or character == '\n' or character == '\t') {
            if (Token.find(&.{character})) |separator_type|
                try _tokens.append(allocator, .{
                    .unlocalized = separator_type,
                    .row = row,
                    .column = column,
                });
        } else if (head == null) {
            head = index;
        } else if (head) |_head| {
            const text = source[_head..index];
            if (Token.find(text)) |token_type| {
                try _tokens.append(allocator, .{
                    .unlocalized = token_type,
                    .row = row,
                    .column = column,
                });
            } else {
                try diagnostics.append(allocator, .{
                    .text = text,
                    .row = row,
                    .column = column,
                    .message = try std.fmt.allocPrint(
                        allocator,
                        "Unexpected token: {s}",
                        .{text},
                    ),
                });
            }
            head = null;
        }
        switch (character) {
            0 => break,
            '\n' => row += 1,
            else => column += 1,
        }
        index += 1;
    }
    _tokens.deinit(allocator);
    diagnostics.deinit(allocator);
    return .{
        .tokens = _tokens.items,
        .diagnostics = &.{},
    };
}

test "returns empty slice for empty source" {
    const parsed_source = try tokenize(std.testing.allocator, "");
    try testing.expectEqual(parsed_source.tokens.len, 0);
}

test "tokenizes 'hello, world' source" {
    const allocator = std.testing.allocator;
    const parsed_source = try tokenize(allocator,
        \\ link standard/io
        \\
        \\ let main _:@string[] {
        \\   let message = "Hello, world!";
        \\   print message;
        \\ }
    );
    var token_types = try std.ArrayList(Token).initCapacity(
        allocator,
        parsed_source.tokens.len,
    );
    for (parsed_source.tokens) |token|
        try token_types.append(allocator, token.unlocalized);
    try testing.expectEqualSlices(
        Token,
        &[_]Token{
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
        },
        token_types.items,
    );
}
