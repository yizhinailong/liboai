#pragma once

/*
    models.h : Models component class for OpenAI.
        This class contains all the methods for the Models component
        of the OpenAI API. This class provides access to 'Models'
        endpoints on the OpenAI API and should be accessed via the
        liboai.h header file through an instantiated liboai::OpenAI
        object after setting necessary authentication information
        through the liboai::Authorization::Authorizer() singleton
        object.
*/

#include "liboai/core/authorization.hpp"
#include "liboai/core/response.hpp"

namespace liboai {
    class Models final : private Network {
    public:
        explicit Models(const std::string& root) : Network(root) {}
        NON_COPYABLE(Models)
        NON_MOVABLE(Models)
        ~Models() = default;

        /*
            @brief List all available models.

            @returns A liboai::Response object containing the image(s)
                data in JSON format.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto list() const& noexcept(false) -> liboai::Response;

        /*
            @brief Asynchronously list all available models.

            @returns A liboai::Response future containing the image(s)
                data in JSON format.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto list_async() const& noexcept(false) -> liboai::FutureResponse;

        /*
            @brief Retrieve a specific model's information.

            #param *model   The model to retrieve information for.

            @returns A liboai::Response object containing the image(s)
                data in JSON format.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto retrieve(const std::string& model) const& noexcept(false)
            -> liboai::Response;

        /*
            @brief Asynchronously retrieve a specific model's information.

            @param *model   The model to retrieve information for.

            @returns A liboai::Response future containing the image(s)
                data in JSON format.
        */
        [[nodiscard]]
        LIBOAI_EXPORT auto retrieve_async(const std::string& model) const& noexcept(false)
            -> liboai::FutureResponse;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };
} // namespace liboai
