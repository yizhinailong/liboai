/**
 * @file moderations.cpp
 *
 * Implementation of Moderations component for content moderation API.
 */
#include "liboai/components/moderations.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

    auto
    Moderations::Create(const std::string& input, std::optional<std::string> model) const& noexcept
        -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("input", input);
        jcon.push_back("model", std::move(model));

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/moderations",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Moderations::CreateAsync(
        const std::string& input,
        std::optional<std::string> model
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(std::launch::async, &Moderations::Create, this, input, model);
    }

} // namespace liboai
