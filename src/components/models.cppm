module;

/**
 * @file models.cppm
 *
 * Models component class for OpenAI. This class contains all the methods
 * for the Models component of the OpenAI API. This class provides access
 * to 'Models' endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI object after
 * setting necessary authentication information through the
 * liboai::Authorization::Authorizer() singleton object.
 */

export module liboai:components.models;

import std;
import :core.authorization;
import :core.error;
import :core.response;
import :core.network;

export namespace liboai {
    class Models final : private Network {
    public:
        explicit Models(const std::string& root) : Network(root) {}

        Models(const Models&) = delete;
        Models& operator=(const Models&) = delete;
        Models(Models&&) = delete;
        Models& operator=(Models&&) = delete;
        ~Models() = default;

        /**
         * @brief List all available models.
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        auto List() const& noexcept -> Result<Response>;

        /**
         * @brief Asynchronously list all available models.
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        auto ListAsync() const& noexcept -> FutureExpected<Response>;

        /**
         * @brief Retrieve a specific model's information.
         *
         * @param model The model to retrieve information for.
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        auto Retrieve(const std::string& model) const& noexcept
            -> liboai::Result<liboai::Response>;

        /**
         * @brief Asynchronously retrieve a specific model's information.
         *
         * @param model The model to retrieve information for.
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        auto RetrieveAsync(const std::string& model) const& noexcept
            -> FutureExpected<Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };

    // Implementation
    auto Models::List() const& noexcept -> Result<Response> {
        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/models",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Models::ListAsync() const& noexcept -> FutureExpected<Response> {
        return std::async(std::launch::async, &liboai::Models::List, this);
    }

    auto Models::Retrieve(const std::string& model) const& noexcept -> Result<Response> {
        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/models/" + model,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Models::RetrieveAsync(const std::string& model) const& noexcept
        -> FutureExpected<Response> {
        return std::async(std::launch::async, &liboai::Models::Retrieve, this, model);
    }

} // namespace liboai
