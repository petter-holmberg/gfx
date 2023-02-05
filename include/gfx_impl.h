/*
gfx - A simple C++20 library for 2D graphics. Version 0.0.1.

Written in 2023 by Petter Holmberg petter.holmberg@usa.net

Uses libsdl: https://www.libsdl.org/

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

It is also recommended that you include a file called COPYING (or COPYING.txt) containing the CC0 legalcode as plain text.
*/
#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace gfx {

inline namespace v0 {

struct point;

struct vector;

struct color;

class texture;

enum class visibility;

enum class vsync;

enum class fill;

namespace impl {

void global_context_destroy() noexcept;

void global_context_create() noexcept;

void window_destroy(void* handle) noexcept;

void* window_create(point pos, vector size, std::string const& title, visibility vis) noexcept;

void window_show(void* handle) noexcept;

void window_hide(void* handle) noexcept;

void texture_destroy(void* handle) noexcept;

void* texture_load(void* handle, std::filesystem::path const& path) noexcept;

vector texture_size(void* handle) noexcept;

void font_destroy(void* handle) noexcept;

void* font_create(std::filesystem::path const& path, int32_t size) noexcept;

vector font_text_size(void* handle, char const* text) noexcept;

void canvas_destroy(void* handle) noexcept;

void* canvas_create(void* window_handle, vsync vs) noexcept;

color canvas_color_pick(void* handle, point const& p) noexcept;

vector canvas_size(void* handle) noexcept;

color canvas_color_get(void* handle) noexcept;

void canvas_color_set(void* handle, color const& col) noexcept;

void canvas_draw_point(void* handle, point const& p) noexcept;

void canvas_draw_rect(void* handle, point const& p, vector const& v, fill f) noexcept;

void canvas_draw_line(void* handle, point const& p0, point const& p1) noexcept;

void canvas_draw_texture(void* handle, void* texture_handle) noexcept;

void canvas_draw_texture(void* handle, void* texture_handle, point const& p) noexcept;

void canvas_draw_texture(void* handle, void* texture_handle, point const& p, vector const& s) noexcept;

void canvas_draw_texture(void* handle, void* texture_handle, point const& p, vector const& s, point const& tp, vector const& ts) noexcept;

void canvas_draw_text(void* handle, std::string const& text, void* font_handle, point const& p, color const& col) noexcept;

void canvas_render(void* handle) noexcept;

void canvas_clear(void* handle, color const& col) noexcept;

}

}

}
