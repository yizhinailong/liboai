#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;

    // create a conversation
    Conversation convo;

    // holds next user input
    std::string input;

    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        while (true) {
            // get next user input
            std::cout << "You: ";
            std::getline(std::cin, input);

            // add user input to conversation
            convo.AddUserData(input);

            // get response from OpenAI
            auto response = oai.ChatCompletion->Create("gpt-3.5-turbo", convo);

            if (response) {
                // update our conversation with the response
                convo.Update(response.value());

                // print the response
                auto lastResponse = convo.GetLastResponse();
                if (lastResponse) {
                    std::cout << "Bot: " << lastResponse.value() << std::endl;
                }
            } else {
                std::cout << response.error().message << std::endl;
                break;
            }
        }
    }
}
