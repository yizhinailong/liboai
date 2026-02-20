module;

#include <cpr/cpr.h>

/**
 * @file fine_tunes.cppm
 *
 * Fine-tunes component class for OpenAI. This class contains all the methods
 * for the Fine-tunes component of the OpenAI API. This class provides access
 * to 'Fine-tunes' endpoints on the OpenAI API and should be accessed via the
 * liboai.h header file through an instantiated liboai::OpenAI object after
 * setting necessary authentication information through the
 * liboai::Authorization::Authorizer() singleton object.
 */

export module liboai:components.fine_tunes;

import std;
import :core.authorization;
import :core.error;
import :core.response;
import :core.network;

export namespace liboai {
    class FineTunes final : private Network {
    public:
        explicit FineTunes(const std::string& root) : Network(root) {}

        FineTunes(const FineTunes&) = delete;
        FineTunes& operator=(const FineTunes&) = delete;
        FineTunes(FineTunes&&) = delete;
        FineTunes& operator=(FineTunes&&) = delete;
        ~FineTunes() = default;

        using StreamCallback = std::function<bool(std::string, intptr_t)>;

        /**
         * @brief Creates a job that fine-tunes a specified model from a given
         *        dataset.
         *
         * @param *training_file The ID of an uploaded file that contains
         *                       training data.
         * @param validation_file The ID of an uploaded file that contains
         *                        validation data.
         * @param model The name of the base model to fine-tune.
         * @param n_epochs The number of epochs to train for.
         * @param batch_size The batch size to use for training.
         * @param learning_rate_multiplier The learning rate multiplier to use
         *                                 for training.
         * @param prompt_loss_weight The prompt loss weight to use for training.
         * @param compute_classification_metrics If set, we calculate
         *                                       classification-specific metrics
         *                                       such as accuracy and F-1 score
         *                                       using the validation set at the
         *                                       end of every epoch.
         * @param classification_n_classes The number of classes in the
         *                                 classification task.
         * @param classification_positive_class The positive class in binary
         *                                      classification.
         * @param classification_betas If this is provided, we calculate F-beta
         *                             scores at the specified beta values.
         * @param suffix A suffix to append to the model name.
         *
         * @return A liboai::Response object containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto Create(
            const std::string& training_file,
            std::optional<std::string> validation_file = std::nullopt,
            std::optional<std::string> model_id = std::nullopt,
            std::optional<uint8_t> n_epochs = std::nullopt,
            std::optional<uint16_t> batch_size = std::nullopt,
            std::optional<float> learning_rate_multiplier = std::nullopt,
            std::optional<float> prompt_loss_weight = std::nullopt,
            std::optional<bool> compute_classification_metrics = std::nullopt,
            std::optional<uint16_t> classification_n_classes = std::nullopt,
            std::optional<std::string> classification_positive_class = std::nullopt,
            std::optional<std::vector<float>> classification_betas = std::nullopt,
            std::optional<std::string> suffix = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously creates a job that fine-tunes a specified model
         *        from a given dataset.
         *
         * @param *training_file The ID of an uploaded file that contains
         *                       training data.
         * @param validation_file The ID of an uploaded file that contains
         *                        validation data.
         * @param model The name of the base model to fine-tune.
         * @param n_epochs The number of epochs to train for.
         * @param batch_size The batch size to use for training.
         * @param learning_rate_multiplier The learning rate multiplier to use
         *                                 for training.
         * @param prompt_loss_weight The prompt loss weight to use for training.
         * @param compute_classification_metrics If set, we calculate
         *                                       classification-specific metrics
         *                                       such as accuracy and F-1 score
         *                                       using the validation set at the
         *                                       end of every epoch.
         * @param classification_n_classes The number of classes in the
         *                                 classification task.
         * @param classification_positive_class The positive class in binary
         *                                      classification.
         * @param classification_betas If this is provided, we calculate F-beta
         *                             scores at the specified beta values.
         * @param suffix A suffix to append to the model name.
         *
         * @return A liboai::Response future containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto CreateAsync(
            const std::string& training_file,
            std::optional<std::string> validation_file = std::nullopt,
            std::optional<std::string> model_id = std::nullopt,
            std::optional<uint8_t> n_epochs = std::nullopt,
            std::optional<uint16_t> batch_size = std::nullopt,
            std::optional<float> learning_rate_multiplier = std::nullopt,
            std::optional<float> prompt_loss_weight = std::nullopt,
            std::optional<bool> compute_classification_metrics = std::nullopt,
            std::optional<uint16_t> classification_n_classes = std::nullopt,
            std::optional<std::string> classification_positive_class = std::nullopt,
            std::optional<std::vector<float>> classification_betas = std::nullopt,
            std::optional<std::string> suffix = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief List your organization's fine-tuning jobs.
         *
         * @return A liboai::Response object containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto List() const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously list your organization's fine-tuning jobs.
         *
         * @return A liboai::Response future containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto ListAsync() const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Returns information about a specific file.
         *
         * @param *fine_tune_id The ID of the fine-tune job.
         *
         * @return A liboai::Response object containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto Retrieve(const std::string& fine_tune_id) const& noexcept
            -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously returns information about a specific file.
         *
         * @param *fine_tune_id The ID of the fine-tune job.
         *
         * @return A liboai::Response future containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto RetrieveAsync(const std::string& fine_tune_id) const& noexcept
            -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Immediately cancel a fine-tune job.
         *
         * @param *fine_tune_id The ID of the fine-tune job to cancel.
         *
         * @return A liboai::Response object containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto Cancel(const std::string& fine_tune_id) const& noexcept
            -> liboai::Expected<liboai::Response>;

        /**
         * @brief Immediately cancel a fine-tune job asynchronously.
         *
         * @param *fine_tune_id The ID of the fine-tune job to cancel.
         *
         * @return A liboai::Response future containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto CancelAsync(const std::string& fine_tune_id) const& noexcept
            -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Get fine-grained status updates for a fine-tune job.
         *
         * @param *fine_tune_id The ID of the fine-tune job to get events for.
         * @param stream Callback to stream events for the fine-tune job. If no
         *               callback is supplied, this parameter is disabled and the
         *               API will wait until the completion is finished before
         *               returning the response.
         *
         * @return A liboai::Response future containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto ListEvents(
            const std::string& fine_tune_id,
            std::optional<StreamCallback> stream = std::nullopt
        ) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously get fine-grained status updates for a
         *        fine-tune job.
         *
         * @param *fine_tune_id The ID of the fine-tune job to get events for.
         * @param stream Callback to stream events for the fine-tune job. If no
         *               callback is supplied, this parameter is disabled and the
         *               API will wait until the completion is finished before
         *               returning the response.
         *
         * @return A liboai::Response future containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto ListEventsAsync(
            const std::string& fine_tune_id,
            std::optional<StreamCallback> stream = std::nullopt
        ) const& noexcept -> liboai::FutureExpected<liboai::Response>;

        /**
         * @brief Delete a fine-tuned model. You must have the Owner role in
         *        your organization.
         *
         * @param *model The model to delete.
         *
         * @return A liboai::Response future containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto Remove(const std::string& model) const& noexcept -> liboai::Expected<liboai::Response>;

        /**
         * @brief Asynchronously deletes a fine-tuned model. You must have the
         *        Owner role in your organization.
         *
         * @param *model The model to delete.
         *
         * @return A liboai::Response future containing the image(s) data in
         *         JSON format.
         */
        [[nodiscard]]
        auto RemoveAsync(const std::string& model) const& noexcept
            -> liboai::FutureExpected<liboai::Response>;

