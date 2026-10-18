// Copyright (C) 2026 Plan-B Foundation
// SPDX-License-Identifier: Apache-2.0
#include <avena/Environment.hpp>
#include <avena/network/virtual_/Socket.hpp>
#include <avena/openssl/Connection.hpp>
#include <openssl/err.h>
#include <iostream>
#include <memory>

bool checkRetry(bool writing) {
  using namespace avena;
  using namespace avena::network::virtual_;
  std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)> context(
      SSL_CTX_new(TLS_server_method()), SSL_CTX_free);
  std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)> clientContext(
      SSL_CTX_new(TLS_client_method()), SSL_CTX_free);
  if (!context || !clientContext ||
      SSL_CTX_use_certificate_chain_file(context.get(), CERT_CRT_PATH) != 1 ||
      SSL_CTX_use_PrivateKey_file(context.get(), CERT_PEM_PATH, SSL_FILETYPE_PEM) != 1)
    return false;
  auto ssl = SSL_new(context.get());
  if (!ssl) return false;
  std::unique_ptr<SSL, decltype(&SSL_free)> client(SSL_new(clientContext.get()), SSL_free);
  BIO* serverBio = nullptr;
  BIO* clientBio = nullptr;
  if (!client || BIO_new_bio_pair(&serverBio, 0, &clientBio, 0) != 1) {
    SSL_free(ssl);
    return false;
  }
  SSL_set_bio(ssl, serverBio, serverBio);
  SSL_set_bio(client.get(), clientBio, clientBio);
  SSL_set_accept_state(ssl);
  SSL_set_connect_state(client.get());
  for (int i = 0; i < 100 &&
       (!SSL_is_init_finished(ssl) || !SSL_is_init_finished(client.get())); ++i) {
    SSL_do_handshake(client.get());
    SSL_do_handshake(ssl);
  }
  if (!SSL_is_init_finished(ssl) || !SSL_is_init_finished(client.get())) {
    SSL_free(ssl);
    return false;
  }
  auto socket = Socket::createShared(Pipe::createShared(), Pipe::createShared());
  openssl::Connection connection(ssl, {socket, nullptr});
  connection.setInputStreamIOMode(data::stream::IOMode::ASYNCHRONOUS);
  connection.setOutputStreamIOMode(data::stream::IOMode::ASYNCHRONOUS);
  async::Action action;
  char buffer = 0;
  // Fill the transport so a write has to wait for space.
  if (writing) {
    char filler[4096] = {};
    bool full = false;
    for (int i = 0; i < 1024; ++i) {
      async::Action fillAction;
      if (socket->write(filler, sizeof(filler), fillAction) == IOError::RETRY_WRITE) {
        full = true;
        break;
      }
    }
    if (!full) return false;
  }
  // Simulate an unrelated connection leaving an error on this executor thread.
  ERR_put_error(ERR_LIB_USER, 0, 1, __FILE__, __LINE__);
  const auto result = writing ? connection.write(&buffer, 1, action)
                              : connection.read(&buffer, 1, action);
  ERR_clear_error();
  const auto expected = writing ? IOError::RETRY_WRITE : IOError::RETRY_READ;
  if (result != expected) {
    std::cerr << (writing ? "write" : "read") << " returned " << result
              << ", expected retry " << expected << '\n';
    return false;
  }
  return true;
}

int main() {
  avena::Environment::init();
  const bool readPassed = checkRetry(false);
  const bool writePassed = checkRetry(true);
  const auto remaining = avena::Environment::getObjectsCount();
  avena::Environment::destroy();
  return readPassed && writePassed && remaining == 0 ? 0 : 1;
}
