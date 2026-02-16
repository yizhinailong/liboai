#include "liboai/core/netimpl.hpp"

namespace liboai::netimpl {

    CurlHolder::CurlHolder() {
        std::lock_guard<std::mutex> lock{ this->curl_easy_get_mutex_() };

        if (!_flag) {
            curl_version_info_data* data = curl_version_info(CURLVERSION_NOW);

            // if curl doesn't have ssl enabled, throw an exception
            if (!(data->features & CURL_VERSION_SSL)) {
                throw liboai::exception::OpenAIException(
                    "Curl does not have SSL enabled.",
                    liboai::exception::EType::E_CURLERROR,
                    "liboai::netimpl::CurlHolder::CurlHolder()"
                );
            } else {
                // flag set to true to avoid future checks if SSL present
                _flag = true;

#if defined(LIBOAI_DEBUG)
                _liboai_dbg("[dbg] [@%s] SSL is enabled; check flag set.\n", __func__);
#endif
            }
        }

        this->m_curl = curl_easy_init();
        if (!this->m_curl) {
            throw liboai::exception::OpenAIException(
                curl_easy_strerror(CURLE_FAILED_INIT),
                liboai::exception::EType::E_CURLERROR,
                "liboai::netimpl::CurlHolder::CurlHolder()"
            );
        }

#if defined(LIBOAI_DEBUG)
        curl_easy_setopt(this->m_curl, CURLOPT_VERBOSE, 1L);
#endif

#if defined(LIBOAI_DISABLE_PEERVERIFY)
    #if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] LIBOAI_DISABLE_PEERVERIFY set; peer verification disabled.\n",
            __func__
        );
    #endif
        curl_easy_setopt(this->m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
#else
    #if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] LIBOAI_DISABLE_PEERVERIFY not set; peer verification enabled.\n",
            __func__
        );
    #endif
        curl_easy_setopt(this->m_curl, CURLOPT_SSL_VERIFYPEER, 1L);
#endif
    }

    CurlHolder::~CurlHolder() {
        if (this->m_curl) {
            curl_easy_cleanup(this->m_curl);
            this->m_curl = nullptr;

#if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] curl_easy_cleanup() called.\n", __func__);
#endif
        }
    }

    Session::~Session() {
        if (this->m_headers) {
            curl_slist_free_all(this->m_headers);
            this->m_headers = nullptr;

#if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] curl_slist_free_all() called.\n", __func__);
#endif
        }

#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
        if (this->m_form) {
            curl_formfree(this->m_form);
            this->m_form = nullptr;

    #if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] curl_formfree() called.\n", __func__);
    #endif
        }
#endif

#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
        if (this->m_mime) {
            curl_mime_free(this->m_mime);
            this->m_mime = nullptr;

    #if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] curl_mime_free() called.\n", __func__);
    #endif
        }
#endif
    }

    void Session::Prepare() {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[11];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        // add parameters to base url
        if (!this->m_parameter_string.empty()) {
            this->m_url += "?";
            this->m_url += this->m_parameter_string;
        }
        this->m_url_str = this->m_url;

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set URL for Session (0x%p) to %s.\n",
            __func__,
            this,
            this->url_str.c_str()
        );
#endif

        e[0] = curl_easy_setopt(this->m_curl, CURLOPT_URL, this->m_url.c_str());

        const std::string protocol_socket5_hostname = "socket5_hostname";
        if (m_proxies.has(protocol_socket5_hostname)) {
            e[1] = curl_easy_setopt(
                this->m_curl,
                CURLOPT_PROXY,
                m_proxies[protocol_socket5_hostname].c_str()
            );
            e[2] = curl_easy_setopt(this->m_curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);

            if (m_proxyAuth.has(protocol_socket5_hostname)) {
                e[3] = curl_easy_setopt(
                    this->m_curl,
                    CURLOPT_PROXYUSERNAME,
                    m_proxyAuth.GetUsername(protocol_socket5_hostname)
                );
                e[4] = curl_easy_setopt(
                    this->m_curl,
                    CURLOPT_PROXYPASSWORD,
                    m_proxyAuth.GetPassword(protocol_socket5_hostname)
                );
            }
        } else {
            // set proxy if available
            const std::string protocol = m_url.substr(0, m_url.find(':'));
            if (m_proxies.has(protocol)) {
                e[1] = curl_easy_setopt(this->m_curl, CURLOPT_PROXY, m_proxies[protocol].c_str());

#if defined(LIBOAI_DEBUG)
                _liboai_dbg(
                    "[dbg] [@%s] Set CURLOPT_PROXY for Session (0x%p) to %s.\n",
                    __func__,
                    this,
                    proxies_[protocol].c_str()
                );
#endif

                if (m_proxyAuth.has(protocol)) {
                    e[2] = curl_easy_setopt(
                        this->m_curl,
                        CURLOPT_PROXYUSERNAME,
                        m_proxyAuth.GetUsername(protocol)
                    );
                    e[3] = curl_easy_setopt(
                        this->m_curl,
                        CURLOPT_PROXYPASSWORD,
                        m_proxyAuth.GetPassword(protocol)
                    );

#if defined(LIBOAI_DEBUG)
                    _liboai_dbg(
                        "[dbg] [@%s] Set CURLOPT_PROXYUSERNAME and CURLOPT_PROXYPASSWORD for " "Ses" "sio" "n " "(0x%p) to %s and %s.\n",
                        __func__,
                        this,
                        proxyAuth_.GetUsername(protocol),
                        proxyAuth_.GetPassword(protocol)
                    );
#endif
                }
            }
        }

        // accept all encoding types
        e[5] = curl_easy_setopt(this->m_curl, CURLOPT_ACCEPT_ENCODING, "");

#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 71)
        e[6] = curl_easy_setopt(this->m_curl, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);

    #if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set CURLOPT_SSL_OPTIONS for Session (0x%p) to CURLSSLOPT_NATIVE_CA.\n",
            __func__,
            this
        );
    #endif
