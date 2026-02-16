#pragma once

/*
    Copyright (c) 2017-2021 Huu Nguyen
    Copyright (c) 2022 libcpr and many other contributors

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    netimpl.h : Holds the internal network control-flow implementation.
        This header file provides the internal interface(s) used to
        allow files such as network.h to properly work. It contains
        the internal cURL network wrapping functionality and all
        other network-related functionality.

        This was created to remove the dependency on the library
        cURL for People (CPR).
*/

#include <fstream>
#include <future>
#include <mutex>
#include <optional>
#include <sstream>

#include <curl/curl.h>

#include "liboai/core/response.hpp"

namespace liboai::netimpl {
    static bool _flag = false;

    void ErrorCheck(CURLcode* ecodes, size_t size, std::string_view where);
    void ErrorCheck(CURLcode ecode, std::string_view where);

#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
    void ErrorCheck(CURLFORMcode* ecodes, size_t size, std::string_view where);
    void ErrorCheck(CURLFORMcode ecode, std::string_view where);
#endif

    class CurlHolder {
    public:
        CurlHolder();
        NON_COPYABLE(CurlHolder)
        NON_MOVABLE(CurlHolder)
        virtual ~CurlHolder();

        std::string urlEncode(const std::string& s);
        std::string urlDecode(const std::string& s);

    private:
        static std::mutex& curl_easy_get_mutex_() {
            static std::mutex g_curl_mutex;
            return g_curl_mutex;
        }

    protected:
        CURL* m_curl = nullptr;
    };

    /*
        Contains all components that can be passed to below free methods
        Get, Post, and Delete such as Url, Headers, Body, Multipart,
        etc.
    */
    namespace components {
        template <class T> class StringHolder {
        public:
            StringHolder() = default;

            explicit StringHolder(std::string str) : m_str(std::move(str)) {}

            explicit StringHolder(std::string_view str) : m_str(str) {}

            explicit StringHolder(const char* str) : m_str(str) {}

            StringHolder(const char* str, size_t len) : m_str(str, len) {}

            StringHolder(const std::initializer_list<std::string> args) {
                m_str = std::accumulate(args.begin(), args.end(), m_str);
            }

            StringHolder(const StringHolder& other) = default;
            StringHolder(StringHolder&& old) noexcept = default;
            virtual ~StringHolder() = default;

            StringHolder& operator=(StringHolder&& old) noexcept = default;
            StringHolder& operator=(const StringHolder& other) = default;

            explicit operator std::string() const { return m_str; }

            T operator+(const char* rhs) const { return T(m_str + rhs); }

            T operator+(const std::string& rhs) const { return T(m_str + rhs); }

            T operator+(const StringHolder<T>& rhs) const { return T(m_str + rhs.m_str); }

            void operator+=(const char* rhs) { m_str += rhs; }

            void operator+=(const std::string& rhs) { m_str += rhs; }

            void operator+=(const StringHolder<T>& rhs) { m_str += rhs; }

            bool operator==(const char* rhs) const { return m_str == rhs; }

            bool operator==(const std::string& rhs) const { return m_str == rhs; }

            bool operator==(const StringHolder<T>& rhs) const { return m_str == rhs.m_str; }

            bool operator!=(const char* rhs) const { return m_str.c_str() != rhs; }

            bool operator!=(const std::string& rhs) const { return m_str != rhs; }

            bool operator!=(const StringHolder<T>& rhs) const { return m_str != rhs.m_str; }

            const std::string& str() { return m_str; }

            const std::string& str() const { return m_str; }

            const char* c_str() const { return m_str.c_str(); }

            const char* data() const { return m_str.data(); }

        protected:
            std::string m_str{};
        };

        struct File final {
            File(const File& other) {
                this->filepath = other.filepath;
                this->overrided_filename = other.overrided_filename;
            }

            File(File&& old) noexcept {
                this->filepath = std::move(old.filepath);
                this->overrided_filename = std::move(old.overrided_filename);
            }

            explicit File(std::string p_filepath, const std::string& p_overrided_filename = {})
                : filepath(std::move(p_filepath)), overrided_filename(p_overrided_filename) {}

