module;

#include <cpr/cpr.h>

/**
 * @file audio.cppm
 *
 * Audio component class for OpenAI. This class contains all the methods
 * for the Audio component of the OpenAI API. This class provides access
 * to 'Audio' endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI object after
 * setting necessary authentication information through the
 * liboai::Authorization::Authorizer() singleton object.
 */

export module liboai:components.audio;

import std;
import :core.authorization;
import :core.error;
import :core.response;
import :core.network;

export namespace liboai {
    class Audio final : private Network {
    public:
        explicit Audio(const std::string& root) : Network(root) {}

        ~Audio() = default;
        Audio(const Audio&) = delete;
        Audio(Audio&&) = delete;
        Audio& operator=(const Audio&) = delete;
        Audio& operator=(Audio&&) = delete;

        /**
         * @brief Transcribes audio into the input language.
         *
         * @param *file The audio file to transcribe.
         * @param *model The model to use for transcription.
         *   - Only 'whisper-1' is currently available.
         * @param prompt An optional text to guide the model's style
         *   or continue a previous audio segment. The
         *   prompt should match the audio language.
         * @param response_format The format of the transcript output.
         * @param temperature The sampling temperature, between 0 and 1.
         *   - Higher values like 0.8 will make the output more random.
         *   - Lower values like 0.2 will make it more focused and deterministic.
         *   - If set to 0, the model will use log probability to automatically
         *     increase the temperature until certain thresholds are hit.
         * @param language The language of the audio file.
         *
         * @return A liboai::Response object containing the data in JSON format.
         */
        [[nodiscard]]
        auto Transcribe(
            const std::filesystem::path& file,
            const std::string& model,
            std::optional<std::string> prompt = std::nullopt,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<float> temperature = std::nullopt,
            std::optional<std::string> language = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously transcribes audio into the input language.
         *
         * @param *file The audio file to transcribe.
         * @param *model The model to use for transcription.
         *   - Only 'whisper-1' is currently available.
         * @param prompt An optional text to guide the model's style
         *   or continue a previous audio segment. The
         *   prompt should match the audio language.
         * @param response_format The format of the transcript output.
         * @param temperature The sampling temperature, between 0 and 1.
         *   - Higher values like 0.8 will make the output more random.
         *   - Lower values like 0.2 will make it more focused and deterministic.
         *   - If set to 0, the model will use log probability to automatically
         *     increase the temperature until certain thresholds are hit.
         * @param language The language of the audio file.
         *
         * @return A liboai::Response future containing the data in JSON format.
         */
        [[nodiscard]]
        auto TranscribeAsync(
            const std::filesystem::path& file,
            const std::string& model,
            const std::optional<std::string>& prompt = std::nullopt,
            const std::optional<std::string>& response_format = std::nullopt,
            std::optional<float> temperature = std::nullopt,
            const std::optional<std::string>& language = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Translates audio into English.
         *
         * @param *file The audio file to translate.
         * @param *model The model to use for translation.
         *   - Only 'whisper-1' is currently available.
         * @param prompt An optional text to guide the model's style
         *   or continue a previous audio segment.
         * @param response_format The format of the transcript output.
         * @param temperature The sampling temperature, between 0 and 1.
         *   - Higher values like 0.8 will make the output more random.
         *   - Lower values like 0.2 will make it more focused and deterministic.
         *   - If set to 0, the model will use log probability to automatically
         *     increase the temperature until certain thresholds are hit.
         *
         * @return A liboai::Response object containing the data in JSON format.
         */
        [[nodiscard]]
        auto Translate(
            const std::filesystem::path& file,
            const std::string& model,
            std::optional<std::string> prompt = std::nullopt,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<float> temperature = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously translates audio into English.
         *
         * @param *file The audio file to translate.
         * @param *model The model to use for translation.
         *   - Only 'whisper-1' is currently available.
         * @param prompt An optional text to guide the model's style
         *   or continue a previous audio segment.
         * @param response_format The format of the transcript output.
         * @param temperature The sampling temperature, between 0 and 1.
         *   - Higher values like 0.8 will make the output more random.
         *   - Lower values like 0.2 will make it more focused and deterministic.
         *   - If set to 0, the model will use log probability to automatically
         *     increase the temperature until certain thresholds are hit.
         *
         * @return A liboai::Response future containing the data in JSON format.
         */
        [[nodiscard]]
        auto TranslateAsync(
            const std::filesystem::path& file,
            const std::string& model,
            const std::optional<std::string>& prompt = std::nullopt,
            const std::optional<std::string>& response_format = std::nullopt,
            std::optional<float> temperature = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Turn text into lifelike spoken audio.
         *
         * @param *model The model to use for translation.
         *   - 'tts-1'
         *   - 'tts-1-hd'
         * @param *voice The voice to use when generating the audio.
         *   - alloy
         *   - echo
         *   - fable
         *   - onyx
         *   - nova
         *   - shimmer
         * @param *input The text to generate audio for.
         *   - The maximum length is 4096 characters.
         * @param response_format The format to audio in.
         *   - mp3
         *   - opus
         *   - aac
         *   - flac
         *   - wav
         *   - pcm
         * @param speed The speed of the generated audio.
         *   - Select a value from 0.25 to 4.0. 1.0 is the default.
         *
         * @return A liboai::Response object containing the data in JSON format.
         */
        [[nodiscard]]
        auto Speech(
            const std::string& model,
            const std::string& voice,
            const std::string& input,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<float> speed = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously turn text into lifelike spoken audio.
         *
         * @param *model The model to use for translation.
         *   - 'tts-1'
         *   - 'tts-1-hd'
         * @param *voice The voice to use when generating the audio.
         *   - alloy
         *   - echo
         *   - fable
         *   - onyx
         *   - nova
         *   - shimmer
         * @param *input The text to generate audio for.
         *   - The maximum length is 4096 characters.
         * @param response_format The format to audio in.
         *   - mp3
         *   - opus
         *   - aac
         *   - flac
         *   - wav
         *   - pcm
         * @param speed The speed of the generated audio.
         *   - Select a value from 0.25 to 4.0. 1.0 is the default.
         *
         * @return A liboai::Response object containing the data in JSON format.
         */
        [[nodiscard]]
        auto SpeechAsync(
            const std::string& model,
            const std::string& voice,
            const std::string& input,
            const std::optional<std::string>& response_format = std::nullopt,
            std::optional<float> speed = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };

    // Implementation
    auto Audio::Transcribe(
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

    auto Audio::TranscribeAsync(
        const std::filesystem::path& file,
        const std::string& model,
        const std::optional<std::string>& prompt,
        const std::optional<std::string>& response_format,
        std::optional<float> temperature,
        const std::optional<std::string>& language
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::Audio::Transcribe,
            this,
            file,
            model,
            prompt,
            response_format,
            temperature,
            language
        );
    }

    auto Audio::Translate(
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

    auto Audio::TranslateAsync(
        const std::filesystem::path& file,
        const std::string& model,
        const std::optional<std::string>& prompt,
        const std::optional<std::string>& response_format,
        std::optional<float> temperature
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::Audio::Translate,
            this,
            file,
            model,
            prompt,
            response_format,
            temperature
        );
    }

    auto Audio::Speech(
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

    auto Audio::SpeechAsync(
        const std::string& model,
        const std::string& voice,
        const std::string& input,
        const std::optional<std::string>& response_format,
        std::optional<float> speed
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::Audio::Speech,
            this,
            model,
            voice,
            input,
            response_format,
            speed
        );
    }

} // namespace liboai
