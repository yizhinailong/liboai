set_project("liboai")
set_version("2.0.0")
set_languages("c++23")
set_policy("build.c++.modules.reuse", true)
set_policy("build.c++.modules.hide_dependencies", false)

add_rules("mode.debug", "mode.release")

if not get_config("build_examples") then
    add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})
end

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
