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
            this->m_openai_root,
            "/moderations",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            netimpl::components::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
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
