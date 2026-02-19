/**
 * @file images.cpp
 *
 * Implementation of Images component for DALL-E image generation API.
 */
#include "liboai/components/images.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

    auto Images::create(
        const std::string& prompt,
        std::optional<uint8_t> n,
        std::optional<std::string> size,
        std::optional<std::string> response_format,
        std::optional<std::string> user
    ) const& noexcept -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("prompt", prompt);
        jcon.push_back("n", std::move(n));
        jcon.push_back("size", std::move(size));
        jcon.push_back("response_format", std::move(response_format));
        jcon.push_back("user", std::move(user));

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/images/generations",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Images::create_async(
        const std::string& prompt,
        std::optional<uint8_t> n,
        std::optional<std::string> size,
        std::optional<std::string> response_format,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Images::create,
            this,
            prompt,
            n,
            size,
            response_format,
            user
        );
    }

    auto Images::create_edit(
        const std::filesystem::path& image,
        const std::string& prompt,
        std::optional<std::filesystem::path> mask,
        std::optional<uint8_t> n,
        std::optional<std::string> size,
        std::optional<std::string> response_format,
        std::optional<std::string> user
    ) const& noexcept -> Expected<Response> {
        if (!this->Validate(image)) {
            return std::unexpected(
                OpenAIError::file_error(
                    "File path provided is non-existent, is not a file, or is empty."
                )
            );
        }

        cpr::Multipart form = {
            { "prompt",                              prompt },
            {  "image", cpr::File{ image.generic_string() } }
        };

        if (mask) {
            if (!this->Validate(mask.value())) {
                return std::unexpected(
                    OpenAIError::file_error(
                        "File path provided is non-existent, is not a file, or is empty."
                    )
                );
            }
            form.parts.emplace_back("mask", cpr::File{ mask.value().generic_string() });
        }
        if (n) {
            form.parts.emplace_back("n", n.value());
        }
        if (size) {
            form.parts.emplace_back("size", size.value());
        }
        if (response_format) {
            form.parts.emplace_back("response_format", response_format.value());
        }
        if (user) {
            form.parts.emplace_back("user", user.value());
        }

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/images/edits",
            "multipart/form-data",
            this->m_auth.GetAuthorizationHeaders(),
            std::move(form),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Images::create_edit_async(
        const std::filesystem::path& image,
        const std::string& prompt,
        std::optional<std::filesystem::path> mask,
        std::optional<uint8_t> n,
        std::optional<std::string> size,
        std::optional<std::string> response_format,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Images::create_edit,
            this,
            image,
            prompt,
            mask,
            n,
            size,
            response_format,
            user
        );
    }

    auto Images::create_variation(
        const std::filesystem::path& image,
        std::optional<uint8_t> n,
        std::optional<std::string> size,
        std::optional<std::string> response_format,
        std::optional<std::string> user
    ) const& noexcept -> Expected<Response> {
        if (!this->Validate(image)) {
            return std::unexpected(
                OpenAIError::file_error(
                    "File path provided is non-existent, is not a file, or is empty."
                )
            );
        }

        cpr::Multipart form = {
            { "image", cpr::File{ image.generic_string() } }
        };

        if (n) {
            form.parts.emplace_back("n", n.value());
        }
        if (size) {
            form.parts.emplace_back("size", size.value());
        }
        if (response_format) {
            form.parts.emplace_back("response_format", response_format.value());
        }
        if (user) {
            form.parts.emplace_back("user", user.value());
        }

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/images/variations",
            "multipart/form-data",
            this->m_auth.GetAuthorizationHeaders(),
            std::move(form),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto Images::create_variation_async(
        const std::filesystem::path& image,
        std::optional<uint8_t> n,
        std::optional<std::string> size,
        std::optional<std::string> response_format,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Images::create_variation,
            this,
            image,
            n,
            size,
            response_format,
            user
        );
    }

} // namespace liboai
