/**
 * @file files.cpp
 *
 * Implementation of Files component for file management API.
 */
#include "liboai/components/files.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

    auto Files::List() const& noexcept -> Expected<Response> {
        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/files",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Files::ListAsync() const& noexcept -> FutureExpected<Response> {
        return std::async(std::launch::async, &Files::List, this);
    }

    auto
    Files::Create(const std::filesystem::path& file, const std::string& purpose) const& noexcept
        -> Expected<Response> {
        if (!this->Validate(file)) {
            return std::unexpected(
                OpenAIError::file_error(
                    "File path provided is non-existent, is not a file, or is empty."
                )
            );
        }

        cpr::Multipart form = {
            { "purpose",                            purpose },
            {    "file", cpr::File{ file.generic_string() } }
        };

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/files",
            "multipart/form-data",
            this->m_auth.GetAuthorizationHeaders(),
            std::move(form),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Files::CreateAsync(
        const std::filesystem::path& file,
        const std::string& purpose
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(std::launch::async, &Files::Create, this, file, purpose);
    }

    auto Files::Remove(const std::string& file_id) const& noexcept -> Expected<Response> {
        return this->Request(
            Method::HTTP_DELETE,
            this->GetOpenAIRoot(),
            "/files/" + file_id,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Files::RemoveAsync(const std::string& file_id) const& noexcept
        -> FutureExpected<Response> {
        return std::async(std::launch::async, &Files::Remove, this, file_id);
    }

    auto Files::Retrieve(const std::string& file_id) const& noexcept -> Expected<Response> {
        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/files/" + file_id,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Files::RetrieveAsync(const std::string& file_id) const& noexcept
        -> FutureExpected<Response> {
        return std::async(std::launch::async, &Files::Retrieve, this, file_id);
    }

    auto Files::Download(const std::string& file_id, const std::string& save_to) const& noexcept
        -> Expected<bool> {
        return Network::Download(
            save_to,
            ("https://api.openai.com/v1/files/" + file_id + "/content"),
            this->m_auth.GetAuthorizationHeaders()
        );
    }

    auto
    Files::DownloadAsync(const std::string& file_id, const std::string& save_to) const& noexcept
        -> FutureExpected<bool> {
        return std::async(std::launch::async, &Files::Download, this, file_id, save_to);
    }

} // namespace liboai
