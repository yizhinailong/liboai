#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;

    Conversation convo;
    convo.AddUserData("Hi, how are you?");

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        // call async method; returns a future
        auto fut =
            oai.Azure
                ->CreateChatCompletionAsync("resource", "deploymentID", "api_version", convo);

        // do other work...

        // check if the future is ready
        fut.wait();

        // get the contained response
        auto res = fut.get();

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
