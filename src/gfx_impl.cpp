/*
gfx - A simple C++20 library for 2D graphics. Version 0.0.1.

Written in 2023 by Petter Holmberg petter.holmberg@usa.net

Uses libsdl: https://www.libsdl.org/

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

It is also recommended that you include a file called COPYING (or COPYING.txt) containing the CC0 legalcode as plain text.
*/
#include "gfx_impl.h"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "gfx.h"

namespace gfx {

namespace v0 {

namespace impl {

void global_context_destroy() noexcept
{
    ::TTF_Quit();
}

void global_context_create() noexcept
{
    ::TTF_Init();
}

void font_destroy(void* handle) noexcept
{
    ::TTF_CloseFont(reinterpret_cast<::TTF_Font*>(handle));
}

void* font_create(std::filesystem::path const& path, int32_t size) noexcept
{
    return ::TTF_OpenFont(path.string().c_str(), size);
}

vector font_text_size(void* handle, char const* text) noexcept
{
    int w, h;
    ::TTF_SizeUTF8(reinterpret_cast<::TTF_Font*>(handle), text, &w, &h);
    return {w, h};
}

void window_destroy(void* handle) noexcept
{
    ::SDL_DestroyWindow(reinterpret_cast<::SDL_Window*>(handle));
}

void* window_create(point pos, vector size, std::string const& title, visibility vis) noexcept
{
    ::Uint32 v = SDL_WINDOW_SHOWN;
    if (vis == visibility::off) {
        v = SDL_WINDOW_HIDDEN;
    }

    void* handle;
    if (size == vector{}) {
        handle = ::SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, SDL_WINDOW_FULLSCREEN_DESKTOP | v);
    } else {
        if (pos == point{}) {
            handle = ::SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, v);
        } else {
            handle = ::SDL_CreateWindow(title.c_str(), pos.x, pos.y, size.x, size.y, v);
        }
    }