#endif

        // set string the response will be sent to
        if (!this->m_write.callback) {
            e[7] = curl_easy_setopt(this->m_curl, CURLOPT_WRITEFUNCTION, components::writeFunction);
            e[8] = curl_easy_setopt(this->m_curl, CURLOPT_WRITEDATA, &this->m_response_string);

#if defined(LIBOAI_DEBUG)
            _liboai_dbg(
                "[dbg] [@%s] No user supplied WriteCallback. Set CURLOPT_WRITEFUNCTION and " "CURLO" "PT_" "WRITE" "DATA " "for " "Sessi" "on " "(0x%" "p) " "to " "0x%p " "and " "0x%p." "\n",
                __func__,
                this,
                components::writeFunction,
                &this->response_string_
            );
#endif
        }

        // set string the raw headers will be sent to
        e[9] = curl_easy_setopt(this->m_curl, CURLOPT_HEADERFUNCTION, components::writeFunction);
        e[10] = curl_easy_setopt(this->m_curl, CURLOPT_HEADERDATA, &this->m_header_string);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set CURLOPT_HEADERFUNCTION and CURLOPT_HEADERDATA for Session (0x%p) to " "0x%p " "and 0x%p.\n",
            __func__,
            this,
            components::writeFunction,
            &this->header_string_
        );
#endif

        ErrorCheck(e, 11, "liboai::netimpl::Session::Prepare()");
    }

    void Session::PrepareDownloadInternal() {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[7];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        if (!this->m_parameter_string.empty()) {
            this->m_url += "?";
            this->m_url += this->m_parameter_string;
        }
        this->m_url_str = this->m_url;

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set URL for Session (0x%p) to %s.\n",
            __func__,
            this,
            this->url_str.c_str()
        );
#endif

        e[0] = curl_easy_setopt(this->m_curl, CURLOPT_URL, this->m_url.c_str());

        const std::string protocol_socket5_hostname = "socket5_hostname";
        if (m_proxies.has(protocol_socket5_hostname)) {
            e[1] = curl_easy_setopt(
                this->m_curl,
                CURLOPT_PROXY,
                m_proxies[protocol_socket5_hostname].c_str()
            );
            e[2] = curl_easy_setopt(this->m_curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);
            if (m_proxyAuth.has(protocol_socket5_hostname)) {
                e[3] = curl_easy_setopt(
                    this->m_curl,
                    CURLOPT_PROXYUSERNAME,
                    m_proxyAuth.GetUsername(protocol_socket5_hostname)
                );
                e[4] = curl_easy_setopt(
                    this->m_curl,
                    CURLOPT_PROXYPASSWORD,
                    m_proxyAuth.GetPassword(protocol_socket5_hostname)
                );
            }
        } else {
            const std::string protocol = m_url.substr(0, m_url.find(':'));
            if (m_proxies.has(protocol)) {
                e[1] = curl_easy_setopt(this->m_curl, CURLOPT_PROXY, m_proxies[protocol].c_str());

#if defined(LIBOAI_DEBUG)
                _liboai_dbg(
                    "[dbg] [@%s] Set CURLOPT_PROXY for Session (0x%p) to %s.\n",
                    __func__,
                    this,
                    proxies_[protocol].c_str()
                );
#endif

                if (m_proxyAuth.has(protocol)) {
                    e[2] = curl_easy_setopt(
                        this->m_curl,
                        CURLOPT_PROXYUSERNAME,
                        m_proxyAuth.GetUsername(protocol)
                    );
                    e[3] = curl_easy_setopt(
                        this->m_curl,
                        CURLOPT_PROXYPASSWORD,
                        m_proxyAuth.GetPassword(protocol)
                    );

#if defined(LIBOAI_DEBUG)
                    _liboai_dbg(
                        "[dbg] [@%s] Set CURLOPT_PROXYUSERNAME and CURLOPT_PROXYPASSWORD for " "Ses" "sio" "n " "(0x%p) to %s and %s.\n",
                        __func__,
                        this,
                        proxyAuth_.GetUsername(protocol),
                        proxyAuth_.GetPassword(protocol)
                    );
#endif
                }
            }
        }

        e[5] = curl_easy_setopt(this->m_curl, CURLOPT_HEADERFUNCTION, components::writeFunction);
        e[6] = curl_easy_setopt(this->m_curl, CURLOPT_HEADERDATA, &this->m_header_string);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set CURLOPT_HEADERFUNCTION and CURLOPT_HEADERDATA for Session (0x%p) to " "0x%p " "and 0x%p.\n",
            __func__,
            this,
            components::writeFunction,
            &this->header_string_
        );
#endif

        ErrorCheck(e, 7, "liboai::netimpl::Session::PrepareDownloadInternal()");
    }

    CURLcode Session::Perform() {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called curl_easy_perform() for Session (0x%p).\n", __func__, this);
#endif

        CURLcode e = curl_easy_perform(this->m_curl);
        ErrorCheck(e, "liboai::netimpl::Session::Perform()");
        return e;
    }

    liboai::Response Session::BuildResponseObject() {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[3];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Called ParseResponseHeader() for Session (0x%p).\n",
            __func__,
            this
        );
#endif

        // fill status line and reason
        this->ParseResponseHeader(this->m_header_string, &this->m_status_line, &this->m_reason);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Called curl_easy_getinfo() for Session (0x%p) to get status code.\n",
            __func__,
            this
        );
#endif

        // get status code
        e[0] = curl_easy_getinfo(this->m_curl, CURLINFO_RESPONSE_CODE, &this->m_status_code);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Called curl_easy_getinfo() for Session (0x%p) to get elapsed time.\n",
            __func__,
            this
        );
#endif

        // get elapsed time
        e[1] = curl_easy_getinfo(this->m_curl, CURLINFO_TOTAL_TIME, &this->m_elapsed);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Called curl_easy_getinfo() for Session (0x%p) to get effective url.\n",
            __func__,
            this
        );
