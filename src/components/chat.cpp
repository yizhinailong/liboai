#include "liboai/components/chat.hpp"

namespace liboai {

    Conversation::Conversation() {
        this->m_conversation["messages"] = nlohmann::json::array();
    }

    Conversation::Conversation(const Conversation& other) {
        this->m_conversation = other.m_conversation;
        this->m_functions = other.m_functions;
        this->m_last_resp_is_fc = other.m_last_resp_is_fc;
    }

    Conversation::Conversation(Conversation&& old) noexcept {
        this->m_conversation = std::move(old.m_conversation);
        this->m_functions = std::move(old.m_functions);
        this->m_last_resp_is_fc = old.m_last_resp_is_fc;

        old.m_conversation = nlohmann::json::object();
        old.m_functions = nlohmann::json::object();
    }

    Conversation::Conversation(std::string_view system_data) {
        this->m_conversation["messages"] = nlohmann::json::array();
        auto result = this->SetSystemData(system_data);
    }

    Conversation::Conversation(std::string_view system_data, std::string_view user_data) {
        this->m_conversation["messages"] = nlohmann::json::array();
        auto result = this->SetSystemData(system_data);
        result = this->AddUserData(user_data);
    }

    Conversation::Conversation(
        std::string_view system_data,
        std::initializer_list<std::string_view> user_data
    ) {
        this->m_conversation["messages"] = nlohmann::json::array();
        auto result = this->SetSystemData(system_data);

        for (auto& data : user_data) {
            auto result = this->AddUserData(data);
        }
    }

    Conversation::Conversation(std::initializer_list<std::string_view> user_data) {
        this->m_conversation["messages"] = nlohmann::json::array();

        for (auto& data : user_data) {
            auto result = this->AddUserData(data);
        }
    }

    Conversation::Conversation(const std::vector<std::string>& user_data) {
        this->m_conversation["messages"] = nlohmann::json::array();

        for (auto& data : user_data) {
            auto result = this->AddUserData(data);
        }
    }

    auto Conversation::operator=(const Conversation& other) -> Conversation& {
        this->m_conversation = other.m_conversation;
        this->m_functions = other.m_functions;
        this->m_last_resp_is_fc = other.m_last_resp_is_fc;
        return *this;
    }

    auto Conversation::operator=(Conversation&& old) noexcept -> Conversation& {
        this->m_conversation = std::move(old.m_conversation);
        this->m_functions = std::move(old.m_functions);
        this->m_last_resp_is_fc = old.m_last_resp_is_fc;

        old.m_conversation = nlohmann::json::object();
        old.m_functions = nlohmann::json::object();

        return *this;
    }

    auto Conversation::ChangeFirstSystemMessage(std::string_view new_data) & noexcept(false)
        -> bool {
        if (!new_data.empty() && !this->m_conversation["messages"].empty()) {
            if (this->m_conversation["messages"][0]["role"].get<std::string>() == "system") {
                this->m_conversation["messages"][0]["content"] = new_data;
                return true; // System message changed successfuly
            }
            return false;    // First message is not a system message
        }
        return false;        // New data is empty or conversation is empty
    }

    auto Conversation::SetSystemData(std::string_view data) & noexcept(false) -> bool {
        // if data provided is non-empty
        if (!data.empty()) {
            // if system is not set already - only one system message shall exist in any
            // conversation
            for (auto& message : this->m_conversation["messages"].items()) {
                if (message.value()["role"].get<std::string>() == "system") {
                    return false; // system already set
                }
            }
            this->m_conversation["messages"].push_back(
                {
                    {    "role", "system" },
                    { "content",     data }
            }
            );
            return true; // system set successfully
        }
        return false;    // data is empty
    }

    auto Conversation::PopSystemData() & noexcept(false) -> bool {
        // if conversation is non-empty
        if (!this->m_conversation["messages"].empty()) {
            // if first message is system
            if (this->m_conversation["messages"][0]["role"].get<std::string>() == "system") {
                this->m_conversation["messages"].erase(0);
                return true; // system message popped successfully
            }
            return false;    // first message is not system
        }
        return false;        // conversation is empty
    }

    auto Conversation::EraseExtra() -> void {
        if (m_conversation["messages"].size() > m_max_history_size) {
            // Ensure the system message is preserved
            auto first_msg = m_conversation["messages"].begin();
            if (first_msg != m_conversation["messages"].end() &&
                (*first_msg)["role"].get<std::string>() == "system") {
                m_conversation["messages"].erase(first_msg + 1);
            } else {
                m_conversation["messages"].erase(first_msg);
            }
        }
    }

    auto Conversation::AddUserData(std::string_view data) & noexcept(false) -> bool {
        // if data provided is non-empty
        if (!data.empty()) {
            EraseExtra();
            this->m_conversation["messages"].push_back(
                {
                    {    "role", "user" },
                    { "content",   data }
            }
            );
            return true; // user data added successfully
        }
        return false;    // data is empty
    }

