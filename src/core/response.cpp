/**
 * @file response.cpp
 *
 * Implementation of Response class for handling API responses.
 */
#include "liboai/core/response.hpp"

#include <cpr/cpr.h>

namespace liboai {

    Response::Response(const Response& other) noexcept
        : status_code(other.status_code),
          elapsed(other.elapsed),
          status_line(other.status_line),
          content(other.content),
          url(other.url),
          reason(other.reason),
          raw_json(other.raw_json) {}

    Response::Response(Response&& other) noexcept
        : status_code(other.status_code),
          elapsed(other.elapsed),
          status_line(std::move(other.status_line)),
          content(std::move(other.content)),
          url(std::move(other.url)),
          reason(std::move(other.reason)),
          raw_json(std::move(other.raw_json)) {}

    Response::Response(
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

    auto Response::create(
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

    Response& Response::operator=(const Response& other) noexcept {
        this->status_code = other.status_code;
        this->elapsed = other.elapsed;
        this->status_line = other.status_line;
        this->content = other.content;
        this->url = other.url;
        this->reason = other.reason;
        this->raw_json = other.raw_json;

        return *this;
    }

    Response& Response::operator=(Response&& other) noexcept {
        this->status_code = other.status_code;
        this->elapsed = other.elapsed;
        this->status_line = std::move(other.status_line);
        this->content = std::move(other.content);
        this->url = std::move(other.url);
        this->reason = std::move(other.reason);
        this->raw_json = std::move(other.raw_json);

        return *this;
    }

    auto operator<<(std::ostream& os, const Response& r) -> std::ostream& {
        !r.raw_json.empty() ? os << r.raw_json.dump(4) : os << "null";
        return os;
    }

    auto Response::CheckResponse() const -> Expected<void> {
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

    auto to_liboai_response(cpr::Response&& cpr_res) -> Expected<Response> {
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