#endif

        // get url
        char* effective_url = nullptr;
        e[2] = curl_easy_getinfo(this->m_curl, CURLINFO_EFFECTIVE_URL, &effective_url);
        this->m_url_str = (effective_url ? effective_url : "");

        ErrorCheck(e, 3, "liboai::netimpl::Session::BuildResponseObject()");

        // fill content
        this->m_content = this->m_response_string;

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Constructed response object.\n", __func__);
#endif

        return liboai::Response{
            std::move(this->m_url_str), std::move(this->m_content), std::move(this->m_status_line),
            std::move(this->m_reason),  this->m_status_code,        this->m_elapsed
        };
    }

    liboai::Response Session::Complete() {
        this->m_hasBody = false;

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called BuildResponseObject().\n", __func__);
#endif

        return this->BuildResponseObject();
    }

    liboai::Response Session::CompleteDownload() {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[2];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        e[0] = curl_easy_setopt(this->m_curl, CURLOPT_HEADERFUNCTION, nullptr);
        e[1] = curl_easy_setopt(this->m_curl, CURLOPT_HEADERDATA, 0);

        ErrorCheck(e, 2, "liboai::netimpl::Session::CompleteDownload()");

        this->m_hasBody = false;

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called BuildResponseObject().\n", __func__);
#endif

        return this->BuildResponseObject();
    }

    void Session::PrepareGet() {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[5];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        if (this->m_hasBody) {
            e[0] = curl_easy_setopt(this->m_curl, CURLOPT_NOBODY, 0L);
            e[1] = curl_easy_setopt(this->m_curl, CURLOPT_CUSTOMREQUEST, "GET");

#if defined(LIBOAI_DEBUG)
            _liboai_dbg(
                "[dbg] [@%s] Set CURLOPT_NOBODY and CURLOPT_CUSTOMREQUEST for Session (0x%p) to 0L " "and " "\"GET\".\n",
                __func__,
                this
            );
#endif
        } else {
            e[2] = curl_easy_setopt(this->m_curl, CURLOPT_NOBODY, 0L);
            e[3] = curl_easy_setopt(this->m_curl, CURLOPT_CUSTOMREQUEST, nullptr);
            e[4] = curl_easy_setopt(this->m_curl, CURLOPT_HTTPGET, 1L);

#if defined(LIBOAI_DEBUG)
            _liboai_dbg(
                "[dbg] [@%s] Set CURLOPT_NOBODY, CURLOPT_CUSTOMREQUEST and CURLOPT_HTTPGET for " "S" "e" "s" "s" "i" "o" "n" " " "(0x%p) to 0L, nullptr and 1L.\n",
                __func__,
                this
            );
#endif
        }

        ErrorCheck(e, 5, "liboai::netimpl::Session::PrepareGet()");

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Prepare().\n", __func__);
#endif

        this->Prepare();
    }

    liboai::Response Session::Get() {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called PrepareGet().\n", __func__);
#endif

        this->PrepareGet();

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Perform().\n", __func__);
#endif

        this->Perform();

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Complete().\n", __func__);
#endif

        return Complete();
    }

    void Session::PreparePost() {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[4];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        e[0] = curl_easy_setopt(this->m_curl, CURLOPT_NOBODY, 0L);
        if (this->m_hasBody) {
            e[1] = curl_easy_setopt(this->m_curl, CURLOPT_CUSTOMREQUEST, nullptr);

#if defined(LIBOAI_DEBUG)
            _liboai_dbg(
                "[dbg] [@%s] Set CURLOPT_NOBODY and CURLOPT_CUSTOMREQUEST for Session (0x%p) to 0L " "and " "nullptr.\n",
                __func__,
                this
            );
#endif
        } else {
            e[2] = curl_easy_setopt(this->m_curl, CURLOPT_POSTFIELDS, "");
            e[3] = curl_easy_setopt(this->m_curl, CURLOPT_CUSTOMREQUEST, "POST");

#if defined(LIBOAI_DEBUG)
            _liboai_dbg(
                "[dbg] [@%s] Set CURLOPT_NOBODY, CURLOPT_POSTFIELDS and CURLOPT_CUSTOMREQUEST for " "Session (0x%p) to 0L, \"\" and \"POST\".\n",
                __func__,
                this
            );
#endif
        }

        ErrorCheck(e, 4, "liboai::netimpl::Session::PreparePost()");

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Prepare().\n", __func__);
#endif

        this->Prepare();
    }

    liboai::Response Session::Post() {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called PreparePost().\n", __func__);
#endif

        this->PreparePost();

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Perform().\n", __func__);
#endif

        Perform();

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Complete().\n", __func__);
#endif

        return Complete();
    }

    void Session::PrepareDelete() {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[3];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        e[0] = curl_easy_setopt(this->m_curl, CURLOPT_HTTPGET, 0L);
        e[1] = curl_easy_setopt(this->m_curl, CURLOPT_NOBODY, 0L);
        e[2] = curl_easy_setopt(this->m_curl, CURLOPT_CUSTOMREQUEST, "DELETE");

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set CURLOPT_HTTPGET, CURLOPT_NOBODY and CURLOPT_CUSTOMREQUEST for Session " "(0x%p) to 0L, 0L and \"DELETE\".\n",
            __func__,
            this
        );
#endif

        ErrorCheck(e, 3, "liboai::netimpl::Session::PrepareDelete()");

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Prepare().\n", __func__);
#endif

        this->Prepare();
    }

    liboai::Response Session::Delete() {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called PrepareDelete().\n", __func__);
#endif

        this->PrepareDelete();

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Perform().\n", __func__);
#endif

        Perform();

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Complete().\n", __func__);
#endif

        return Complete();
    }

    void Session::PrepareDownload(std::ofstream& file) {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[5];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        e[0] = curl_easy_setopt(this->m_curl, CURLOPT_NOBODY, 0L);
        e[1] = curl_easy_setopt(this->m_curl, CURLOPT_HTTPGET, 1);
        e[2] = curl_easy_setopt(this->m_curl, CURLOPT_WRITEFUNCTION, components::writeFileFunction);
        e[3] = curl_easy_setopt(this->m_curl, CURLOPT_WRITEDATA, &file);
        e[4] = curl_easy_setopt(this->m_curl, CURLOPT_CUSTOMREQUEST, nullptr);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set CURLOPT_NOBODY, CURLOPT_HTTPGET, CURLOPT_WRITEFUNCTION, " "CURLOPT_" "WRITEDATA " "and CURLOPT_CUSTOMREQUEST for Session (0x%p) to 0L, 1L, " "liboai" "::" "netimp" "l::" "compon" "ents::" "writeF" "ileFun" "ction," " &" "file " "and " "nullpt" "r.\n",
            __func__,
            this
        );
