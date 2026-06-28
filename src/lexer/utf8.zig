const std = @import("std");

pub fn lookUp(iterator: *std.unicode.Utf8Iterator, text: std.unicode.Utf8View) !bool {
    // because we are the ones passing 'text' into here, no check on its
    // contents (e.g., is it empty?) is done. we always assume that it's
    // sensical.

    const len = try std.unicode.utf8CountCodepoints(text.bytes);
    if (!std.mem.eql(u8, iterator.peek(len), text.bytes))
        return false;
    for (0..len) |_|
        _ = iterator.nextCodepoint();
    return true;
}