            File& operator=(const File& other) {
                this->filepath = other.filepath;
                this->overrided_filename = other.overrided_filename;
                return *this;
            }

            File& operator=(File&& old) noexcept {
                this->filepath = std::move(old.filepath);
                this->overrided_filename = std::move(old.overrided_filename);
                return *this;
            }

            std::string filepath;
            std::string overrided_filename;

            bool hasOverridedFilename() const noexcept { return !overrided_filename.empty(); };
        };

        class Files final {
        public:
            Files() = default;

            Files(const Files& other) { this->m_files = other.m_files; }

            Files(Files&& old) noexcept { this->m_files = std::move(old.m_files); }

            Files(const File& p_file) : m_files{ p_file } {};
            Files(const std::initializer_list<File>& p_files) : m_files{ p_files } {};

            Files(const std::initializer_list<std::string>& p_filepaths) {
                for (const std::string& filepath : p_filepaths) {
                    m_files.emplace_back(File(filepath));
                }
            };

            ~Files() noexcept = default;

            Files& operator=(const Files& other) {
                this->m_files = other.m_files;
                return *this;
            }

            Files& operator=(Files&& old) noexcept {
                this->m_files = std::move(old.m_files);
                return *this;
            }

            using iterator = std::vector<File>::iterator;
            using const_iterator = std::vector<File>::const_iterator;

            iterator begin();
            iterator end();
            const_iterator begin() const;
            const_iterator end() const;
            const_iterator cbegin() const;
            const_iterator cend() const;
            void emplace_back(const File& file);
            void push_back(const File& file);
            void pop_back();

        private:
            std::vector<File> m_files;
        };

        class Url final : public StringHolder<Url> {
        public:
            Url() = default;

            Url(std::string url) : StringHolder<Url>(std::move(url)) {}

            Url(std::string_view url) : StringHolder<Url>(url) {}

            Url(const char* url) : StringHolder<Url>(url) {}

            Url(const char* str, size_t len) : StringHolder<Url>(std::string(str, len)) {}

            Url(const std::initializer_list<std::string> args) : StringHolder<Url>(args) {}

            Url(const Url& other) = default;
            Url(Url&& old) noexcept = default;
            ~Url() override = default;

            Url& operator=(Url&& old) noexcept = default;
            Url& operator=(const Url& other) = default;
        };

        class Body final : public StringHolder<Body> {
        public:
            Body() = default;

            Body(const Body& other) { this->m_str = other.m_str; }

            Body(Body&& old) noexcept { this->m_str = std::move(old.m_str); }

            Body(std::string body) : StringHolder<Body>(std::move(body)) {}

            Body(std::string_view body) : StringHolder<Body>(body) {}

            Body(const char* body) : StringHolder<Body>(body) {}

            Body(const char* str, size_t len) : StringHolder<Body>(str, len) {}

            Body(const std::initializer_list<std::string> args) : StringHolder<Body>(args) {}

            Body(const File& file) {
                std::ifstream is(file.filepath, std::ifstream::binary);
                if (!is) {
                    throw std::invalid_argument("Can't open the file for HTTP request body!");
                }

                is.seekg(0, std::ios::end);
                const std::streampos length = is.tellg();
                is.seekg(0, std::ios::beg);
                std::string buffer;
                buffer.resize(static_cast<size_t>(length));
                is.read(buffer.data(), length);
                m_str = std::move(buffer);
            }

            ~Body() override = default;

            Body& operator=(Body&& old) noexcept {
                this->m_str = std::move(old.m_str);
                return *this;
            }

            Body& operator=(const Body& other) {
                this->m_str = other.m_str;
                return *this;
            }
        };

        struct Buffer final {
            using data_t = const unsigned char*;

            template <typename Iterator>
            Buffer(Iterator begin, Iterator end, std::filesystem::path&& p_filename)
                : data{ reinterpret_cast<data_t>(&(*begin)) },
                  datalen{ static_cast<long>(std::distance(begin, end)) },
                  filename(std::move(p_filename)) {
                is_random_access_iterator(begin, end);
                static_assert(sizeof(*begin) == 1, "Only byte buffers can be used");
            }

