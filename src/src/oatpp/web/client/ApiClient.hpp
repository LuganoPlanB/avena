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

#ifndef oatpp_web_client_Client_hpp
#define oatpp_web_client_Client_hpp

#include "./RequestExecutor.hpp"

#include "oatpp/web/protocol/http/incoming/Response.hpp"
#include "oatpp/web/protocol/http/outgoing/BufferBody.hpp"

#include "oatpp/encoding/Base64.hpp"

#include "oatpp/data/share/StringTemplate.hpp"
#include "oatpp/data/mapping/ObjectMapper.hpp"
#include "oatpp/Types.hpp"

#include "oatpp/utils/Conversion.hpp"
#include "oatpp/base/Log.hpp"

#include <string>
#include <list>
#include <unordered_map>

namespace avena { namespace web { namespace client {

/**
 * ApiClient class provides convenient Retrofit-like interface over the avena::web::client::RequestExecutor.
 */
class ApiClient : public avena::base::Countable {
public:
  static constexpr const char* const TAG = "Client";
public:
  /**
   * Convenience typedef for &id:avena::web::protocol::http::Status;.
   */
  typedef avena::web::protocol::http::Status Status;

  /**
   * Convenience typedef for &id:avena::web::protocol::http::Header;.
   */
  typedef avena::web::protocol::http::Header Header;
public:

  /**
   * Convenience typedef for &id:avena::data::type::String;.
   */
  typedef avena::String String;

  /**
   * Convenience typedef for &id:avena::data::type::Int8;.
   */
  typedef avena::Int8 Int8;

  /**
   * Convenience typedef for &id:avena::data::type::UInt8;.
   */
  typedef avena::UInt8 UInt8;

  /**
   * Convenience typedef for &id:avena::data::type::Int16;.
   */
  typedef avena::Int16 Int16;

  /**
   * Convenience typedef for &id:avena::data::type::UInt16;.
   */
  typedef avena::UInt16 UInt16;

  /**
   * Convenience typedef for &id:avena::data::type::Int32;.
   */
  typedef avena::Int32 Int32;

  /**
   * Convenience typedef for &id:avena::data::type::UInt32;.
   */
  typedef avena::UInt32 UInt32;

  /**
   * Convenience typedef for &id:avena::data::type::Int64;.
   */
  typedef avena::Int64 Int64;

  /**
   * Convenience typedef for &id:avena::data::type::UInt64;.
   */
  typedef avena::UInt64 UInt64;

  /**
   * Convenience typedef for &id:avena::data::type::Float32;.
   */
  typedef avena::Float32 Float32;

  /**
   * Convenience typedef for &id:atpp::data::type::Float64;.
   */
  typedef avena::Float64 Float64;

  /**
   * Convenience typedef for &id:avena::data::type::Boolean;.
   */
  typedef avena::Boolean Boolean;

  template <class T>
  using Enum = avena::data::type::Enum<T>;

  template <class T>
  using Object = avena::Object<T>;
public:

  typedef avena::data::share::StringTemplate StringTemplate;

  /**
   * Convenience typedef for &id:avena::web::protocol::http::Headers;.
   */
  typedef avena::web::protocol::http::Headers Headers;

  /**
   * Convenience typedef for &id:avena::web::protocol::http::incoming::Response;.
   */
  typedef avena::web::protocol::http::incoming::Response Response;
public:

  /**
   * Convenience typedef for &id:avena::web::client::RequestExecutor::AsyncCallback;.
   */
  typedef RequestExecutor::AsyncCallback AsyncCallback;
protected:

  struct PathTemplateExtra {
    avena::String name;
    bool hasQueryParams;
  };

protected:

  StringTemplate parsePathTemplate(const avena::String& name, const avena::String& text);

  avena::String formatPath(const StringTemplate& pathTemplate,
                           const std::unordered_map<avena::String, avena::String>& pathParams,
                           const std::unordered_map<avena::String, avena::String>& queryParams);

protected:
  std::shared_ptr<RequestExecutor> m_requestExecutor;
  std::shared_ptr<avena::data::mapping::ObjectMapper> m_objectMapper;
  
public:
  ApiClient(const std::shared_ptr<RequestExecutor>& requestExecutor,
            const std::shared_ptr<avena::data::mapping::ObjectMapper>& objectMapper)
    : m_requestExecutor(requestExecutor)
    , m_objectMapper(objectMapper)
  {}
public:
  
  static std::shared_ptr<ApiClient> createShared(const std::shared_ptr<RequestExecutor>& requestExecutor,
                                                 const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper) {
    return std::make_shared<ApiClient>(requestExecutor, objectMapper);
  }
  
public:

  /**
   * Call &id:avena::web::client::RequestExecutor::getConnection;.
   * @return - &id:avena::web::client::RequestExecutor::ConnectionHandle;.
   */
  virtual std::shared_ptr<RequestExecutor::ConnectionHandle> getConnection();

