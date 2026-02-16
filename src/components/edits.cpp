#include "liboai/components/edits.hpp"

namespace liboai {

    auto Edits::create(
        const std::string& model_id,
        std::optional<std::string> input,
        std::optional<std::string> instruction,
        std::optional<uint16_t> n,
        std::optional<float> temperature,
        std::optional<float> top_p
    ) const& noexcept(false) -> Response {
        JsonConstructor jcon;
        jcon.push_back("model", model_id);
        jcon.push_back("input", std::move(input));
        jcon.push_back("instruction", std::move(instruction));
        jcon.push_back("n", std::move(n));
        jcon.push_back("temperature", std::move(temperature));
        jcon.push_back("top_p", std::move(top_p));

        Response res;
        res = this->Request(
            Method::HTTP_POST,
            this->m_openai_root,
            "/edits",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            netimpl::components::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto Edits::create_async(
        const std::string& model_id,
        std::optional<std::string> input,
        std::optional<std::string> instruction,
        std::optional<uint16_t> n,
        std::optional<float> temperature,
        std::optional<float> top_p
    ) const& noexcept(false) -> FutureResponse {
        return std::async(
            std::launch::async,
            &Edits::create,
            this,
            model_id,
            input,
            instruction,
            n,
            temperature,
            top_p
        );
    }

} // namespace liboai