#endif

        ErrorCheck(e, 5, "liboai::netimpl::Session::PrepareDownload()");

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called PrepareDownloadInternal().\n", __func__);
#endif

        this->PrepareDownloadInternal();
    }

    liboai::Response Session::Download(std::ofstream& file) {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called PrepareDownload().\n", __func__);
#endif

        this->PrepareDownload(file);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called Perform().\n", __func__);
#endif

        this->Perform();

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called CompleteDownload().\n", __func__);
#endif

        return CompleteDownload();
    }

    void Session::ClearContext() {
        if (m_curl) {
            curl_easy_reset(m_curl);
        }
        m_status_code = 0;
        m_elapsed = 0.0;
        m_status_line.clear();
        m_content.clear();
        m_url_str.clear();
        m_reason.clear();

        if (this->m_headers) {
            curl_slist_free_all(this->m_headers);
            this->m_headers = nullptr;

#if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] curl_slist_free_all() called.\n", __func__);
#endif
        }

#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
        if (this->m_form) {
            curl_formfree(this->m_form);
            this->m_form = nullptr;

    #if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] curl_formfree() called.\n", __func__);
    #endif
        }
#endif

#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
        if (this->m_mime) {
            curl_mime_free(this->m_mime);
            this->m_mime = nullptr;

    #if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] curl_mime_free() called.\n", __func__);
    #endif
        }
#endif

        m_hasBody = false;
        m_parameter_string.clear();
        m_url.clear();
        m_response_string.clear();
        m_header_string.clear();
        m_write = components::WriteCallback{};
    }

    void Session::ParseResponseHeader(
        const std::string& headers,
        std::string* status_line,
        std::string* reason
    ) {
        std::vector<std::string> lines;
        std::istringstream stream(headers);
        {
            std::string line;
            while (std::getline(stream, line, '\n')) {
                lines.push_back(line);
            }
        }

        for (std::string& line : lines) {
            if (line.substr(0, 5) == "HTTP/") {
                // set the status_line if it was given
                if ((status_line != nullptr) || (reason != nullptr)) {
                    line.resize(
                        std::min<size_t>(line.size(), line.find_last_not_of("\t\n\r ") + 1)
                    );
                    if (status_line != nullptr) {
                        *status_line = line;
                    }

                    // set the reason if it was given
                    if (reason != nullptr) {
                        const size_t pos1 = line.find_first_of("\t ");
                        size_t pos2 = std::string::npos;
                        if (pos1 != std::string::npos) {
                            pos2 = line.find_first_of("\t ", pos1 + 1);
                        }
                        if (pos2 != std::string::npos) {
                            line.erase(0, pos2 + 1);
                            *reason = line;
                        }
                    }
                }
            }

            if (line.length() > 0) {
                const size_t found = line.find(':');
                if (found != std::string::npos) {
                    std::string value = line.substr(found + 1);
                    value.erase(0, value.find_first_not_of("\t "));
                    value.resize(
                        std::min<size_t>(value.size(), value.find_last_not_of("\t\n\r ") + 1)
                    );
                }
            }
        }

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Parsed response header.\n", __func__);
#endif
    }

    void Session::SetOption(const components::Url& url) {
        this->SetUrl(url);
    }

    void Session::SetUrl(const components::Url& url) {
        this->m_url = url.str();

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set base URL for Session (0x%p) to \"%s\".\n",
            __func__,
            this,
            this->url_.c_str()
        );
#endif
    }

    void Session::SetOption(const components::Body& body) {
        this->SetBody(body);
    }

    void Session::SetBody(const components::Body& body) {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[2];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        this->m_hasBody = true;
        e[0] = curl_easy_setopt(
            this->m_curl,
            CURLOPT_POSTFIELDSIZE_LARGE,
            static_cast<curl_off_t>(body.str().length())
        );
        e[1] = curl_easy_setopt(this->m_curl, CURLOPT_POSTFIELDS, body.c_str());

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set CURLOPT_POSTFIELDSIZE_LARGE and CURLOPT_POSTFIELDS for Session (0x%p) " "to " "%lld and \"%s\".\n",
            __func__,
            this,
            static_cast<curl_off_t>(body.str().length()),
            body.c_str()
        );
#endif

        ErrorCheck(e, 2, "liboai::netimpl::Session::SetBody()");
    }

    void Session::SetOption(components::Body&& body) {
        this->SetBody(std::move(body));
    }

    void Session::SetBody(components::Body&& body) {
        // holds error codes - all init to OK to prevent errors
        // when checking unset values
        CURLcode e[2];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        this->m_hasBody = true;
        e[0] = curl_easy_setopt(
            this->m_curl,
            CURLOPT_POSTFIELDSIZE_LARGE,
            static_cast<curl_off_t>(body.str().length())
        );
        e[1] = curl_easy_setopt(this->m_curl, CURLOPT_COPYPOSTFIELDS, body.c_str());

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set CURLOPT_POSTFIELDSIZE_LARGE and CURLOPT_COPYPOSTFIELDS for Session " "(0x%p) " "to %lld and \"%s\".\n",
            __func__,
            this,
            static_cast<curl_off_t>(body.str().length()),
            body.c_str()
        );
