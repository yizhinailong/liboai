target("models_list_models", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/list_models.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("models_list_models_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/list_models_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("models_retrieve_model", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/retrieve_model.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("models_retrieve_model_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/retrieve_model_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)
