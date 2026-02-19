#pragma once

/**
 * @file models.h
 *
 * Models component class for OpenAI. This class contains all the methods
 * for the Models component of the OpenAI API. This class provides access
 * to 'Models' endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI object after
 * setting necessary authentication information through the
 * liboai::Authorization::Authorizer() singleton object.
 */

#include "liboai/core/authorization.hpp"
#include "liboai/core/error.hpp"
#include "liboai/core/response.hpp"

namespace liboai {
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
        LIBOAI_EXPORT auto list() const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously list all available models.
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto list_async() const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Retrieve a specific model's information.
         *
         * @param model The model to retrieve information for.
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto retrieve(const std::string& model) const& noexcept
            -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously retrieve a specific model's information.
         *
         * @param model The model to retrieve information for.
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto retrieve_async(const std::string& model) const& noexcept
            -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };
} // namespace liboai
