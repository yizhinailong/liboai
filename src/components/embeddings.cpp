#include "liboai/components/embeddings.hpp"

namespace liboai {

    auto Embeddings::create(
        const std::string& model_id,
        std::optional<std::string> input,
        std::optional<std::string> user
    ) const& noexcept(false) -> Response {
        JsonConstructor jcon;
        jcon.push_back("model", model_id);
        jcon.push_back("input", std::move(input));
        jcon.push_back("user", std::move(user));

        Response res;
        res = this->Request(
            Method::HTTP_POST,
            this->m_openai_root,
            "/embeddings",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            netimpl::components::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto Embeddings::create_async(
        const std::string& model_id,
        std::optional<std::string> input,
        std::optional<std::string> user
    ) const& noexcept(false) -> FutureResponse {
        return std::async(std::launch::async, &Embeddings::create, this, model_id, input, user);
    }

} // namespace liboai
