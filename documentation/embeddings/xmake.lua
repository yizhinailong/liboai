target("embeddings_create_embedding", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/create_embedding.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("embeddings_create_embedding_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/create_embedding_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)
