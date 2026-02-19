/**
 * @file response.cppm
 * @brief liboai response container implementation.
 *
 * This module provides declarations for the liboai Response
 * implementation. Each component class will use the Response
 * class to return data to the user.
 *
 * - For instance, making a call to liboai::Image::Create(...) will
 *   return a liboai::Response object. The user can then check the
 *   object and retrieve the data found in the response as needed.
 * - This class will construct itself from the output of
 *   liboai::Network::Request(...) (cpr::Response) and parse it
 *   into a usable format for the user to access via this class.
 */

module;

// Standard library headers
#include <expected>
#include <future>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>

// Third-party library headers
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

export module liboai:core.response;

import :core.error;

// Import std::unexpected for use in implementation
using std::unexpected;

export namespace liboai {

    // SFINAE trait templates
    template <typename T, typename = void> struct has_value_type : std::false_type {};

    template <typename T>
    struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

    template <typename T> inline constexpr const bool has_value_type_v = has_value_type<T>::value;

    class JsonConstructor final {
    public:
        JsonConstructor() {}

        JsonConstructor(const JsonConstructor& other) noexcept : m_json(other.m_json) {}

        JsonConstructor(JsonConstructor&& old) noexcept : m_json(std::move(old.m_json)) {}

        template <class _Ty> void push_back(std::string_view key, const _Ty& value) {
            if constexpr (std::is_same_v<
                              _Ty,
                              std::optional<std::function<bool(std::string, intptr_t)>>>) {
                if (value) {
                    this->m_json[key.data()] = true;
                }
            } else if constexpr (std::is_same_v<_Ty, std::function<bool(std::string, intptr_t)>>) {
                if (value) {
                    this->m_json[key.data()] = true;
                }
            } else {
                this->m_json[key.data()] = value;
            }
        }

        template <
            class _Ty,
            std::enable_if_t<
                std::conjunction_v<
                    has_value_type<_Ty>,
                    std::is_same<_Ty, std::optional<typename _Ty::value_type>>>,
                int> = 0>
        void push_back(std::string_view key, _Ty&& value) {
            if (value) {
                this->m_json[key.data()] = std::forward<typename _Ty::value_type>(value.value());
            }
        }

        [[nodiscard]]
        std::string dump() const {
            return this->m_json.dump(4);
        }

    private:
        nlohmann::json m_json;
    };

    class Response final {
    public:
        Response() = default;
        Response(const liboai::Response& other) noexcept;
        Response(liboai::Response&& old) noexcept;
        Response(
            std::string&& url,
            std::string&& content,
            std::string&& status_line,
            std::string&& reason,
            long status_code,
            double elapsed
        ) noexcept;

        /**
         * @brief Factory method to create a validated Response.
         *
         * Constructs a Response and validates it. Returns std::expected
         * with the Response on success or OpenAIError on failure.
         */
        [[nodiscard]]
        static auto create(
            std::string&& url,
            std::string&& content,
            std::string&& status_line,
            std::string&& reason,
            long status_code,
            double elapsed
        ) -> Expected<Response>;

        Response& operator=(const liboai::Response& other) noexcept;
        Response& operator=(liboai::Response&& old) noexcept;

        /**
         * @brief Transparent operator[] wrapper to nlohmann::json.
         *
         * Allows access to the Response object as if it were a json object.
         */
        template <class _Ty>
        [[nodiscard]]
        auto operator[](const _Ty& key) const noexcept -> nlohmann::json::const_reference {
            return this->raw_json[key];
        }

        /**
         * @brief std::ostream operator<< overload.
         *
         * Allows for pretty printing of the Response object.
         */
        friend auto operator<<(std::ostream& os, const Response& r) -> std::ostream&;

    public:
        long status_code = 0;
        double elapsed = 0.0;
        std::string status_line{}, content{}, url{}, reason{};
        nlohmann::json raw_json{};

    private:
        /**
         * @brief Validate response for errors.
         *
         * Used internally to check the response for errors.
         * Returns std::expected<void, OpenAIError>.
         */
        auto CheckResponse() const -> Expected<void>;
    };

    [[nodiscard]]
    auto to_liboai_response(cpr::Response&& cpr_res) -> Expected<Response>;

    using FutureResponse = std::future<liboai::Response>;

