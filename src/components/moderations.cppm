module;

#include <expected>
#include <future>
#include <cpr/cpr.h>
#include <optional>
#include <string>

/**
 * @file moderations.cppm
 *
 * Moderations component class for OpenAI. This class contains all the methods
 * for the Moderations component of the OpenAI API. This class provides access
 * to 'Moderations' endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI object after
 * setting necessary authentication information through the
 * liboai::Authorization::Authorizer() singleton object.
 */

export module liboai:components.moderations;

import :core.authorization;
import :core.error;
import :core.response;
import :core.network;

export namespace liboai {
    class Moderations final : private Network {
    public:
        explicit Moderations(const std::string& root) : Network(root) {}

        Moderations(const Moderations&) = delete;
        Moderations& operator=(const Moderations&) = delete;
        Moderations(Moderations&&) = delete;
        Moderations& operator=(Moderations&&) = delete;
        ~Moderations() = default;

        /**
         * @brief Create a new moderation and classify
         *        if the given text is safe or unsafe.
         *
         * @param *input The text to be moderated.
         * @param model  The model to use for the moderation.
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        auto Create(
            const std::string& input,
            std::optional<std::string> model = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously creates a new moderation and classifies
         *        if the given text is safe or unsafe.
         *
         * @param *input The text to be moderated.
         * @param model  The model to use for the moderation.
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        auto CreateAsync(
            const std::string& input,
            std::optional<std::string> model = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };

    // Implementation
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