    return handle;
}

void window_show(void* handle) noexcept
{
    ::SDL_ShowWindow(reinterpret_cast<::SDL_Window*>(handle));
}

void window_hide(void* handle) noexcept
{
    ::SDL_HideWindow(reinterpret_cast<::SDL_Window*>(handle));
}

void texture_destroy(void* handle) noexcept
{
    ::SDL_DestroyTexture(reinterpret_cast<::SDL_Texture*>(handle));
}

void* texture_load(void* handle, std::filesystem::path const& path) noexcept
{
    ::SDL_Texture* tp{};
    ::SDL_Surface* surf = ::IMG_Load(path.string().c_str());
    if (surf != nullptr) {
        tp = ::SDL_CreateTextureFromSurface(reinterpret_cast<::SDL_Renderer*>(handle), surf);
        ::SDL_FreeSurface(surf);
    }
    return tp;
}

vector texture_size(void* handle) noexcept
{
    vector size;
    if (handle) {
        ::SDL_QueryTexture(reinterpret_cast<::SDL_Texture*>(handle), nullptr, nullptr, &size.x, &size.y);
    }
    return size;
}

void canvas_destroy(void* handle) noexcept
{
    ::SDL_DestroyRenderer(reinterpret_cast<::SDL_Renderer*>(handle));
}

void* canvas_create(void* window_handle, vsync vs) noexcept
{
    ::Uint32 flags = SDL_RENDERER_ACCELERATED;
    if (vs == vsync::on) {
        flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    void* handle = ::SDL_CreateRenderer(reinterpret_cast<::SDL_Window*>(window_handle), -1, flags);
    ::SDL_SetRenderDrawBlendMode(reinterpret_cast<::SDL_Renderer*>(handle), SDL_BLENDMODE_NONE);
    return handle;
}

color canvas_color_pick(void* handle, point const& p) noexcept
{
    color c;
    ::SDL_Rect rect{p.x, p.y, 1, 1};
    ::SDL_RenderReadPixels(reinterpret_cast<::SDL_Renderer*>(handle), &rect, SDL_PIXELFORMAT_RGBA32, &c, 4);
    return c;
}

vector canvas_size(void* handle) noexcept
{
    vector size;
    ::SDL_GetRendererOutputSize(reinterpret_cast<::SDL_Renderer*>(handle), &size.x, &size.y);
    return size;
}

color canvas_color_get(void* handle) noexcept
{
    color c;
    ::SDL_GetRenderDrawColor(reinterpret_cast<::SDL_Renderer*>(handle), &c.r, &c.g, &c.b, &c.a);
    return c;
}

void canvas_color_set(void* handle, color const& col) noexcept
{
    ::SDL_SetRenderDrawColor(reinterpret_cast<::SDL_Renderer*>(handle), col.r, col.g, col.b, col.a);
}

void canvas_draw_point(void* handle, point const& p) noexcept
{
    ::SDL_RenderDrawPoint(reinterpret_cast<::SDL_Renderer*>(handle), p.x, p.y);
}

void canvas_draw_line(void* handle, point const& p0, point const& p1) noexcept
{
    ::SDL_RenderDrawLine(reinterpret_cast<::SDL_Renderer*>(handle), p0.x, p0.y, p1.x, p1.y);
}

void canvas_draw_rect(void* handle, point const& p, vector const& v, fill f) noexcept
{
    ::SDL_Rect rect{p.x, p.y, v.x, v.y};
    if (f == fill::off) {
        ::SDL_RenderDrawRect(reinterpret_cast<::SDL_Renderer*>(handle), &rect);
    } else {
        ::SDL_RenderFillRect(reinterpret_cast<::SDL_Renderer*>(handle), &rect);
    }
}

void canvas_draw_texture(void* handle, void* texture_handle) noexcept
{
    ::SDL_RenderCopy(reinterpret_cast<::SDL_Renderer*>(handle), reinterpret_cast<::SDL_Texture*>(texture_handle), nullptr, nullptr);
}

void canvas_draw_texture(void* handle, void* texture_handle, point const& p) noexcept
{
    int w, h;
    ::SDL_QueryTexture(reinterpret_cast<::SDL_Texture*>(texture_handle), nullptr, nullptr, &w, &h);
    canvas_draw_texture(handle, texture_handle, p, {w, h});
}

void canvas_draw_texture(void* handle, void* texture_handle, point const& p, vector const& s) noexcept
{
    ::SDL_Rect rect{p.x, p.y, s.x, s.y};
    ::SDL_RenderCopy(reinterpret_cast<::SDL_Renderer*>(handle), reinterpret_cast<::SDL_Texture*>(texture_handle), nullptr, &rect);
}

void canvas_draw_texture(void* handle, void* texture_handle, point const& p, vector const& s, point const& tp, vector const& ts) noexcept
{
    ::SDL_Rect trect{tp.x, tp.y, ts.x, ts.y};
    ::SDL_Rect rect{p.x, p.y, s.x, s.y};
    ::SDL_RenderCopy(reinterpret_cast<::SDL_Renderer*>(handle), reinterpret_cast<::SDL_Texture*>(texture_handle), &trect, &rect);
}

void canvas_draw_text(void* handle, std::string const& text, void* font_handle, point const& p, color const& col) noexcept
{
    ::SDL_Color color{col.r, col.g, col.b, col.a};
    ::SDL_Surface* surf = ::TTF_RenderUTF8_Solid(reinterpret_cast<::TTF_Font*>(font_handle), text.c_str(), color);
    ::SDL_Texture* text_texture = ::SDL_CreateTextureFromSurface(reinterpret_cast<::SDL_Renderer*>(handle), surf);
    ::SDL_Rect dest = {p.x, p.y, surf->w, surf->h};
    ::SDL_RenderCopy(reinterpret_cast<::SDL_Renderer*>(handle), text_texture, nullptr, &dest);
    ::SDL_DestroyTexture(text_texture);
    ::SDL_FreeSurface(surf);
}

void canvas_render(void* handle) noexcept
{
    ::SDL_RenderPresent(reinterpret_cast<::SDL_Renderer*>(handle));
}

void canvas_clear(void* handle, color const& col) noexcept
{
    ::SDL_SetRenderDrawColor(reinterpret_cast<::SDL_Renderer*>(handle), col.r, col.g, col.b, col.a);
    ::SDL_RenderClear(reinterpret_cast<::SDL_Renderer*>(handle));
}

void color_set(void* handle, color const& col) noexcept
{
    ::SDL_SetRenderDrawColor(reinterpret_cast<::SDL_Renderer*>(handle), col.r, col.g, col.b, col.a);
}

void canvas_line(void* handle, point const& p0, point const& p1) noexcept
{
    ::SDL_RenderDrawLine(reinterpret_cast<::SDL_Renderer*>(handle), p0.x, p0.y, p1.x, p1.y);
}

}

}

}
