/**
 * @file fine_tunes.cpp
 *
 * Implementation of FineTunes component for model fine-tuning API.


 */
#include "liboai/components/fine_tunes.hpp"

#include "liboai/core/error.hpp"

namespace liboai {

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
        std::optional<std::function<bool(std::string, intptr_t)>> stream
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
        std::optional<std::function<bool(std::string, intptr_t)>> stream
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
