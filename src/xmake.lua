target("oai", function()
    set_kind("static")
    set_languages("c++23")
    set_toolchains("clang")

    add_files("**.cppm")

    add_packages("nlohmann_json", "cpr")
end)
