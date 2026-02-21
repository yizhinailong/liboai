-- Example programs entry point
-- All example targets are defined here

function example_target(name, source)
    target(name, function()
        set_kind("binary")
        set_default(false)
        set_languages("c++23")
        add_files(source)
        add_packages("nlohmann_json", "cpr")
        add_deps("oai")
    end)
end

-- Audio examples
example_target("audio_create_speech", "audio/examples/create_speech.cpp")
example_target("audio_create_speech_async", "audio/examples/create_speech_async.cpp")
example_target("audio_create_transcription", "audio/examples/create_transcription.cpp")
example_target("audio_create_transcription_async", "audio/examples/create_transcription_async.cpp")
example_target("audio_create_translation", "audio/examples/create_translation.cpp")
example_target("audio_create_translation_async", "audio/examples/create_translation_async.cpp")

-- Authorization examples
example_target("authorization_set_azure_key", "authorization/examples/set_azure_key.cpp")
example_target("authorization_set_azure_key_env", "authorization/examples/set_azure_key_env.cpp")
example_target("authorization_set_azure_key_file", "authorization/examples/set_azure_key_file.cpp")
example_target("authorization_set_key", "authorization/examples/set_key.cpp")
example_target("authorization_set_key_env_var", "authorization/examples/set_key_env_var.cpp")
example_target("authorization_set_key_file", "authorization/examples/set_key_file.cpp")
example_target("authorization_set_organization", "authorization/examples/set_organization.cpp")
example_target("authorization_set_organization_env_var", "authorization/examples/set_organization_env_var.cpp")
example_target("authorization_set_organization_file", "authorization/examples/set_organization_file.cpp")
example_target("authorization_set_proxies", "authorization/examples/set_proxies.cpp")
example_target("authorization_set_proxy_auth", "authorization/examples/set_proxy_auth.cpp")

-- Azure examples
example_target("azure_create_chat_completion", "azure/examples/create_chat_completion.cpp")
example_target("azure_create_chat_completion_async", "azure/examples/create_chat_completion_async.cpp")
example_target("azure_create_completion", "azure/examples/create_completion.cpp")
example_target("azure_create_completion_async", "azure/examples/create_completion_async.cpp")
example_target("azure_create_embedding", "azure/examples/create_embedding.cpp")
example_target("azure_create_embedding_async", "azure/examples/create_embedding_async.cpp")
example_target("azure_delete_generated_image", "azure/examples/delete_generated_image.cpp")
example_target("azure_delete_generated_image_async", "azure/examples/delete_generated_image_async.cpp")
example_target("azure_get_generated_image", "azure/examples/get_generated_image.cpp")
example_target("azure_get_generated_image_async", "azure/examples/get_generated_image_async.cpp")
example_target("azure_request_image_generation", "azure/examples/request_image_generation.cpp")
example_target("azure_request_image_generation_async", "azure/examples/request_image_generation_async.cpp")

-- Chat examples
example_target("chat_create_chat_completion", "chat/examples/create_chat_completion.cpp")
example_target("chat_create_chat_completion_async", "chat/examples/create_chat_completion_async.cpp")
example_target("chat_ongoing_user_convo", "chat/examples/ongoing_user_convo.cpp")
example_target("chat_conversation_adduserdata", "chat/conversation/examples/adduserdata.cpp")
example_target("chat_conversation_getjsonobject", "chat/conversation/examples/getjsonobject.cpp")
example_target("chat_conversation_getlastresponse", "chat/conversation/examples/getlastresponse.cpp")
example_target("chat_conversation_getrawconversation", "chat/conversation/examples/getrawconversation.cpp")
example_target("chat_conversation_poplastresponse", "chat/conversation/examples/poplastresponse.cpp")
example_target("chat_conversation_popsystemdata", "chat/conversation/examples/popsystemdata.cpp")
example_target("chat_conversation_popuserdata", "chat/conversation/examples/popuserdata.cpp")
example_target("chat_conversation_setsystemdata", "chat/conversation/examples/setsystemdata.cpp")
example_target("chat_conversation_update", "chat/conversation/examples/update.cpp")