            template <typename Iterator>
            typename std::enable_if<std::is_same<
                typename std::iterator_traits<Iterator>::iterator_category,
                std::random_access_iterator_tag>::value>::type
            is_random_access_iterator(Iterator /* begin */, Iterator /* end */) {}

            data_t data;
            long datalen;
            const std::filesystem::path filename;
        };

        struct Part final {
            Part(const Part& other) {
                this->name = other.name;
                this->value = other.value;
                this->content_type = other.content_type;
                this->data = other.data;
                this->datalen = other.datalen;
                this->is_file = other.is_file;
                this->is_buffer = other.is_buffer;
                this->files = other.files;
            }

            Part(Part&& old) noexcept {
                this->name = std::move(old.name);
                this->value = std::move(old.value);
                this->content_type = std::move(old.content_type);
                this->data = old.data;
                this->datalen = old.datalen;
                this->is_file = old.is_file;
                this->is_buffer = old.is_buffer;
                this->files = std::move(old.files);
            }

            Part(
                const std::string& p_name,
                const std::string& p_value,
                const std::string& p_content_type = {}
            )
                : name{ p_name },
                  value{ p_value },
                  content_type{ p_content_type },
                  is_file{ false },
                  is_buffer{ false } {}

            Part(
                const std::string& p_name,
                const std::int32_t& p_value,
                const std::string& p_content_type = {}
            )
                : name{ p_name },
                  value{ std::to_string(p_value) },
                  content_type{ p_content_type },
                  is_file{ false },
                  is_buffer{ false } {}

            Part(
                const std::string& p_name,
                const Files& p_files,
                const std::string& p_content_type = {}
            )
                : name{ p_name },
                  value{},
                  content_type{ p_content_type },
                  is_file{ true },
                  is_buffer{ false },
                  files{ p_files } {}

            Part(const std::string& p_name, Files&& p_files, const std::string& p_content_type = {})
                : name{ p_name },
                  value{},
                  content_type{ p_content_type },
                  is_file{ true },
                  is_buffer{ false },
                  files{ std::move(p_files) } {}

            Part(
                const std::string& p_name,
                const Buffer& buffer,
                const std::string& p_content_type = {}
            )
                : name{ p_name },
                  value{ buffer.filename.string() },
                  content_type{ p_content_type },
                  data{ buffer.data },
                  datalen{ buffer.datalen },
                  is_file{ false },
                  is_buffer{ true } {}

            Part& operator=(const Part& other) {
                this->name = other.name;
                this->value = other.value;
                this->content_type = other.content_type;
                this->data = other.data;
                this->datalen = other.datalen;
                this->is_file = other.is_file;
                this->is_buffer = other.is_buffer;
                this->files = other.files;
                return *this;
            }

            Part& operator=(Part&& old) noexcept {
                this->name = std::move(old.name);
                this->value = std::move(old.value);
                this->content_type = std::move(old.content_type);
                this->data = old.data;
                this->datalen = old.datalen;
                this->is_file = old.is_file;
                this->is_buffer = old.is_buffer;
                this->files = std::move(old.files);
                return *this;
            }

            std::string name;
            std::string value;
            std::string content_type;
            Buffer::data_t data{ nullptr };
            long datalen{ 0 };
            bool is_file;
            bool is_buffer;

            Files files;
        };

        class Multipart final {
        public:
            Multipart() = default;

            Multipart(const Multipart& other) { this->parts = other.parts; }

            Multipart(Multipart&& old) noexcept { this->parts = std::move(old.parts); }

            Multipart& operator=(const Multipart& other) {
                this->parts = other.parts;
                return *this;
            }

            Multipart& operator=(Multipart&& old) noexcept {
                this->parts = std::move(old.parts);
                return *this;
            }

            Multipart(const std::initializer_list<Part>& parts);

            std::vector<Part> parts;
        };

