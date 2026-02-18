-- liboai library target
target("oai", function()
    set_kind("static")
    
    -- Components (11 files)
    add_files(
        "components/audio.cpp",
        "components/azure.cpp",
        "components/chat.cpp",
        "components/completions.cpp",
        "components/edits.cpp",
        "components/embeddings.cpp",
        "components/files.cpp",
        "components/fine_tunes.cpp",
        "components/images.cpp",
        "components/models.cpp",
        "components/moderations.cpp",
        -- Core (2 files)
        "core/authorization.cpp",
        "core/response.cpp"
    )
    
    add_includedirs("../include", {public = true})
    add_packages("nlohmann_json", "cpr")
end)
