/*
gfx - A simple C++20 library for 2D graphics. Version 0.0.1.

Written in 2023 by Petter Holmberg petter.holmberg@usa.net

Uses libsdl: https://www.libsdl.org/

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

It is also recommended that you include a file called COPYING (or COPYING.txt) containing the CC0 legalcode as plain text.
*/
#include "gfx.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

#include "gfx_impl.h"

namespace {

struct gfx_global
{
    ~gfx_global()
    {
        gfx::impl::global_context_destroy();
    }

    gfx_global()
    {
        gfx::impl::global_context_create();
    }

    gfx_global(gfx_global const&) noexcept = delete;

    gfx_global& operator=(gfx_global const&) noexcept = delete;
};

gfx_global gfx_global_context;

float srgb2linear(float x)
{
    return x * (x * (x * 0.30530611f + 0.682171111f) + 0.012522878f);
}

float linear2srgb(float x)
{
    float s0 = sqrtf(x);
    float s1 = sqrtf(s0);
    float s2 = sqrtf(s1);
    return std::clamp(0.662002687f * s0 + 0.684122060f * s1 - 0.323583601f * s2 - 0.0225411470f * x, 0.f, 1.f);
}

uint8_t blend_component(float x, float y, float fraction)
{
    return static_cast<uint8_t>(linear2srgb(srgb2linear(float(x) / 256.f) * fraction + srgb2linear(float(y) / 256.f) * (1 - fraction)) * 256.f);
}

}

