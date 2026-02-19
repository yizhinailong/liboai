import liboai;

using namespace liboai;

int main() {
    OpenAI oai;

    Conversation convo;
    convo.AddUserData("Hi, how are you?");

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        auto res =
            oai.Azure->CreateChatCompletion("resource", "deploymentID", "api_version", convo);

        if (res) {
            // update the conversation with the response
            convo.Update(res.value());

            // print the response from the API
            auto lastResponse = convo.GetLastResponse();
            if (lastResponse) {
                std::cout << lastResponse.value() << std::endl;
            }
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
