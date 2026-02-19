/**
 * @file embeddings.cpp
 *
 * Implementation of Embeddings component for text embedding API.
 */

#include "liboai/components/embeddings.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

    auto Embeddings::create(
        const std::string& model_id,
        std::optional<std::string> input,
        std::optional<std::string> user
    ) const& noexcept -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("model", model_id);
        jcon.push_back("input", std::move(input));
        jcon.push_back("user", std::move(user));

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/embeddings",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Embeddings::create_async(
        const std::string& model_id,
        std::optional<std::string> input,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(std::launch::async, &Embeddings::create, this, model_id, input, user);
    }

} // namespace liboai
