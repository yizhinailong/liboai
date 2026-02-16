set_project("liboai")
set_version("4.0.1")
set_languages("c++17")

add_requires("nlohmann_json", "libcurl", "gtest")

option("build_examples")
    set_default(false)
    set_showmenu(true)
    set_description("Build example programs")
option_end()

includes("src")

if get_config("build_examples") then
    includes("documentation")
end
