/**
 * @file completions.cpp
 *
 * Implementation of Completions component for text completion
 * API.

 */
#include "liboai/components/completions.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

    auto Completions::create(
        const std::string& model_id,
        std::optional<std::string> prompt,
        std::optional<std::string> suffix,
        std::optional<uint16_t> max_tokens,
        std::optional<float> temperature,
        std::optional<float> top_p,
        std::optional<uint16_t> n,
        std::optional<std::function<bool(std::string, intptr_t)>> stream,
        std::optional<uint8_t> logprobs,
        std::optional<bool> echo,
        std::optional<std::vector<std::string>> stop,
        std::optional<float> presence_penalty,
        std::optional<float> frequency_penalty,
        std::optional<uint16_t> best_of,
        std::optional<std::unordered_map<std::string, int8_t>> logit_bias,
        std::optional<std::string> user
    ) const& noexcept -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("model", model_id);
        jcon.push_back("prompt", std::move(prompt));
        jcon.push_back("suffix", std::move(suffix));
        jcon.push_back("max_tokens", std::move(max_tokens));
        jcon.push_back("temperature", std::move(temperature));
        jcon.push_back("top_p", std::move(top_p));
        jcon.push_back("n", std::move(n));
        jcon.push_back("stream", stream);
        jcon.push_back("logprobs", std::move(logprobs));
        jcon.push_back("echo", std::move(echo));
        jcon.push_back("stop", std::move(stop));
        jcon.push_back("presence_penalty", std::move(presence_penalty));
        jcon.push_back("frequency_penalty", std::move(frequency_penalty));
        jcon.push_back("best_of", std::move(best_of));
        jcon.push_back("logit_bias", std::move(logit_bias));
        jcon.push_back("user", std::move(user));

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/completions",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            stream ? cpr::WriteCallback{ [cb = std::move(stream.value())](
                                             std::string_view data,
                                             intptr_t userdata
                                         ) -> bool { return cb(std::string(data), userdata); } } :
                     cpr::WriteCallback{},
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Completions::create_async(
        const std::string& model_id,
        std::optional<std::string> prompt,
        std::optional<std::string> suffix,
        std::optional<uint16_t> max_tokens,
        std::optional<float> temperature,
        std::optional<float> top_p,
        std::optional<uint16_t> n,
        std::optional<std::function<bool(std::string, intptr_t)>> stream,
        std::optional<uint8_t> logprobs,
        std::optional<bool> echo,
        std::optional<std::vector<std::string>> stop,
        std::optional<float> presence_penalty,
        std::optional<float> frequency_penalty,
        std::optional<uint16_t> best_of,
        std::optional<std::unordered_map<std::string, int8_t>> logit_bias,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::Completions::create,
            this,
            model_id,
            prompt,
            suffix,
            max_tokens,
            temperature,
            top_p,
            n,
            stream,
            logprobs,
            echo,
            stop,
            presence_penalty,
            frequency_penalty,
            best_of,
            logit_bias,
            user
        );
    }

} // namespace liboai
