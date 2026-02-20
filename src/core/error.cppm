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
#include <unordered_map>

export module liboai:core.error;

export namespace liboai {

    /**
     * @brief Error codes mapping to existing exception types.
     *
     * Values correspond to exception::EType enum values.
     */
    enum class ErrorCode : std::uint16_t {
        Success = 0,
        FailureToParse = 1001,
        BadRequest = 1002,
        APIError = 1003,
        RateLimited = 1004,
        ConnectionError = 1005,
        FileError = 1006,
        CURLError = 1007
    };

    inline auto GetHttpStatus(ErrorCode code) -> int {
        static const std::unordered_map<ErrorCode, int> status_map = {
            {         ErrorCode::Success, 200 },
            {  ErrorCode::FailureToParse,   0 },
            {      ErrorCode::BadRequest, 400 },
            {        ErrorCode::APIError, 500 },
            {     ErrorCode::RateLimited, 429 },
            { ErrorCode::ConnectionError,   0 },
            {       ErrorCode::FileError,   0 },
            {       ErrorCode::CURLError,   0 },
        };
        auto it = status_map.find(code);
        return it != status_map.end() ? it->second : 0;
    }

    inline auto GetErrorMessage(ErrorCode code) -> std::string {
        static const std::unordered_map<ErrorCode, std::string> message_map = {
            {         ErrorCode::Success,                  "Success" },
            {  ErrorCode::FailureToParse, "Failed to parse response" },
            {      ErrorCode::BadRequest,              "Bad request" },
            {        ErrorCode::APIError,                "API error" },
            {     ErrorCode::RateLimited,             "Rate limited" },
            { ErrorCode::ConnectionError,         "Connection error" },
            {       ErrorCode::FileError,               "File error" },
            {       ErrorCode::CURLError,               "CURL error" },
        };
        auto it = message_map.find(code);
        return it != message_map.end() ? it->second : "Unknown error";
    }

    inline auto ToInt(ErrorCode code) -> std::uint16_t {
        return static_cast<std::uint16_t>(code);
    }

    inline auto IsSuccess(ErrorCode code) -> bool {
        return code == ErrorCode::Success;
    }

    inline auto IsClientError(ErrorCode code) -> bool {
        int status = GetHttpStatus(code);
        return status >= 400 && status < 500;
    }

    inline auto IsServerError(ErrorCode code) -> bool {
        int status = GetHttpStatus(code);
        return status >= 500;
    }

    /**
     * @brief Error information for std::expected error channel.
     *
     * Contains error code, message, and optional HTTP status / retry-after
     * information for API errors.
     */
    struct OpenAIError {
        ErrorCode code = ErrorCode::Success;
        std::string message;
        int http_status = 0;
        std::optional<std::chrono::seconds> retry_after;

        OpenAIError() = default;

        explicit OpenAIError(ErrorCode c)
            : code(c),
              message(GetErrorMessage(c)),
              http_status(GetHttpStatus(c)),
              retry_after(std::nullopt) {}

        OpenAIError(
            ErrorCode c,
            std::string msg,
            int status = 0,
            std::optional<std::chrono::seconds> retry = std::nullopt
        ) : code(c),
            message(std::move(msg)),
            http_status(status),
            retry_after(retry) {}

        [[nodiscard]]
        auto HttpStatus() const noexcept -> int {
            return http_status;
        }

        [[nodiscard]]
        auto CodeInt() const noexcept -> std::uint16_t {
            return ToInt(code);
        }

        static OpenAIError parse_error(std::string msg) {
            return { ErrorCode::FailureToParse, std::move(msg) };
        }

        static OpenAIError bad_request(std::string msg, int status = 0) {
            return { ErrorCode::BadRequest, std::move(msg), status };
        }

        static OpenAIError api_error(std::string msg, int status) {
            return { ErrorCode::APIError, std::move(msg), status };
        }

        static OpenAIError rate_limited(std::string msg, int status, std::chrono::seconds retry) {
            return { ErrorCode::RateLimited, std::move(msg), status, retry };
        }

        static OpenAIError connection_error(std::string msg) {
            return { ErrorCode::ConnectionError, std::move(msg) };
        }

        static OpenAIError file_error(std::string msg) {
            return { ErrorCode::FileError, std::move(msg) };
        }

        static OpenAIError curl_error(std::string msg) {
            return { ErrorCode::CURLError, std::move(msg) };
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

    template <typename T, typename U>
    [[nodiscard]]
    auto PropagateError(const Result<U>& result) -> Result<T> {
        return std::unexpected(result.error());
    }

} // namespace liboai
