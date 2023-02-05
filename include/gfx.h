/*
gfx - A simple C++20 library for 2D graphics. Version 0.0.1.

Written in 2023 by Petter Holmberg petter.holmberg@usa.net

Uses libsdl: https://www.libsdl.org/

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

It is also recommended that you include a file called COPYING (or COPYING.txt) containing the CC0 legalcode as plain text.
*/
#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include <functional>
#include <optional>
#include <utility>

namespace gfx {

inline namespace v0 {

struct point
{
    int32_t x{};
    int32_t y{};

    [[nodiscard]] friend constexpr bool operator==(point p0, point p1) = default;
};

struct vector
{
    int32_t x{};
    int32_t y{};

    [[nodiscard]] friend constexpr bool operator==(vector v0, vector v1) = default;
};

[[nodiscard]] constexpr vector
operator+(vector v0, vector v1) noexcept
{
    return {v0.x + v1.x, v0.y + v1.y};
}

[[nodiscard]] constexpr vector&
operator+=(vector& v0, vector v1) noexcept
{
    v0.x += v1.x;
    v0.y += v1.y;
    return v0;
}

[[nodiscard]] constexpr vector
operator-(vector const& v) noexcept
{
    return {-v.x, -v.y};
}

[[nodiscard]] constexpr vector
operator-(vector v0, vector v1) noexcept
{
    return v0 + -v1;
}

[[nodiscard]] constexpr vector&
operator-=(vector& v0, vector v1) noexcept
{
    v0.x -= v1.x;
    v0.y -= v1.y;
    return v0;
}

template <typename S>
[[nodiscard]] constexpr vector
operator*(S s, vector v) noexcept
{
    return {s * v.x, s * v.y};
}

template <typename S>
[[nodiscard]] constexpr vector
operator*(vector v, S s) noexcept
{
    return {v.x * s, v.y * s};
}

template <typename S>
[[nodiscard]] constexpr vector
operator/(vector v, S s) noexcept
{
    return {v.x / s, v.y / s};
}

[[nodiscard]] constexpr point
operator+(point const& p, vector v) noexcept
{
    return {p.x + v.x, p.y + v.y};
}

[[nodiscard]] constexpr point
operator+(vector v, point p) noexcept
{
    return p + v;
}

[[nodiscard]] constexpr point&
operator+=(point& p, vector v) noexcept
{
    p.x += v.x;
    p.y += v.y;
    return p;
}

[[nodiscard]] constexpr point
operator-(point p, vector v) noexcept
{
    return p + (-v);
}

[[nodiscard]] constexpr vector
operator-(point p0, point p1) noexcept
{
    return {p0.x - p1.x, p0.y - p1.y};
}

[[nodiscard]] constexpr point&
operator-=(point& p, vector v) noexcept
{
    p.x -= v.x;
    p.y -= v.y;
    return p;
}

struct color
{
    uint8_t r{};
    uint8_t g{};
    uint8_t b{};
    uint8_t a{};
};

[[nodiscard]] constexpr auto
operator==(color c0, color c1) noexcept -> bool
{
    return c0.r == c1.r && c0.g == c1.g && c0.b == c1.b && c0.a == c1.a;
}

[[nodiscard]] constexpr auto
operator!=(color c0, color c1) noexcept -> bool
{
    return !(c0 == c1);
}

inline constexpr color black{0, 0, 0};
inline constexpr color silver{192, 192, 192};
inline constexpr color gray{128, 128, 128};
inline constexpr color white{255, 255, 255};
inline constexpr color maroon{128, 0, 0};
inline constexpr color red{255, 0, 0};
inline constexpr color purple{128, 0, 128};
inline constexpr color fuchsia{255, 0, 255};
inline constexpr color green{0, 128, 0};
inline constexpr color lime{0, 255, 0};
inline constexpr color olive{128, 128, 0};
inline constexpr color yellow{255, 255, 0};
inline constexpr color navy{0, 0, 128};
inline constexpr color blue{0, 0, 255};
inline constexpr color teal{0, 128, 128};
inline constexpr color aqua{0, 255, 255};

color color_blend(color c0, color c1, float fraction) noexcept;

enum class visibility
{
    on,
    off
};

class window
{
    void* handle{};

public:
    ~window();

    explicit window(point pos = {}, vector size = {}, std::string const& title = "", visibility vis = visibility::on) noexcept;

    window(window const&) = delete;

    window& operator=(window const&) = delete;

    constexpr window(window&& rhs) noexcept
        : handle{std::exchange(rhs.handle, nullptr)}
    {}

    window& operator=(window&& rhs) noexcept;

    void show() noexcept;

    void hide() noexcept;

    friend class canvas;
};

class canvas;

class texture
{
    void* handle{};

    explicit constexpr texture(void* tp) noexcept : handle{tp} {}

public:
    ~texture();

    texture() = default;

    texture(texture const&) = delete;

    texture& operator=(texture const&) = delete;

    constexpr texture(texture&& rhs) noexcept
        : handle{std::exchange(rhs.handle, nullptr)}
    {}

    texture& operator=(texture&& rhs) noexcept;

    [[nodiscard]] vector size() const noexcept;

    [[nodiscard]] static std::optional<texture> load(canvas& can, std::filesystem::path const& path) noexcept;

    friend void draw_texture(canvas& can, texture const& tex) noexcept;

    friend void draw_texture(canvas& can, texture const& tex, point const& p) noexcept;

    friend void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s) noexcept;

    friend void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s, point const& tp, vector const& ts) noexcept;
};

