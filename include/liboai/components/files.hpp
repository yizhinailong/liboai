#pragma once

/**
 * @file files.h
 *
 * Files component class for OpenAI.
 * This class contains all the methods for the Files component
 * of the OpenAI API. This class provides access to 'Files'
 * endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI
 * object after setting necessary authentication information
 * through the liboai::Authorization::Authorizer() singleton
 * object.
 */

#include "liboai/core/authorization.hpp"
#include "liboai/core/response.hpp"

namespace liboai {
    class Files final : private Network {
    public:
        explicit Files(const std::string& root) : Network(root) {}

        Files(const Files&) = delete;
        Files& operator=(const Files&) = delete;
        Files(Files&&) = delete;
        Files& operator=(Files&&) = delete;
        ~Files() = default;

        /**
         * @brief Returns a list of files that belong to the user's organization.
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto list() const& noexcept(false) -> liboai::Response;

        /**
         * @brief Asynchronously returns a list of files that belong to the
         *        user's organization.
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto list_async() const& noexcept(false) -> liboai::FutureResponse;

        /**
         * @brief Upload a file that contains document(s) to be
         *        used across various endpoints/features. Currently,
         *        the size of all the files uploaded by one organization
         *        can be up to 1 GB.
         *
         * @param file     The JSON Lines file to be uploaded (path).
         * @param purpose  The intended purpose of the uploaded documents.
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto
        create(const std::filesystem::path& file, const std::string& purpose) const& noexcept(false)
            -> liboai::Response;

        /**
         * @brief Asynchronously upload a file that contains document(s)
         *        to be used across various endpoints/features. Currently,
         *        the size of all the files uploaded by one organization
         *        can be up to 1 GB.
         *
         * @param file     The JSON Lines file to be uploaded (path).
         * @param purpose  The intended purpose of the uploaded documents.
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto create_async(
            const std::filesystem::path& file,
            const std::string& purpose
        ) const& noexcept(false) -> liboai::FutureResponse;

        /**
         * @brief Delete [remove] a file.
         *
         * @param *file_id   The ID of the file to use for this request
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto remove(const std::string& file_id) const& noexcept(false)
            -> liboai::Response;

        /**
         * @brief Asynchronously delete [remove] a file.
         *
         * @param *file_id   The ID of the file to use for this request
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto remove_async(const std::string& file_id) const& noexcept(false)
            -> liboai::FutureResponse;

        /**
         * @brief Returns information about a specific file.
         *
         * @param *file_id   The ID of the file to use for this request
         *
         * @return A liboai::Response object containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto retrieve(const std::string& file_id) const& noexcept(false)
            -> liboai::Response;

        /**
         * @brief Asynchronously returns information about a specific file.
         *
         * @param *file_id   The ID of the file to use for this request
         *
         * @return A liboai::Response future containing the image(s)
         *         data in JSON format.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto retrieve_async(const std::string& file_id) const& noexcept(false)
            -> liboai::FutureResponse;

        /**
         * @brief Downloads the contents of the specified file
         *        to the specified path.
         *
         * @param *file_id    The ID of the file to use for this request
         * @param *save_to    The path to save the file to
         *
         * @return a boolean value indicating whether the file was
         *         successfully downloaded or not.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto
        download(const std::string& file_id, const std::string& save_to) const& noexcept(false)
            -> bool;

        /**
         * @brief Asynchronously downloads the contents of the specified file
         *        to the specified path.
         *
         * @param *file_id    The ID of the file to use for this request
         * @param *save_to    The path to save the file to
         *
         * @return a boolean future indicating whether the file was
         *         successfully downloaded or not.
         */
        [[nodiscard]]
        LIBOAI_EXPORT auto download_async(
            const std::string& file_id,
            const std::string& save_to
        ) const& noexcept(false) -> std::future<bool>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };
} // namespace liboai