namespace gfx {

namespace v0 {

color color_blend(color c0, color c1, float fraction) noexcept
{
    return {
        blend_component(c0.r, c1.r, fraction),
        blend_component(c0.g, c1.g, fraction),
        blend_component(c0.b, c1.b, fraction),
        static_cast<uint8_t>(((float(c0.a) / 256.f) * fraction + (float(c1.a) / 256.f) * (1 - fraction)) * 256.f)
    };
}

window::~window()
{
    impl::window_destroy(handle);
}

window::window(point pos, vector size, std::string const& title, visibility vis) noexcept
    : handle{impl::window_create(pos, size, title, vis)}
{}

window& window::operator=(window&& rhs) noexcept
{
    auto* temp = rhs.handle;
    rhs.handle = nullptr;
    impl::window_destroy(handle);
    handle = temp;
    return *this;
}

void window::show() noexcept
{
    impl::window_show(handle);
}

void window::hide() noexcept
{
    impl::window_hide(handle);
}

texture::~texture()
{
    impl::texture_destroy(handle);
}

texture& texture::operator=(texture&& rhs) noexcept
{
    auto* temp = rhs.handle;
    rhs.handle = nullptr;
    impl::texture_destroy(handle);
    handle = temp;
    return *this;
}

[[nodiscard]] vector texture::size() const noexcept
{
    return impl::texture_size(handle);
}

[[nodiscard]] std::optional<texture>
texture::load(canvas& can, std::filesystem::path const& path) noexcept
{
    void* tp = impl::texture_load(can.handle, path);
    if (tp == nullptr) {
        return {};
    } else {
        return texture{tp};
    }
}

font::~font()
{
    impl::font_destroy(handle);
}

font& font::operator=(font&& rhs) noexcept
{
    auto* temp = rhs.handle;
    rhs.handle = nullptr;
    impl::font_destroy(handle);
    handle = temp;
    return *this;
}

[[nodiscard]] std::optional<font>
font::load(std::filesystem::path const& path, int32_t size) noexcept
{
    void* fp = impl::font_create(path.c_str(), size);
    if (fp == nullptr) {
        return {};
    } else {
        return font{fp};
    }
}

[[nodiscard]] vector
font::text_size(font& f, char const* text) noexcept
{
    return impl::font_text_size(f.handle, text);
}

[[nodiscard]] vector
font::text_size(font& f, std::string const& text) noexcept
{
    return text_size(f, text.c_str());
}

canvas::~canvas()
{
    impl::canvas_destroy(handle);
}

canvas::canvas(window const& window, vsync vs) noexcept
    : handle{impl::canvas_create(window.handle, vs)}
{}

canvas& canvas::operator=(canvas&& rhs) noexcept
{
    auto* temp = rhs.handle;
    rhs.handle = nullptr;
    impl::canvas_destroy(handle);
    handle = temp;
    return *this;
}

[[nodiscard]] color
canvas::operator[](point const& p) const noexcept
{
    return impl::canvas_color_pick(handle, p);
}

[[nodiscard]] vector
canvas::size() const noexcept
{
    return impl::canvas_size(handle);
}

[[nodiscard]] point
canvas::first() const noexcept
{
    return {0, 0};
}

[[nodiscard]] point
canvas::last() const noexcept
{
    return point{-1, -1} + size();
}

void render(canvas& can) noexcept
{
    return impl::canvas_render(can.handle);
}

void clear(canvas& can, color const& col) noexcept
{
    impl::canvas_clear(can.handle, col);
}

[[nodiscard]] color
color_get(canvas& can) noexcept
{
    return impl::canvas_color_get(can.handle);
}

void color_set(canvas& can, color const& col) noexcept
{
    impl::canvas_color_set(can.handle, col);
}

void draw_point(canvas& can, point const& p) noexcept
{
    impl::canvas_draw_point(can.handle, p);
}

void draw_point(canvas& can, point const& p, color const& col) noexcept
{
    auto old_col{color_get(can)};
    color_set(can, col);
    draw_point(can, p);
    color_set(can, old_col);
}

void draw_line(canvas& can, point const& p0, point const& p1) noexcept
{
    impl::canvas_draw_line(can.handle, p0, p1);
}

void draw_line(canvas& can, point const& p0, point const& p1, color const& col) noexcept
{
    auto old{color_get(can)};
    color_set(can, col);
    draw_line(can, p0, p1);
    color_set(can, old);
}

void draw_circle(canvas& can, point const& center, int32_t radius, fill f) noexcept
{
    if (f == fill::off) {
        auto const diameter = (radius * 2);

        auto x = (radius - 1);
        auto y = 0;
        auto tx = 1;
        auto ty = 1;
        auto error = (tx - diameter);

        while (x >= y) {
            impl::canvas_draw_point(can.handle, {center.x - x, center.y - y});
            impl::canvas_draw_point(can.handle, {center.x - x, center.y + y});
            impl::canvas_draw_point(can.handle, {center.x - y, center.y - x});
            impl::canvas_draw_point(can.handle, {center.x - y, center.y + x});
            impl::canvas_draw_point(can.handle, {center.x + x, center.y - y});
            impl::canvas_draw_point(can.handle, {center.x + x, center.y + y});
            impl::canvas_draw_point(can.handle, {center.x + y, center.y - x});
            impl::canvas_draw_point(can.handle, {center.x + y, center.y + x});

            if (error <= 0) {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0) {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    } else {
        auto const diameter = (radius * 2);

        auto x = (radius - 1);
        auto y = 0;
        auto tx = 1;
        auto ty = 1;
        auto error = (tx - diameter);

        while (x >= y) {
            impl::canvas_draw_line(can.handle, {center.x - x, center.y - y}, {center.x + x, center.y - y});
            impl::canvas_draw_line(can.handle, {center.x - x, center.y + y}, {center.x + x, center.y + y});
            impl::canvas_draw_line(can.handle, {center.x - y, center.y - x}, {center.x + y, center.y - x});
            impl::canvas_draw_line(can.handle, {center.x - y, center.y + x}, {center.x + y, center.y + x});

            if (error <= 0) {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0) {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }
}

void draw_circle(canvas& can, point const& center, int32_t radius, color const& col, fill f) noexcept
{
    auto old{color_get(can)};
    color_set(can, col);
    draw_circle(can, center, radius, f);
    color_set(can, old);
}

void draw_rect(canvas& can, point const& p, vector const& v, fill f) noexcept
{
    impl::canvas_draw_rect(can.handle, p, v, f);
}

void draw_rect(canvas& can, point const& p, vector const& v, color const& col, fill f) noexcept
{
    auto old{color_get(can)};
    color_set(can, col);
    draw_rect(can, p, v, f);
    color_set(can, old);
}

void draw_texture(canvas& can, texture const& tex) noexcept
{
    impl::canvas_draw_texture(can.handle, tex.handle);
}

void draw_texture(canvas& can, texture const& tex, point const& p) noexcept
{
    impl::canvas_draw_texture(can.handle, tex.handle, p);
}

void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s) noexcept
{
    impl::canvas_draw_texture(can.handle, tex.handle, p, s);
}

void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s, point const& tp, vector const& ts) noexcept
{
    impl::canvas_draw_texture(can.handle, tex.handle, p, s, tp, ts);
}

void draw_text(canvas& can, std::string const& text, font const& f, point const& p) noexcept
{
    impl::canvas_draw_text(can.handle, text, f.handle, p, color_get(can));
}

void draw_text(canvas& can, std::string const& text, font const& f, point const& p, color const& col) noexcept
{
    impl::canvas_draw_text(can.handle, text, f.handle, p, col);
}

}

}
