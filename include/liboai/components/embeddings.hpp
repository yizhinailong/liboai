#pragma once

/**
 * @file embeddings.hpp
 * @brief Embeddings component class for OpenAI.
 *
 * This class contains all the methods for the Embeddings component
 * of the OpenAI API. This class provides access to 'Embeddings'
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
        LIBOAI_EXPORT auto create(
            const std::string& model_id,
            std::optional<std::string> input = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

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
        LIBOAI_EXPORT auto create_async(
            const std::string& model_id,
            std::optional<std::string> input = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };
} // namespace liboai
