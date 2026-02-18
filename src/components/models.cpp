/**
 * @file models.cpp
 *
 * Implementation of Models component for model listing/retrieval API.

 */

#include "liboai/components/models.hpp"

namespace liboai {

    auto Models::list() const& noexcept(false) -> Response {
        Response res;
        res = this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/models",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto Models::list_async() const& noexcept(false) -> FutureResponse {
        return std::async(std::launch::async, &liboai::Models::list, this);
    }

    auto Models::retrieve(const std::string& model) const& noexcept(false) -> Response {
        Response res;
        res = this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/models/" + model,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto Models::retrieve_async(const std::string& model) const& noexcept(false) -> FutureResponse {
        return std::async(std::launch::async, &liboai::Models::retrieve, this, model);
    }

} // namespace liboai
