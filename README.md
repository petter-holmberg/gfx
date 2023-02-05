# gfx

A simple C++20 library for 2D graphics.

The intended purpose of this library is to provide a portable set of 2D drawing primitives with the minimum of fuss for quickly getting started with basic graphics applications.

The library is very small and does not provide many advanced features. It is optimized for simplicity of use rather than performance or flexibility. Practical usage experience will guide the future evolution of the API.

Internally it uses the SDL_2 library with the SDL2_image and SDL2_ttf extension libraries for additional features.

## Build instructions

Uses [xmake](https://xmake.io). Builds a static library and a test application. Build with `xmake` and run with `xmake run`.

# Dependencies

A C++20 compiler and SDL_2 with the SDL2_image and SDL2_ttf extension libraries.

## Usage examples

```cpp
    #include "gfx.h"
```

### Create a fullscreen window and draw in it

```cpp
    // Create a fullscreen window with the current resolution
    gfx::window window;

    // Create a canvas in the window to draw on
    gfx::canvas canvas{window};

    // Draw a white diagonal line from top-left to bottom-right corner
    gfx::line(canvas, canvas.first(), canvas.last(), gfx::white);

    // Render the canvas (line is not visible until now)
    gfx::render(canvas);
```

### Draw two textures in a window

```cpp
    // Create a 800x600 window near the upper left corner of the screen with a title
    gfx::window window{{100, 100}, {800, 600}, "Portraits with background"};

    // Create a canvas in the window to draw on
    gfx::canvas canvas{window};

    // Load two bitmap files
    auto background = gfx::texture::load(canvas, "background.png");
    assert(background);
    auto portrait = gfx::texture::load(canvas, "portrait.png");
    assert(portrait);

    // Draw background texture over the whole canvas
    gfx::draw_texture(canvas, *background);

    // Draw portrait texture in two places, one in original size and one upscaled
    gfx::draw_texture(canvas, *portrait, {0, 0});
    gfx::draw_texture(canvas, *portrait, {350, 250}, 2 * portrait->size());

    // Render the canvas
    gfx::render(canvas);
```

## Namespace reference

### `gfx`

The namespace containing all library components.

## Class reference

### `point`

A `std::regular` type representing a 2D position.

#### Member objects

| Member name | Type      |
|-------------|-----------|
| `x`         | `int32_t` |
| `y`         | `int32_t` |

### `vector`

A `std::regular` type representing the difference between two `point` objects.

`vector` is used to represent sizes and to modify `point`s.

Affine vector/scalar/point arithmetic is supported:

```
v + v -> v
-v -> v
v - v -> v
s * v -> v
v * s -> v
v / s -> v
p + v -> p
p - v -> p
p - p -> v
```

#### Member objects

| Member name   | Type      |
|---------------|-----------|
| `x`           | `int32_t` |
| `y`           | `int32_t` |

### `color`

A `std::regular` type representing a color in RGBA color space.

#### Member objects

| Member name | Type      |
|-------------|-----------|
| `r`         | `uint8_t` |
| `g`         | `uint8_t` |
| `b`         | `uint8_t` |
| `a`         | `uint8_t` |

#### Predefined colors

The following 16 basic colors from the [CSS Color Module Level 3 specification](https://www.w3.org/TR/css-color-3/#html4) are defined for convenience:

```cpp
gfx::black{0, 0, 0};
gfx::silver{192, 192, 192};
gfx::gray{128, 128, 128};
gfx::white{255, 255, 255};
gfx::maroon{128, 0, 0};
gfx::red{255, 0, 0};
gfx::purple{128, 0, 128};
gfx::fuchsia{255, 0, 255};
gfx::green{0, 128, 0};
gfx::lime{0, 255, 0};
gfx::olive{128, 128, 0};
gfx::yellow{255, 255, 0};
gfx::navy{0, 0, 128};
gfx::blue{0, 0, 255};
gfx::teal{0, 128, 128};
gfx::aqua{0, 255, 255};
```

#### Color blending

```cpp
color color_blend(color c0, color c1, float fraction);
```

Blends two colors in the sRGB color space with a gamma correction of 2.2 to produce accurate colors.

### `visibility`

An enum class used to determine if a `window` should be visible or not when created.

#### Member values

| Member name | Meaning   |
|-------------|-----------|
| `on`        | Visible   |
| `off`       | Invisible |

### `window`

A `std::movable` type representing a window for 2D graphics.

A default-constructed window will be fullscreen and in the current desktop resolution.

A window constructed with a non-zero position and size will have a border and an optional title text.

#### Member functions

```cpp
explicit window(point pos = {}, vector size = {}, std::string const& title = "", visibility vis = visibility::on) noexcept
```

Constructor. Takes a `point` for window placement, a `vector` for window size, a `std::string` for window title, and a `visibility` for determining if the window should start out visible or not.

```cpp
void show() noexcept
```

Makes the `window` visible.

```cpp
void hide() noexcept
```

Makes the `window` invisible.

### `vsync`

An enum class used to determine if vertical sync should be on or off when a `canvas` is rendered.

#### Member values

| Member name | Meaning   |
|-------------|-----------|
| `on`        | Visible   |
| `off`       | Invisible |

### `fill`

An enum class used to determine if shape drawing functions should draw a filled shape or not.

#### Member values

| Member name | Meaning    |
|-------------|------------|
| `on`        | Fill       |
| `off`       | Don't fill |

### `canvas`

A `std::movable` type representing a drawable surface in a `window`.

A `canvas` is used by all drawing functions in the library. Drawing on a canvas will not update the contents of the window the canvas belongs to. Updates are done by calling the function `render`. This makes it possible to use multiple `canvas` objects in the same window for double-buffering etc.

#### Member functions

```cpp
explicit canvas(window const& window, vsync vs = vsync::on) noexcept
```

Constructor. Takes a `window` and optionally determines if vsync should be on or off.

```cpp
color operator[](point const& p) const noexcept
```

Get the color of the pixel at the given position p.

```cpp
vector size() const noexcept
```

Returns the canvas size in pixels.

```cpp
point first() const noexcept
```

Returns the first point of the canvas (top left).

```cpp
point last() const noexcept
```

Returns the last point of the canvas (bottom right).

### `texture`

A `std::movable` type representing a RGBA texture that can be drawn on a `canvas`.

#### Member functions

```cpp
vector size() const noexcept
```

Returns the canvas size in pixels.

#### Static member functions

```cpp
std::optional<texture> load(canvas& can, std::filesystem::path const& path) noexcept
```

Loads a bitmap from file. Supported file formats include BMP, GIF, JPEG, LBM, PCX, PNG, PNM (PPM/PGM/PBM), QOI, TGA, XCF, XPM, and simple SVG format images.

Returns an empty `std::optional` if loading fails.

### `font`

A `std::movable` type representing a TrueType font for displaying text on a `canvas`.

#### Member functions

```cpp
vector text_size(font& f, char const* text) noexcept
```

Returns the size that the given text will occupy if drawn.

```cpp
vector text_size(font& f, std::string const& text) noexcept
```

Returns the size that the given text will occupy if drawn.

#### Static member functions

```cpp
std::optional<font> load(std::filesystem::path const& path, int32_t size) noexcept
```

Loads a font from a TTF file with the given font size.

Returns an empty `std::optional` if loading fails for whatever reason.

## Function reference

```cpp
void render(canvas& can) noexcept
```

Renders the given canvas in the window it belongs to.


```cpp
void clear(canvas& can, color const& col = black) noexcept
```

Fills the given canvas with the given color.

```cpp
color color_get(canvas& can) noexcept

```

Gets the current drawing color of the given canvas.

All drawing functions have overloads without a color parameter for drawing with the currently set color.

```cpp
void color_set(canvas& can, color const& col) noexcept
```

Sets the current drawing color of the given canvas.

All drawing functions have overloads without a color parameter for drawing with the currently set color.

```cpp
void draw_point(canvas& can, point const&) noexcept
```

Draws a pixel at the given position with the current drawing color.

```cpp
void draw_point(canvas& can, point const& p, color const& c) noexcept
```

Draws a pixel at the given position with the given color.

```cpp
void draw_line(canvas& can, point const& p0, point const& p1) noexcept
```

Draws a line starting at position p0 and ending at position p1 with the current drawing color.

```cpp
void draw_line(canvas& can, point const& p0, point const& p1, color const& c) noexcept
```

Draws a line starting at position p0 and ending at position p1 with the given color.

```cpp
void draw_circle(canvas& can, point const& center, int32_t radius, fill f = fill::off) noexcept
```

Draws a circle with the given center and radius in pixels with the current drawing color, filled or not.

```cpp
void circle(canvas& can, point const& center, int32_t radius, color const& c, fill f = fill::off) noexcept
```

Draws a circle with the given center and radius in pixels with the given color, filled or not.

```cpp
void draw_rect(canvas& can, point const& p, vector const& v, fill f = fill::off) noexcept
```

Draws a rectangle with opposite edges at position p and position p + v with the current drawing color, filled or not.

```cpp
void rect(canvas& can, point const& p, vector const& v, color const& c, fill f = fill::off) noexcept
```

Draws a rectangle with opposite edges at position p and position p + v with the given color, filled or not.

```cpp
void draw_texture(canvas&, texture const&) noexcept
```

Draws a texture scaled to cover the given canvas.

```cpp
void draw_texture(canvas& can, texture const& tex, point const& p) noexcept
```

Draws a texture in its original size with upper left corner at the given point.

```cpp
void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s) noexcept
```

Draws a texture scaled to the given size with upper left corner at the given point.

```cpp
void draw_texture(canvas& can, texture const& tex, point const& p, vector const& s, point const& tp, vector const& ts) noexcept
```

Draws a portion of a texture scaled to the given size with upper left corner at the given point. `tp` and `ts` define the position and size of the region within the texture to draw.

```cpp
void draw_text(canvas& can, std::string const& text, font const& f, point const& p) noexcept
```

Prints a UTF-8 string with given font at the given position with the current drawing color.


```cpp
void draw_text(canvas& can, std::string const& text, font const& f, point const& p, color const& col) noexcept
```

Prints a UTF-8 string with given font at the given position and color.
