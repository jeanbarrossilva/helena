const std = @import("std");
const lexer = @import("helena").lexer;

pub fn main() !void {
    var allocator_wrapper = std.heap.DebugAllocator(.{}){};
    const allocator = allocator_wrapper.allocator();
    defer _ = allocator_wrapper.deinit();
    _ = try lexer.tokenize(allocator, "");
    std.debug.print("Hello, world!\n", .{});
}
