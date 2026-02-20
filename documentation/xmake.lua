-- Example programs entry point
-- All example targets are defined in subdirectories

function example_target(name, source)
    target(name, function()
        set_kind("binary")
        set_default(false)
        set_languages("c++23")
        set_toolchains("clang")
        add_files(source)
        add_packages("nlohmann_json", "cpr")
        add_deps("oai")
    end)
end

includes("audio")
includes("authorization")
includes("azure")
includes("chat")
includes("completions")
includes("edits")
includes("embeddings")
includes("files")
includes("fine-tunes")
includes("images")
includes("models")
includes("moderations")
