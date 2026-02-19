#pragma once

/**
 * @file edits.h
 * Edits component class for OpenAI.
 * This class contains all the methods for the Edits component
 * of the OpenAI API. This class provides access to 'Edits'
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
    class Edits final : private Network {
    public:
        explicit Edits(const std::string& root) : Network(root) {}

        Edits(const Edits&) = delete;
        Edits& operator=(const Edits&) = delete;
        Edits(Edits&&) = delete;
        Edits& operator=(Edits&&) = delete;
        ~Edits() = default;

        /**
         * @brief Creates a new edit for the provided input,
         * instruction, and parameters
         *
         * @param *model       The model to use for the edit.
         * @param input        The input text to edit.
         * @param instruction  The instruction to edit the input.
         * @param n            The number of edits to return.
         * @param temperature  Higher values means the model will take more
         *                     risks. Try 0.9 for more creative applications,
         *                     and 0 (argmax sampling) for ones with a
         *                     well-defined answer.
         * @param top_p        An alternative to sampling with temperature,
         *                     called nucleus sampling, where the model
         *                     considers the results of the tokens with
         *                     top_p probability mass. So 0.1 means only
         *                     the tokens comprising the top 10% probability
         *                     mass are considered.
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto Create(
            const std::string& model_id,
            std::optional<std::string> input = std::nullopt,
            std::optional<std::string> instruction = std::nullopt,
            std::optional<uint16_t> n = std::nullopt,
            std::optional<float> temperature = std::nullopt,
            std::optional<float> top_p = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously creates a new edit for the
         * provided input, instruction, and parameters
         *
         * @param *model       The model to use for the edit.
         * @param input        The input text to edit.
         * @param instruction  The instruction to edit the input.
         * @param n            The number of edits to return.
         * @param temperature  Higher values means the model will take more
         *                     risks. Try 0.9 for more creative applications,
         *                     and 0 (argmax sampling) for ones with a
         *                     well-defined answer.
         * @param top_p        An alternative to sampling with temperature,
         *                     called nucleus sampling, where the model
         *                     considers the results of the tokens with
         *                     top_p probability mass. So 0.1 means only
         *                     the tokens comprising the top 10% probability
         *                     mass are considered.
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto CreateAsync(
            const std::string& model_id,
            std::optional<std::string> input = std::nullopt,
            std::optional<std::string> instruction = std::nullopt,
            std::optional<uint16_t> n = std::nullopt,
            std::optional<float> temperature = std::nullopt,
            std::optional<float> top_p = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };
} // namespace liboai
