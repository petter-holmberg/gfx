#include "gfx.h"

#include <chrono>
#include <thread>

int main()
{
    using namespace std::chrono_literals;

    // Create a fullscreen window with the current resolution
    gfx::window window;

    // Create a canvas in the window to draw on
    gfx::canvas can{window};

    // Vertical bars with predefined colors
    auto const bar_width = can.size().x / 16;
    auto const bar_height = can.size().y;
    gfx::draw_rect(can, {0 * bar_width, 0}, {bar_width, bar_height}, gfx::black, gfx::fill::on);
    gfx::draw_rect(can, {1 * bar_width, 0}, {bar_width, bar_height}, gfx::silver, gfx::fill::on);
    gfx::draw_rect(can, {2 * bar_width, 0}, {bar_width, bar_height}, gfx::gray, gfx::fill::on);
    gfx::draw_rect(can, {3 * bar_width, 0}, {bar_width, bar_height}, gfx::white, gfx::fill::on);
    gfx::draw_rect(can, {4 * bar_width, 0}, {bar_width, bar_height}, gfx::maroon, gfx::fill::on);
    gfx::draw_rect(can, {5 * bar_width, 0}, {bar_width, bar_height}, gfx::red, gfx::fill::on);
    gfx::draw_rect(can, {6 * bar_width, 0}, {bar_width, bar_height}, gfx::purple, gfx::fill::on);
    gfx::draw_rect(can, {7 * bar_width, 0}, {bar_width, bar_height}, gfx::fuchsia, gfx::fill::on);
    gfx::draw_rect(can, {8 * bar_width, 0}, {bar_width, bar_height}, gfx::green, gfx::fill::on);
    gfx::draw_rect(can, {9 * bar_width, 0}, {bar_width, bar_height}, gfx::lime, gfx::fill::on);
    gfx::draw_rect(can, {10 * bar_width, 0}, {bar_width, bar_height}, gfx::olive, gfx::fill::on);
    gfx::draw_rect(can, {11 * bar_width, 0}, {bar_width, bar_height}, gfx::yellow, gfx::fill::on);
    gfx::draw_rect(can, {12 * bar_width, 0}, {bar_width, bar_height}, gfx::navy, gfx::fill::on);
    gfx::draw_rect(can, {13 * bar_width, 0}, {bar_width, bar_height}, gfx::blue, gfx::fill::on);
    gfx::draw_rect(can, {14 * bar_width, 0}, {bar_width, bar_height}, gfx::teal, gfx::fill::on);
    gfx::draw_rect(can, {15 * bar_width, 0}, {bar_width, bar_height}, gfx::aqua, gfx::fill::on);

    // White circle with black border
    gfx::color_set(can, gfx::white);
    gfx::draw_circle(can, {can.first() + (can.size() / 2)}, can.size().y / 2, gfx::fill::on);
    gfx::draw_circle(can, {can.first() + (can.size() / 2)}, can.size().y / 2, gfx::black);

    // sRGB color blending
    auto start{can.first() + can.size() / 2 - gfx::vector{256 + 128, can.size().y / 4 + 50}};
    for (auto i = 0; i < 256; ++i) {
        gfx::draw_line(can, start + gfx::vector{i, 0}, start + gfx::vector{i, 100}, gfx::color_blend(gfx::green, gfx::red, i / 256.f));
    }
    for (auto i = 0; i < 256; ++i) {
        gfx::draw_line(can, start + gfx::vector{i + 256, 0}, start + gfx::vector{i + 256, 100}, gfx::color_blend(gfx::blue, gfx::green, i / 256.f));
    }
    for (auto i = 0; i < 256; ++i) {
        gfx::draw_line(can, start + gfx::vector{i + 512, 0}, start + gfx::vector{i + 512, 100}, gfx::color_blend(gfx::red, gfx::blue, i / 256.f));
    }

    // Black diagonal line from top-left to bottom-right corner
    gfx::draw_line(can, can.first(), can.last(), gfx::black);

    // Red dotted diagonal line made out of points from middle to bottom edge
    for (auto i = 0; i < can.size().y / 2; i += 2) {
        gfx::draw_point(can, can.first() + (can.size() / 2) + gfx::vector{i, i}, gfx::red);
        gfx::draw_point(can, can.first() + (can.size() / 2) + gfx::vector{-i, i}, gfx::yellow);
    }

    // Centered texture
    auto tex = gfx::texture::load(can, "test.png");
    if (tex) {
        gfx::draw_texture(can, *tex, {(can.first() + (can.size() / 2 - tex->size() / 2)).x, (can.last() - tex->size()).y + 1});
    }

    // Centered text
    auto font = gfx::font::load("test.ttf", 48);
    if (font) {
        std::string text{"The quick brown fox jumps over the lazy dog"};
        auto text_size{gfx::font::text_size(*font, text)};
        gfx::draw_text(can, text, *font, can.first() + (can.size() / 2 - text_size / 2), gfx::black);
    }

    // Render the canvas
    gfx::render(can);

    // Sleep to allow viewing of the result
    std::this_thread::sleep_for(5000ms);
}