    auto Conversation::AddUserData(std::string_view data, std::string_view name) & noexcept(false)
        -> bool {
        // if data provided is non-empty
        if (!data.empty()) {
            EraseExtra();
            this->m_conversation["messages"].push_back(
                {
                    {    "role", "user" },
                    { "content",   data },
                    {    "name",   name }
            }
            );
            return true; // user data added successfully
        }
        return false;    // data is empty
    }

    auto Conversation::PopUserData() & noexcept(false) -> bool {
        // if conversation is not empty
        if (!this->m_conversation["messages"].empty()) {
            // if last message is user message
            if (this->m_conversation["messages"].back()["role"].get<std::string>() == "user") {
                this->m_conversation["messages"].erase(this->m_conversation["messages"].end() - 1);
                return true; // user data popped successfully
            }
            return false;    // last message is not user message
        }
        return false;        // conversation is empty
    }

    auto Conversation::GetLastResponse() const& noexcept -> std::string {
        // if conversation is not empty
        if (!this->m_conversation["messages"].empty()) {
            // if last message is from assistant
            if (this->m_conversation["messages"].back()["role"].get<std::string>() == "assistant") {
                return this->m_conversation["messages"].back()["content"].get<std::string>();
            }
        }
        return ""; // no response found
    }

    auto Conversation::LastResponseIsFunctionCall() const& noexcept -> bool {
        return this->m_last_resp_is_fc;
    }

    auto Conversation::GetLastFunctionCallName() const& noexcept(false) -> std::string {
        if (this->m_conversation.contains("function_call")) {
            if (this->m_conversation["function_call"].contains("name")) {
                return this->m_conversation["function_call"]["name"].get<std::string>();
            }
        }

        return "";
    }

    auto Conversation::GetLastFunctionCallArguments() const& noexcept(false) -> std::string {
        if (this->m_conversation.contains("function_call")) {
            if (this->m_conversation["function_call"].contains("arguments")) {
                return this->m_conversation["function_call"]["arguments"].get<std::string>();
            }
        }

        return "";
    }

    auto Conversation::PopLastResponse() & noexcept(false) -> bool {
        // if conversation is not empty
        if (!this->m_conversation["messages"].empty()) {
            // if last message is assistant message
            if (this->m_conversation["messages"].back()["role"].get<std::string>() == "assistant") {
                this->m_conversation["messages"].erase(this->m_conversation["messages"].end() - 1);
                return true; // assistant data popped successfully
            }
            return false;    // last message is not assistant message
        }
        return false;        // conversation is empty
    }