        struct CaseInsensitiveCompare {
            bool operator()(const std::string& a, const std::string& b) const noexcept {
                return std::lexicographical_compare(
                    a.begin(),
                    a.end(),
                    b.begin(),
                    b.end(),
                    [](unsigned char ac, unsigned char bc) {
                        return std::tolower(ac) < std::tolower(bc);
                    }
                );
            }
        };

        using Header = std::map<std::string, std::string, CaseInsensitiveCompare>;

        struct Parameter final {
            Parameter() = default;

            Parameter(const Parameter& other) {
                this->key = other.key;
                this->value = other.value;
            }

            Parameter(Parameter&& old) noexcept {
                this->key = std::move(old.key);
                this->value = std::move(old.value);
            }

            Parameter(std::string p_key, std::string p_value)
                : key{ std::move(p_key) }, value{ std::move(p_value) } {}

            Parameter& operator=(const Parameter& other) {
                this->key = other.key;
                this->value = other.value;
                return *this;
            }

            Parameter& operator=(Parameter&& old) noexcept {
                this->key = std::move(old.key);
                this->value = std::move(old.value);
                return *this;
            }

            std::string key;
            std::string value;
        };

        class Parameters final {
        public:
            Parameters() = default;

            Parameters(const Parameters& other) { this->m_parameters = other.m_parameters; }

            Parameters(Parameters&& old) noexcept {
                this->m_parameters = std::move(old.m_parameters);
            }

            Parameters(const std::initializer_list<Parameter>& parameters);

            Parameters& operator=(const Parameters& other) {
                this->m_parameters = other.m_parameters;
                return *this;
            }

            Parameters& operator=(Parameters&& old) noexcept {
                this->m_parameters = std::move(old.m_parameters);
                return *this;
            }

            void Add(const std::initializer_list<Parameter>& parameters);
            void Add(const Parameter& parameter);
            bool Empty() const;

            std::string BuildParameterString() const;

        private:
            std::vector<Parameter> m_parameters;
        };

        class Timeout final {
        public:
            Timeout(const std::chrono::milliseconds& duration) : ms{ duration } {}

            Timeout(const std::int32_t& milliseconds)
                : Timeout{ std::chrono::milliseconds(milliseconds) } {}

            long Milliseconds() const;

            std::chrono::milliseconds ms;
        };

        class Proxies final {
        public:
            Proxies() = default;

            Proxies(const Proxies& other) { this->m_hosts = other.m_hosts; }

            Proxies(Proxies&& old) noexcept { this->m_hosts = std::move(old.m_hosts); }

            Proxies(const std::initializer_list<std::pair<const std::string, std::string>>& hosts);
            Proxies(const std::map<std::string, std::string>& hosts);

            Proxies& operator=(const Proxies& other) {
                this->m_hosts = other.m_hosts;
                return *this;
            }

            Proxies& operator=(Proxies&& old) noexcept {
                this->m_hosts = std::move(old.m_hosts);
                return *this;
            }

            bool has(const std::string& protocol) const;
            const std::string& operator[](const std::string& protocol);

        private:
            std::map<std::string, std::string> m_hosts;
        };

        std::string urlEncodeHelper(const std::string& s);
        std::string urlDecodeHelper(const std::string& s);

        class ProxyAuthentication;

        class EncodedAuthentication final {
            friend ProxyAuthentication;

        public:
            EncodedAuthentication() = default;

            EncodedAuthentication(const EncodedAuthentication& other) {
                this->m_username = other.m_username;
                this->m_password = other.m_password;
            }

            EncodedAuthentication(EncodedAuthentication&& old) noexcept {
                this->m_username = std::move(old.m_username);
                this->m_password = std::move(old.m_password);
            }

            EncodedAuthentication(const std::string& p_username, const std::string& p_password)
                : m_username(urlEncodeHelper(p_username)),
                  m_password(urlEncodeHelper(p_password)) {}

            virtual ~EncodedAuthentication() noexcept;

            EncodedAuthentication& operator=(EncodedAuthentication&& old) noexcept {
                this->m_username = std::move(old.m_username);
                this->m_password = std::move(old.m_password);
                return *this;
            }

