const std = @import("std");
const testing = std.testing;
const tokens = @import("tokens.zig");
const Token = tokens.Token;

pub const TokenizationResult = struct {
    locations: []const Location,
    diagnostics: []const Diagnostic,

    pub fn destroy(
        self: *const TokenizationResult,
        allocator: std.mem.Allocator,
    ) void {
        if (self == &empty_tokenization_result)
            return;
        allocator.free(self.locations);
        allocator.free(self.diagnostics);
        allocator.destroy(self);
    }
};
pub const Location = struct {
    token: Token,
    row: usize,
    column: usize,
};
pub const Diagnostic = union(enum) {
    unterminated_literal: struct {
        row: usize,
        column: usize,
    },
};

const Context = union(enum) {
    literal: struct {
        row: usize,
        column: usize,
        index: usize,
    },
};

const empty_tokenization_result = TokenizationResult{
    .locations = &.{},
    .diagnostics = &.{},
};

pub fn tokenize(
    allocator: std.mem.Allocator,
    src: []const u8,
) !*const TokenizationResult {
    if (src.len == 0)
        return &empty_tokenization_result;
    var context: ?Context = null;
    var locations = std.ArrayList(Location).empty;
    var diagnostics = std.ArrayList(Diagnostic).empty;
    const result = try allocator.create(TokenizationResult);
    var row: usize = 0;
    var column: usize = 0;
    var word_index: ?usize = null;
    var character_index: usize = 0;
    errdefer result.destroy(allocator);
    errdefer locations.deinit(allocator);
    errdefer diagnostics.deinit(allocator);
    while (true) : (character_index += 1) {
        const character = if (character_index < src.len)
            src[character_index]
        else
            null;
        if (character) |char| {
            if (context) |ctx| {
                switch (ctx) {
                    .literal => |c| if (Token.isLiteralDelimiter(char)) {
                        context = null;
                        word_index = null;
                        try locations.append(allocator, .{
                            .token = .{
                                .literal = src[(c.index + Token.literal_delimiter_len)..character_index],
                            },
                            .row = c.row,
                            .column = c.column,
                        });
                    },
                }
            } else if (Token.isLiteralDelimiter(char)) {
                context = .{
                    .literal = .{
                        .row = row,
                        .column = column,
                        .index = character_index,
                    },
                };
                word_index = character_index;
            } else if (word_index == null)
                word_index = character_index;
        }
        if (context) |ctx| {
            switch (ctx) {
                .literal => |location| if (character == null)
                    try diagnostics.append(allocator, .{
                        .unterminated_literal = .{
                            .row = location.row,
                            .column = location.column,
                        },
                    }),
            }
        } else {
            if (word_index) |word_idx| {
                const text = src[word_idx..character_index];
                if (Token.word(text)) |word|
                    try locations.append(allocator, .{
                        .token = word,
                        .row = row,
                        .column = column -| text.len,
                    });
            }
            if (character) |char| {
                if (Token.separator(char)) |separator| {
                    try locations.append(allocator, .{
                        .token = separator,
                        .row = row,
                        .column = column,
                    });
                }
            }
            word_index = null;
        }
        if (character) |char| {
            if (Token.isLineDelimiter(char)) {
                row += 1;
                column = 0;
            } else {
                column += 1;
            }
        } else {
            break;
        }
    }
    result.* = .{
        .locations = try locations.toOwnedSlice(allocator),
        .diagnostics = try diagnostics.toOwnedSlice(allocator),
    };
    return result;
}

test "returns empty slice for empty source" {
    const result = try tokenize(std.testing.allocator, "");
    try testing.expectEqual(empty_tokenization_result, result.*);
    _ = result.destroy(std.testing.allocator);
}

test "tokenizes literal" {
    const result = try tokenize(
        std.testing.allocator,
        "\"Hello, world!\"",
    );
    try testing.expectEqualDeep(
        TokenizationResult{
            .locations = &.{.{
                .token = .{ .literal = "Hello, world!" },
                .row = 0,
                .column = 0,
            }},
            .diagnostics = &.{},
        },
        result.*,
    );
    _ = result.destroy(std.testing.allocator);
}

test "tokenizes link" {
    const result = try tokenize(
        std.testing.allocator,
        "link standard/io",
    );
    try testing.expectEqualDeep(
        TokenizationResult{
            .locations = &.{
                .{ .token = .link, .row = 0, .column = 0 },
                .{
                    .token = .{ .identifier = "standard/io" },
                    .row = 0,
                    .column = 5,
                },
            },
            .diagnostics = &.{},
        },
        result.*,
    );
    _ = result.destroy(std.testing.allocator);
}

test "tokenizes 'hello, world' source" {
    const result = try tokenize(std.testing.allocator,
        \\ link standard/io
        \\
        \\ let main _:@string[] {
        \\   let message = "Hello, world!";
        \\   print message;
        \\ }
    );
    var _tokens = try std.ArrayList(Token).initCapacity(
        std.testing.allocator,
        result.locations.len,
    );
    for (result.locations) |location|
        try _tokens.append(std.testing.allocator, location.token);
    try testing.expectEqualSlices(
        Token,
        &[_]Token{
            .link,
            .whitespace,
            .{ .identifier = "standard/io" },
            .newline,
            .newline,
            .let,
            .{ .identifier = "main" },
            .whitespace,
            .{ .identifier = "_" },
            .colon,
            .at,
            .{ .identifier = "string" },
            .left_square_bracket,
            .right_square_bracket,
            .whitespace,
            .left_curly_brace,
            .newline,
            .whitespace,
            .whitespace,
            .let,
            .whitespace,
            .{ .identifier = "message" },
            .whitespace,
            .equals,
            .whitespace,
            .{ .literal = "Hello, world!" },
            .semicolon,
            .newline,
            .whitespace,
            .whitespace,
            .{ .identifier = "print" },
            .whitespace,
            .{ .identifier = "message" },
        },
        _tokens.items,
    );
    _ = result.destroy(std.testing.allocator);
    tokens.deinit(std.testing.allocator);
}
