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

#ifndef oatpp_web_protocol_http_incoming_Response_hpp
#define oatpp_web_protocol_http_incoming_Response_hpp

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/web/protocol/http/incoming/BodyDecoder.hpp"
#include "oatpp/data/Bundle.hpp"

namespace avena { namespace web { namespace protocol { namespace http { namespace incoming {

/**
 * Class http::incoming::Response AKA IncomingResponse represents server's incoming response
 */
class Response : public avena::base::Countable {
private:
  v_int32 m_statusCode;
  avena::String m_statusDescription;
  http::Headers m_headers;
  std::shared_ptr<avena::data::stream::InputStream> m_bodyStream;
  
  /*
   * Response should be preconfigured with default BodyDecoder.
   * Entity that created response object is responsible for providing correct BodyDecoder.
   * Custom BodyDecoder can be set on demand
   */
  std::shared_ptr<const http::incoming::BodyDecoder> m_bodyDecoder;
  
  std::shared_ptr<avena::data::stream::IOStream> m_connection;

  data::Bundle m_bundle;
  
public:
  /**
   * Constructor.
   * @param statusCode - http status code.
   * @param statusDescription - http status description.
   * @param headers - &id:avena::web::protocol::http::Headers;.
   * @param bodyStream - &id:avena::data::stream::InputStream;.
   * @param bodyDecoder - &id:avena::web::protocol::http::incoming::BodyDecoder;.
   */
  Response(v_int32 statusCode,
           const avena::String& statusDescription,
           const http::Headers& headers,
           const std::shared_ptr<avena::data::stream::InputStream>& bodyStream,
           const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);
public:

  /**
   * Create shared Response.
   * @param statusCode - http status code.
   * @param statusDescription - http status description.
   * @param headers - &id:avena::web::protocol::http::Headers;.
   * @param bodyStream - &id:avena::data::stream::InputStream;.
   * @param bodyDecoder - &id:avena::web::protocol::http::incoming::BodyDecoder;.
   * @return - `std::shared_ptr` to Response.
   */
  static std::shared_ptr<Response> createShared(v_int32 statusCode,
                                                const avena::String& statusDescription,
                                                const http::Headers& headers,
                                                const std::shared_ptr<avena::data::stream::InputStream>& bodyStream,
                                                const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);

  /**
   * Get http status code.
   * @return - http status code.
   */
  v_int32 getStatusCode() const;

  /**
   * Get http status description.
   * @return - http status description.
   */
  avena::String getStatusDescription() const;

  /**
   * Get response http headers as &id:avena::web::protocol::http::Headers;.
   * @return - response http headers as &id:avena::web::protocol::http::Headers;.
   */
  const http::Headers& getHeaders() const;

  /**
   * Add http header.
   * @param key - &id:avena::String;.
   * @param value - &id:avena::String;.
   */
  void putHeader(const avena::String& key, const avena::String& value);

  /**
   * Add http header if not already exists.
   * @param key - &id:avena::String;.
   * @param value - &id:avena::String;.
   * @return - `true` if header was added.
   */
  bool putHeaderIfNotExists(const avena::String& key, const avena::String& value);

  /**
   * Replaces or adds header.
   * @param key - &id:avena::String;.
   * @param value - &id:avena::String;.
   * @return - `true` if header was replaces, `false` if header was added.
   */
  bool putOrReplaceHeader(const avena::String& key, const avena::String& value);

  /**
   * Replaces or adds header.
   * @param key - &id:avena::data::share::StringKeyLabelCI;.
   * @param value - &id:avena::data::share::StringKeyLabel;.
   * @return - `true` if header was replaces, `false` if header was added.
   */
  bool putOrReplaceHeader_Unsafe(const avena::data::share::StringKeyLabelCI& key, const avena::data::share::StringKeyLabel& value);

  /**
   * Add http header.
   * @param key - &id:avena::data::share::StringKeyLabelCI;.
   * @param value - &id:avena::data::share::StringKeyLabel;.
   */
  void putHeader_Unsafe(const avena::data::share::StringKeyLabelCI& key, const avena::data::share::StringKeyLabel& value);

  /**
   * Add http header if not already exists.
   * @param key - &id:avena::data::share::StringKeyLabelCI;.
   * @param value - &id:avena::data::share::StringKeyLabel;.
   * @return - `true` if header was added.
   */
  bool putHeaderIfNotExists_Unsafe(const avena::data::share::StringKeyLabelCI& key, const avena::data::share::StringKeyLabel& value);

