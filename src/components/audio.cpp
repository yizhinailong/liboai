/**
 * @file audio.cpp
 *
 * Implementation of Audio component for speech/transcription/translation.
 */
#include "liboai/components/audio.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

    auto Audio::transcribe(
        const std::filesystem::path& file,
        const std::string& model,
        std::optional<std::string> prompt,
        std::optional<std::string> response_format,
        std::optional<float> temperature,
        std::optional<std::string> language
    ) const& noexcept -> Expected<Response> {
        if (!this->Validate(file)) {
            return std::unexpected(
                OpenAIError::file_error(
                    "File path provided is non-existent, is not a file, or is empty."
                )
            );
        }

        cpr::Multipart form = {
            {  "file", cpr::File{ file.generic_string() } },
            { "model",                              model }
        };

        if (prompt) {
            form.parts.emplace_back("prompt", prompt.value());
        }
        if (response_format) {
            form.parts.emplace_back("response_format", response_format.value());
        }
        if (temperature) {
            form.parts.emplace_back("temperature", std::to_string(temperature.value()));
        }
        if (language) {
            form.parts.emplace_back("language", language.value());
        }

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/audio/transcriptions",
            "multipart/form-data",
            this->m_auth.GetAuthorizationHeaders(),
            std::move(form),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Audio::transcribe_async(
        const std::filesystem::path& file,
        const std::string& model,
        const std::optional<std::string>& prompt,
        const std::optional<std::string>& response_format,
        std::optional<float> temperature,
        const std::optional<std::string>& language
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::Audio::transcribe,
            this,
            file,
            model,
            prompt,
            response_format,
            temperature,
            language
        );
    }

    auto Audio::translate(
        const std::filesystem::path& file,
        const std::string& model,
        std::optional<std::string> prompt,
        std::optional<std::string> response_format,
        std::optional<float> temperature
    ) const& noexcept -> Expected<Response> {
        if (!this->Validate(file)) {
            return std::unexpected(
                OpenAIError::file_error(
                    "File path provided is non-existent, is not a file, or is empty."
                )
            );
        }

        cpr::Multipart form = {
            {  "file", cpr::File{ file.generic_string() } },
            { "model",                              model }
        };

        if (prompt) {
            form.parts.emplace_back("prompt", std::move(prompt.value()));
        }
        if (response_format) {
            form.parts.emplace_back("response_format", std::move(response_format.value()));
        }
        if (temperature) {
            form.parts.emplace_back("temperature", std::to_string(temperature.value()));
        }

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/audio/translations",
            "multipart/form-data",
            this->m_auth.GetAuthorizationHeaders(),
            std::move(form),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Audio::translate_async(
        const std::filesystem::path& file,
        const std::string& model,
        const std::optional<std::string>& prompt,
        const std::optional<std::string>& response_format,
        std::optional<float> temperature
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::Audio::translate,
            this,
            file,
            model,
            prompt,
            response_format,
            temperature
        );
    }

    auto Audio::speech(
        const std::string& model,
        const std::string& voice,
        const std::string& input,
        std::optional<std::string> response_format,
        std::optional<float> speed
    ) const& noexcept -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("model", model);
        jcon.push_back("voice", voice);
        jcon.push_back("input", input);

        if (response_format) {
            jcon.push_back("response_format", response_format.value());
        }
        if (speed) {
            jcon.push_back("speed", speed.value());
        }

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/audio/speech",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Audio::speech_async(
        const std::string& model,
        const std::string& voice,
        const std::string& input,
        const std::optional<std::string>& response_format,
        std::optional<float> speed
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::Audio::speech,
            this,
            model,
            voice,
            input,
            response_format,
            speed
        );
    }

} // namespace liboai