  /**
   * Call &id:avena::web::client::RequestExecutor::getConnectionAsync;.
   * @return - &id:avena::async::CoroutineStarterForResult;.
   */
  virtual avena::async::CoroutineStarterForResult<const std::shared_ptr<RequestExecutor::ConnectionHandle>&> getConnectionAsync();

  /**
   * Invalidate connection.
   * @param connectionHandle
   */
  void invalidateConnection(const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle);

  /**
   * Get ObjectMapper that was passed to constructor.
   * @return - &id:avena::data::mapping::ObjectMapper;.
   */
  std::shared_ptr<avena::data::mapping::ObjectMapper> getObjectMapper();

  virtual std::shared_ptr<Response> executeRequest(const avena::String& method,
                                                   const StringTemplate& pathTemplate,
                                                   const Headers& headers,
                                                   const std::unordered_map<avena::String, avena::String>& pathParams,
                                                   const std::unordered_map<avena::String, avena::String>& queryParams,
                                                   const std::shared_ptr<RequestExecutor::Body>& body,
                                                   const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle = nullptr);
  
  virtual avena::async::CoroutineStarterForResult<const std::shared_ptr<Response>&>
  executeRequestAsync(const avena::String& method,
                      const StringTemplate& pathTemplate,
                      const Headers& headers,
                      const std::unordered_map<avena::String, avena::String>& pathParams,
                      const std::unordered_map<avena::String, avena::String>& queryParams,
                      const std::shared_ptr<RequestExecutor::Body>& body,
                      const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle = nullptr);

public:

  template<typename T>
  struct TypeInterpretation {

    static avena::String toString(const avena::String& typeName, const T& parameter) {

      (void) parameter;

      OATPP_LOGe("[avena::web::client::ApiClient::TypeInterpretation::toString()]",
                 "Error. No conversion from '{}' to '{}' is defined.", typeName, "avena::String")

      throw std::runtime_error(
        "[avena::web::client::ApiClient::TypeInterpretation::toString()]: Error. "
        "No conversion from '" + *typeName + "' to 'avena::String' is defined. "
        "Please define type conversion."
      );

    }

  };

};

template<>
struct ApiClient::TypeInterpretation<avena::String> {
  static avena::String toString(const avena::String &typeName, const avena::String &parameter) {
    (void) typeName;
    return parameter;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::Int8> {
  static avena::String toString(const avena::String &typeName, const avena::Int8 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::int32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::UInt8> {
  static avena::String toString(const avena::String &typeName, const avena::UInt8 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::uint32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::Int16> {
  static avena::String toString(const avena::String &typeName, const avena::Int16 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::int32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::UInt16> {
  static avena::String toString(const avena::String &typeName, const avena::UInt16 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::uint32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::Int32> {
  static avena::String toString(const avena::String &typeName, const avena::Int32 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::int32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::UInt32> {
  static avena::String toString(const avena::String &typeName, const avena::UInt32 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::uint32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::Int64> {
  static avena::String toString(const avena::String &typeName, const avena::Int64 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::int64ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::UInt64> {
  static avena::String toString(const avena::String &typeName, const avena::UInt64 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::uint64ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::Float32> {
  static avena::String toString(const avena::String &typeName, const avena::Float32 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::float32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::Float64> {
  static avena::String toString(const avena::String &typeName, const avena::Float64 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::Conversion::float64ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<avena::Boolean> {
  static avena::String toString(const avena::String &typeName, const avena::Boolean &parameter) {
    (void) typeName;
    if(parameter != nullptr) {
        return utils::Conversion::boolToStr(*parameter);
    }
    return nullptr;
  }
};

template<class T, class I>
struct ApiClient::TypeInterpretation<data::type::EnumObjectWrapper<T, I>> {

  typedef data::type::EnumObjectWrapper<T, I> EnumOW;
  typedef typename I::UnderlyingTypeObjectWrapper UTOW;

  static avena::String toString(const avena::String &typeName, const EnumOW &parameter) {

    data::type::EnumInterpreterError error = data::type::EnumInterpreterError::OK;
    const auto& value = I::toInterpretation(parameter, false, error);

    switch(error){
      case data::type::EnumInterpreterError::OK: break;
      case data::type::EnumInterpreterError::CONSTRAINT_NOT_NULL:
        throw std::runtime_error(
          "[avena::web::client::ApiClient::TypeInterpretation::toString()]: Error. Enum constraint violation - NotNull."
        );
      case data::type::EnumInterpreterError::TYPE_MISMATCH_ENUM:
      case data::type::EnumInterpreterError::TYPE_MISMATCH_ENUM_VALUE:
      case data::type::EnumInterpreterError::ENTRY_NOT_FOUND:
      default:
        throw std::runtime_error(
          "[avena::web::client::ApiClient::TypeInterpretation::toString()]: Error. Can't interpret Enum."
        );
    }

    return ApiClient::TypeInterpretation<UTOW>::toString(typeName, value.template cast<UTOW>());

  }

};
  
}}}

#endif /* oatpp_web_client_Client_hpp */