#endif

        ErrorCheck(e, 2, "liboai::netimpl::Session::SetBody()");
    }

    void Session::SetOption(const components::Multipart& multipart) {
        this->SetMultipart(multipart);
    }

    void Session::SetMultipart(const components::Multipart& multipart) {
#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
        CURLFORMcode fe[2];
        memset(fe, CURLFORMcode::CURL_FORMADD_OK, sizeof(fe));
        CURLcode e;

        curl_httppost* lastptr = nullptr;

        for (const auto& part : multipart.parts) {
            std::vector<curl_forms> formdata;
            if (!part.content_type.empty()) {
                formdata.push_back({ CURLFORM_CONTENTTYPE, part.content_type.c_str() });
            }
            if (part.is_file) {
                CURLFORMcode f;
                for (const auto& file : part.files) {
                    formdata.push_back({ CURLFORM_COPYNAME, part.name.c_str() });
                    formdata.push_back({ CURLFORM_FILE, file.filepath.c_str() });
                    if (file.hasOverridedFilename()) {
                        formdata.push_back({ CURLFORM_FILENAME, file.overrided_filename.c_str() });
                    }
                    formdata.push_back({ CURLFORM_END, nullptr });
                    f = curl_formadd(
                        &this->m_form,
                        &lastptr,
                        CURLFORM_ARRAY,
                        formdata.data(),
                        CURLFORM_END
                    );

                    // check each file
                    ErrorCheck(f, "liboai::netimpl::Session::SetMultipart() @ is_file[formadd]");

                    formdata.clear();
                }
            } else if (part.is_buffer) {
                fe[0] = curl_formadd(
                    &this->m_form,
                    &lastptr,
                    CURLFORM_COPYNAME,
                    part.name.c_str(),
                    CURLFORM_BUFFER,
                    part.value.c_str(),
                    CURLFORM_BUFFERPTR,
                    part.data,
                    CURLFORM_BUFFERLENGTH,
                    part.datalen,
                    CURLFORM_END
                );
            } else {
                formdata.push_back({ CURLFORM_COPYNAME, part.name.c_str() });
                formdata.push_back({ CURLFORM_COPYCONTENTS, part.value.c_str() });
                formdata.push_back({ CURLFORM_END, nullptr });
                fe[1] = curl_formadd(
                    &this->m_form,
                    &lastptr,
                    CURLFORM_ARRAY,
                    formdata.data(),
                    CURLFORM_END
                );
            }
        }
        e = curl_easy_setopt(this->m_curl, CURLOPT_HTTPPOST, this->m_form);

    #if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set multipart for Session (0x%p) using curl_formadd() and " "CURLOPT_" "HT" "TP" "PO" "ST" "." "\n",
            __func__,
            this
        );
    #endif

        ErrorCheck(fe, 2, "liboai::netimpl::Session::SetMultipart()");
        ErrorCheck(e, "liboai::netimpl::Session::SetMultipart()");

        this->hasBody = true;
#endif

#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
        CURLcode e[6];
        memset(e, CURLcode::CURLE_OK, sizeof(e));

        this->m_mime = curl_mime_init(this->m_curl);
        if (!this->m_mime) {
            throw liboai::exception::OpenAIException(
                "curl_mime_init() failed",
                liboai::exception::EType::E_CURLERROR,
                "liboai::netimpl::Session::SetMultipart()"
            );
        }

        for (const auto& part : multipart.parts) {
            std::vector<curl_mimepart*> mimedata;
            if (!part.content_type.empty()) {
                mimedata.push_back(curl_mime_addpart(this->m_mime));
                e[0] = curl_mime_type(mimedata.back(), part.content_type.c_str());
            }
            if (part.is_file) {
                CURLcode fe[3];
                memset(fe, CURLcode::CURLE_OK, sizeof(fe));
                for (const auto& file : part.files) {
                    mimedata.push_back(curl_mime_addpart(this->m_mime));
                    fe[0] = curl_mime_name(mimedata.back(), part.name.c_str());
                    fe[1] = curl_mime_filedata(mimedata.back(), file.filepath.c_str());
                    if (file.hasOverridedFilename()) {
                        fe[2] =
                            curl_mime_filename(mimedata.back(), file.overrided_filename.c_str());
                    }

                    // check each file
                    ErrorCheck(fe, 3, "liboai::netimpl::Session::SetMultipart() @ is_file[mime]");
                }
            } else if (part.is_buffer) {
                mimedata.push_back(curl_mime_addpart(this->m_mime));
                e[1] = curl_mime_name(mimedata.back(), part.name.c_str());
                e[2] = curl_mime_filename(mimedata.back(), part.value.c_str());
                e[3] = curl_mime_data(
                    mimedata.back(),
                    reinterpret_cast<const char*>(part.data),
                    part.datalen
                );
            } else {
                mimedata.push_back(curl_mime_addpart(this->m_mime));
                e[3] = curl_mime_name(mimedata.back(), part.name.c_str());
                e[4] = curl_mime_data(mimedata.back(), part.value.c_str(), CURL_ZERO_TERMINATED);
            }
        }
        e[5] = curl_easy_setopt(this->m_curl, CURLOPT_MIMEPOST, this->m_mime);

    #if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set multipart for Session (0x%p) using curl_mime_addpart() and " "CURLOPT_" "MIMEPOST" ".\n",
            __func__,
            this
        );
    #endif

        ErrorCheck(e, 6, "liboai::netimpl::Session::SetMultipart()");

        this->m_hasBody = true;
