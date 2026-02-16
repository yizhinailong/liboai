#include "liboai/components/moderations.hpp"

namespace liboai {

    auto Moderations::create(
        const std::string& input,
        std::optional<std::string> model
    ) const& noexcept(false) -> Response {
        JsonConstructor jcon;
        jcon.push_back("input", input);
        jcon.push_back("model", std::move(model));

        Response res;
        res = this->Request(
            Method::HTTP_POST,
            this->openai_root_,
            "/moderations",
            "application/json",
            this->auth_.GetAuthorizationHeaders(),
            netimpl::components::Body{ jcon.dump() },
            this->auth_.GetProxies(),
            this->auth_.GetProxyAuth(),
            this->auth_.GetMaxTimeout()
        );

        return res;
    }

    auto Moderations::create_async(
        const std::string& input,
        std::optional<std::string> model
    ) const& noexcept(false) -> FutureResponse {
        return std::async(std::launch::async, &Moderations::create, this, input, model);
    }

} // namespace liboai
