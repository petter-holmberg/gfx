set_languages("c++20")

set_warnings("everything", "error")

add_rules("mode.debug", "mode.release")
if is_mode("debug") then
    add_defines("DEBUG")
elseif is_mode("release") then
    add_defines("NDEBUG")
    set_optimize("fastest")
end

add_requires("libsdl2")
add_requires("libsdl_image")
add_requires("libsdl_ttf")

target("gfx")
    set_kind("static")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_packages("libsdl2")
    add_packages("libsdl_image")
    add_packages("libsdl_ttf")

target("test")
    add_files("test/*.cpp")
    add_includedirs("include")
    add_deps("gfx")
    after_build(function (target)
        os.cp("test/*.png", target:targetdir())
        os.cp("test/*.ttf", target:targetdir())
    end)