#endif
    }

    void Session::SetOption(components::Multipart&& multipart) {
        this->SetMultipart(std::move(multipart));
    }

    void Session::SetMultipart(components::Multipart&& multipart) {
#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
        CURLFORMcode fe[2];
        memset(fe, CURLFORMcode::CURL_FORMADD_OK, sizeof(fe));
        CURLcode e;

        curl_httppost* lastptr = nullptr;

        for (const auto& part : multipart.parts) {
            std::vector<curl_forms> formdata;
            if (!part.content_type.empty()) {
                formdata.push_back({ CURLFORM_CONTENTTYPE, part.content_type.c_str() });
            }
            if (part.is_file) {
                CURLFORMcode f;
                for (const auto& file : part.files) {
                    formdata.push_back({ CURLFORM_COPYNAME, part.name.c_str() });
                    formdata.push_back({ CURLFORM_FILE, file.filepath.c_str() });
                    if (file.hasOverridedFilename()) {
                        formdata.push_back({ CURLFORM_FILENAME, file.overrided_filename.c_str() });
                    }
                    formdata.push_back({ CURLFORM_END, nullptr });
                    f = curl_formadd(
                        &this->m_form,
                        &lastptr,
                        CURLFORM_ARRAY,
                        formdata.data(),
                        CURLFORM_END
                    );

                    // check each file
                    ErrorCheck(f, "liboai::netimpl::Session::SetMultipart() @ is_file[formadd]");

                    formdata.clear();
                }
            } else if (part.is_buffer) {
                fe[0] = curl_formadd(
                    &this->m_form,
                    &lastptr,
                    CURLFORM_COPYNAME,
                    part.name.c_str(),
                    CURLFORM_BUFFER,
                    part.value.c_str(),
                    CURLFORM_BUFFERPTR,
                    part.data,
                    CURLFORM_BUFFERLENGTH,
                    part.datalen,
                    CURLFORM_END
                );
            } else {
                formdata.push_back({ CURLFORM_COPYNAME, part.name.c_str() });
                formdata.push_back({ CURLFORM_COPYCONTENTS, part.value.c_str() });
                formdata.push_back({ CURLFORM_END, nullptr });
                fe[1] = curl_formadd(
                    &this->m_form,
                    &lastptr,
                    CURLFORM_ARRAY,
                    formdata.data(),
                    CURLFORM_END
                );
            }
        }
        e = curl_easy_setopt(this->m_curl, CURLOPT_HTTPPOST, this->m_form);

    #if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set multipart for Session (0x%p) using curl_formadd() and " "CURLOPT_" "HT" "TP" "PO" "ST" "." "\n",
            __func__,
            this
        );
    #endif

        ErrorCheck(fe, 2, "liboai::netimpl::Session::SetMultipart()");
        ErrorCheck(e, "liboai::netimpl::Session::SetMultipart()");

        this->hasBody = true;
#endif

#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
        CURLcode e[6];
        memset(e, CURLcode::CURLE_OK, sizeof(e));
        curl_mimepart* _part = nullptr;

        this->m_mime = curl_mime_init(this->m_curl);
        if (!this->m_mime) {
            throw liboai::exception::OpenAIException(
                "curl_mime_init() failed",
                liboai::exception::EType::E_CURLERROR,
                "liboai::netimpl::Session::SetMultipart()"
            );
        }

        for (const auto& part : multipart.parts) {
            std::vector<curl_mimepart*> mimedata;
            if (!part.content_type.empty()) {
                mimedata.push_back(curl_mime_addpart(this->m_mime));
                e[0] = curl_mime_type(mimedata.back(), part.content_type.c_str());
            }
            if (part.is_file) {
                CURLcode fe[3];
                memset(fe, CURLcode::CURLE_OK, sizeof(fe));
                for (const auto& file : part.files) {
                    mimedata.push_back(curl_mime_addpart(this->m_mime));
                    fe[0] = curl_mime_name(mimedata.back(), part.name.c_str());
                    fe[1] = curl_mime_filedata(mimedata.back(), file.filepath.c_str());
                    if (file.hasOverridedFilename()) {
                        fe[2] =
                            curl_mime_filename(mimedata.back(), file.overrided_filename.c_str());
                    }

                    // check each file
                    ErrorCheck(fe, 3, "liboai::netimpl::Session::SetMultipart() @ is_file[mime]");
                }
            } else if (part.is_buffer) {
                mimedata.push_back(curl_mime_addpart(this->m_mime));
                e[1] = curl_mime_name(mimedata.back(), part.name.c_str());
                e[2] = curl_mime_filename(mimedata.back(), part.value.c_str());
                e[3] = curl_mime_data(
                    mimedata.back(),
                    reinterpret_cast<const char*>(part.data),
                    part.datalen
                );
            } else {
                mimedata.push_back(curl_mime_addpart(this->m_mime));
                e[3] = curl_mime_name(mimedata.back(), part.name.c_str());
                e[4] = curl_mime_data(mimedata.back(), part.value.c_str(), CURL_ZERO_TERMINATED);
            }
        }
        e[5] = curl_easy_setopt(this->m_curl, CURLOPT_MIMEPOST, this->m_mime);

    #if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set multipart for Session (0x%p) using curl_mime_addpart() and " "CURLOPT_" "MIMEPOST" ".\n",
            __func__,
            this
        );
    #endif

        ErrorCheck(e, 6, "liboai::netimpl::Session::SetMultipart()");

        this->m_hasBody = true;
#endif
    }

    std::string CurlHolder::urlEncode(const std::string& s) {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] URL-encode string \"%s\".\n", __func__, s.c_str());
#endif

        char* output = curl_easy_escape(this->m_curl, s.c_str(), static_cast<int>(s.length()));
        if (output) {
            std::string result = output;
            curl_free(output);
            return result;
        }
        return "";
    }

    std::string CurlHolder::urlDecode(const std::string& s) {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] URL-decode string \"%s\".\n", __func__, s.c_str());
