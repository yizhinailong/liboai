#pragma once

/*
    moderations.h : Moderations component class for OpenAI.
        This class contains all the methods for the Moderations component
        of the OpenAI API. This class provides access to 'Moderations'
        endpoints on the OpenAI API and should be accessed via the
        liboai.h header file through an instantiated liboai::OpenAI
        object after setting necessary authentication information
        through the liboai::Authorization::Authorizer() singleton
        object.
*/

#include "liboai/core/authorization.h"
#include "liboai/core/response.h"

namespace liboai {
    class Moderations final : private Network {
    public:
        Moderations(const std::string& root) : Network(root) {}
        NON_COPYABLE(Moderations)
        NON_MOVABLE(Moderations)
        ~Moderations() = default;

        /*
            @brief Create a new moderation and classify
                if the given text is safe or unsafe.

            @param *input    The text to be moderated.
            @param model     The model to use for the moderation.

            @return A liboai::Response object containing the image(s)
                data in JSON format.
        */
        LIBOAI_EXPORT liboai::Response create(
            const std::string& input,
            std::optional<std::string> model = std::nullopt) const& noexcept(false);

        /*
            @brief Asynchronously creates a new moderation and classifies
                if the given text is safe or unsafe.

            @param *input    The text to be moderated.
            @param model     The model to use for the moderation.

            @return A liboai::Response future containing the image(s)
                data in JSON format.
        */
        LIBOAI_EXPORT liboai::FutureResponse create_async(
            const std::string& input,
            std::optional<std::string> model = std::nullopt) const& noexcept(false);

    private:
        Authorization& auth_ = Authorization::Authorizer();
    };
} // namespace liboai