-- Completions examples
example_target("completions_generate_completion", "completions/examples/generate_completion.cpp")
example_target("completions_generate_completion_async", "completions/examples/generate_completion_async.cpp")

-- Edits examples
example_target("edits_create_edit", "edits/examples/create_edit.cpp")
example_target("edits_create_edit_async", "edits/examples/create_edit_async.cpp")

-- Embeddings examples
example_target("embeddings_create_embedding", "embeddings/examples/create_embedding.cpp")
example_target("embeddings_create_embedding_async", "embeddings/examples/create_embedding_async.cpp")

-- Files examples
example_target("files_delete_file", "files/examples/delete_file.cpp")
example_target("files_delete_file_async", "files/examples/delete_file_async.cpp")
example_target("files_download_uploaded_file", "files/examples/download_uploaded_file.cpp")
example_target("files_download_uploaded_file_async", "files/examples/download_uploaded_file_async.cpp")
example_target("files_list_files", "files/examples/list_files.cpp")
example_target("files_list_files_async", "files/examples/list_files_async.cpp")
example_target("files_retrieve_file", "files/examples/retrieve_file.cpp")
example_target("files_retrieve_file_async", "files/examples/retrieve_file_async.cpp")
example_target("files_upload_file", "files/examples/upload_file.cpp")
example_target("files_upload_file_async", "files/examples/upload_file_async.cpp")

-- Fine-tunes examples
example_target("fine_tunes_cancel_fine_tune", "fine-tunes/examples/cancel_fine_tune.cpp")
example_target("fine_tunes_cancel_fine_tune_async", "fine-tunes/examples/cancel_fine_tune_async.cpp")
example_target("fine_tunes_create_fine_tune", "fine-tunes/examples/create_fine_tune.cpp")
example_target("fine_tunes_create_fine_tune_async", "fine-tunes/examples/create_fine_tune_async.cpp")
example_target("fine_tunes_delete_fine_tune_model", "fine-tunes/examples/delete_fine_tune_model.cpp")
example_target("fine_tunes_delete_fine_tune_model_async", "fine-tunes/examples/delete_fine_tune_model_async.cpp")
example_target("fine_tunes_list_fine_tune_events", "fine-tunes/examples/list_fine_tune_events.cpp")
example_target("fine_tunes_list_fine_tune_events_async", "fine-tunes/examples/list_fine_tune_events_async.cpp")
example_target("fine_tunes_list_fine_tunes", "fine-tunes/examples/list_fine_tunes.cpp")
example_target("fine_tunes_list_fine_tunes_async", "fine-tunes/examples/list_fine_tunes_async.cpp")
example_target("fine_tunes_retrieve_fine_tune", "fine-tunes/examples/retrieve_fine_tune.cpp")
example_target("fine_tunes_retrieve_fine_tune_async", "fine-tunes/examples/retrieve_fine_tune_async.cpp")

-- Images examples
example_target("images_download_generated_image", "images/examples/download_generated_image.cpp")
example_target("images_generate_edit", "images/examples/generate_edit.cpp")
example_target("images_generate_edit_async", "images/examples/generate_edit_async.cpp")
example_target("images_generate_image", "images/examples/generate_image.cpp")
example_target("images_generate_image_async", "images/examples/generate_image_async.cpp")
example_target("images_generate_variation", "images/examples/generate_variation.cpp")
example_target("images_generate_variation_async", "images/examples/generate_variation_async.cpp")

-- Models examples
example_target("models_list_models", "models/examples/list_models.cpp")
example_target("models_list_models_async", "models/examples/list_models_async.cpp")
example_target("models_retrieve_model", "models/examples/retrieve_model.cpp")
example_target("models_retrieve_model_async", "models/examples/retrieve_model_async.cpp")

-- Moderations examples
example_target("moderations_create_moderation", "moderations/examples/create_moderation.cpp")
example_target("moderations_create_moderation_async", "moderations/examples/create_moderation_async.cpp")
