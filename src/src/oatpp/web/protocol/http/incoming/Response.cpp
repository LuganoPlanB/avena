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

#include "./Response.hpp"

namespace avena { namespace web { namespace protocol { namespace http { namespace incoming {

Response::Response(v_int32 statusCode,
                   const avena::String& statusDescription,
                   const http::Headers& headers,
                   const std::shared_ptr<avena::data::stream::InputStream>& bodyStream,
                   const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder)
  : m_statusCode(statusCode)
  , m_statusDescription(statusDescription)
  , m_headers(headers)
  , m_bodyStream(bodyStream)
  , m_bodyDecoder(bodyDecoder)
{}

std::shared_ptr<Response> Response::createShared(v_int32 statusCode,
                                                 const avena::String& statusDescription,
                                                 const http::Headers& headers,
                                                 const std::shared_ptr<avena::data::stream::InputStream>& bodyStream,
                                                 const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder)
{
  return std::make_shared<Response>(statusCode, statusDescription, headers, bodyStream, bodyDecoder);
}

v_int32 Response::getStatusCode() const {
  return m_statusCode;
}

avena::String Response::getStatusDescription() const {
  return m_statusDescription;
}

const http::Headers& Response::getHeaders() const {
  return m_headers;
}

void Response::putHeader(const avena::String& key, const avena::String& value) {
  m_headers.put(key, value);
}

bool Response::putHeaderIfNotExists(const avena::String& key, const avena::String& value) {
  return m_headers.putIfNotExists(key, value);
}

bool Response::putOrReplaceHeader(const String &key, const String &value) {
  return m_headers.putOrReplace(key, value);
}

bool Response::putOrReplaceHeader_Unsafe(const data::share::StringKeyLabelCI& key,
                                         const data::share::StringKeyLabel &value) {
  return m_headers.putOrReplace(key, value);
}

void Response::putHeader_Unsafe(const avena::data::share::StringKeyLabelCI& key, const avena::data::share::StringKeyLabel& value) {
  m_headers.put(key, value);
}

bool Response::putHeaderIfNotExists_Unsafe(const avena::data::share::StringKeyLabelCI& key, const avena::data::share::StringKeyLabel& value) {
  return m_headers.putIfNotExists(key, value);
}

avena::String Response::getHeader(const avena::data::share::StringKeyLabelCI& headerName) const{
  return m_headers.get(headerName);
}

void Response::putBundleData(const avena::String& key, const avena::Void& polymorph) {
  m_bundle.put(key, polymorph);
}

const data::Bundle& Response::getBundle() const {
  return m_bundle;
}

std::shared_ptr<avena::data::stream::InputStream> Response::getBodyStream() const {
  return m_bodyStream;
}

std::shared_ptr<const http::incoming::BodyDecoder> Response::getBodyDecoder() const {
  return m_bodyDecoder;
}

void Response::transferBody(const base::ObjectHandle<data::stream::WriteCallback>& writeCallback) const {
  m_bodyDecoder->decode(m_headers, m_bodyStream.get(), writeCallback.get(), m_connection.get());
}

void Response::transferBodyToStream(const base::ObjectHandle<avena::data::stream::OutputStream>& toStream) const {
  m_bodyDecoder->decode(m_headers, m_bodyStream.get(), toStream.get(), m_connection.get());
}

avena::String Response::readBodyToString() const {
  return m_bodyDecoder->decodeToString(m_headers, m_bodyStream.get(), m_connection.get());
}

async::CoroutineStarter Response::transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const {
  return m_bodyDecoder->decodeAsync(m_headers, m_bodyStream, writeCallback, m_connection);
}

avena::async::CoroutineStarter Response::transferBodyToStreamAsync(const std::shared_ptr<avena::data::stream::OutputStream>& toStream) const {
  return m_bodyDecoder->decodeAsync(m_headers, m_bodyStream, toStream, m_connection);
}

}}}}}