    // Implementation
    inline Response::Response(const Response& other) noexcept
        : status_code(other.status_code),
          elapsed(other.elapsed),
          status_line(other.status_line),
          content(other.content),
          url(other.url),
          reason(other.reason),
          raw_json(other.raw_json) {}

    inline Response::Response(Response&& other) noexcept
        : status_code(other.status_code),
          elapsed(other.elapsed),
          status_line(std::move(other.status_line)),
          content(std::move(other.content)),
          url(std::move(other.url)),
          reason(std::move(other.reason)),
          raw_json(std::move(other.raw_json)) {}

    inline Response::Response(
        std::string&& url,
        std::string&& content,
        std::string&& status_line,
        std::string&& reason,
        long status_code,
        double elapsed
    ) noexcept
        : status_code(status_code),
          elapsed(elapsed),
          status_line(std::move(status_line)),
          content(std::move(content)),
          url(url),
          reason(std::move(reason)) {
        try {
            if (!this->content.empty() && this->content[0] == '{') {
                this->raw_json = nlohmann::json::parse(this->content);
            }
        } catch (...) {
            // Parse failure - leave raw_json empty, don't throw
        }
    }

    inline auto Response::create(
        std::string&& url,
        std::string&& content,
        std::string&& status_line,
        std::string&& reason,
        long status_code,
        double elapsed
    ) -> Expected<Response> {
        // Check for JSON parse errors first
        if (!content.empty() && content[0] == '{') {
            try {
                nlohmann::json::parse(content);
            } catch (const nlohmann::json::parse_error& e) {
                return std::unexpected(OpenAIError::parse_error(e.what()));
            }
        }

        Response resp(
            std::move(url),
            std::move(content),
            std::move(status_line),
            std::move(reason),
            status_code,
            elapsed
        );

        auto result = resp.CheckResponse();
        if (!result) {
            return std::unexpected(result.error());
        }

        return resp;
    }

    inline Response& Response::operator=(const Response& other) noexcept {
        this->status_code = other.status_code;
        this->elapsed = other.elapsed;
        this->status_line = other.status_line;
        this->content = other.content;
        this->url = other.url;
        this->reason = other.reason;
        this->raw_json = other.raw_json;

        return *this;
    }

    inline Response& Response::operator=(Response&& other) noexcept {
        this->status_code = other.status_code;
        this->elapsed = other.elapsed;
        this->status_line = std::move(other.status_line);
        this->content = std::move(other.content);
        this->url = std::move(other.url);
        this->reason = std::move(other.reason);
        this->raw_json = std::move(other.raw_json);

        return *this;
    }

    inline auto operator<<(std::ostream& os, const Response& r) -> std::ostream& {
        !r.raw_json.empty() ? os << r.raw_json.dump(4) : os << "null";
        return os;
    }

    inline auto Response::CheckResponse() const -> Expected<void> {
        if (this->status_code == 429) {
            return std::unexpected(
                OpenAIError::rate_limited(
                    !this->reason.empty() ? this->reason : "Rate limited",
                    this->status_code,
                    std::chrono::seconds(0)
                )
            );
        }
        if (this->status_code == 0) {
            return std::unexpected(OpenAIError::connection_error("A connection error occurred"));
        }
        if (this->status_code < 200 || this->status_code >= 300) {
            if (this->raw_json.contains("error")) {
                try {
                    return std::unexpected(
                        OpenAIError::api_error(
                            this->raw_json["error"]["message"].get<std::string>(),
                            this->status_code
                        )
                    );
                } catch (const nlohmann::json::parse_error& e) {
                    return std::unexpected(OpenAIError::parse_error(e.what()));
                }
            }
            return std::unexpected(
                OpenAIError::bad_request(
                    !this->reason.empty() ? this->reason : "An unknown error occurred",
                    this->status_code
                )
            );
        }
        return {};
    }

    inline auto to_liboai_response(cpr::Response&& cpr_res) -> Expected<Response> {
        if (cpr_res.error && cpr_res.status_code == 0) {
            return std::unexpected(OpenAIError::curl_error(cpr_res.error.message));
        }

        return Response::create(
            std::string(cpr_res.url.str()),
            std::move(cpr_res.text),
            std::move(cpr_res.status_line),
            std::move(cpr_res.reason),
            cpr_res.status_code,
            cpr_res.elapsed
        );
    }

} // namespace liboai
