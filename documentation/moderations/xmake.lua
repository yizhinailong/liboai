target("moderations_create_moderation", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/create_moderation.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("moderations_create_moderation_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/create_moderation_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)
