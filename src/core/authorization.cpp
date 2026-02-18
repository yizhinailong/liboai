/**
 * @file authorization.cpp
 *
 * Implementation of the Authorization singleton for API key
 * management.
 */
#include "liboai/core/authorization.hpp"

#include <cstdlib>
#include <fstream>

namespace liboai {

    Authorization::~Authorization() {
        // Securely clear the key memory
        if (!this->m_key.empty()) {
            volatile char* p = const_cast<volatile char*>(this->m_key.data());
            for (size_t i = 0; i < this->m_key.size(); ++i) {
                p[i] = '\0';
            }
        }
    }

    auto Authorization::SetKey(std::string_view key) noexcept -> bool {
        if (!key.empty()) {
            this->m_key = key;
            if (this->m_openai_auth_headers.count("Authorization") > 0) {
                this->m_openai_auth_headers.erase("Authorization");
            }
            this->m_openai_auth_headers["Authorization"] = ("Bearer " + this->m_key);
            return true;
        }
        return false;
    }

    auto Authorization::SetAzureKey(std::string_view key) noexcept -> bool {
        if (!key.empty()) {
            this->m_key = key;
            if (this->m_azure_auth_headers.size() > 0) {
                this->m_azure_auth_headers.clear();
            }
            this->m_azure_auth_headers["api-key"] = this->m_key;
            return true;
        }
        return false;
    }

    auto Authorization::SetAzureKeyAD(std::string_view key) noexcept -> bool {
        if (!key.empty()) {
            this->m_key = key;
            if (this->m_azure_auth_headers.size() > 0) {
                this->m_azure_auth_headers.clear();
            }
            this->m_azure_auth_headers["Authorization"] = ("Bearer " + this->m_key);
            return true;
        }
        return false;
    }

    auto Authorization::SetKeyFile(const std::filesystem::path& path) noexcept -> bool {
        if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path) &&
            std::filesystem::file_size(path) > 0) {
            std::ifstream file(path);
            if (file.is_open()) {
                std::getline(file, this->m_key);
                if (this->m_openai_auth_headers.count("Authorization") > 0) {
                    this->m_openai_auth_headers.erase("Authorization");
                }
                this->m_openai_auth_headers["Authorization"] = ("Bearer " + this->m_key);
                return true;
            }
        }
        return false;
    }

    auto Authorization::SetAzureKeyFile(const std::filesystem::path& path) noexcept -> bool {
        if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path) &&
            std::filesystem::file_size(path) > 0) {
            std::ifstream file(path);
            if (file.is_open()) {
                std::getline(file, this->m_key);
                if (this->m_azure_auth_headers.size() > 0) {
                    this->m_azure_auth_headers.clear();
                }
                this->m_azure_auth_headers["api-key"] = this->m_key;
                return true;
            }
        }
        return false;
    }

    auto Authorization::SetAzureKeyFileAD(const std::filesystem::path& path) noexcept -> bool {
        if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path) &&
            std::filesystem::file_size(path) > 0) {
            std::ifstream file(path);
            if (file.is_open()) {
                std::getline(file, this->m_key);
                if (this->m_azure_auth_headers.size() > 0) {
                    this->m_azure_auth_headers.clear();
                }
                this->m_azure_auth_headers["Authorization"] = ("Bearer " + this->m_key);
                return true;
            }
        }
        return false;
    }

    auto Authorization::SetKeyEnv(std::string_view var) noexcept -> bool {
        if (!var.empty()) {
            const char* key = std::getenv(var.data());
            if (key != nullptr) {
                this->m_key = key;
                if (this->m_openai_auth_headers.count("Authorization") > 0) {
                    this->m_openai_auth_headers.erase("Authorization");
                }
                this->m_openai_auth_headers["Authorization"] = ("Bearer " + this->m_key);
                return true;
            }
            return false;
        }
        return false;
    }

    auto Authorization::SetAzureKeyEnv(std::string_view var) noexcept -> bool {
        if (!var.empty()) {
            const char* key = std::getenv(var.data());
            if (key != nullptr) {
                this->m_key = key;
                if (this->m_azure_auth_headers.size() > 0) {
                    this->m_azure_auth_headers.clear();
                }
                this->m_azure_auth_headers["api-key"] = this->m_key;
                return true;
            }
            return false;
        }
        return false;
    }

    auto Authorization::SetAzureKeyEnvAD(std::string_view var) noexcept -> bool {
        if (!var.empty()) {
            const char* key = std::getenv(var.data());
            if (key != nullptr) {
                this->m_key = key;
                if (this->m_azure_auth_headers.size() > 0) {
                    this->m_azure_auth_headers.clear();
                }
                this->m_azure_auth_headers["Authorization"] = ("Bearer " + this->m_key);
                return true;
            }
            return false;
        }
        return false;
    }

    auto Authorization::SetOrganization(std::string_view org) noexcept -> bool {
        if (!org.empty()) {
            this->m_org = std::move(org);
            if (this->m_openai_auth_headers.count("OpenAI-Organization") > 0) {
                this->m_openai_auth_headers.erase("OpenAI-Organization");
            }
            this->m_openai_auth_headers["OpenAI-Organization"] = this->m_org;
            return true;
        }
        return false;
    }

    auto Authorization::SetOrganizationFile(const std::filesystem::path& path) noexcept -> bool {
        if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path) &&
            std::filesystem::file_size(path) > 0) {
            std::ifstream file(path);
            if (file.is_open()) {
                std::getline(file, this->m_key);
                if (this->m_openai_auth_headers.count("OpenAI-Organization") > 0) {
                    this->m_openai_auth_headers.erase("OpenAI-Organization");
                }
                this->m_openai_auth_headers["OpenAI-Organization"] = this->m_org;
                return true;
            }
        }
        return false;
    }

    auto Authorization::SetOrganizationEnv(std::string_view var) noexcept -> bool {
        if (!var.empty()) {
            const char* org = std::getenv(var.data());
            if (org != nullptr) {
                this->m_org = org;
                if (this->m_openai_auth_headers.count("OpenAI-Organization") > 0) {
                    this->m_openai_auth_headers.erase("OpenAI-Organization");
                }
                this->m_openai_auth_headers["OpenAI-Organization"] = this->m_org;
                return true;
            }
            return false;
        }
        return false;
    }

    auto Authorization::SetProxies(
        const std::initializer_list<std::pair<const std::string, std::string>>& hosts
    ) noexcept -> void {
        this->m_proxies = cpr::Proxies(hosts);
    }

    auto Authorization::SetProxies(
        std::initializer_list<std::pair<const std::string, std::string>>&& hosts
    ) noexcept -> void {
        this->m_proxies = cpr::Proxies(std::move(hosts));
    }

    auto Authorization::SetProxies(const std::map<std::string, std::string>& hosts) noexcept
        -> void {
        this->m_proxies = cpr::Proxies(hosts);
    }

    auto Authorization::SetProxies(std::map<std::string, std::string>&& hosts) noexcept -> void {
        this->m_proxies = cpr::Proxies(std::move(hosts));
    }

    auto Authorization::SetProxyAuth(
        const std::map<std::string, cpr::EncodedAuthentication>& proto_up
    ) noexcept -> void {
        this->m_proxyAuth = cpr::ProxyAuthentication(proto_up);
    }

} // namespace liboai