    private:
        Authorization& m_auth = Authorization::Authorizer();
    };

    // Implementation
    auto FineTunes::Create(
        const std::string& training_file,
        std::optional<std::string> validation_file,
        std::optional<std::string> model_id,
        std::optional<uint8_t> n_epochs,
        std::optional<uint16_t> batch_size,
        std::optional<float> learning_rate_multiplier,
        std::optional<float> prompt_loss_weight,
        std::optional<bool> compute_classification_metrics,
        std::optional<uint16_t> classification_n_classes,
        std::optional<std::string> classification_positive_class,
        std::optional<std::vector<float>> classification_betas,
        std::optional<std::string> suffix
    ) const& noexcept -> Expected<Response> {
        JsonConstructor jcon;
        jcon.push_back("training_file", training_file);
        jcon.push_back("validation_file", std::move(validation_file));
        jcon.push_back("model_id", std::move(model_id));
        jcon.push_back("n_epochs", std::move(n_epochs));
        jcon.push_back("batch_size", std::move(batch_size));
        jcon.push_back("learning_rate_multiplier", std::move(learning_rate_multiplier));
        jcon.push_back("prompt_loss_weight", std::move(prompt_loss_weight));
        jcon.push_back("compute_classification_metrics", std::move(compute_classification_metrics));
        jcon.push_back("classification_n_classes", std::move(classification_n_classes));
        jcon.push_back("classification_positive_class", std::move(classification_positive_class));
        jcon.push_back("classification_betas", std::move(classification_betas));
        jcon.push_back("suffix", std::move(suffix));

        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/fine-tunes",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            cpr::Body{ jcon.dump() },
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto FineTunes::CreateAsync(
        const std::string& training_file,
        std::optional<std::string> validation_file,
        std::optional<std::string> model_id,
        std::optional<uint8_t> n_epochs,
        std::optional<uint16_t> batch_size,
        std::optional<float> learning_rate_multiplier,
        std::optional<float> prompt_loss_weight,
        std::optional<bool> compute_classification_metrics,
        std::optional<uint16_t> classification_n_classes,
        std::optional<std::string> classification_positive_class,
        std::optional<std::vector<float>> classification_betas,
        std::optional<std::string> suffix
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::FineTunes::Create,
            this,
            training_file,
            validation_file,
            model_id,
            n_epochs,
            batch_size,
            learning_rate_multiplier,
            prompt_loss_weight,
            compute_classification_metrics,
            classification_n_classes,
            classification_positive_class,
            classification_betas,
            suffix
        );
    }

