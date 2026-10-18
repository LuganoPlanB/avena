// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.
/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *                         Benedikt-Alexander Mokroß <bam@icognize.de>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_web_server_handler_AuthorizationHandler_hpp
#define oatpp_web_server_handler_AuthorizationHandler_hpp

#include "oatpp/web/protocol/http/incoming/Request.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/data/type/Type.hpp"

namespace avena { namespace web { namespace server { namespace handler {

/**
 * The AuthorizationObject superclass, all AuthorizationObjects have to extend this class.
 */
class AuthorizationObject : public avena::base::Countable {
protected:
  AuthorizationObject() = default;
};

/**
 * Abstract Authorization Handler.
 */
class AuthorizationHandler {
public:
  /**
   * Convenience typedef for &l:AuthorizationObject;.
   */
  typedef avena::web::server::handler::AuthorizationObject AuthorizationObject;

  /**
   * Convenience typedef for &id:avena::data::stream::BufferOutputStream;.
   */
  typedef avena::data::stream::BufferOutputStream BufferOutputStream;

  /**
   * Convenience typedef for &id:avena::web::protocol::http::Headers;.
   */
  typedef avena::web::protocol::http::Headers Headers;
private:
  avena::String m_scheme;
  avena::String m_realm;
public:

  /**
   * Constructor.
   * @param scheme - authorization type scheme. &id:avena::String;.
   * @param realm - realm. &id:avena::String;.
   */
  AuthorizationHandler(const avena::String& scheme, const avena::String& realm);

  /**
   * Default virtual destructor.
   */
  virtual ~AuthorizationHandler() = default;

  /**
   * Implement this method! Return nullptr if authorization should be denied.
   * @param header - `Authorization` header. &id:avena::String;.
   * @return - `std::shared_ptr` to &id:avena::web::server::handler::AuthorizationObject;.
   */
  virtual std::shared_ptr<AuthorizationObject> handleAuthorization(const avena::String& authorizationHeader) = 0;

  /**
   * Render WWW-Authenicate header value. <br>
   * Custom Authorization handlers may override this method in order to provide additional information.
   * @param stream - &id:avena::data::stream::BufferOutputStream;.
   */
  virtual void renderAuthenticateHeaderValue(BufferOutputStream& stream);

  /**
   * Add authorization error headers to the headers map. <br>
   * @param headers - &id:avena::web::protocol::http::Headers;.
   */
  virtual void addErrorResponseHeaders(Headers& headers);

  /**
   * Get authorization scheme.
   * @return
   */
  avena::String getScheme();

  /**
   * Get authorization realm.
   * @return
   */
  avena::String getRealm();

};

/**
 * Default Basic AuthorizationObject - Convenience object to enable Basic-Authorization without the need to implement anything.
 */
class DefaultBasicAuthorizationObject : public AuthorizationObject {
public:

  /**
   * User-Id. &id:avena::String;.
   */
  avena::String userId;

  /**
   * Password. &id:avena::String;.
   */
  avena::String password;

};

/**
 * AuthorizationHandler for Authorization Type `Basic`. <br>
 * See [RFC 7617](https://tools.ietf.org/html/rfc7617). <br>
 * Extend this class to implement Custom Basic Authorization.
 */
class BasicAuthorizationHandler : public AuthorizationHandler {
public:

  /**
   * Constructor.
   * @param realm
   */
  BasicAuthorizationHandler(const avena::String& realm = "API");

  /**
   * Implementation of &l:AuthorizationHandler::handleAuthorization ();
   * @param header - &id:avena::String;.
   * @return - std::shared_ptr to &id:avena::web::server::handler::AuthorizationObject;.
   */
  std::shared_ptr<AuthorizationObject> handleAuthorization(const avena::String &header) override;

  /**
   * Implement this method! Do the actual authorization here. When not implemented returns &l:DefaultBasicAuthorizationObject;.
   * @param userId - user id. &id:avena::String;.
   * @param password - password. &id:avena::String;.
   * @return - `std::shared_ptr` to &l:AuthorizationObject;. `nullptr` - for "Unauthorized".
   */
  virtual std::shared_ptr<AuthorizationObject> authorize(const avena::String& userId, const avena::String& password);

};

/**
 * Default Bearer AuthorizationObject - Convenience object to enable Bearer-Authorization without the need to implement anything.
 */
class DefaultBearerAuthorizationObject : public AuthorizationObject {
public:

  /**
   * Token. &id:avena::String;.
   */
  avena::String token;

};

/**
 * AuthorizationHandler for Authorization Type `Bearer`. <br>
 * See [RFC 6750](https://tools.ietf.org/html/rfc6750). <br>
 * Extend this class to implement Custom Bearer Authorization.
 */
class BearerAuthorizationHandler : public AuthorizationHandler {
public:

  /**
   * Constructor.
   * @param realm
   */
  BearerAuthorizationHandler(const avena::String& realm = "API");

  /**
   * Implementation of &l:AuthorizationHandler::handleAuthorization ();
   * @param header - &id:avena::String;.
   * @return - std::shared_ptr to &id:avena::web::server::handler::AuthorizationObject;.
   */
  std::shared_ptr<AuthorizationObject> handleAuthorization(const avena::String &header) override;

  /**
   * Implement this method! Do the actual authorization here. When not implemented returns &l:DefaultBearerAuthorizationObject;.
   * @param token - access token.
   * @return - `std::shared_ptr` to &l:AuthorizationObject;. `nullptr` - for "Unauthorized".
   */
  virtual std::shared_ptr<AuthorizationObject> authorize(const avena::String& token);

};

}}}}

#endif /* oatpp_web_server_handler_ErrorHandler_hpp */
