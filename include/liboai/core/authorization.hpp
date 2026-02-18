#pragma once

/*
    authorization.h : liboai authorization header.
        This header file provides declarations for authorization
        directives for authorizing requests with the OpenAI API.
        Each component class makes use of a single object accessed
        via liboai::Authorization::Authorizer() to retrieve and use
        user-set authorization information to successfully complete
        component API requests.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <filesystem>
#include <string>

#include "liboai/core/network.hpp"

namespace liboai {
    class Authorization final {
    public: // cons/des, operator deletions
        Authorization() = default;
        Authorization(const Authorization&) = delete;
        Authorization& operator=(const Authorization&) = delete;
        Authorization(Authorization&&) = delete;
        Authorization& operator=(Authorization&&) = delete;
        ~Authorization();

    public: // member methods
        /*
            @brief Singleton paradigm access method.
            @return A reference to the singleton instance of this class
                to be used in all component classes.
        */
        [[nodiscard]]
        static Authorization& Authorizer() noexcept {
            static Authorization instance;
            return instance;
        }

        /*
            @brief Sets the authorization key for the OpenAI API
                as the passed string.
            @param key : The authorization key to use in component calls.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetKey(std::string_view key) noexcept -> bool;

        /*
            @brief Sets the authorization key for the Azure OpenAI API
                as the passed string.
            @param key : The authorization key to use in Azure component calls.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetAzureKey(std::string_view key) noexcept -> bool;

        /*
            @brief Sets the Active Directory authorization token for the Azure OpenAI API
                as the passed string.
            @param key : The authorization key to use in Azure component calls.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetAzureKeyAD(std::string_view key) noexcept -> bool;

        /*
            @brief Sets the authorization key for the OpenAI API
                as the first line present in the file at the passed path.
            @param path : The path to the file containing the authorization key.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetKeyFile(const std::filesystem::path& path) noexcept -> bool;

        /*
            @brief Sets the authorization key for the Azure OpenAI API
                as the first line present in the file at the passed path.
            @param key : The path to the file containing the authorization key.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetAzureKeyFile(const std::filesystem::path& path) noexcept -> bool;

        /*
            @brief Sets the Active Directory authorization token for the Azure OpenAI API
                as the first line present in the file at the passed path.
            @param key : The path to the file containing the authorization key.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetAzureKeyFileAD(const std::filesystem::path& path) noexcept -> bool;

        /*
            @brief Sets the authorization key for the OpenAI API
                as the value stored in the environment variable with
                the passed name.
            @param var : The name of the environment variable to
                retrieve the authorization key from.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetKeyEnv(std::string_view var) noexcept -> bool;

        /*
            @brief Sets the authorization key for the Azure OpenAI API
                as the value stored in the environment variable with
                the passed name.
            @param var : The name of the environment variable to
                retrieve the authorization key from.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetAzureKeyEnv(std::string_view var) noexcept -> bool;

        /*
            @brief Sets the Active Directory authorization token for the Azure OpenAI API
                as the value stored in the environment variable with
                the passed name.
            @param var : The name of the environment variable to
                retrieve the authorization key from.
            @returns True if the key was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetAzureKeyEnvAD(std::string_view var) noexcept -> bool;

        /*
            @brief Sets the organization identifier as the passed
                string for use in component calls.
            @param org : The organization identifier to use in
                component calls.
            @returns True if the ID was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetOrganization(std::string_view org) noexcept -> bool;

        /*
            @brief Sets the organization identifier as the first
                line present in the file at the passed path for use
                in component calls.
            @param path : The path to the file containing the
                organization identifier.
            @returns True if the ID was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetOrganizationFile(const std::filesystem::path& path) noexcept -> bool;

        /*
            @brief Sets the organization identifier as the value
                stored in the environment variable with the passed
                name for use in component calls.
            @param var : The name of the environment variable to
                retrieve the organization identifier from.
            @returns True if the ID was set successfully, false otherwise.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto SetOrganizationEnv(std::string_view var) noexcept -> bool;

        /*
            @brief Sets proxies to use for component calls.
            @param hosts : The hosts to use as proxies in
                paired { "protocol", "host" } format.
        */
        LIBOAI_EXPORT auto SetProxies(
            const std::initializer_list<std::pair<const std::string, std::string>>& hosts
        ) noexcept -> void;

        /*
            @brief Sets proxies to use for component calls.
            @param hosts : The hosts to use as proxies in
                paired { "protocol", "host" } format.
        */
        LIBOAI_EXPORT auto SetProxies(
            std::initializer_list<std::pair<const std::string, std::string>>&& hosts
        ) noexcept -> void;

        /*
            @brief Sets proxies to use for component calls.
            @param hosts : The hosts to use as proxies in
                paired { "protocol", "host" } format.
        */
        LIBOAI_EXPORT auto SetProxies(const std::map<std::string, std::string>& hosts) noexcept
            -> void;

        /*
            @brief Sets proxies to use for component calls.
            @param hosts : The hosts to use as proxies in
                paired { "protocol", "host" } format.
        */
        LIBOAI_EXPORT auto SetProxies(std::map<std::string, std::string>&& hosts) noexcept -> void;

        /*
            @brief Sets authentication information for proxies per-protocol.

            @param proto_up : A {protocol, {uname, passwd}} map to use for
                authentication with proxies on a per-protocol basis.
        */
        LIBOAI_EXPORT auto SetProxyAuth(
            const std::map<std::string, netimpl::components::EncodedAuthentication>& proto_up
        ) noexcept -> void;

        /*
            @brief Sets the timeout for component calls in milliseconds.
        */
        LIBOAI_EXPORT auto SetMaxTimeout(int32_t ms) noexcept -> void {
            this->m_timeout = netimpl::components::Timeout(ms);
        }

        /*
            @brief Returns currently the set authorization key.
        */
        [[nodiscard]]
        constexpr auto GetKey() const noexcept -> const std::string& {
            return this->m_key;
        }

        /*
            @brief Returns the currently set organization identifier.
        */
        [[nodiscard]]
        constexpr auto GetOrganization() const noexcept -> const std::string& {
            return this->m_org;
        }

        /*
            @returns The currently set proxies.
        */
        [[nodiscard]]
        auto GetProxies() const noexcept -> netimpl::components::Proxies {
            return this->m_proxies;
        }

        /*
            @returns The currently set proxy authentication information.
        */
        [[nodiscard]]
        auto GetProxyAuth() const noexcept -> netimpl::components::ProxyAuthentication {
            return this->m_proxyAuth;
        }

        /*
            @returns The currently set timeout.
        */
        [[nodiscard]]
        auto GetMaxTimeout() const noexcept -> netimpl::components::Timeout {
            return this->m_timeout;
        }

        /*
            @returns An authorization header with the
                currently set authorization information for use
                in component calls.
        */
        [[nodiscard]]
        constexpr auto GetAuthorizationHeaders() const noexcept
            -> const netimpl::components::Header& {
            return this->m_openai_auth_headers;
        }

        /*
            @returns An authorization header with the
                currently set Azure authorization information for use
                in Azure component calls.
        */
        [[nodiscard]]
        constexpr auto GetAzureAuthorizationHeaders() const noexcept
            -> const netimpl::components::Header& {
            return this->m_azure_auth_headers;
        }

    private: // member variables
        std::string m_key, m_org;
        netimpl::components::Header m_openai_auth_headers, m_azure_auth_headers;
        netimpl::components::Proxies m_proxies;
        netimpl::components::ProxyAuthentication m_proxyAuth;
        netimpl::components::Timeout m_timeout{ 30000 };
    };
} // namespace liboai
