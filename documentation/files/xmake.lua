target("files_delete_file", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/delete_file.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_delete_file_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/delete_file_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_download_uploaded_file", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/download_uploaded_file.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_download_uploaded_file_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/download_uploaded_file_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_list_files", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/list_files.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_list_files_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/list_files_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_retrieve_file", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/retrieve_file.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_retrieve_file_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/retrieve_file_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_upload_file", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/upload_file.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("files_upload_file_async", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/upload_file_async.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)
