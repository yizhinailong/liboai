#include "liboai/components/files.hpp"

namespace liboai {

    auto Files::list() const& noexcept(false) -> Response {
        Response res;
        res = this->Request(
            Method::HTTP_GET,
            this->m_openai_root,
            "/files",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto Files::list_async() const& noexcept(false) -> FutureResponse {
        return std::async(std::launch::async, &Files::list, this);
    }

    auto Files::create(
        const std::filesystem::path& file,
        const std::string& purpose
    ) const& noexcept(false) -> Response {
        if (!this->Validate(file)) {
            throw exception::OpenAIException(
                "File path provided is non-existent, is not a file, or is empty.",
                exception::EType::E_FILEERROR,
                "liboai::Files::create(...)"
            );
        }

        netimpl::components::Multipart form = {
            { "purpose",                                            purpose },
            {    "file", netimpl::components::File{ file.generic_string() } }
        };

        Response res;
        res = this->Request(
            Method::HTTP_POST,
            this->m_openai_root,
            "/files",
            "multipart/form-data",
            this->m_auth.GetAuthorizationHeaders(),
            std::move(form),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto Files::create_async(
        const std::filesystem::path& file,
        const std::string& purpose
    ) const& noexcept(false) -> FutureResponse {
        return std::async(std::launch::async, &Files::create, this, file, purpose);
    }

    auto Files::remove(const std::string& file_id) const& noexcept(false) -> Response {
        Response res;
        res = this->Request(
            Method::HTTP_DELETE,
            this->m_openai_root,
            "/files/" + file_id,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto Files::remove_async(const std::string& file_id) const& noexcept(false) -> FutureResponse {
        return std::async(std::launch::async, &Files::remove, this, file_id);
    }

    auto Files::retrieve(const std::string& file_id) const& -> Response {
        Response res;
        res = this->Request(
            Method::HTTP_GET,
            this->m_openai_root,
            "/files/" + file_id,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto Files::retrieve_async(const std::string& file_id) const& noexcept(false)
        -> FutureResponse {
        return std::async(std::launch::async, &Files::retrieve, this, file_id);
    }

    auto
    Files::download(const std::string& file_id, const std::string& save_to) const& noexcept(false)
        -> bool {
        return Network::Download(
            save_to,
            ("https://api.openai.com/v1/files/" + file_id + "/content"),
            this->m_auth.GetAuthorizationHeaders()
        );
    }

    auto Files::download_async(
        const std::string& file_id,
        const std::string& save_to
    ) const& noexcept(false) -> std::future<bool> {
        return std::async(std::launch::async, &Files::download, this, file_id, save_to);
    }

} // namespace liboai
