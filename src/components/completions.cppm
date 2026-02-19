module;

#include <expected>
#include <future>
#include <cpr/cpr.h>

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @file completions.cppm
 *
 * Completions component class for OpenAI. This class contains all the methods
 * for the Completions component of the OpenAI API. This class provides access
 * to 'Completions' endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI object after
 * setting necessary authentication information through the
 * liboai::Authorization::Authorizer() singleton object.
 */

export module liboai:components.completions;

import :core.authorization;
import :core.error;
import :core.response;
import :core.network;

export namespace liboai {
    class Completions final : private Network {
    public:
        explicit Completions(const std::string& root) : Network(root) {}

        Completions(const Completions&) = delete;
        Completions& operator=(const Completions&) = delete;
        Completions(Completions&&) = delete;
        Completions& operator=(Completions&&) = delete;
        ~Completions() = default;

        using StreamCallback = std::function<bool(std::string, intptr_t)>;

        /**
         * @brief Given a prompt, the model will return one or more
         * predicted completions asynchronously, and can also
         * return the probabilities of alternative tokens at each
         * position.
         *
         * @param *model             The model to use for completion.
         * @param prompt             The prompt(s) to generate completions for.
         * @param suffix             The suffix that comes after a completion of inserted text.
         * @param max_tokens         The maximum number of tokens to generate in a completion.
         * @param temperature        The temperature for the model. Higher values will result in
         *                           more creative completions, while lower values will result in
         * more repetitive completions.
         * @param top_p              The top_p for the model. This is the probability mass that the
         *                           model will consider when making predictions. Lower values
         *                           will result in more creative completions, while higher values
         * will result in more repetitive completions.
         * @param n                  The number of completions to generate.
         * @param stream             Stream partial progress back to the client. A callback
         *                           function that is called each time new data is received from the
         * API. If no callback is supplied, this parameter is disabled and the API will wait until
         * the completion is finished before returning the response.
         *
         * @param logprobs           The number of log probabilities to return for each token.
         * @param echo               Whether to include the prompt in the returned completion.
         * @param stop               A list of tokens that the model will stop generating
         *                           completions at. This can be a single token or a list of tokens.
         * @param presence_penalty   The presence penalty for the model. This is a number between
         *                           -2.0 and 2.0. Positive values penalize new tokens based on
         *                           whether they appear in the text so far, increasing the
         *                           model's likelihood to talk about new topics.
         * @param frequency_penalty  The frequency penalty for the model. This is a number between
         *                           -2.0 and 2.0. Positive values penalize new tokens based on
         *                           their existing frequency in the text so far, decreasing the
         *                           model's likelihood to repeat the same line verbatim.
         * @param best_of            Generates best_of completions server-side and returns the
         *                           "best" one. When used with n, best_of controls the number of
         * candidate completions and n specifies how many to return – best_of must be greater than n
         *
         *                           Because this parameter generates many completions, it can
         *                           quickly consume your token quota. Use carefully and ensure that
         * you have reasonable settings for max_tokens and stop.
         * @param logit_bias         Modify the likelihood of specified tokens appearing in the
         *                           completion. Accepts a json object that maps tokens (specified
         * by their token ID in the GPT tokenizer) to an associated bias value from -100 to 100.
         * @param user               A unique identifier representing your end-user.
         *
         * @return A liboai::Response future containing the image(s)
         * data in JSON format.
         */
        [[nodiscard]]
        auto Create(
            const std::string& model_id,
            std::optional<std::string> prompt = std::nullopt,
            std::optional<std::string> suffix = std::nullopt,
            std::optional<uint16_t> max_tokens = std::nullopt,
            std::optional<float> temperature = std::nullopt,
            std::optional<float> top_p = std::nullopt,
            std::optional<uint16_t> n = std::nullopt,
            std::optional<StreamCallback> stream = std::nullopt,
            std::optional<uint8_t> logprobs = std::nullopt,
            std::optional<bool> echo = std::nullopt,
            std::optional<std::vector<std::string>> stop = std::nullopt,
            std::optional<float> presence_penalty = std::nullopt,
            std::optional<float> frequency_penalty = std::nullopt,
            std::optional<uint16_t> best_of = std::nullopt,
            std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Given a prompt, the model will return one or more
         * predicted completions asynchronously, and can also
         * return the probabilities of alternative tokens at each
         * position.
         *
         * @param *model             The model to use for completion.
         * @param prompt             The prompt(s) to generate completions for.
         * @param suffix             The suffix that comes after a completion of inserted text.
         * @param max_tokens         The maximum number of tokens to generate in a completion.
         * @param temperature        The temperature for the model. Higher values will result in
         *                           more creative completions, while lower values will result in
         *                           more repetitive completions.
         * @param top_p              The top_p for the model. This is the probability mass that the
         *                           model will consider when making predictions. Lower values
         *                           will result in more creative completions, while higher values
         *                           will result in more repetitive completions.
         * @param n                  The number of completions to generate.
         * @param stream             Stream partial progress back to the client. A callback
         *                           function that is called each time new data is received from
         *                           the API. If no callback is supplied, this parameter is disabled
         *                           and the API will wait until the completion is finished before
         *                           returning the response.
         *
         * @param logprobs           The number of log probabilities to return for each token.
         * @param echo               Whether to include the prompt in the returned completion.
         * @param stop               A list of tokens that the model will stop generating
         *                           completions at. This can be a single token or a list of
         *                           tokens.
         * @param presence_penalty   The presence penalty for the model. This is a number between
         *                           -2.0 and 2.0. Positive values penalize new tokens based on
         *                           whether they appear in the text so far, increasing the
         *                           model's likelihood to talk about new topics.
         * @param frequency_penalty  The frequency penalty for the model. This is a number between
         *                           -2.0 and 2.0. Positive values penalize new tokens based on
         *                           their existing frequency in the text so far, decreasing the
         *                           model's likelihood to repeat the same line verbatim.
         * @param best_of            Generates best_of completions server-side and returns the
         *                           "best" one. When used with n, best_of controls the number of
         *                           candidate completions and n specifies how many to return –
         *                           best_of must be greater than n.
         *
         *                           Because this parameter generates many completions, it can
         *                           quickly consume your token quota. Use carefully and ensure
         *                           that you have reasonable settings for max_tokens and stop.
         * @param logit_bias         Modify the likelihood of specified tokens appearing in the
         *                           completion. Accepts a json object that maps tokens (specified
         *                           by their token ID in the GPT tokenizer) to an associated bias
         *                           value from -100 to 100.
         * @param user               A unique identifier representing your end-user.
         *
         * @return A liboai::Response future containing the image(s)
         * data in JSON format.
         */
        [[nodiscard]]
        auto CreateAsync(
            const std::string& model_id,
            std::optional<std::string> prompt = std::nullopt,
            std::optional<std::string> suffix = std::nullopt,
            std::optional<uint16_t> max_tokens = std::nullopt,
            std::optional<float> temperature = std::nullopt,
            std::optional<float> top_p = std::nullopt,
            std::optional<uint16_t> n = std::nullopt,
            std::optional<StreamCallback> stream = std::nullopt,
            std::optional<uint8_t> logprobs = std::nullopt,
            std::optional<bool> echo = std::nullopt,
            std::optional<std::vector<std::string>> stop = std::nullopt,
            std::optional<float> presence_penalty = std::nullopt,
            std::optional<float> frequency_penalty = std::nullopt,
            std::optional<uint16_t> best_of = std::nullopt,
            std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };

    // Implementation
    auto Completions::Create(
        const std::string& model_id,
        std::optional<std::string> prompt,
        std::optional<std::string> suffix,
        std::optional<uint16_t> max_tokens,
        std::optional<float> temperature,
        std::optional<float> top_p,
        std::optional<uint16_t> n,
        std::optional<StreamCallback> stream,
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

    auto Completions::CreateAsync(
        const std::string& model_id,
        std::optional<std::string> prompt,
        std::optional<std::string> suffix,
        std::optional<uint16_t> max_tokens,
        std::optional<float> temperature,
        std::optional<float> top_p,
        std::optional<uint16_t> n,
        std::optional<StreamCallback> stream,
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
            &liboai::Completions::Create,
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
