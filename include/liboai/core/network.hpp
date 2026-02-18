#pragma once

/*
    network.h : liboai network implementation.
        This header file provides declarations for the abstracted liboai
        Network implementation. Each component class will inherit from
        this class to make use of the network functionality provided by
        it.

        For instance, making a call to liboai::Image::Create(...) will
        make use of both this class to send the request to the OpenAI API
        and liboai::Authorization to provide the user's authorization
        information to successfully complete the request.
*/

#include <future>
#include <optional>

#include "liboai/core/netimpl.hpp"

namespace liboai {
    class Network {
    public:
        /*
            @brief Initialise the Network instance to use
                the provided API url.

            @param root     The URL to direct API calls to.
        */
        explicit Network(const std::string& root) noexcept : m_openai_root(root) {}
        NON_COPYABLE(Network)
        NON_MOVABLE(Network)

        /*
            @brief Function to download a file at 'from'
                to file path 'to.' Useful for downloading
                images from the OpenAI API given a URL to
                'from.'

                This function is not to be confused with
                liboai::File::download(...) which is used
                to download .jsonl files from the OpenAI API.

            @param *to     The path and filename to download the file to.
            @param *from   Where to download the file data from
                (such as a URL).

            @returns Bool indicating success or failure.
        */
        [[nodiscard]]
        static inline auto Download(
            const std::string& to,
            const std::string& from,
            netimpl::components::Header authorization
        ) noexcept(false) -> bool {
            std::ofstream file(to, std::ios::binary);
            Response res;
            res =
                netimpl::Download(file, netimpl::components::Url{ from }, std::move(authorization));

            return res.status_code == 200;
        }

        [[nodiscard]]
        static inline auto DownloadWithSession(
            const std::string& to,
            const std::string& from,
            netimpl::components::Header authorization,
            netimpl::Session& session
        ) noexcept(false) -> bool {
            std::ofstream file(to, std::ios::binary);
            Response res;
            res = netimpl::DownloadWithSession(
                session,
                file,
                netimpl::components::Url{ from },
                std::move(authorization)
            );

            return res.status_code == 200;
        }

        /*
            @brief Function to asynchronously download a
                file at 'from' to file path 'to.' Useful
                for downloading images from the OpenAI API
                given a URL to 'from.'

                This function is not to be confused with
                liboai::File::download(...) which is used
                to download .jsonl files from the OpenAI API.

            @param *to     The path and filename to download the file to.
            @param *from   Where to download the file data from
                (such as a URL).

            @returns Future bool indicating success or failure.
        */
        [[nodiscard]]
        static inline auto DownloadAsync(
            const std::string& to,
            const std::string& from,
            netimpl::components::Header authorization
        ) noexcept(false) -> std::future<bool> {
            return std::async(std::launch::async, [&]() -> bool {
                std::ofstream file(to, std::ios::binary);
                Response res;
                res = netimpl::Download(
                    file,
                    netimpl::components::Url{ from },
                    std::move(authorization)
                );

                return res.status_code == 200;
            });
        }

        [[nodiscard]]
        static inline auto DownloadAsyncWithSession(
            const std::string& to,
            const std::string& from,
            netimpl::components::Header authorization,
            netimpl::Session& session
        ) noexcept(false) -> std::future<bool> {
            return std::async(std::launch::async, [&]() -> bool {
                std::ofstream file(to, std::ios::binary);
                Response res;
                res = netimpl::DownloadWithSession(
                    session,
                    file,
                    netimpl::components::Url{ from },
                    std::move(authorization)
                );

                return res.status_code == 200;
            });
        }

    protected:
        enum class Method : uint8_t {
            HTTP_GET,   // GET
            HTTP_POST,  // POST
            HTTP_DELETE // DELETE
        };

