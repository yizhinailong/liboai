target("authorization_set_azure_key", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_azure_key.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_azure_key_env", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_azure_key_env.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_azure_key_file", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_azure_key_file.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_key", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_key.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_key_env_var", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_key_env_var.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_key_file", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_key_file.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_organization", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_organization.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_organization_env_var", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_organization_env_var.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_organization_file", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_organization_file.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_proxies", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_proxies.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)

target("authorization_set_proxy_auth", function()
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    set_toolchains("clang")
    
    
    add_files("examples/set_proxy_auth.cpp")
    add_packages("nlohmann_json", "cpr")
    add_deps("oai")
end)