            EncodedAuthentication& operator=(const EncodedAuthentication& other) {
                this->m_username = other.m_username;
                this->m_password = other.m_password;
                return *this;
            }

            [[nodiscard]] const std::string& GetUsername() const;
            [[nodiscard]] const std::string& GetPassword() const;

            void SecureStringClear(std::string& str);

        private:
            std::string m_username;
            std::string m_password;
        };

        class ProxyAuthentication final {
        public:
            ProxyAuthentication() = default;

            ProxyAuthentication(const ProxyAuthentication& other) {
                this->m_proxyAuth = other.m_proxyAuth;
            }

            ProxyAuthentication(ProxyAuthentication&& old) noexcept {
                this->m_proxyAuth = std::move(old.m_proxyAuth);
            }

            ProxyAuthentication(
                const std::initializer_list<std::pair<const std::string, EncodedAuthentication>>&
                    auths
            )
                : m_proxyAuth{ auths } {}

            explicit ProxyAuthentication(const std::map<std::string, EncodedAuthentication>& auths)
                : m_proxyAuth{ auths } {}

            ProxyAuthentication& operator=(const ProxyAuthentication& other) {
                this->m_proxyAuth = other.m_proxyAuth;
                return *this;
            }

            ProxyAuthentication& operator=(ProxyAuthentication&& old) noexcept {
                this->m_proxyAuth = std::move(old.m_proxyAuth);
                return *this;
            }

            [[nodiscard]] bool has(const std::string& protocol) const;
            const char* GetUsername(const std::string& protocol);
            const char* GetPassword(const std::string& protocol);

        private:
            std::map<std::string, EncodedAuthentication> m_proxyAuth;
        };

        class WriteCallback final {
        public:
            WriteCallback() = default;

            WriteCallback(const WriteCallback& other)
                : callback(other.callback), userdata(other.userdata) {}

            WriteCallback(WriteCallback&& old) noexcept
                : callback(std::move(old.callback)), userdata(std::move(old.userdata)) {}

            WriteCallback(
                std::function<bool(std::string data, intptr_t userdata)> p_callback,
                intptr_t p_userdata = 0
            )
                : userdata(p_userdata), callback(std::move(p_callback)) {}

            WriteCallback& operator=(const WriteCallback& other) {
                this->callback = other.callback;
                this->userdata = other.userdata;
                return *this;
            }

            WriteCallback& operator=(WriteCallback&& old) noexcept {
                this->callback = std::move(old.callback);
                this->userdata = std::move(old.userdata);
                return *this;
            }

            [[nodiscard]] bool operator()(std::string data) const {
                return callback(std::move(data), userdata);
            }

            intptr_t userdata{};
            std::function<bool(std::string data, intptr_t userdata)> callback;
        };

        size_t writeUserFunction(char* ptr, size_t size, size_t nmemb, const WriteCallback* write);
        size_t writeFunction(char* ptr, size_t size, size_t nmemb, std::string* data);
        size_t writeFileFunction(char* ptr, size_t size, size_t nmemb, std::ofstream* file);
    }; // namespace components

    /*
        Class for sessions; each session is a single request.
            Each call to Network::Request should follow the
            following schema:

            1. Create a session object.
            2. Set the session's options.
            3. Call the session's X() method where X is the
                request method (GET, POST, etc.).
            4. Return the resulting Response object.
    */
    class Session final : private CurlHolder {
    public:
        Session() = default;
        ~Session() override;

        liboai::Response Get();
        liboai::Response Post();
        liboai::Response Delete();
        liboai::Response Download(std::ofstream& file);
        void ClearContext();

    private:
        template <class... _Options> friend void set_options(Session&, _Options&&...);

        void Prepare();
        void PrepareDownloadInternal();
        CURLcode Perform();
        liboai::Response BuildResponseObject();
        liboai::Response Complete();
        liboai::Response CompleteDownload();

        void PrepareGet();
        void PreparePost();
        void PrepareDelete();
        void PrepareDownload(std::ofstream& file);

