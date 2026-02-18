/**
 * @file audio.cpp
 *
 * Implementation of Audio component for speech/transcription/translation.
 */
#include "liboai/components/audio.hpp"

namespace liboai {

    auto Audio::transcribe(
        const std::filesystem::path& file,
        const std::string& model,
        std::optional<std::string> prompt,
        std::optional<std::string> response_format,
        std::optional<float> temperature,
        std::optional<std::string> language
    ) const& noexcept(false) -> Response {
        if (!this->Validate(file)) {
            throw liboai::exception::OpenAIException(
                "File path provided is non-existent, is not a file, or is empty.",
                liboai::exception::EType::E_FILEERROR,
                "liboai::Audio::transcribe(...)"
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

        Response res;
        res = this->Request(
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

        return res;
    }

    auto Audio::transcribe_async(
        const std::filesystem::path& file,
        const std::string& model,
        std::optional<std::string> prompt,
        std::optional<std::string> response_format,
        std::optional<float> temperature,
        std::optional<std::string> language
    ) const& noexcept(false) -> FutureResponse {
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
    ) const& noexcept(false) -> Response {
        if (!this->Validate(file)) {
            throw liboai::exception::OpenAIException(
                "File path provided is non-existent, is not a file, or is empty.",
                liboai::exception::EType::E_FILEERROR,
                "liboai::Audio::translate(...)"
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

        Response res;
        res = this->Request(
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

        return res;
    }

    auto Audio::translate_async(
        const std::filesystem::path& file,
        const std::string& model,
        std::optional<std::string> prompt,
        std::optional<std::string> response_format,
        std::optional<float> temperature
    ) const& noexcept(false) -> FutureResponse {
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
    ) const& noexcept(false) -> Response {
        JsonConstructor jcon;
        jcon.push_back("model", model);
        jcon.push_back("voice", voice);
        jcon.push_back("input", input);

        if (response_format) {
            jcon.push_back("response_format", std::move(response_format.value()));
        }
        if (speed) {
            jcon.push_back("speed", speed.value());
        }

        Response res;
        res = this->Request(
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

        return res;
    }

    auto Audio::speech_async(
        const std::string& model,
        const std::string& voice,
        const std::string& input,
        std::optional<std::string> response_format,
        std::optional<float> speed
    ) const& noexcept(false) -> FutureResponse {
        return std::async(
            std::launch::async,
            &liboai::Audio::translate,
            this,
            model,
            voice,
            input,
            response_format,
            speed
        );
    }

} // namespace liboai