    auto FineTunes::List() const& noexcept -> Expected<Response> {
        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/fine-tunes",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto FineTunes::ListAsync() const& noexcept -> FutureExpected<Response> {
        return std::async(std::launch::async, &liboai::FineTunes::List, this);
    }

    auto FineTunes::Retrieve(const std::string& fine_tune_id) const& noexcept
        -> Expected<Response> {
        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/fine-tunes/" + fine_tune_id,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto FineTunes::RetrieveAsync(const std::string& fine_tune_id) const& noexcept
        -> FutureExpected<Response> {
        return std::async(std::launch::async, &liboai::FineTunes::Retrieve, this, fine_tune_id);
    }

    auto FineTunes::Cancel(const std::string& fine_tune_id) const& noexcept -> Expected<Response> {
        return this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/fine-tunes/" + fine_tune_id + "/cancel",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto FineTunes::CancelAsync(const std::string& fine_tune_id) const& noexcept
        -> FutureExpected<Response> {
        return std::async(std::launch::async, &liboai::FineTunes::Cancel, this, fine_tune_id);
    }

    auto FineTunes::ListEvents(
        const std::string& fine_tune_id,
        std::optional<StreamCallback> stream
    ) const& noexcept -> Expected<Response> {
        cpr::Parameters params;
        stream ? params.Add({ "stream", "true" }) : void();

        return this->Request(
            Method::HTTP_GET,
            this->GetOpenAIRoot(),
            "/fine-tunes/" + fine_tune_id + "/events",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            std::move(params),
            stream ? cpr::WriteCallback{ [cb = std::move(stream.value())](
                                             std::string_view data,
                                             intptr_t userdata
                                         ) -> bool { return cb(std::string(data), userdata); } } :
                     cpr::WriteCallback{},
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto FineTunes::ListEventsAsync(
        const std::string& fine_tune_id,
        std::optional<StreamCallback> stream
    ) const& noexcept -> FutureExpected<Response> {
        return std::async(
            std::launch::async,
            &liboai::FineTunes::ListEvents,
            this,
            fine_tune_id,
            stream
        );
    }

    auto FineTunes::Remove(const std::string& model) const& noexcept -> Expected<Response> {
        return this->Request(
            Method::HTTP_DELETE,
            this->GetOpenAIRoot(),
            "/models/" + model,
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );
    }

    auto FineTunes::RemoveAsync(const std::string& model) const& noexcept
        -> FutureExpected<Response> {
        return std::async(std::launch::async, &liboai::FineTunes::Remove, this, model);
    }

} // namespace liboai