#endif

        char* output =
            curl_easy_unescape(this->m_curl, s.c_str(), static_cast<int>(s.length()), nullptr);
        if (output) {
            std::string result = output;
            curl_free(output);
            return result;
        }
        return "";
    }

    std::string components::urlEncodeHelper(const std::string& s) {
        CurlHolder c;
        return c.urlEncode(s);
    }

    std::string components::urlDecodeHelper(const std::string& s) {
        CurlHolder c;
        return c.urlDecode(s);
    }

    size_t components::writeUserFunction(
        char* ptr,
        size_t size,
        size_t nmemb,
        const WriteCallback* write
    ) {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called with %zu bytes.\n", __func__, size * nmemb);
#endif

        size *= nmemb;
        return (*write)({ ptr, size }) ? size : 0;
    }

    size_t components::writeFunction(char* ptr, size_t size, size_t nmemb, std::string* data) {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called with %zu bytes.\n", __func__, size * nmemb);
#endif

        size *= nmemb;
        data->append(ptr, size);
        return size;
    }

    size_t
    components::writeFileFunction(char* ptr, size_t size, size_t nmemb, std::ofstream* file) {
#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Called with %zu bytes.\n", __func__, size * nmemb);
#endif

        size *= nmemb;
        file->write(ptr, static_cast<std::streamsize>(size));
        return size;
    }

    long components::Timeout::Milliseconds() const {
        static_assert(
            std::is_same<std::chrono::milliseconds, decltype(this->ms)>::value,
            "Following casting expects milliseconds."
        );

        if (ms.count() >
            static_cast<std::chrono::milliseconds::rep>((std::numeric_limits<long>::max)())) {
            throw std::overflow_error(
                "cpr::Timeout: timeout value overflow: " + std::to_string(ms.count()) + " ms."
            );
        }

        if (ms.count() <
            static_cast<std::chrono::milliseconds::rep>((std::numeric_limits<long>::min)())) {
            throw std::underflow_error(
                "cpr::Timeout: timeout value underflow: " + std::to_string(ms.count()) + " ms."
            );
        }

        return static_cast<long>(ms.count());
    }

    components::Files::iterator components::Files::begin() {
        return this->m_files.begin();
    }

    components::Files::iterator components::Files::end() {
        return this->m_files.end();
    }

    components::Files::const_iterator components::Files::begin() const {
        return this->m_files.begin();
    }

    components::Files::const_iterator components::Files::end() const {
        return this->m_files.end();
    }

    components::Files::const_iterator components::Files::cbegin() const {
        return this->m_files.cbegin();
    }

    components::Files::const_iterator components::Files::cend() const {
        return this->m_files.cend();
    }

    void components::Files::emplace_back(const File& file) {
        this->m_files.emplace_back(file);
    }

    void components::Files::push_back(const File& file) {
        this->m_files.push_back(file);
    }

    void components::Files::pop_back() {
        this->m_files.pop_back();
    }

    components::Multipart::Multipart(const std::initializer_list<Part>& parts) : parts{ parts } {}

    components::Parameters::Parameters(const std::initializer_list<Parameter>& parameters) {
        this->Add(parameters);
    }

    void components::Parameters::Add(const std::initializer_list<Parameter>& parameters) {
        for (const auto& parameter : parameters) {
            this->m_parameters.emplace_back(parameter.key, parameter.value);

#if defined(LIBOAI_DEBUG)
            _liboai_dbg(
                "[dbg] [@%s] Added parameter \"%s\" with value \"%s\".\n",
                __func__,
                parameter.key.c_str(),
                parameter.value.c_str()
            );
#endif
        }
    }

    void components::Parameters::Add(const Parameter& parameter) {
        this->m_parameters.emplace_back(parameter.key, parameter.value);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Added parameter \"%s\" with value \"%s\".\n",
            __func__,
            parameter.key.c_str(),
            parameter.value.c_str()
        );
#endif
    }

    bool components::Parameters::Empty() const {
        return this->m_parameters.empty();
    }

    std::string components::Parameters::BuildParameterString() const {
        std::string parameter_string;

        if (this->m_parameters.size() == 1) {
            parameter_string +=
                this->m_parameters.front().key + "=" + this->m_parameters.front().value;
        } else {
            for (const auto& parameter : this->m_parameters) {
                parameter_string += parameter.key + "=" + parameter.value + "&";
            }
            parameter_string.pop_back();
        }

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Built parameter string \"%s\".\n",
            __func__,
            parameter_string.c_str()
        );
#endif

        return parameter_string;
    }

    void Session::SetOption(const components::Header& header) {
        this->SetHeader(header);
    }

    void Session::SetHeader(const components::Header& header) {
        CURLcode e;

        for (const std::pair<const std::string, std::string>& item : header) {
            std::string header_string = item.first;
            if (item.second.empty()) {
                header_string += ";";
            } else {
                header_string += ": " + item.second;
            }

            curl_slist* temp = curl_slist_append(this->m_headers, header_string.c_str());
            if (temp) {
                this->m_headers = temp;
            }
        }

        curl_slist* temp;
        //  Causes cURL error for simple GET requests
        //    curl_slist* temp = curl_slist_append(this->headers, "Transfer-Encoding: chunked");
        //    if (temp) {
        //		this->headers = temp;
        //    }

        // remove preset curl headers for files >1MB
        temp = curl_slist_append(this->m_headers, "Expect:");
        if (temp) {
            this->m_headers = temp;
        }

        e = curl_easy_setopt(this->m_curl, CURLOPT_HTTPHEADER, this->m_headers);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Set headers.\n", __func__);
#endif

        ErrorCheck(e, "liboai::netimpl::Session::SetHeader()");
    }

    void Session::SetOption(const components::Parameters& parameters) {
        this->SetParameters(parameters);
    }

    void Session::SetParameters(const components::Parameters& parameters) {
        if (!parameters.Empty()) {
            this->m_parameter_string = parameters.BuildParameterString();

#if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] Set parameters.\n", __func__);
#endif
        }
    }

    void Session::SetOption(components::Parameters&& parameters) {
        this->SetParameters(std::move(parameters));
    }

    void Session::SetParameters(components::Parameters&& parameters) {
        if (!parameters.Empty()) {
            this->m_parameter_string = parameters.BuildParameterString();

#if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] Set parameters.\n", __func__);
#endif
        }
    }

    void Session::SetOption(const components::Timeout& timeout) {
        this->SetTimeout(timeout);
    }

    void Session::SetTimeout(const components::Timeout& timeout) {
        CURLcode e = curl_easy_setopt(this->m_curl, CURLOPT_TIMEOUT_MS, timeout.Milliseconds());

#if defined(LIBOAI_DEBUG)
        _liboai_dbg(
            "[dbg] [@%s] Set timeout to %ld milliseconds\n",
            __func__,
            timeout.Milliseconds()
        );
#endif

        ErrorCheck(e, "liboai::netimpl::Session::SetTimeout()");
    }

    void Session::SetOption(const components::Proxies& proxies) {
        this->SetProxies(proxies);
    }

    void Session::SetProxies(const components::Proxies& proxies) {
        this->m_proxies = proxies;

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Set proxies.\n", __func__);
#endif
    }

    void Session::SetOption(components::Proxies&& proxies) {
        this->SetProxies(std::move(proxies));
    }

    void Session::SetProxies(components::Proxies&& proxies) {
        this->m_proxies = std::move(proxies);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Set proxies.\n", __func__);
#endif
    }

    void Session::SetOption(const components::ProxyAuthentication& proxy_auth) {
        this->SetProxyAuthentication(proxy_auth);
    }

    void Session::SetProxyAuthentication(const components::ProxyAuthentication& proxy_auth) {
        this->m_proxyAuth = proxy_auth;

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Set proxy authentication.\n", __func__);
#endif
    }

    void Session::SetOption(components::ProxyAuthentication&& proxy_auth) {
        this->SetProxyAuthentication(std::move(proxy_auth));
    }

    void Session::SetProxyAuthentication(components::ProxyAuthentication&& proxy_auth) {
        this->m_proxyAuth = std::move(proxy_auth);

#if defined(LIBOAI_DEBUG)
        _liboai_dbg("[dbg] [@%s] Set proxy authentication.\n", __func__);
#endif
    }

    void Session::SetOption(const components::WriteCallback& write) {
        this->SetWriteCallback(write);
    }

    void Session::SetWriteCallback(const components::WriteCallback& write) {
        if (write.callback) {
            CURLcode e[2];
            memset(e, CURLcode::CURLE_OK, sizeof(e));

            e[0] = curl_easy_setopt(
                this->m_curl,
                CURLOPT_WRITEFUNCTION,
                components::writeUserFunction
            );
            this->m_write = write;
            e[1] = curl_easy_setopt(this->m_curl, CURLOPT_WRITEDATA, &this->m_write);

#if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] Set user supplied write callback.\n", __func__);
#endif

            ErrorCheck(e, 2, "liboai::netimpl::Session::SetWriteCallback()");
        }
    }

    void Session::SetOption(components::WriteCallback&& write) {
        this->SetWriteCallback(std::move(write));
    }

    void Session::SetWriteCallback(components::WriteCallback&& write) {
        if (write.callback) {
            CURLcode e[2];
            memset(e, CURLcode::CURLE_OK, sizeof(e));

            e[0] = curl_easy_setopt(
                this->m_curl,
                CURLOPT_WRITEFUNCTION,
                components::writeUserFunction
            );
            this->m_write = std::move(write);
            e[1] = curl_easy_setopt(this->m_curl, CURLOPT_WRITEDATA, &this->m_write);

#if defined(LIBOAI_DEBUG)
            _liboai_dbg("[dbg] [@%s] Set user supplied write callback.\n", __func__);
#endif

            ErrorCheck(e, 2, "liboai::netimpl::Session::SetWriteCallback()");
        }
    }

    components::Proxies::Proxies(
        const std::initializer_list<std::pair<const std::string, std::string>>& hosts
    )
        : m_hosts{ hosts } {}

    components::Proxies::Proxies(const std::map<std::string, std::string>& hosts)
        : m_hosts{ hosts } {}

    bool components::Proxies::has(const std::string& protocol) const {
        return m_hosts.count(protocol) > 0;
    }

    const std::string& components::Proxies::operator[](const std::string& protocol) {
        return m_hosts[protocol];
    }

    components::EncodedAuthentication::~EncodedAuthentication() noexcept {
        this->SecureStringClear(this->m_username);
        this->SecureStringClear(this->m_password);
    }

    const std::string& components::EncodedAuthentication::GetUsername() const {
        return this->m_username;
    }

    const std::string& components::EncodedAuthentication::GetPassword() const {
        return this->m_password;
    }

