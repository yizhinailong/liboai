#pragma once

/**
 * @file response.h
 * @brief liboai response container implementation.
 *
 * This header file provides declarations for the liboai Response
 * implementation. Each component class will include this header
 * and use the Response class to return data to the user.
 *
 * - For instance, making a call to liboai::Image::Create(...) will
 *   return a liboai::Response object. The user can then check the
 *   object and retrieve the data found in the response as needed.
 * - This class will construct itself from the output of
 *   liboai::Network::Request(...) (cpr::Response) and parse it
 *   into a usable format for the user to access via this class.
 */

#if defined(__linux__) || defined(__APPLE__)
    #define LIBOAI_EXPORT
#else
    #define LIBOAI_EXPORT __declspec(dllexport)
#endif

#include <future>
#include <iostream>
#include <optional>

#include <nlohmann/json.hpp>

#include "liboai/core/error.hpp"

namespace cpr {
    class Response;
}

namespace liboai {
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
        LIBOAI_EXPORT friend auto operator<<(std::ostream& os, const Response& r) -> std::ostream&;

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
        LIBOAI_EXPORT auto CheckResponse() const -> Expected<void>;
    };

    [[nodiscard]]
    auto to_liboai_response(cpr::Response&& cpr_res) -> Expected<Response>;

    using FutureResponse = std::future<liboai::Response>;
} // namespace liboai
