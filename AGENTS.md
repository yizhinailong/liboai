# PROJECT KNOWLEDGE BASE

**Generated:** 2026-02-16
**Commit:** b692b33
**Branch:** main

## OVERVIEW

Unofficial C++17 library for OpenAI API. Spiritual port of Python's `openai` library with similar structure. Single-header include design (`#include "liboai/liboai.hpp"`).

## STRUCTURE

```
include/liboai/
├── liboai.hpp              # Main entry - include only this
├── components/             # API endpoint wrappers (audio, chat, images, etc.)
└── core/                   # Infrastructure (auth, network, response, exception)
src/
├── components/             # Component implementations (.cpp)
└── core/                   # Core implementations (.cpp)
documentation/              # Examples per API feature
```

## WHERE TO LOOK

| Task | Location | Notes |
|------|----------|-------|
| Add new API endpoint | `include/liboai/components/` + `src/components/` | Create .hpp/.cpp pair, add to CMakeLists.txt, include in liboai.hpp |
| Modify auth behavior | `include/liboai/core/authorization.hpp` | Singleton pattern |
| Change network layer | `include/liboai/core/netimpl.hpp` | cURL wrapper, 47k lines |
| Response handling | `include/liboai/core/response.hpp` | JSON via nlohmann::json |
| Usage examples | `documentation/{feature}/examples/` | Sync + async variants |
| Build configuration | `CMakeLists.txt` | |

## CODE MAP

| Symbol | Type | Location | Role |
|--------|------|----------|------|
| `OpenAI` | Class | `include/liboai/liboai.hpp` | Facade - owns all component pointers |
| `Authorization` | Class | `include/liboai/core/authorization.hpp` | Singleton for API keys/proxies |
| `Response` | Class | `include/liboai/core/response.hpp` | JSON response wrapper |
| `FutureResponse` | Type | `include/liboai/core/response.hpp` | `std::future<liboai::Response>` |
| `JsonConstructor` | Class | `include/liboai/core/response.hpp` | Build JSON payloads |
| `Audio` | Component | `include/liboai/components/audio.hpp` | Speech/transcription/translation |
| `ChatCompletion` | Component | `include/liboai/components/chat.hpp` | ChatGPT API (largest, 40k lines) |
| `Azure` | Component | `include/liboai/components/azure.hpp` | Azure OpenAI endpoints |
| `Images` | Component | `include/liboai/components/images.hpp` | DALL-E generation |
| `Models` | Component | `include/liboai/components/models.hpp` | Model listing/retrieval |

## CONVENTIONS

- **C++17** required
- **Namespace**: `liboai`
- **Single header**: Users include only `liboai/liboai.hpp`
- **Singleton auth**: `Authorization::Authorizer()` for global auth state
- **Non-copyable/movable**: Core classes use `NON_COPYABLE(Class)` and `NON_MOVABLE(Class)` macros
- **Return type**: `Response` for sync, `FutureResponse` for async
- **Component pattern**: Each API area = component class with `create()`, `create_async()` methods
- **Error handling**: Throws `liboai::exception::OpenAIException`

## ANTI-PATTERNS

- **DO NOT** include individual component headers - use `liboai/liboai.hpp`
- **DO NOT** copy/move `OpenAI`, `Authorization`, `Response` instances (deleted ops)
- **DO NOT** access `raw_json` directly before checking `status_code`

## DEPENDENCIES

- **nlohmann_json** - JSON parsing
- **cURL** - HTTP requests
- Install via **vcpkg** on Windows

## COMMANDS

```bash
# Build library only
cmake -B build && cmake --build build

# Build with examples
cmake -B build -DBUILD_EXAMPLES=ON && cmake --build build

# Windows (vcpkg)
set VCPKG_ROOT=<path> && cmake -B build
```

## NOTES

- Repository no longer actively maintained - refer to forks
- Azure OpenAI supported via separate `Azure` component
- Chat component includes `Conversation` helper class for multi-turn
- All async methods: `{method}_async()` suffix, returns `FutureResponse`
- Default timeout: 30000ms (set via `auth.SetMaxTimeout()`)
