module;

#include <cpr/cpr.h>

/**
 * @file embeddings.cppm
 *
 * Embeddings component class for OpenAI. This class contains all the methods
 * for the Embeddings component of the OpenAI API. This class provides access
 * to 'Embeddings' endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI object after
 * setting necessary authentication information through the
 * liboai::Authorization::Authorizer() singleton object.
 */

export module liboai:components.embeddings;

import std;
import :core.authorization;
import :core.error;
import :core.response;
import :core.network;

export namespace liboai {
    class Embeddings final : private Network {
    public:
        explicit Embeddings(const std::string& root) : Network(root) {}

        Embeddings(const Embeddings&) = delete;
        Embeddings& operator=(const Embeddings&) = delete;
        Embeddings(Embeddings&&) = delete;
        Embeddings& operator=(Embeddings&&) = delete;
        ~Embeddings() = default;

        /**
         * @brief Creates an embedding vector representing the input text.
         *
         * @param *model       The model to use for the edit.
         * @param input        The input text to edit.
         * @param user         A unique identifier representing your end-user
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        auto Create(
            const std::string& model_id,
            std::optional<std::string> input = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> Result<Response>;

        /**
         * @brief Asynchronously creates an embedding vector representing the input text.
         *
         * @param *model       The model to use for the edit.
         * @param input        The input text to edit.
         * @param user         A unique identifier representing your end-user
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        auto CreateAsync(
            const std::string& model_id,
            std::optional<std::string> input = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> FutureExpected<Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };

    // Implementation
    auto Embeddings::Create(
        const std::string& model_id,
        std::optional<std::string> input,
        std::optional<std::string> user
    ) const& noexcept -> Result<Response> {
        JsonConstructor jcon;
        jcon.push_back("model", model_id);
        jcon.push_back("input", std::move(input));
        jcon.push_back("user", std::move(user));

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/embeddings",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Embeddings::CreateAsync(
        const std::string& model_id,
        std::optional<std::string> input,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(std::launch::async, &Embeddings::Create, this, model_id, input, user);
    }

} // namespace liboai