class font
{
    void* handle{};

    explicit constexpr font(void* fp) noexcept : handle{fp} {}

public:
    ~font();

    font() = default;

    font(font const&) = delete;

    font& operator=(font const&) = delete;

    constexpr font(font&& rhs) noexcept
        : handle{std::exchange(rhs.handle, nullptr)}
    {}

    font& operator=(font&& rhs) noexcept;

    [[nodiscard]] static std::optional<font> load(std::filesystem::path const&, int32_t) noexcept;

    [[nodiscard]] static vector text_size(font& f, char const* text) noexcept;

    [[nodiscard]] static vector text_size(font& f, std::string const& text) noexcept;

    friend void draw_text(canvas& can, std::string const& text, font const& f, point const& p) noexcept;

    friend void draw_text(canvas& can, std::string const& text, font const& f, point const& p, color const& col) noexcept;
};

enum class vsync
{
    on,
    off
};

enum class fill
{
    on,
    off
};

class canvas
{
    void* handle{};

public:

    ~canvas();

    explicit canvas(window const& window, vsync vs = vsync::on) noexcept;

    canvas(canvas const&) = delete;

    canvas& operator=(canvas const&) = delete;

    constexpr canvas(canvas&& rhs) noexcept
        : handle{std::exchange(rhs.handle, nullptr)}
    {}

    canvas& operator=(canvas&& rhs) noexcept;

    [[nodiscard]] color operator[](point const& p) const noexcept;

    [[nodiscard]] vector size() const noexcept;

    [[nodiscard]] point first() const noexcept;

    [[nodiscard]] point last() const noexcept;

    friend class texture;

    friend void render(canvas&) noexcept;

    friend void clear(canvas&, color const& col) noexcept;

    friend color color_get(canvas&) noexcept;

    friend void color_set(canvas&, color const&) noexcept;

    friend void draw_point(canvas&, point const&) noexcept;

    friend void draw_line(canvas&, point const&, point const&) noexcept;

    friend void draw_circle(canvas&, point const&, int32_t, fill) noexcept;

    friend void draw_rect(canvas&, point const&, vector const&, fill) noexcept;

    friend void draw_texture(canvas&, texture const&) noexcept;

    friend void draw_texture(canvas& can, texture const& tex) noexcept;

    friend void draw_texture(canvas& can, texture const& tex, point const& p) noexcept;

    friend void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s) noexcept;

    friend void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s, point const& tp, vector const& ts) noexcept;

    friend void draw_text(canvas& can, std::string const& text, font const& f, point const& p) noexcept;

    friend void draw_text(canvas& can, std::string const& text, font const& f, point const& p, color const& col) noexcept;
};

void render(canvas& can) noexcept;

void clear(canvas& can, color const& col = black) noexcept;

[[nodiscard]] color color_get(canvas& can) noexcept;

void color_set(canvas& can, color const& col) noexcept;

void draw_point(canvas& can, point const& p) noexcept;

void draw_point(canvas& can, point const& p, color const& c) noexcept;

void draw_line(canvas& can, point const& p0, point const& p1) noexcept;

void draw_line(canvas& can, point const& p0, point const& p1, color const& col) noexcept;

void draw_circle(canvas& can, point const& center, int32_t radius, fill f = fill::off) noexcept;

void draw_circle(canvas& can, point const& center, int32_t radius, color const& col, fill f = fill::off) noexcept;

void draw_rect(canvas& can, point const& p, vector const& v, fill f = fill::off) noexcept;

void draw_rect(canvas& can, point const& p, vector const& v, color const& col, fill f = fill::off) noexcept;

void draw_texture(canvas& can, texture const& tex) noexcept;

void draw_texture(canvas& can, texture const& tex, point const& p) noexcept;

void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s) noexcept;

void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s, point const& tp, vector const& ts) noexcept;

void draw_text(canvas& can, std::string const& text, font const& f, point const& p) noexcept;

void draw_text(canvas& can, std::string const& text, font const& f, point const& p, color const& col) noexcept;

}

}

namespace std {

    template <>
    struct less<gfx::point>
    {
        constexpr auto
        operator()(gfx::point const& p0, gfx::point const& p1) const -> bool
        {
            return p0.x < p1.x || (!(p1.x < p0.x) && (p0.y < p1.y));
        }
    };

    template <>
    struct less<gfx::vector>
    {
        constexpr auto
        operator()(gfx::vector const& v0, gfx::vector const& v1) const -> bool
        {
            return v0.x < v1.x || (!(v1.x < v0.x) && (v0.y < v1.y));
        }
    };

}
