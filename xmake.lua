set_project("liboai")
set_version("6.0.0")
set_languages("c++23")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

add_requires("nlohmann_json", "cpr")

option("build_examples")
    set_default(false)
    set_showmenu(true)
    set_description("Build example programs")
option_end()

includes("src")

if get_config("build_examples") then
    includes("documentation")
end