    auto Conversation::Update(std::string_view response) & noexcept(false) -> bool {
        // reset "last response is function call" flag
        if (this->m_last_resp_is_fc) {
            if (this->m_conversation.contains("function_call")) {
                this->m_conversation.erase("function_call");
            }
            this->m_last_resp_is_fc = false;
        }

        // if response is non-empty
        if (!response.empty()) {
            nlohmann::json j = nlohmann::json::parse(response);
            if (j.contains("choices")) { // top level, several messages
                for (auto& choice : j["choices"].items()) {
                    if (choice.value().contains("message")) {
                        if (choice.value()["message"].contains("role") &&
                            choice.value()["message"].contains("content")) {
                            if (!choice.value()["message"]["content"].is_null()) {
                                EraseExtra();
                                this->m_conversation["messages"].push_back(
                                    {
                                        {    "role",    choice.value()["message"]["role"] },
                                        { "content", choice.value()["message"]["content"] }
                                }
                                );
                            } else {
                                EraseExtra();
                                this->m_conversation["messages"].push_back(
                                    {
                                        {    "role", choice.value()["message"]["role"] },
                                        { "content",                                "" }
                                }
                                );
                            }

                            if (choice.value()["message"].contains("function_call")) {
                                // if a function_call is present in the response, the
                                // conversation is not updated as there is no assistant
                                // response to be added. However, we do add the function
                                // information

                                this->m_conversation["function_call"] = nlohmann::json::object();
                                if (choice.value()["message"]["function_call"].contains("name")) {
                                    this->m_conversation["function_call"]["name"] =
                                        choice.value()["message"]["function_call"]["name"];
                                }
                                if (choice.value()["message"]["function_call"].contains(
                                        "arguments"
                                    )) {
                                    this->m_conversation["function_call"]["arguments"] =
                                        choice.value()["message"]["function_call"]["arguments"];
                                }

                                this->m_last_resp_is_fc = true;
                            }

                            return true;  // conversation updated successfully
                        } else {
                            return false; // response is not valid
                        }
                    } else {
                        return false; // no response found
                    }
                }
            } else if (j.contains("message")) { // mid level, single message
                if (j["message"].contains("role") && j["message"].contains("content")) {
                    if (j["message"]["content"].is_null()) {
                        EraseExtra();
                        this->m_conversation["messages"].push_back(
                            {
                                {    "role",    j["message"]["role"] },
                                { "content", j["message"]["content"] }
                        }
                        );
                    } else {
                        EraseExtra();
                        this->m_conversation["messages"].push_back(
                            {
                                {    "role", j["message"]["role"] },
                                { "content",                   "" }
                        }
                        );
                    }

                    if (j["message"].contains("function_call")) {
                        // if a function_call is present in the response, the
                        // conversation is not updated as there is no assistant
                        // response to be added. However, we do add the function
                        // information

                        this->m_conversation["function_call"] = nlohmann::json::object();
                        if (j["message"]["function_call"].contains("name")) {
                            this->m_conversation["function_call"]["name"] =
                                j["message"]["function_call"]["name"];
                        }
                        if (j["message"]["function_call"].contains("arguments")) {
                            this->m_conversation["function_call"]["arguments"] =
                                j["message"]["function_call"]["arguments"];
                        }

                        this->m_last_resp_is_fc = true;
                    }

                    return true;  // conversation updated successfully
                } else {
                    return false; // response is not valid
                }
            } else if (j.contains("role") && j.contains("content")) { // low level, single message
                if (j["message"]["content"].is_null()) {
                    EraseExtra();
                    this->m_conversation["messages"].push_back(
                        {
                            {    "role",    j["message"]["role"] },
                            { "content", j["message"]["content"] }
                    }
                    );
                } else {
                    EraseExtra();
                    this->m_conversation["messages"].push_back(
                        {
                            {    "role", j["message"]["role"] },
                            { "content",                   "" }
                    }
                    );
                }

                if (j["message"].contains("function_call")) {
                    // if a function_call is present in the response, the
                    // conversation is not updated as there is no assistant
                    // response to be added. However, we do add the function
                    // information
                    this->m_conversation["function_call"] = nlohmann::json::object();
                    if (j["message"]["function_call"].contains("name")) {
                        this->m_conversation["function_call"]["name"] =
                            j["message"]["function_call"]["name"];
                    }
                    if (j["message"]["function_call"].contains("arguments")) {
                        this->m_conversation["function_call"]["arguments"] =
                            j["message"]["function_call"]["arguments"];
                    }

                    this->m_last_resp_is_fc = true;
                }

                return true;  // conversation updated successfully
            } else {
                return false; // invalid response
            }
        }
        return false; // response is empty
    }

    auto Conversation::Update(const Response& response) & noexcept(false) -> bool {
        return this->Update(response.content);
    }

    auto Conversation::Export() const& noexcept(false) -> std::string {
        nlohmann::json j;

        if (!this->m_conversation.empty()) {
            j["messages"] = this->m_conversation["messages"];

            if (this->m_functions) {
                j["functions"] = this->m_functions.value()["functions"];
            }

            return j.dump(4); // conversation exported successfully
        }

        return ""; // conversation is empty
    }

    auto Conversation::Import(std::string_view json) & noexcept(false) -> bool {
        if (!json.empty()) {
            nlohmann::json j = nlohmann::json::parse(json);

            if (j.contains("messages")) {
                this->m_conversation["messages"] = j["messages"];

                if (j.contains("functions")) {
                    this->m_functions = nlohmann::json();
                    this->m_functions.value()["functions"] = j["functions"];
                }

                return true; // conversation imported successfully
            }

            return false; // no messages found
        }

        return false; // json is empty
    }

    auto Conversation::AppendStreamData(std::string data) & noexcept(false) -> bool {
        if (!data.empty()) {
            std::string delta;
            bool completed = false;
            return this->ParseStreamData(data, delta, completed);
        }

        return false; // data is empty
    }

    auto Conversation::AppendStreamData(
        std::string data,
        std::string& delta,
        bool& completed
    ) & noexcept(false) -> bool {
        if (!data.empty()) {
            return this->ParseStreamData(data, delta, completed);
        }

        return false;
    }

    auto Conversation::SetFunctions(Functions functions) & noexcept(false) -> bool {
        nlohmann::json j = functions.GetJSON();

        if (!j.empty() && j.contains("functions") && j["functions"].size() > 0) {
            this->m_functions = std::move(j);
            return true; // functions set successfully
        }

        return false; // functions are empty
    }

    auto Conversation::PopFunctions() & noexcept(false) -> void {
        this->m_functions = std::nullopt;
    }

    auto Conversation::GetRawConversation() const& noexcept -> std::string {
        return this->m_conversation.dump(4);
    }

    auto Conversation::GetJSON() const& noexcept -> const nlohmann::json& {
        return this->m_conversation;
    }

    auto Conversation::GetRawFunctions() const& noexcept -> std::string {
        return this->HasFunctions() ? this->m_functions.value().dump(4) : "";
    }