        void ParseResponseHeader(
            const std::string& headers,
            std::string* status_line,
            std::string* reason
        );

        void SetOption(const components::Url& url);
        void SetUrl(const components::Url& url);

        void SetOption(const components::Body& body);
        void SetBody(const components::Body& body);
        void SetOption(components::Body&& body);
        void SetBody(components::Body&& body);

        void SetOption(const components::Multipart& multipart);
        void SetMultipart(const components::Multipart& multipart);
        void SetOption(components::Multipart&& multipart);
        void SetMultipart(components::Multipart&& multipart);

        void SetOption(const components::Header& header);
        void SetHeader(const components::Header& header);

        void SetOption(const components::Parameters& parameters);
        void SetParameters(const components::Parameters& parameters);
        void SetOption(components::Parameters&& parameters);
        void SetParameters(components::Parameters&& parameters);

        void SetOption(const components::Timeout& timeout);
        void SetTimeout(const components::Timeout& timeout);

        void SetOption(const components::Proxies& proxies);
        void SetProxies(const components::Proxies& proxies);
        void SetOption(components::Proxies&& proxies);
        void SetProxies(components::Proxies&& proxies);

        void SetOption(const components::ProxyAuthentication& proxy_auth);
        void SetProxyAuthentication(const components::ProxyAuthentication& proxy_auth);
        void SetOption(components::ProxyAuthentication&& proxy_auth);
        void SetProxyAuthentication(components::ProxyAuthentication&& proxy_auth);

        void SetOption(const components::WriteCallback& write);
        void SetWriteCallback(const components::WriteCallback& write);
        void SetOption(components::WriteCallback&& write);
        void SetWriteCallback(components::WriteCallback&& write);

        long m_status_code = 0;
        double m_elapsed = 0.0;
        std::string m_status_line{}, m_content{}, m_url_str{}, m_reason{};

        // internally-used members...
        curl_slist* m_headers = nullptr;
#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
        curl_httppost* m_form = nullptr;
#endif

#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
        curl_mime* m_mime = nullptr;
#endif

        bool m_hasBody = false;
        std::string m_parameter_string, m_url, m_response_string, m_header_string;
        components::Proxies m_proxies;
        components::ProxyAuthentication m_proxyAuth;
        components::WriteCallback m_write;
    };

    template <class... _Options> liboai::Response Get(_Options&&... options) {
        Session session;
        set_options(session, std::forward<_Options>(options)...);
        return session.Get();
    }

    template <class... _Options>
    liboai::Response GetWithSession(Session& session, _Options&&... options) {
        session.ClearContext();
        set_options(session, std::forward<_Options>(options)...);
        return session.Get();
    }

    template <class... _Options> liboai::Response Post(_Options&&... options) {
        Session session;
        set_options(session, std::forward<_Options>(options)...);
        return session.Post();
    }

    template <class... _Options>
    liboai::Response PostWithSession(Session& session, _Options&&... options) {
        session.ClearContext();
        set_options(session, std::forward<_Options>(options)...);
        return session.Post();
    }

    template <class... _Options> liboai::Response Delete(_Options&&... options) {
        Session session;
        set_options(session, std::forward<_Options>(options)...);
        return session.Delete();
    }

    template <class... _Options>
    liboai::Response DeleteWithSession(Session& session, _Options&&... options) {
        session.ClearContext();
        set_options(session, std::forward<_Options>(options)...);
        return session.Delete();
    }

    template <class... _Options>
    liboai::Response Download(std::ofstream& file, _Options&&... options) {
        Session session;
        set_options(session, std::forward<_Options>(options)...);
        return session.Download(file);
    }

    template <class... _Options>
    liboai::Response
    DownloadWithSession(Session& session, std::ofstream& file, _Options&&... options) {
        session.ClearContext();
        set_options(session, std::forward<_Options>(options)...);
        return session.Download(file);
    }

    template <class... _Options> void set_options(Session& session, _Options&&... opts) {
        (session.SetOption(std::forward<_Options>(opts)), ...);
    }
} // namespace liboai::netimpl
