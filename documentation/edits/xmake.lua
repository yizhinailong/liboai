target("edits_create_edit", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/create_edit.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("edits_create_edit_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/create_edit_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)