        template <
            class... _Params,
            std::enable_if_t<
                std::conjunction_v<std::negation<std::is_lvalue_reference<_Params>>...>,
                int> = 0>
        [[nodiscard]]
        inline auto Request(
            const Method& http_method,
            const std::string& root,
            const std::string& endpoint,
            const std::string& content_type,
            std::optional<netimpl::components::Header> headers = std::nullopt,
            _Params&&... parameters
        ) const -> Response {
            netimpl::components::Header _headers = {
                { "Content-Type", content_type }
            };
            if (headers) {
                if (headers.value().size() != 0) {
                    for (auto& i : headers.value()) {
                        _headers.insert(std::move(i));
                    }
                }
            }

            Response res;
            if constexpr (sizeof...(parameters) > 0) {
                res = Network::MethodSchema<netimpl::components::Header&&, _Params&&...>::_method
                    [static_cast<uint8_t>(http_method)](
                        netimpl::components::Url{ root + endpoint },
                        std::move(_headers),
                        std::forward<_Params>(parameters)...
                    );
            } else {
                res = Network::MethodSchema<netimpl::components::Header&&>::_method
                    [static_cast<uint8_t>(http_method)](
                        netimpl::components::Url{ root + endpoint },
                        std::move(_headers)
                    );
            }

            return res;
        }

        template <
            class... _Params,
            std::enable_if_t<
                std::conjunction_v<std::negation<std::is_lvalue_reference<_Params>>...>,
                int> = 0>
        [[nodiscard]]
        inline auto RequestWithSession(
            const Method& http_method,
            const std::string& root,
            const std::string& endpoint,
            const std::string& content_type,
            netimpl::Session& session,
            std::optional<netimpl::components::Header> headers = std::nullopt,
            _Params&&... parameters
        ) const -> Response {
            netimpl::components::Header _headers = {
                { "Content-Type", content_type }
            };
            if (headers) {
                if (headers.value().size() != 0) {
                    for (auto& i : headers.value()) {
                        _headers.insert(std::move(i));
                    }
                }
            }

            Response res;
            if constexpr (sizeof...(parameters) > 0) {
                res =
                    Network::MethodSchemaWithSession<netimpl::components::Header&&, _Params&&...>::
                        _method[static_cast<uint8_t>(http_method)](
                            session,
                            netimpl::components::Url{ root + endpoint },
                            std::move(_headers),
                            std::forward<_Params>(parameters)...
                        );
            } else {
                res = Network::MethodSchemaWithSession<netimpl::components::Header&&>::_method
                    [static_cast<uint8_t>(http_method)](
                        session,
                        netimpl::components::Url{ root + endpoint },
                        std::move(_headers)
                    );
            }

            return res;
        }

        /*
				@brief Function to validate the existence and validity of

         * a file located at a provided file path. This is used
					in functions
         * that take a file path as a parameter
					to ensure that the file exists
         * and is valid.
			*/
        [[nodiscard]]
        auto Validate(const std::filesystem::path& path) const -> bool {
            // checks if the file exists, is a regular file, and is not empty
            if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
                return std::filesystem::file_size(path) > 0;
            }
            return false;
        }

        const std::string m_openai_root;
        const std::string m_azure_root = ".openai.azure.com/openai";

    private:
        template <class... T> struct MethodSchema {
            inline static std::function<Response(netimpl::components::Url&&, T...)> _method[3] = {
                netimpl::Get<netimpl::components::Url&&, T...>,
                netimpl::Post<netimpl::components::Url&&, T...>,
                netimpl::Delete<netimpl::components::Url&&, T...>
            };
        };

        template <class... T> struct MethodSchemaWithSession {
            inline static std::function<
                Response(netimpl::Session&, netimpl::components::Url&&, T...)>
                _method[3] = { netimpl::GetWithSession<netimpl::components::Url&&, T...>,
                               netimpl::PostWithSession<netimpl::components::Url&&, T...>,
                               netimpl::DeleteWithSession<netimpl::components::Url&&, T...> };
        };
    };
} // namespace liboai
