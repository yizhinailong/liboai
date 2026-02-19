/**
    @file liboai.cppm
    Copyright (c) 2012-2022 Johnny (pseud. Dread) and others

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    liboai.cppm : main library module interface.
        This module provides an interface to all component classes
        in the library. It is the only module that needs to be
        imported in order to use the library.
*/

module;

#include <memory>
#include <string>

export module liboai;

// Core partitions
export import :core.error;
export import :core.response;
export import :core.network;
export import :core.authorization;

// Component partitions
export import :components.audio;
export import :components.azure;
export import :components.chat;
export import :components.completions;
export import :components.edits;
export import :components.embeddings;
export import :components.files;
export import :components.fine_tunes;
export import :components.images;
export import :components.models;
export import :components.moderations;

export namespace liboai {
    class OpenAI {
    public:
        explicit OpenAI(const std::string& root = "https://api.openai.com/v1")
            : Audio(std::make_unique<liboai::Audio>(root)),
              Azure(std::make_unique<liboai::Azure>(root)),
              ChatCompletion(std::make_unique<liboai::ChatCompletion>(root)),
              Completion(std::make_unique<liboai::Completions>(root)),
              Edit(std::make_unique<liboai::Edits>(root)),
              Embedding(std::make_unique<liboai::Embeddings>(root)),
              File(std::make_unique<liboai::Files>(root)),
              FineTune(std::make_unique<liboai::FineTunes>(root)),
              Image(std::make_unique<liboai::Images>(root)),
              Model(std::make_unique<liboai::Models>(root)),
              Moderation(std::make_unique<liboai::Moderations>(root)) {}

        OpenAI(OpenAI const&) = delete;
        OpenAI(OpenAI&&) = delete;
        OpenAI& operator=(OpenAI const&) = delete;
        OpenAI& operator=(OpenAI&&) = delete;
        ~OpenAI() = default;

        std::unique_ptr<liboai::Audio> Audio;
        std::unique_ptr<liboai::Azure> Azure;
        std::unique_ptr<liboai::ChatCompletion> ChatCompletion;
        std::unique_ptr<liboai::Completions> Completion;
        std::unique_ptr<liboai::Edits> Edit;
        std::unique_ptr<liboai::Embeddings> Embedding;
        std::unique_ptr<liboai::Files> File;
        std::unique_ptr<liboai::FineTunes> FineTune;
        std::unique_ptr<liboai::Images> Image;
        std::unique_ptr<liboai::Models> Model;
        std::unique_ptr<liboai::Moderations> Moderation;

        Authorization& auth = Authorization::Authorizer();
    };
} // namespace liboai