    auto Conversation::GetFunctionsJSON() const& noexcept -> const nlohmann::json& {
        return this->m_functions.value();
    }

    auto Conversation::SplitStreamedData(std::string data) const noexcept(false)
        -> std::vector<std::string> {
        // remove all instances of the string "data: " from the string
        this->RemoveStrings(data, "data: ");

        /*
            Splits the streamed data into a vector of strings
            via delimiter of two newlines.

            For instance, a string of "Hello\n\nWorld" would
            be split into a vector of {"Hello", "World"}, and
            a string of "Hello World" would be split into
            a vector of {"Hello World"}.
        */
        if (!data.empty()) {
            std::vector<std::string> split_data;
            std::string temp;
            std::istringstream iss(data);
            while (std::getline(iss, temp)) {
                if (temp.empty()) {
                    split_data.push_back(temp);
                } else {
                    split_data.push_back(temp);
                }
            }

            // remove empty strings from the vector
            split_data.erase(
                std::remove_if(
                    split_data.begin(),
                    split_data.end(),
                    [](const std::string& s) { return s.empty(); }
                ),
                split_data.end()
            );

            return split_data;
        }

        return {};
    }

    auto Conversation::RemoveStrings(std::string& s, std::string_view p) const noexcept(false)
        -> void {
        std::string::size_type i = s.find(p);
        while (i != std::string::npos) {
            s.erase(i, p.length());
            i = s.find(p, i);
        }
    }

    auto Conversation::SplitFullStreamedData(std::string data) const noexcept(false)
        -> std::vector<std::string> {
        if (data.empty()) {
            return {};
        }

        std::vector<std::string> split_data;
        std::string temp;
        std::istringstream iss(data);
        while (std::getline(iss, temp)) {
            if (temp.empty()) {
                split_data.push_back(temp);
            } else {
                split_data.push_back(temp);
            }
        }

        // remove empty strings from the vector
        split_data.erase(
            std::remove_if(
                split_data.begin(),
                split_data.end(),
                [](const std::string& s) { return s.empty(); }
            ),
            split_data.end()
        );

        return split_data;
    }

