/**
 * @file azure.cpp
 *
 * Implementation of Azure component for Azure OpenAI endpoints.
 */
#include "liboai/components/azure.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

    auto Azure::CreateCompletion(
        const std::string& resource_name,
        const std::string& deployment_id,
        const std::string& api_version,
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

        cpr::Parameters params;
        params.Add({ "api-version", api_version });

        return this->Request(
            Method::HTTP_POST,
            ("https://" + resource_name + this->GetAzureRoot() + "/deployments/" + deployment_id),
            "/completions",
            "application/json",
            this->m_auth.GetAzureAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            std::move(params),
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

    auto Azure::CreateCompletionAsync(
        const std::string& resource_name,
        const std::string& deployment_id,
        const std::string& api_version,
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
            &Azure::CreateCompletion,
            this,
            resource_name,
            deployment_id,
            api_version,
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

    auto Azure::CreateEmbedding(
        const std::string& resource_name,
        const std::string& deployment_id,
        const std::string& api_version,
        const std::string& input,
        std::optional<std::string> user
    ) const& noexcept -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("input", input);
        jcon.push_back("user", std::move(user));

        cpr::Parameters params;
        params.Add({ "api-version", api_version });

        return this->Request(
            Method::HTTP_POST,
            ("https://" + resource_name + this->GetAzureRoot() + "/deployments/" + deployment_id),
            "/embeddings",
            "application/json",
            this->m_auth.GetAzureAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            std::move(params),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Azure::CreateEmbeddingAsync(
        const std::string& resource_name,
        const std::string& deployment_id,
        const std::string& api_version,
        const std::string& input,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Azure::CreateEmbedding,
            this,
            resource_name,
            deployment_id,
            api_version,
            input,
            user
        );
    }

    auto Azure::CreateChatCompletion(
        const std::string& resource_name,
        const std::string& deployment_id,
        const std::string& api_version,
        Conversation& conversation,
        std::optional<std::string> function_call,
        std::optional<float> temperature,
        std::optional<uint16_t> n,
        std::optional<ChatStreamCallback> stream,
        std::optional<std::vector<std::string>> stop,
        std::optional<uint16_t> max_tokens,
        std::optional<float> presence_penalty,
        std::optional<float> frequency_penalty,
        std::optional<std::unordered_map<std::string, int8_t>> logit_bias,
        std::optional<std::string> user
    ) const& noexcept -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("temperature", std::move(temperature));
        jcon.push_back("n", std::move(n));
        jcon.push_back("stop", std::move(stop));
        jcon.push_back("max_tokens", std::move(max_tokens));
        jcon.push_back("presence_penalty", std::move(presence_penalty));
        jcon.push_back("frequency_penalty", std::move(frequency_penalty));
        jcon.push_back("logit_bias", std::move(logit_bias));
        jcon.push_back("user", std::move(user));

        if (function_call) {
            if (function_call.value() == "none" || function_call.value() == "auto") {
                nlohmann::json j;
                j["function_call"] = function_call.value();
                jcon.push_back("function_call", j["function_call"]);
            } else {
                nlohmann::json j;
                j["function_call"] = {
                    { "name", function_call.value() }
                };
                jcon.push_back("function_call", j["function_call"]);
            }
        }

        StrippedStreamCallback _sscb = nullptr;
        if (stream) {
            _sscb = [stream, &conversation](std::string data, intptr_t userdata) -> bool {
                ChatStreamCallback _stream = stream.value();
                return _stream(data, userdata, conversation);
            };

            jcon.push_back("stream", _sscb);
        }

        if (conversation.GetJSON().contains("messages")) {
            jcon.push_back("messages", conversation.GetJSON()["messages"]);
        }

        if (conversation.HasFunctions()) {
            jcon.push_back("functions", conversation.GetFunctionsJSON()["functions"]);
        }

        cpr::Parameters params;
        params.Add({ "api-version", api_version });

        return this->Request(
            Method::HTTP_POST,
            ("https://" + resource_name + this->GetAzureRoot() + "/deployments/" + deployment_id),
            "/chat/completions",
            "application/json",
            this->m_auth.GetAzureAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            std::move(params),
            _sscb ?
                cpr::WriteCallback{
                    [cb = std::move(_sscb)](std::string_view data, intptr_t userdata) -> bool {
                        return cb(std::string(data), userdata);
                    } } :
                cpr::WriteCallback{},
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Azure::CreateChatCompletionAsync(
        const std::string& resource_name,
        const std::string& deployment_id,
        const std::string& api_version,
        Conversation& conversation,
        std::optional<std::string> function_call,
        std::optional<float> temperature,
        std::optional<uint16_t> n,
        std::optional<ChatStreamCallback> stream,
        std::optional<std::vector<std::string>> stop,
        std::optional<uint16_t> max_tokens,
        std::optional<float> presence_penalty,
        std::optional<float> frequency_penalty,
        std::optional<std::unordered_map<std::string, int8_t>> logit_bias,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Azure::CreateChatCompletion,
            this,
            resource_name,
            deployment_id,
            api_version,
            std::ref(conversation),
            function_call,
            temperature,
            n,
            stream,
            stop,
            max_tokens,
            presence_penalty,
            frequency_penalty,
            logit_bias,
            user
        );
    }

    auto Azure::RequestImageGeneration(
        const std::string& resource_name,
        const std::string& api_version,
        const std::string& prompt,
        std::optional<uint8_t> n,
        std::optional<std::string> size
    ) const& noexcept -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("prompt", prompt);
        jcon.push_back("n", std::move(n));
        jcon.push_back("size", std::move(size));

        cpr::Parameters params;
        params.Add({ "api-version", api_version });

        return this->Request(
            Method::HTTP_POST,
            ("https://" + resource_name + this->GetAzureRoot()),
            "/images/generations:submit",
            "application/json",
            this->m_auth.GetAzureAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            std::move(params),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Azure::RequestImageGenerationAsync(
        const std::string& resource_name,
        const std::string& api_version,
        const std::string& prompt,
        std::optional<uint8_t> n,
        std::optional<std::string> size
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Azure::RequestImageGeneration,
            this,
            resource_name,
            api_version,
            prompt,
            n,
            size
        );
    }

    auto Azure::GetGeneratedImage(
        const std::string& resource_name,
        const std::string& api_version,
        const std::string& operation_id
    ) const& noexcept -> Expected<Response> {
        cpr::Parameters params;
        params.Add({ "api-version", api_version });

        return this->Request(
            Method::HTTP_GET,
            ("https://" + resource_name + this->GetAzureRoot()),
            "/operations/images/" + operation_id,
            "application/json",
            this->m_auth.GetAzureAuthorizationHeaders(),
            std::move(params),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Azure::GetGeneratedImageAsync(
        const std::string& resource_name,
        const std::string& api_version,
        const std::string& operation_id
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Azure::GetGeneratedImage,
            this,
            resource_name,
            api_version,
            operation_id
        );
    }

    auto Azure::DeleteGeneratedImage(
        const std::string& resource_name,
        const std::string& api_version,
        const std::string& operation_id
    ) const& noexcept -> Expected<Response> {
        cpr::Parameters params;
        params.Add({ "api-version", api_version });

        return this->Request(
            Method::HTTP_DELETE,
            ("https://" + resource_name + this->GetAzureRoot()),
            "/operations/images/" + operation_id,
            "application/json",
            this->m_auth.GetAzureAuthorizationHeaders(),
            std::move(params),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Azure::DeleteGeneratedImageAsync(
        const std::string& resource_name,
        const std::string& api_version,
        const std::string& operation_id
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Azure::DeleteGeneratedImage,
            this,
            resource_name,
            api_version,
            operation_id
        );
    }

} // namespace liboai
