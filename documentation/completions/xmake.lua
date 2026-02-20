target("completions_generate_completion", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/generate_completion.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("completions_generate_completion_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/generate_completion_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)