    auto
    Conversation::ParseStreamData(std::string data, std::string& delta_content, bool& completed)
        -> bool {
        if (!m_last_incomplete_buffer.empty()) {
            data = m_last_incomplete_buffer + data;
            m_last_incomplete_buffer.clear();
        }

        std::vector<std::string> data_lines = SplitFullStreamedData(data);

        if (data_lines.empty()) {
            return false;
        }

        // create an empty message at the end of the conversation,
        // marked as "pending" to indicate that the response is
        // still being processed. This flag will be removed once
        // the response is processed. If the marking already
        // exists, keep appending to the same message.
        if (this->m_conversation["messages"].empty() ||
            !this->m_conversation["messages"].back().contains("pending")) {
            this->m_conversation["messages"].push_back(
                {
                    {    "role",   "" },
                    { "content",   "" },
                    { "pending", true }
            }
            );
        }

        for (auto& line : data_lines) {
            if (line.find("data: [DONE]") == std::string::npos) {
                /*
                    j should have content in the form of:
                        {"id":"chatcmpl-7SKOck29emvbBbDS6cHg5xwnRrsLO","object":"chat.completion.chunk","created":1686985942,"model":"gpt-3.5-turbo-0613","choices":[{"index":0,"delta":{"content":"."},"finish_reason":null}]}
                    where "delta" may be empty
                */
                this->RemoveStrings(line, "data: ");

                nlohmann::json j;
                try {
                    j = nlohmann::json::parse(line);
                } catch (const std::exception& e) {
                    m_last_incomplete_buffer = line;
                    continue;
                }

                if (j.contains("choices")) {
                    if (j["choices"][0].contains("delta")) {
                        if (!j["choices"][0]["delta"].empty() &&
                            !j["choices"][0]["delta"].is_null()) {
                            if (j["choices"][0]["delta"].contains("role")) {
                                this->m_conversation["messages"].back()["role"] =
                                    j["choices"][0]["delta"]["role"];
                            }

                            if (j["choices"][0]["delta"].contains("content")) {
                                if (!j["choices"][0]["delta"]["content"].empty() &&
                                    !j["choices"][0]["delta"]["content"].is_null()) {
                                    std::string stream_content =
                                        j["choices"][0]["delta"]["content"].get<std::string>();
                                    this->m_conversation["messages"].back()["content"] =
                                        this->m_conversation["messages"]
                                            .back()["content"]
                                            .get<std::string>() +
                                        stream_content;
                                    delta_content += stream_content;
                                }

                                // function calls do not have a content field,
                                // set m_last_resp_is_fc to false and remove any
                                // previously set function_call field in the
                                // conversation
                                if (this->m_last_resp_is_fc) {
                                    if (this->m_conversation.contains("function_call")) {
                                        this->m_conversation.erase("function_call");
                                    }
                                    this->m_last_resp_is_fc = false;
                                }
                            }

                            if (j["choices"][0]["delta"].contains("function_call")) {
                                if (!j["choices"][0]["delta"]["function_call"].empty() &&
                                    !j["choices"][0]["delta"]["function_call"].is_null()) {
                                    if (j["choices"][0]["delta"]["function_call"].contains(
                                            "name"
                                        )) {
                                        if (!j["choices"][0]["delta"]["function_call"]["name"]
                                                 .empty() &&
                                            !j["choices"][0]["delta"]["function_call"]["name"]
                                                 .is_null()) {
                                            if (!this->m_conversation["messages"].back().contains(
                                                    "function_call"
                                                )) {
                                                this->m_conversation["function_call"] = {
                                                    { "name",
                                                     j["choices"][0]["delta"]["function_call"]
                                                       ["name"] }
                                                };
                                                this->m_last_resp_is_fc = true;
                                            }
                                        }
                                    } else if (j["choices"][0]["delta"]["function_call"].contains(
                                                   "arguments"
                                               )) {
                                        if (!j["choices"][0]["delta"]["function_call"]["arguments"]
                                                 .empty() &&
                                            !j["choices"][0]["delta"]["function_call"]["arguments"]
                                                 .is_null()) {
                                            if (!this->m_conversation["function_call"].contains(
                                                    "arguments"
                                                )) {
                                                this->m_conversation["function_call"].push_back(
                                                    { "arguments",
                                                      j["choices"][0]["delta"]["function_call"]
                                                       ["arguments"] }
                                                );
                                            } else {
                                                this->m_conversation["function_call"]["arguments"] =
                                                    this->m_conversation["function_call"]
                                                                        ["arguments"]
                                                                            .get<std::string>() +
                                                    j["choices"][0]["delta"]["function_call"]
                                                     ["arguments"]
                                                         .get<std::string>();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    return false; // no "choices" found - invalid
                }
            } else {
                // the response is complete, erase the "pending" flag
                this->m_conversation["messages"].back().erase("pending");
                completed = true;
            }
        }

        return true; // last message received
    }

    auto ChatCompletion::create(
        const std::string& model,
        Conversation& conversation,
        std::optional<std::string> function_call,
        std::optional<float> temperature,
        std::optional<float> top_p,
        std::optional<uint16_t> n,
        std::optional<ChatStreamCallback> stream,
        std::optional<std::vector<std::string>> stop,
        std::optional<uint16_t> max_tokens,
        std::optional<float> presence_penalty,
        std::optional<float> frequency_penalty,
        std::optional<std::unordered_map<std::string, int8_t>> logit_bias,
        std::optional<std::string> user
    ) const& noexcept(false) -> Response {
        JsonConstructor jcon;
        jcon.push_back("model", model);
        jcon.push_back("temperature", std::move(temperature));
        jcon.push_back("top_p", std::move(top_p));
        jcon.push_back("n", std::move(n));
        jcon.push_back("stop", std::move(stop));
        jcon.push_back("max_tokens", std::move(max_tokens));
        jcon.push_back("presence_penalty", std::move(presence_penalty));
        jcon.push_back("frequency_penalty", std::move(frequency_penalty));
        jcon.push_back("logit_bias", std::move(logit_bias));
        jcon.push_back("user", std::move(user));

        if (function_call) {
            if (function_call.value() == "none" || function_call.value() == "auto") {
                nlohmann::json j;
                j["function_call"] = function_call.value();
                jcon.push_back("function_call", j["function_call"]);
            } else {
                nlohmann::json j;
                j["function_call"] = {
                    { "name", function_call.value() }
                };
                jcon.push_back("function_call", j["function_call"]);
            }
        }

        StrippedStreamCallback _sscb = nullptr;
        if (stream) {
            _sscb = [stream, &conversation](std::string data, intptr_t userdata) -> bool {
                ChatStreamCallback _stream = stream.value();
                return _stream(data, userdata, conversation);
            };

            jcon.push_back("stream", _sscb);
        }

        if (conversation.GetJSON().contains("messages")) {
            jcon.push_back("messages", conversation.GetJSON()["messages"]);
        }

        if (conversation.HasFunctions()) {
            jcon.push_back("functions", conversation.GetFunctionsJSON()["functions"]);
        }

        Response res;
        res = this->Request(
            Method::HTTP_POST,
            this->GetOpenAIRoot(),
            "/chat/completions",
            "application/json",
            this->m_auth.GetAuthorizationHeaders(),
            netimpl::components::Body{ jcon.dump() },
            _sscb ? netimpl::components::WriteCallback{ std::move(_sscb) } :
                    netimpl::components::WriteCallback{},
            this->m_auth.GetProxies(),
            this->m_auth.GetProxyAuth(),
            this->m_auth.GetMaxTimeout()
        );

        return res;
    }

    auto ChatCompletion::create_async(
        const std::string& model,
        Conversation& conversation,
        std::optional<std::string> function_call,
        std::optional<float> temperature,
        std::optional<float> top_p,
        std::optional<uint16_t> n,
        std::optional<ChatStreamCallback> stream,
        std::optional<std::vector<std::string>> stop,
        std::optional<uint16_t> max_tokens,
        std::optional<float> presence_penalty,
        std::optional<float> frequency_penalty,
        std::optional<std::unordered_map<std::string, int8_t>> logit_bias,
        std::optional<std::string> user
    ) const& noexcept(false) -> FutureResponse {
        return std::async(
            std::launch::async,
            &ChatCompletion::create,
            this,
            model,
            std::ref(conversation),
            function_call,
            temperature,
            top_p,
            n,
            stream,
            stop,
            max_tokens,
            presence_penalty,
            frequency_penalty,
            logit_bias,
            user
        );
    }

    auto operator<<(std::ostream& os, const Conversation& conv) -> std::ostream& {
        os << conv.GetRawConversation() << std::endl
           << (conv.HasFunctions() ? conv.GetRawFunctions() : "");

        return os;
    }

    Functions::Functions() {
        this->m_functions["functions"] = nlohmann::json::array();
    }

    Functions::Functions(const Functions& other) {
        this->m_functions = other.m_functions;
    }

    Functions::Functions(Functions&& old) noexcept {
        this->m_functions = std::move(old.m_functions);
        old.m_functions = nlohmann::json::object();
    }

    auto Functions::operator=(const Functions& other) -> Functions& {
        this->m_functions = other.m_functions;
        return *this;
    }

    auto Functions::operator=(Functions&& old) noexcept -> Functions& {
        this->m_functions = std::move(old.m_functions);
        old.m_functions = nlohmann::json::object();
        return *this;
    }

    auto Functions::AddFunction(std::string_view function_name) & noexcept(false) -> bool {
        if (this->GetFunctionIndex(function_name) == -1) {
            this->m_functions["functions"].push_back(
                {
                    { "name", function_name }
            }
            );
            return true; // function added
        }
        return false;    // function already exists
    }

    auto Functions::AddFunctions(std::initializer_list<std::string_view> function_names) & noexcept(
        false
    ) -> bool {
        if (function_names.size() > 0) {
            for (auto& function_name : function_names) {
                if (this->GetFunctionIndex(function_name) == -1) {
                    this->m_functions["functions"].push_back(
                        {
                            { "name", function_name }
                    }
                    );
                }
            }
            return true; // functions added
        }
        return false;    // functions not added (size 0)
    }

    auto Functions::AddFunctions(std::vector<std::string> function_names) & noexcept(false)
        -> bool {
        if (function_names.size() > 0) {
            for (auto& function_name : function_names) {
                if (this->GetFunctionIndex(function_name) == -1) {
                    this->m_functions["functions"].push_back(
                        {
                            { "name", std::move(function_name) }
                    }
                    );
                }
            }
            return true; // functions added
        }
        return false;    // functions not added (size 0)
    }

    auto Functions::PopFunction(std::string_view function_name) & noexcept(false) -> bool {
        auto index = this->GetFunctionIndex(function_name);

        if (index != -1) {
            this->m_functions["functions"].erase(this->m_functions["functions"].begin() + index);
            return true; // function removed
        }

        return false; // function not removed
    }

    auto Functions::PopFunctions(std::initializer_list<std::string_view> function_names) & noexcept(
        false
    ) -> bool {
        if (function_names.size() > 0) {
            for (auto& function_name : function_names) {
                auto index = this->GetFunctionIndex(function_name);

                if (index != -1) {
                    this->m_functions["functions"].erase(
                        this->m_functions["functions"].begin() + index
                    );
                }
            }

            return true; // functions removed
        }

        return false; // functions not removed (size 0)
    }

    auto Functions::PopFunctions(std::vector<std::string> function_names) & noexcept(false)
        -> bool {
        if (function_names.size() > 0) {
            for (auto& function_name : function_names) {
                auto index = this->GetFunctionIndex(function_name);

                if (index != -1) {
                    this->m_functions["functions"].erase(
                        this->m_functions["functions"].begin() + index
                    );
                }
            }
            return true; // functions removed
        }

        return false; // functions not removed (size 0)
    }

    auto Functions::SetDescription(
        std::string_view target,
        std::string_view description
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (!this->m_functions["functions"][i].contains("description")) {
                this->m_functions["functions"][i]["description"] = description;
                return true; // description set successfully
            }
            return false;    // already has a description
        }

        return false; // function does not exist
    }

    auto Functions::PopDescription(std::string_view target) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("description")) {
                this->m_functions["functions"][i].erase("description");
                return true; // description removed successfully
            }
            return false;    // does not have a description
        }

        return false; // function does not exist
    }

    auto Functions::SetRequired(
        std::string_view target,
        std::initializer_list<std::string_view> params
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1 && params.size() > 0) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                for (auto& parameter : params) {
                    this->m_functions["functions"][i]["parameters"]["required"] = std::move(params);
                    return true; // required parameters set successfully
                }
            }
        }

        return false; // required parameters not set
    }