  /**
   * Get header value
   * @param headerName - &id:avena::data::share::StringKeyLabelCI;.
   * @return - &id:avena::String;.
   */
  avena::String getHeader(const avena::data::share::StringKeyLabelCI& headerName) const;

  /**
   * Put data to bundle.
   * @param key
   * @param polymorph
   */
  void putBundleData(const avena::String& key, const avena::Void& polymorph);

  /**
   * Get data from bundle by key.
   * @tparam WrapperType
   * @param key
   * @return
   */
  template<typename WrapperType>
  WrapperType getBundleData(const avena::String& key) const {
    return m_bundle.template get<WrapperType>(key);
  }

  /**
   * Get bundle object.
   * @return
   */
  const data::Bundle& getBundle() const;

  /**
   * Get raw body stream.
   * @return - raw body stream as &id:avena::data::stream::InputStream;.
   */
  std::shared_ptr<data::stream::InputStream> getBodyStream() const;

  /**
   * Get body decoder configured for this response.
   * @return - &id:avena::web::protocol::http::incoming::BodyDecoder;.
   */
  std::shared_ptr<const http::incoming::BodyDecoder> getBodyDecoder() const;

  /**
   * Transfer body. <br>
   * Read body chunk by chunk and pass chunks to the `writeCallback`.
   * @param writeCallback - &id:avena::data::stream::WriteCallback;.
   */
  void transferBody(const base::ObjectHandle<data::stream::WriteCallback>& writeCallback) const;

  /**
   * Decode and transfer body to toStream.
   * Use case example - stream huge body directly to file using relatively small buffer.
   * @param toStream - pointer to &id:avena::data::stream::OutputStream;.
   */
  void transferBodyToStream(const base::ObjectHandle<data::stream::OutputStream>& toStream) const;

  /**
   * Decode and read body to &id:avena::String;.
   * @return - &id:avena::String;.
   */
  avena::String readBodyToString() const;

  /**
   * Read body stream, decode, and deserialize it as DTO Object (see [Data Transfer Object (DTO)](https://oatpp.io/docs/components/dto/)).
   * @tparam Wrapper - ObjectWrapper type.
   * @param objectMapper - `std::shared_ptr` to &id:avena::data::mapping::ObjectMapper;.
   * @return - deserialized DTO object.
   */
  template<class Wrapper>
  Wrapper readBodyToDto(const base::ObjectHandle<data::mapping::ObjectMapper>& objectMapper) const {
    return m_bodyDecoder->decodeToDto<Wrapper>(m_headers, m_bodyStream.get(), m_connection.get(), objectMapper.get());
  }
  
  // Async

  /**
   * Transfer body in Asynchronous manner. <br>
   * Read body chunk by chunk and pass chunks to the `writeCallback`.
   * @param writeCallback - `std::shared_ptr` to &id:avena::data::stream::WriteCallback;.
   * @return - &id:avena::async::CoroutineStarter;.
   */
  async::CoroutineStarter transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const;


  /**
   * Same as &l:Response::readBodyToDto (); but Async.
   * @param toStream - `std::shared_ptr` to &id:avena::data::stream::OutputStream;.
   * @return - &id:avena::async::CoroutineStarter;.
   */
  avena::async::CoroutineStarter transferBodyToStreamAsync(const std::shared_ptr<data::stream::OutputStream>& toStream) const;

  /**
   * Same as &l:Response::readBodyToString (); but Async.
   * @return - &id:avena::async::CoroutineStarterForResult;.
   */
  avena::async::CoroutineStarterForResult<const avena::String&> readBodyToStringAsync() const {
    return m_bodyDecoder->decodeToStringAsync(m_headers, m_bodyStream, m_connection);
  }

  /**
   * Same as &l:Response::readBodyToDto (); but Async.
   * @tparam Wrapper - ObjectWrapper type.
   * @param objectMapper - `std::shared_ptr` to &id:avena::data::mapping::ObjectMapper;.
   * @return - &id:avena::async::CoroutineStarterForResult;.
   */
  template<class Wrapper>
  avena::async::CoroutineStarterForResult<const Wrapper&>
  readBodyToDtoAsync(const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper) const {
    return m_bodyDecoder->decodeToDtoAsync<Wrapper>(m_headers, m_bodyStream, m_connection, objectMapper);
  }
  
};
  
}}}}}

#endif /* oatpp_web_protocol_http_incoming_Response_hpp */
