<!-- Copyright (C) 2026 Plan-B Foundation -->
<!-- Updated C++ namespace examples for avena. -->

> This component is maintained as part of avena. Use the `avena::` C++
> namespace, public `<avena/...>` headers and the `avena::avena` CMake
> target. Build and install from the repository root; see the
> [avena usage instructions](../../README.md). Upstream links below describe
> oatpp; adapt their namespaces and include paths before using examples.

# oatpp-openssl [![Build Status](https://dev.azure.com/lganzzzo/lganzzzo/_apis/build/status/oatpp.oatpp-openssl?branchName=master)](https://dev.azure.com/lganzzzo/lganzzzo/_build/latest?definitionId=32&branchName=master)

This submodule provides secure server and client connection providers for oatpp applications. Based on OpenSSL.

More about oat++:
- Website: [https://oatpp.io](https://oatpp.io)

## Requires

OpenSSL installed.

### Create server connection provider

```cpp

#include <avena/openssl/server/ConnectionProvider.hpp>
#include <avena/openssl/Config.hpp>

...

const char* pemFile = "path/to/file.pem";
const char* crtFile = "path/to/file.crt";

auto config = avena::openssl::Config::createDefaultServerConfigShared(pemFile, crtFile);
auto connectionProvider = avena::openssl::server::ConnectionProvider::createShared(config, {"localhost", 443});

```

### Create client connection provider

```cpp

#include <avena/openssl/client/ConnectionProvider.hpp>
#include <avena/openssl/Config.hpp>
#include <avena/openssl/configurer/TrustStore.hpp>

...

const char* trust = "path/to/truststore";

auto config = avena::openssl::Config::createDefaultClientConfigShared();
config->addContextConfigurer(std::make_shared<avena::openssl::configurer::TrustStore>(trust, nullptr));
auto connectionProvider = avena::openssl::client::ConnectionProvider::createShared(config, {"httpbin.org", 443});

```