    auto Functions::SetRequired(
        std::string_view target,
        std::vector<std::string> params
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1 && params.size() > 0) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                for (auto& parameter : params) {
                    this->m_functions["functions"][i]["parameters"]["required"] = std::move(params);
                    return true; // required parameters set successfully
                }
            }
        }

        return false; // required parameters not set
    }

    auto Functions::PopRequired(std::string_view target) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                if (this->m_functions["functions"][i]["parameters"].contains("required")) {
                    this->m_functions["functions"][i]["parameters"].erase("required");
                    return true; // required parameters removed successfully
                }
            }
        }

        return false; // required parameters not removed
    }

    auto
    Functions::AppendRequired(std::string_view target, std::string_view param) & noexcept(false)
        -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                if (this->m_functions["functions"][i]["parameters"].contains("required")) {
                    this->m_functions["functions"][i]["parameters"]["required"].push_back(param);
                    return true; // required parameter appended successfully
                }
            }
        }

        return false; // required parameter not appended
    }

    auto Functions::AppendRequired(
        std::string_view target,
        std::initializer_list<std::string_view> params
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1 && params.size() > 0) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                if (this->m_functions["functions"][i]["parameters"].contains("required")) {
                    for (auto& param : params) {
                        this->m_functions["functions"][i]["parameters"]["required"].push_back(
                            param
                        );
                    }

                    return true; // required parameters appended successfully
                }
            }
        }

        return false; // required parameters not appended
    }

    auto Functions::AppendRequired(
        std::string_view target,
        std::vector<std::string> params
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1 && params.size() > 0) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                if (this->m_functions["functions"][i]["parameters"].contains("required")) {
                    for (auto& param : params) {
                        this->m_functions["functions"][i]["parameters"]["required"].push_back(
                            std::move(param)
                        );
                    }

                    return true; // required parameters appended successfully
                }
            }
        }

        return false; // required parameters not appended
    }

    auto
    Functions::SetParameter(std::string_view target, FunctionParameter parameter) & noexcept(false)
        -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (!this->m_functions["functions"][i].contains("parameters")) {
                this->m_functions["functions"][i]["parameters"] = nlohmann::json::object();
                this->m_functions["functions"][i]["parameters"]["properties"] =
                    nlohmann::json::object();
                this->m_functions["functions"][i]["parameters"]["type"] = "object";

                this->m_functions["functions"][i]["parameters"]["properties"].push_back(
                    {
                        parameter.name,
                        { { "type", std::move(parameter.type) },
                               { "description", std::move(parameter.description) } }
                }
                );

                if (parameter.enumeration) {
                    this->m_functions["functions"][i]["parameters"]["properties"][parameter.name]
                                     ["enum"] = std::move(parameter.enumeration.value());
                }

                return true; // parameter set successfully
            }
        }

        return false; // function non-existent, or parameters already set (use AppendParameter(s))
    }

    auto Functions::SetParameters(
        std::string_view target,
        std::initializer_list<FunctionParameter> parameters
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (!this->m_functions["functions"][i].contains("parameters") &&
                parameters.size() > 0) {
                this->m_functions["functions"][i]["parameters"] = nlohmann::json::object();
                this->m_functions["functions"][i]["parameters"]["properties"] =
                    nlohmann::json::object();
                this->m_functions["functions"][i]["parameters"]["type"] = "object";

                for (auto& parameter : parameters) {
                    if (!this->m_functions["functions"][i]["parameters"]["properties"].contains(
                            parameter.name
                        )) {
                        this->m_functions["functions"][i]["parameters"]["properties"].push_back(
                            {
                                parameter.name,
                                { { "type", parameter.type },
                                       { "description", parameter.description } }
                        }
                        );

                        if (parameter.enumeration) {
                            this->m_functions["functions"][i]["parameters"]["properties"]
                                             [parameter.name]["enum"] =
                                parameter.enumeration.value();
                        }
                    }
                }

                return true; // parameter set successfully
            }
        }

        return false; // function non-existent, or parameters already set (use AppendParameter(s))
    }

    auto Functions::SetParameters(
        std::string_view target,
        std::vector<FunctionParameter> parameters
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (!this->m_functions["functions"][i].contains("parameters") &&
                parameters.size() > 0) {
                this->m_functions["functions"][i]["parameters"] = nlohmann::json::object();
                this->m_functions["functions"][i]["parameters"]["properties"] =
                    nlohmann::json::object();
                this->m_functions["functions"][i]["parameters"]["type"] = "object";

                for (auto& parameter : parameters) {
                    if (!this->m_functions["functions"][i]["parameters"]["properties"].contains(
                            parameter.name
                        )) {
                        this->m_functions["functions"][i]["parameters"]["properties"].push_back(
                            {
                                parameter.name,
                                { { "type", std::move(parameter.type) },
                                       { "description", std::move(parameter.description) } }
                        }
                        );

                        if (parameter.enumeration) {
                            this->m_functions["functions"][i]["parameters"]["properties"]
                                             [parameter.name]["enum"] =
                                std::move(parameter.enumeration.value());
                        }
                    }
                }

                return true; // parameter set successfully
            }
        }

        return false; // function non-existent, or parameters already set (use AppendParameter(s))
    }

    auto Functions::PopParameters(std::string_view target) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                this->m_functions["functions"][i].erase("parameters");
                return true; // parameters removed successfully
            }
        }

        return false; // parameters not removed
    }

    auto Functions::PopParameters(
        std::string_view target,
        std::initializer_list<std::string_view> param_names
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                for (auto& param_name : param_names) {
                    if (this->m_functions["functions"][i]["parameters"]["properties"].contains(
                            param_name
                        )) {
                        this->m_functions["functions"][i]["parameters"]["properties"].erase(
                            param_name
                        );
                    }
                }

                return true; // parameters removed successfully
            }
        }

        return false; // parameters not removed
    }

    auto Functions::PopParameters(
        std::string_view target,
        std::vector<std::string> param_names
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                for (auto& param_name : param_names) {
                    if (this->m_functions["functions"][i]["parameters"]["properties"].contains(
                            param_name
                        )) {
                        this->m_functions["functions"][i]["parameters"]["properties"].erase(
                            param_name
                        );
                    }
                }

                return true; // parameters removed successfully
            }
        }

        return false; // parameters not removed
    }

    auto Functions::AppendParameter(
        std::string_view target,
        FunctionParameter parameter
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                if (!this->m_functions["functions"][i]["parameters"]["properties"].contains(
                        parameter.name
                    )) {
                    this->m_functions["functions"][i]["parameters"]["properties"].push_back(
                        {
                            parameter.name,
                            { { "type", std::move(parameter.type) },
                                   { "description", std::move(parameter.description) } }
                    }
                    );

                    if (parameter.enumeration) {
                        this->m_functions["functions"][i]["parameters"]["properties"]
                                         [parameter.name]["enum"] =
                            std::move(parameter.enumeration.value());
                    }

                    return true; // parameter appended successfully
                }
            }
        }

        return false; // parameter not appended
    }

    auto Functions::AppendParameters(
        std::string_view target,
        std::initializer_list<FunctionParameter> parameters
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                for (auto& parameter : parameters) {
                    if (!this->m_functions["functions"][i]["parameters"]["properties"].contains(
                            parameter.name
                        )) {
                        this->m_functions["functions"][i]["parameters"]["properties"].push_back(
                            {
                                parameter.name,
                                { { "type", parameter.type },
                                       { "description", parameter.description } }
                        }
                        );

                        if (parameter.enumeration) {
                            this->m_functions["functions"][i]["parameters"]["properties"]
                                             [parameter.name]["enum"] =
                                parameter.enumeration.value();
                        }
                    }
                }

                return true; // parameters appended successfully
            }
        }

        return false; // parameters not appended
    }

    auto Functions::AppendParameters(
        std::string_view target,
        std::vector<FunctionParameter> parameters
    ) & noexcept(false) -> bool {
        index i = this->GetFunctionIndex(target);

        if (i != -1) {
            if (this->m_functions["functions"][i].contains("parameters")) {
                for (auto& parameter : parameters) {
                    if (!this->m_functions["functions"][i]["parameters"]["properties"].contains(
                            parameter.name
                        )) {
                        this->m_functions["functions"][i]["parameters"]["properties"].push_back(
                            {
                                parameter.name,
                                { { "type", std::move(parameter.type) },
                                       { "description", std::move(parameter.description) } }
                        }
                        );

                        if (parameter.enumeration) {
                            this->m_functions["functions"][i]["parameters"]["properties"]
                                             [parameter.name]["enum"] =
                                std::move(parameter.enumeration.value());
                        }
                    }
                }

                return true; // parameters appended successfully
            }
        }

        return false; // parameters not appended
    }

    auto Functions::GetJSON() const& noexcept -> const nlohmann::json& {
        return this->m_functions;
    }

    auto Functions::GetFunctionIndex(std::string_view function_name) const& noexcept(false)
        -> Functions::index {
        index i = 0;

        if (!this->m_functions.empty()) {
            for (auto& [key, value] : this->m_functions["functions"].items()) {
                if (value.contains("name")) {
                    if (value["name"].get<std::string>() == function_name) {
                        return i;
                    }
                }
                i++;
            }
        }

        return -1;
    }
} // namespace liboai
