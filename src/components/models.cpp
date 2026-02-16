#include "liboai/components/models.hpp"

namespace liboai {

    auto Models::list() const& noexcept(false) -> Response {
        Response res;
        res = this->Request(
            Method::HTTP_GET,
            this->openai_root_,
            "/models",
            "application/json",
            this->auth_.GetAuthorizationHeaders(),
            this->auth_.GetProxies(),
            this->auth_.GetProxyAuth(),
            this->auth_.GetMaxTimeout()
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
            this->openai_root_,
            "/models/" + model,
            "application/json",
            this->auth_.GetAuthorizationHeaders(),
            this->auth_.GetProxies(),
            this->auth_.GetProxyAuth(),
            this->auth_.GetMaxTimeout()
        );

        return res;
    }

    auto Models::retrieve_async(const std::string& model) const& noexcept(false) -> FutureResponse {
        return std::async(std::launch::async, &liboai::Models::retrieve, this, model);
    }

} // namespace liboai
