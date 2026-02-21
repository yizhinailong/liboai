<h1>Making Use of <code>liboai</code></h1>
<p>To integrate <code>liboai</code> into your project, build the library with xmake and link against it as a static library.</p>

<h3>Building the Library</h3>
<p><code>liboai</code> uses C++23 modules and xmake for dependency management. Build the library with a single command:</p>

```bash
xmake
```

<p>This produces a static library (<code>liboai.a</code> on Linux/macOS, <code>liboai.lib</code> on Windows) in the build directory. Dependencies (nlohmann_json and cpr) are fetched automatically by xmake.</p>

<h3>Requirements</h3>
<p>Your project must meet these requirements:</p>

* C++23 compatible compiler (Clang 16+)
* xmake build system

<h3>Integration Steps</h3>
<p>To use <code>liboai</code> in an existing C++23 project:</p>

  1. Build <code>liboai</code> with <code>xmake</code> as shown above.
  2. Link your project against the generated static library (<code>liboai.a</code> or <code>liboai.lib</code>).
  3. Import the module in your code with <code>import liboai;</code>.

<p>Example usage in consuming code:</p>

```cpp
import liboai;
import std;

auto main() -> int {
  liboai::OpenAI oai;
  oai.auth.SetKeyEnv("OPENAI_API_KEY");
  
  auto res = oai.Image->Create("A cat playing piano", 1, "1024x1024");
  if (res) {
    std::cout << res.value()["data"][0]["url"].get<std::string>() << std::endl;
  }
}
```

<p>See the main <a href="/README.md">README.md</a> and <a href="/examples">examples</a> for complete usage patterns.</p>
