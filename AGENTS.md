# PROJECT KNOWLEDGE BASE

**Generated:** 2026-02-16
**Commit:** b692b33
**Branch:** main

## OVERVIEW

Unofficial C++17 library for OpenAI API. Spiritual port of Python's `openai` library with similar structure. Single-header include design (`#include "liboai.h"`).

## STRUCTURE

```
liboai/
├── include/
│   ├── liboai.h              # Main entry - include only this
│   ├── components/           # API endpoint wrappers (audio, chat, images, etc.)
│   └── core/                 # Infrastructure (auth, network, response, exception)
├── components/               # Component implementations (.cpp)
├── core/                     # Core implementations (.cpp)
documentation/                # Examples per API feature
```

## WHERE TO LOOK

| Task | Location | Notes |
|------|----------|-------|
| Add new API endpoint | `liboai/include/components/` + `liboai/components/` | Create .h/.cpp pair, add to CMakeLists.txt, include in liboai.h |
| Modify auth behavior | `liboai/include/core/authorization.h` | Singleton pattern |
| Change network layer | `liboai/include/core/netimpl.h` | cURL wrapper, 47k lines |
| Response handling | `liboai/include/core/response.h` | JSON via nlohmann::json |
| Usage examples | `documentation/{feature}/examples/` | Sync + async variants |
| Build configuration | `CMakeLists.txt`, `liboai/CMakeLists.txt` | |

## CODE MAP

| Symbol | Type | Location | Role |
|--------|------|----------|------|
| `OpenAI` | Class | `include/liboai.h` | Facade - owns all component pointers |
| `Authorization` | Class | `include/core/authorization.h` | Singleton for API keys/proxies |
| `Response` | Class | `include/core/response.h` | JSON response wrapper |
| `FutureResponse` | Type | `include/core/response.h` | `std::future<liboai::Response>` |
| `JsonConstructor` | Class | `include/core/response.h` | Build JSON payloads |
| `Audio` | Component | `include/components/audio.h` | Speech/transcription/translation |
| `ChatCompletion` | Component | `include/components/chat.h` | ChatGPT API (largest, 40k lines) |
| `Azure` | Component | `include/components/azure.h` | Azure OpenAI endpoints |
| `Images` | Component | `include/components/images.h` | DALL-E generation |
| `Models` | Component | `include/components/models.h` | Model listing/retrieval |

## CONVENTIONS

- **C++17** required
- **Namespace**: `liboai`
- **Single header**: Users include only `liboai.h`
- **Singleton auth**: `Authorization::Authorizer()` for global auth state
- **Non-copyable/movable**: Core classes use `NON_COPYABLE(Class)` and `NON_MOVABLE(Class)` macros
- **Return type**: `Response` for sync, `FutureResponse` for async
- **Component pattern**: Each API area = component class with `create()`, `create_async()` methods
- **Error handling**: Throws `liboai::exception::OpenAIException`

## ANTI-PATTERNS

- **DO NOT** include individual component headers - use `liboai.h`
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
