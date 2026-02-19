set_project("liboai")
set_version("2.0.0")
set_languages("c++23")

add_rules("mode.debug", "mode.release")
-- add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

add_requires("nlohmann_json", "cpr")

option("build_examples")
    set_default(false)
    set_showmenu(true)
    set_description("Build example programs")
option_end()

target("oai", function()
    set_kind("static")
    set_languages("c++23")
    set_toolchains("clang")

    add_files("src/**.cppm", {public = true})

    add_packages("nlohmann_json", "cpr", {public = true})
end)


if get_config("build_examples") then
    includes("documentation")
end
