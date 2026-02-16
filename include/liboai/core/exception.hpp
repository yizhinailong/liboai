#pragma once

/*
	exception.h : liboai exception header.
		This header file provides declarations for exception
		directives for handling exceptions thrown by liboai
		component classes.
*/

#include <iostream>
#include <exception>
#include <memory>

#if defined(LIBOAI_DEBUG)
	#define _liboai_dbg(fmt, ...) printf(fmt, __VA_ARGS__);
#endif

namespace liboai {
	namespace exception {
		enum class EType : uint8_t {
			E_FAILURETOPARSE,
			E_BADREQUEST,
			E_APIERROR,
			E_RATELIMIT,
			E_CONNECTIONERROR,
			E_FILEERROR,
			E_CURLERROR
		};

		constexpr const char* _etype_strs_[7] = {
			"E_FAILURETOPARSE:0x00",
			"E_BADREQUEST:0x01",
			"E_APIERROR:0x02",
			"E_RATELIMIT:0x03",
			"E_CONNECTIONERROR:0x04",
			"E_FILEERROR:0x05",
			"E_CURLERROR:0x06"
		};

		class OpenAIException : public std::exception {
			public:
				OpenAIException() = default;
                                OpenAIException(const OpenAIException& rhs) noexcept
					: m_error_type(rhs.m_error_type), m_data(rhs.m_data), m_locale(rhs.m_locale) { this->m_fmt_str = (this->m_locale + ": " + this->m_data + " (" + this->GetETypeString(this->m_error_type) + ")"); }
				OpenAIException(OpenAIException&& rhs) noexcept
					: m_error_type(rhs.m_error_type), m_data(std::move(rhs.m_data)), m_locale(std::move(rhs.m_locale)) { this->m_fmt_str = (this->m_locale + ": " + this->m_data + " (" + this->GetETypeString(this->m_error_type) + ")"); }
				OpenAIException(std::string_view data, EType error_type, std::string_view locale) noexcept
					: m_error_type(error_type), m_data(data), m_locale(locale) { this->m_fmt_str = (this->m_locale + ": " + this->m_data + " (" + this->GetETypeString(this->m_error_type) + ")"); }

			auto what() const noexcept -> const char* override {
					return this->m_fmt_str.c_str();
				}

				constexpr auto GetETypeString(EType type) const noexcept -> const char* {
					return _etype_strs_[static_cast<uint8_t>(type)];
				}

			private:
				EType m_error_type;
				std::string m_data, m_locale, m_fmt_str;
		};

		class OpenAIRateLimited : public std::exception {
			public:
				OpenAIRateLimited() = default;
				OpenAIRateLimited(const OpenAIRateLimited& rhs) noexcept
					: m_error_type(rhs.m_error_type), m_data(rhs.m_data), m_locale(rhs.m_locale) { this->m_fmt_str = (this->m_locale + ": " + this->m_data + " (" + this->GetETypeString(this->m_error_type) + ")"); }
				OpenAIRateLimited(OpenAIRateLimited&& rhs) noexcept
					: m_error_type(rhs.m_error_type), m_data(std::move(rhs.m_data)), m_locale(std::move(rhs.m_locale)) { this->m_fmt_str = (this->m_locale + ": " + this->m_data + " (" + this->GetETypeString(this->m_error_type) + ")"); }
				OpenAIRateLimited(std::string_view data, EType error_type, std::string_view locale) noexcept
					: m_error_type(error_type), m_data(data), m_locale(locale) { this->m_fmt_str = (this->m_locale + ": " + this->m_data + " (" + this->GetETypeString(this->m_error_type) + ")"); }

			auto what() const noexcept -> const char* override {
					return this->m_fmt_str.c_str();
				}

				constexpr auto GetETypeString(EType type) const noexcept -> const char* {
					return _etype_strs_[static_cast<uint8_t>(type)];
				}

			private:
				EType m_error_type;
				std::string m_data, m_locale, m_fmt_str;
		};
	}
}
