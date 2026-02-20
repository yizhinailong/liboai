/**
 * @file error.cppm
 * @brief liboai error types for std::expected migration.
 *
 * This module provides ErrorCode enum, OpenAIError struct, and
 * Expected<T> / FutureExpected<T> type aliases for migrating from
 * exception-based error handling to std::expected-based handling.
 */

module;

#include <chrono>
#include <cstdint>
#include <expected>
#include <future>
#include <optional>
#include <string>

export module liboai:core.error;

export namespace liboai {

    /**
     * @brief Error codes mapping to existing exception types.
     *
     * Values correspond to exception::EType enum values.
     */
    enum class ErrorCode : std::uint8_t {
        FailureToParse = 0,  // E_FAILURETOPARSE
        BadRequest = 1,      // E_BADREQUEST
        APIError = 2,        // E_APIERROR
        RateLimited = 3,     // E_RATELIMIT
        ConnectionError = 4, // E_CONNECTIONERROR
        FileError = 5,       // E_FILEERROR
        CURLError = 6        // E_CURLERROR
    };

    /**
     * @brief Error information for std::expected error channel.
     *
     * Contains error code, message, and optional HTTP status / retry-after
     * information for API errors.
     */
    struct OpenAIError {
        ErrorCode code;
        std::string message;
        std::optional<int> http_status;
        std::optional<std::chrono::seconds> retry_after;

        static OpenAIError parse_error(std::string msg) {
            return { .code = ErrorCode::FailureToParse,
                     .message = std::move(msg),
                     .http_status = std::nullopt,
                     .retry_after = std::nullopt };
        }

        static OpenAIError bad_request(std::string msg, int status = 0) {
            return { .code = ErrorCode::BadRequest,
                     .message = std::move(msg),
                     .http_status = status > 0 ? std::optional(status) : std::nullopt,
                     .retry_after = std::nullopt };
        }

        static OpenAIError api_error(std::string msg, int status) {
            return { .code = ErrorCode::APIError,
                     .message = std::move(msg),
                     .http_status = status,
                     .retry_after = std::nullopt };
        }

        static OpenAIError rate_limited(std::string msg, int status, std::chrono::seconds retry) {
            return { .code = ErrorCode::RateLimited,
                     .message = std::move(msg),
                     .http_status = status,
                     .retry_after = retry };
        }

        static OpenAIError connection_error(std::string msg) {
            return { .code = ErrorCode::ConnectionError,
                     .message = std::move(msg),
                     .http_status = std::nullopt,
                     .retry_after = std::nullopt };
        }

        static OpenAIError file_error(std::string msg) {
            return { .code = ErrorCode::FileError,
                     .message = std::move(msg),
                     .http_status = std::nullopt,
                     .retry_after = std::nullopt };
        }

        static OpenAIError curl_error(std::string msg) {
            return { .code = ErrorCode::CURLError,
                     .message = std::move(msg),
                     .http_status = std::nullopt,
                     .retry_after = std::nullopt };
        }
    };

    /**
     * @brief Type alias for std::expected with OpenAIError.
     *
     * Used as the return type for non-throwing API methods.
     */
    template <typename T>
    using Result = std::expected<T, OpenAIError>;

    /**
     * @brief Type alias for async std::expected returns.
     *
     * Used as the return type for async non-throwing API methods.
     */
    template <typename T>
    using FutureExpected = std::future<Result<T>>;

} // namespace liboai
