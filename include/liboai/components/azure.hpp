#pragma once

/**
 * @file azure.hpp
 * Azure component class for OpenAI.
 * Azure provides their own API for access to the OpenAI API.
 * This class provides methods that, provided that the proper
 * Azure authentication information has been set, allows users
 * to access the OpenAI API through Azure.
 */

#include "liboai/components/chat.hpp"
#include "liboai/core/authorization.hpp"
#include "liboai/core/error.hpp"
#include "liboai/core/response.hpp"

namespace liboai {
    class Azure final : private Network {
    public:
        explicit Azure(const std::string& root) : Network(root) {}

        Azure(const Azure&) = delete;
        Azure& operator=(const Azure&) = delete;
        Azure(Azure&&) = delete;
        Azure& operator=(Azure&&) = delete;
        ~Azure() = default;

        using ChatStreamCallback = std::function<bool(std::string, intptr_t, Conversation&)>;
        using StreamCallback = std::function<bool(std::string, intptr_t)>;

        /**
         * @brief Given a prompt, the model will return one or more
         *        predicted completions, and can also return the
         *        probabilities of alternative tokens at each position.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *deployment_id The deployment name you chose when you deployed the model.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param Refer to liboai::Completions::create for more information on the remaining
         * parameters.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto CreateCompletion(
            const std::string& resource_name,
            const std::string& deployment_id,
            const std::string& api_version,
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
         * @brief Given a prompt, the model will asynchronously return
         *        one or more predicted completions, and can also return the
         *        probabilities of alternative tokens at each position.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *deployment_id The deployment name you chose when you deployed the model.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param Refer to liboai::Completions::create for more information on the remaining
         * parameters.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto CreateCompletionAsync(
            const std::string& resource_name,
            const std::string& deployment_id,
            const std::string& api_version,
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

        /**
         * @brief Creates an embedding vector representing the input text.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *deployment_id The deployment name you chose when you deployed the model.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *input Input text to get embeddings for, encoded as a string. The number of input
         * tokens varies depending on what model you are using.
         * @param Refer to liboai::Embeddings::create for more information on the remaining
         * parameters.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto CreateEmbedding(
            const std::string& resource_name,
            const std::string& deployment_id,
            const std::string& api_version,
            const std::string& input,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously creates an embedding vector representing the input text.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *deployment_id The deployment name you chose when you deployed the model.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *input Input text to get embeddings for, encoded as a string. The number of input
         * tokens varies depending on what model you are using.
         * @param Refer to liboai::Embeddings::create for more information on the remaining
         * parameters.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto CreateEmbeddingAsync(
            const std::string& resource_name,
            const std::string& deployment_id,
            const std::string& api_version,
            const std::string& input,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Creates a completion for the chat message.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *deployment_id The deployment name you chose when you deployed the model.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *conversation A Conversation object containing the conversation data.
         * @param Refer to liboai::Chat::create for more information on the remaining parameters.
         *
         * @return A liboai::Response object containing the data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto CreateChatCompletion(
            const std::string& resource_name,
            const std::string& deployment_id,
            const std::string& api_version,
            Conversation& conversation,
            std::optional<std::string> function_call = std::nullopt,
            std::optional<float> temperature = std::nullopt,
            std::optional<uint16_t> n = std::nullopt,
            std::optional<ChatStreamCallback> stream = std::nullopt,
            std::optional<std::vector<std::string>> stop = std::nullopt,
            std::optional<uint16_t> max_tokens = std::nullopt,
            std::optional<float> presence_penalty = std::nullopt,
            std::optional<float> frequency_penalty = std::nullopt,
            std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously creates a completion for the chat message.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *deployment_id The deployment name you chose when you deployed the model.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *conversation A Conversation object containing the conversation data.
         * @param Refer to liboai::Chat::create for more information on the remaining parameters.
         *
         * @return A liboai::Response object containing the data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto CreateChatCompletionAsync(
            const std::string& resource_name,
            const std::string& deployment_id,
            const std::string& api_version,
            Conversation& conversation,
            std::optional<std::string> function_call = std::nullopt,
            std::optional<float> temperature = std::nullopt,
            std::optional<uint16_t> n = std::nullopt,
            std::optional<ChatStreamCallback> stream = std::nullopt,
            std::optional<std::vector<std::string>> stop = std::nullopt,
            std::optional<uint16_t> max_tokens = std::nullopt,
            std::optional<float> presence_penalty = std::nullopt,
            std::optional<float> frequency_penalty = std::nullopt,
            std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Generate a batch of images from a text caption.
         *        Image generation is currently only available with api-version=2023-06-01-preview.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *prompt The text to create an image from.
         * @param n The number of images to create.
         * @param size The size of the image to create.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto RequestImageGeneration(
            const std::string& resource_name,
            const std::string& api_version,
            const std::string& prompt,
            std::optional<uint8_t> n = std::nullopt,
            std::optional<std::string> size = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously generate a batch of images from a text caption.
         *        Image generation is currently only available with api-version=2023-06-01-preview.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *prompt The text to create an image from.
         * @param n The number of images to create.
         * @param size The size of the image to create.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto RequestImageGenerationAsync(
            const std::string& resource_name,
            const std::string& api_version,
            const std::string& prompt,
            std::optional<uint8_t> n = std::nullopt,
            std::optional<std::string> size = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Retrieve the results (URL) of a previously called image generation operation.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *operation_id The GUID that identifies the original image generation request.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto GetGeneratedImage(
            const std::string& resource_name,
            const std::string& api_version,
            const std::string& operation_id
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously retrieve the results (URL) of a previously called image generation
         * operation.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *operation_id The GUID that identifies the original image generation request.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto GetGeneratedImageAsync(
            const std::string& resource_name,
            const std::string& api_version,
            const std::string& operation_id
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Deletes the corresponding image from the Azure server.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *operation_id The GUID that identifies the original image generation request.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto DeleteGeneratedImage(
            const std::string& resource_name,
            const std::string& api_version,
            const std::string& operation_id
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously deletes the corresponding image from the Azure server.
         *
         * @param *resource_name The name of your Azure OpenAI Resource.
         * @param *api_version The API version to use for this operation. This follows the
         * YYYY-MM-DD format.
         * @param *operation_id The GUID that identifies the original image generation request.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto DeleteGeneratedImageAsync(
            const std::string& resource_name,
            const std::string& api_version,
            const std::string& operation_id
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
        using StrippedStreamCallback = std::function<bool(std::string, intptr_t)>;
    };
} // namespace liboai
