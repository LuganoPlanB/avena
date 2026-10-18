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

#ifndef oatpp_data_mapping_ObjectMapper_hpp
#define oatpp_data_mapping_ObjectMapper_hpp

#include <utility>

#include "oatpp/Types.hpp"

#include "oatpp/data/stream/Stream.hpp"

#include "oatpp/utils/parser/Caret.hpp"
#include "oatpp/utils/parser/ParsingError.hpp"

namespace avena { namespace data { namespace mapping {

/**
 * Error stack
 */
struct ErrorStack {

  /**
   * stack
   */
  std::list<avena::String> stack;

  /**
   * Push error
   * @param error
   */
  void push(const avena::String& error);

  /**
   * Push all errors from other error stack.
   * @param errorStack
   */
  void splice(ErrorStack& errorStack);

  /**
   * Stacktrace as string.
   * @return
   */
  avena::String stacktrace() const;

  /**
   * Check if error stack is empty.
   * @return
   */
  bool empty() const;

};

/**
 * Mapping error
 */
class MappingError : public std::runtime_error {
private:
  ErrorStack m_stack;
public:

  /**
   * Constructor.
   * @param errorStack
   */
  MappingError(const ErrorStack& errorStack);

  /**
   * Constructor.
   * @param errorStack
   */
  MappingError(ErrorStack&& errorStack);

  /**
   * Get error stack
   * @return - See &id:avena::data::mapping::ErrorStack;.
   */
  const ErrorStack& errorStack() const;

  /**
   * Get error stack
   * @return - See &id:avena::data::mapping::ErrorStack;.
   */
  ErrorStack& errorStack();

};

/**
 * Abstract ObjectMapper class.
 */
class ObjectMapper {
public:

  /**
   * Metadata for ObjectMapper.
   */
  class Info {
  public:

    /**
     * Constructor
     */
    Info(const avena::String& pMimeType, const avena::String& pMimeSubtype)
      : httpContentType(pMimeType + "/" + pMimeSubtype)
      , mimeType(pMimeType)
      , mimeSubtype(pMimeSubtype)
    {}

    /**
     * Value for Content-Type http header when DTO is serialized via specified ObjectMapper.
     */
    const avena::String httpContentType;

    /**
     * Mime type
     */
    const avena::String mimeType;

    /**
     * Mime subtype
     */
    const avena::String mimeSubtype;

  };
private:
  Info m_info;
public:

  /**
   * Constructor.
   * @param info - Metadata for ObjectMapper.
   */
  ObjectMapper(const Info& info);

  /**
   * Get ObjectMapper metadata.
   * @return - ObjectMapper metadata.
   */
  const Info& getInfo() const;

  /**
   * Serialize object to stream. Implement this method.
   * @param stream - &id:avena::data::stream::ConsistentOutputStream; to serialize object to.
   * @param errorStack - See &id:avena::data::mapping::ErrorStack;.
   * @param variant - Object to serialize.
   */
  virtual void write(data::stream::ConsistentOutputStream* stream, const avena::Void& variant, ErrorStack& errorStack) const = 0;

  /**
   * Deserialize object. Implement this method.
   * @param caret - &id:avena::utils::parser::Caret; over serialized buffer.
   * @param type - pointer to object type. See &id:avena::data::type::Type;.
   * @param errorStack - See &id:avena::data::mapping::ErrorStack;.
   * @return - deserialized object wrapped in &id:avena::Void;.
   */
  virtual avena::Void read(avena::utils::parser::Caret& caret, const avena::Type* type, ErrorStack& errorStack) const = 0;

  /**
   * Serialize object to String.
   * @param variant - Object to serialize.
   * @return - serialized object as &id:avena::String;.
   * @throws - &id:avena::data::mapping::MappingError;
   * @throws - depends on implementation.
   */
  avena::String writeToString(const avena::Void& variant) const;

  /**
   * Deserialize object.
   * If nullptr is returned - check caret.getError()
   * @tparam Wrapper - ObjectWrapper type.
   * @param caret - &id:avena::utils::parser::Caret; over serialized buffer.
   * @return - deserialized Object.
   * @throws - &id:avena::data::mapping::MappingError;
   * @throws - depends on implementation.
   */
  template<class Wrapper>
  Wrapper readFromCaret(avena::utils::parser::Caret& caret) const {
    auto type = Wrapper::Class::getType();
    ErrorStack errorStack;
    const auto& result = read(caret, type, errorStack).template cast<Wrapper>();
    if(!errorStack.empty()) {
      throw MappingError(std::move(errorStack));
    }
    return result;
  }

  /**
   * Deserialize object.
   * @tparam Wrapper - ObjectWrapper type.
   * @param str - serialized data.
   * @return - deserialized Object.
   * @throws - &id:avena::data::mapping::MappingError;
   * @throws - depends on implementation.
   */
  template<class Wrapper>
  Wrapper readFromString(const avena::String& str) const {
    auto type = Wrapper::Class::getType();
    avena::utils::parser::Caret caret(str);
    ErrorStack errorStack;
    const auto& result = read(caret, type, errorStack).template cast<Wrapper>();
    if(!errorStack.empty()) {
      throw MappingError(std::move(errorStack));
    }
    return result;
  }
  
};
  
}}}

#endif /* oatpp_data_mapping_ObjectMapper_hpp */
