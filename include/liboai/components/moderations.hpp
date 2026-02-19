#pragma once

/**
 * @file moderations.h
 * @brief Moderations component class for OpenAI.
 *
 * This class contains all the methods for the Moderations component
 * of the OpenAI API. This class provides access to 'Moderations'
 * endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI
 * object after setting necessary authentication information
 * through the liboai::Authorization::Authorizer() singleton
 * object.
 */

#include "liboai/core/authorization.hpp"
#include "liboai/core/error.hpp"
#include "liboai/core/response.hpp"

namespace liboai {
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
        LIBOAI_EXPORT auto Create(
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
        LIBOAI_EXPORT auto CreateAsync(
            const std::string& input,
            std::optional<std::string> model = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };
} // namespace liboai