#if defined(__STDC_LIB_EXT1__)
    void components::EncodedAuthentication::SecureStringClear(std::string& s) {
        if (s.empty()) {
            return;
        }
        memset_s(&s.front(), s.length(), 0, s.length());
        s.clear();
    }
#elif defined(_WIN32)
    void components::EncodedAuthentication::SecureStringClear(std::string& s) {
        if (s.empty()) {
            return;
        }
        SecureZeroMemory(&s.front(), s.length());
        s.clear();
    }
#else
    #if defined(__clang__)
        #pragma clang optimize off // clang
    #elif defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW32__) || defined(__MINGW64__)
        #pragma GCC push_options   // g++
        #pragma GCC optimize("O0") // g++
    #endif
    void components::EncodedAuthentication::SecureStringClear(std::string& s) {
        if (s.empty()) {
            return;
        }

        char* ptr = &(s[0]);
        memset(ptr, '\0', s.length());
        s.clear();
    }

    #if defined(__clang__)
        #pragma clang optimize on // clang
    #elif defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW32__) || defined(__MINGW64__)
        #pragma GCC pop_options   // g++
    #endif
#endif

    bool components::ProxyAuthentication::has(const std::string& protocol) const {
        return m_proxyAuth.count(protocol) > 0;
    }

    const char* components::ProxyAuthentication::GetUsername(const std::string& protocol) {
        return m_proxyAuth[protocol].m_username.c_str();
    }

    const char* components::ProxyAuthentication::GetPassword(const std::string& protocol) {
        return m_proxyAuth[protocol].m_password.c_str();
    }

    void ErrorCheck(CURLcode* ecodes, size_t size, std::string_view where) {
        if (ecodes) {
            for (size_t i = 0; i < size; ++i) {
                if (ecodes[i] != CURLE_OK) {
                    throw liboai::exception::OpenAIException(
                        curl_easy_strerror(ecodes[i]),
                        liboai::exception::EType::E_CURLERROR,
                        where
                    );
                }
            }
        }
    }

    void ErrorCheck(CURLcode ecode, std::string_view where) {
        if (ecode != CURLE_OK) {
            throw liboai::exception::OpenAIException(
                curl_easy_strerror(ecode),
                liboai::exception::EType::E_CURLERROR,
                where
            );
        }
    }

#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
    void ErrorCheck(CURLFORMcode* ecodes, size_t size, std::string_view where) {
        if (ecodes) {
            for (size_t i = 0; i < size; ++i) {
                if (ecodes[i] != CURL_FORMADD_OK) {
                    throw liboai::exception::OpenAIException(
                        "curl_formadd() failed.",
                        liboai::exception::EType::E_CURLERROR,
                        where
                    );
                }
            }
        }
    }

    void ErrorCheck(CURLFORMcode ecode, std::string_view where) {
        if (ecode != CURL_FORMADD_OK) {
            throw liboai::exception::OpenAIException(
                "curl_formadd() failed.",
                liboai::exception::EType::E_CURLERROR,
                where
            );
        }
    }
#endif

} // namespace liboai::netimpl
