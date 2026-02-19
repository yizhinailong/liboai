module;

#include <expected>
#include <future>
#include <cpr/cpr.h>
#include <string>
#include <filesystem>
#include <cstdint>

/**
 * @file images.cppm
 *
 * Images component class for OpenAI. This class contains all the methods
 * for the Images component of the OpenAI API. This class provides access
 * to 'Images' endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI object after
 * setting necessary authentication information through the
 * liboai::Authorization::Authorizer() singleton object.
 */

export module liboai:components.images;

import :core.authorization;
import :core.error;
import :core.response;
import :core.network;

export namespace liboai {
    class Images final : private Network {
    public:
        explicit Images(const std::string& root) : Network(root) {}

        Images(const Images&) = delete;
        Images& operator=(const Images&) = delete;
        Images(Images&&) = delete;
        Images& operator=(Images&&) = delete;
        ~Images() = default;

        /**
         * @brief Images component method to create an image from provided text.
         *
         * @param prompt           The text to create an image from.
         * @param n                The number of images to create.
         * @param size             The size of the image to create.
         * @param response_format  The format of the response.
         * @param user             A unique identifier representing an end-user.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        auto Create(
            const std::string& prompt,
            std::optional<uint8_t> n = std::nullopt,
            std::optional<std::string> size = std::nullopt,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Images component method to asynchronously create an image from provided text.
         *
         * @param prompt           The text to create an image from.
         * @param n                The number of images to create.
         * @param size             The size of the image to create.
         * @param response_format  The format of the response.
         * @param user             A unique identifier representing an end-user.
         *
         * @return A liboai::Response future containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        auto CreateAsync(
            const std::string& prompt,
            std::optional<uint8_t> n = std::nullopt,
            std::optional<std::string> size = std::nullopt,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Images component method to produce an edited image from a provided
         *        base image and mask image according to given text.
         *
         * @param image            The image to edit (path).
         * @param prompt           The text description of the desired image.
         * @param mask             The mask to edit the image with (path).
         * @param n                The number of images to create.
         * @param size             The size of the image to create.
         * @param response_format  The format of the response.
         * @param user             A unique identifier representing an end-user.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        auto CreateEdit(
            const std::filesystem::path& image,
            const std::string& prompt,
            std::optional<std::filesystem::path> mask = std::nullopt,
            std::optional<uint8_t> n = std::nullopt,
            std::optional<std::string> size = std::nullopt,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Images component method to asynchronously produce an edited image
         *        from a provided base image and mask image according to given text.
         *
         * @param image            The image to edit (path).
         * @param prompt           The text description of the desired image.
         * @param mask             The mask to edit the image with (path).
         * @param n                The number of images to create.
         * @param size             The size of the image to create.
         * @param response_format  The format of the response.
         * @param user             A unique identifier representing an end-user.
         *
         * @return A liboai::Response future containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        auto CreateEditAsync(
            const std::filesystem::path& image,
            const std::string& prompt,
            std::optional<std::filesystem::path> mask = std::nullopt,
            std::optional<uint8_t> n = std::nullopt,
            std::optional<std::string> size = std::nullopt,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Images component method to produce a variation of a supplied image.
         *
         * @param image            The image to produce a variation of (path).
         * @param n                The number of images to create.
         * @param size             The size of the image to create.
         * @param response_format  The format of the response.
         * @param user             A unique identifier representing an end-user.
         *
         * @return A liboai::Response object containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        auto CreateVariation(
            const std::filesystem::path& image,
            std::optional<uint8_t> n = std::nullopt,
            std::optional<std::string> size = std::nullopt,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Images component method to asynchronously produce a variation
         *        of a supplied image.
         *
         * @param image            The image to produce a variation of (path).
         * @param n                The number of images to create.
         * @param size             The size of the image to create.
         * @param response_format  The format of the response.
         * @param user             A unique identifier representing an end-user.
         *
         * @return A liboai::Response future containing the image(s) data in JSON format.
         */
        [[nodiscard]]
        auto CreateVariationAsync(
            const std::filesystem::path& image,
            std::optional<uint8_t> n = std::nullopt,
            std::optional<std::string> size = std::nullopt,
            std::optional<std::string> response_format = std::nullopt,
            std::optional<std::string> user = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };

    // Implementation
    auto Images::Create(
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

    auto Images::CreateAsync(
        const std::string& prompt,
        std::optional<uint8_t> n,
        std::optional<std::string> size,
        std::optional<std::string> response_format,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Images::Create,
            this,
            prompt,
            n,
            size,
            response_format,
            user
        );
    }

    auto Images::CreateEdit(
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

    auto Images::CreateEditAsync(
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
            &Images::CreateEdit,
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

    auto Images::CreateVariation(
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

    auto Images::CreateVariationAsync(
        const std::filesystem::path& image,
        std::optional<uint8_t> n,
        std::optional<std::string> size,
        std::optional<std::string> response_format,
        std::optional<std::string> user
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &Images::CreateVariation,
            this,
            image,
            n,
            size,
            response_format,
            user
        );
    }

} // namespace liboai
