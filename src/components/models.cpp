/**
 * @file models.cpp
 *
 * Implementation of Models component for model listing/retrieval API.


 */

#include "liboai/components/models.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

    auto Models::list() const& noexcept -> Expected<Response> {
        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/models",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Models::list_async() const& noexcept -> FutureExpected<Response> {
        return std::async(std::launch::async, &liboai::Models::list, this);
    }

    auto Models::retrieve(const std::string& model) const& noexcept -> Expected<Response> {
        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/models/" + model,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Models::retrieve_async(const std::string& model) const& noexcept
        -> FutureExpected<Response> {
        return std::async(std::launch::async, &liboai::Models::retrieve, this, model);
    }

} // namespace liboai
