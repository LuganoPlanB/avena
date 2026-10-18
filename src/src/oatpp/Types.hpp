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

#ifndef oatpp_Types_hpp
#define oatpp_Types_hpp

#include "oatpp/data/type/Object.hpp"
#include "oatpp/data/mapping/Tree.hpp"

namespace avena {

  /**
   * &id:avena::data::type::Type;.
   */
  typedef avena::data::type::Type Type;

  /**
   * &id:avena::data::type::ClassId;.
   */
  typedef avena::data::type::ClassId ClassId;

  /**
   * ObjectWrapper.
   */
  template <class T, class Clazz = avena::data::type::__class::Void>
  using ObjectWrapper = avena::data::type::ObjectWrapper<T, Clazz>;

  /**
   * Mapping enabled &id:avena::data::mapping::Tree;
   * `mapping::Tree` and `type::Tree` are inter-mappable;
   */
  typedef avena::data::type::Tree Tree;

  /**
   * ObjectWrapper over the `void*`.
   */
  typedef avena::data::type::Void Void;

  /**
   * `Any` - container for mapping-enabled types.
   * &id:avena::data::type::Any;
   */
  typedef avena::data::type::Any Any;

  /**
   * Mapping-Enabled String type. &id:avena::data::type::String; <br>
   * For `avena::String` methods see `std::string`
   */
  typedef avena::data::type::String String;

  /**
   * Mapping-Enabled 8-bits int. Can hold nullptr value. &id:avena::data::type::Int8;
   */
  typedef avena::data::type::Int8 Int8;

  /**
   * Mapping-Enabled 8-bits unsigned int. Can hold nullptr value. &id:avena::data::type::UInt8;
   */
  typedef avena::data::type::UInt8 UInt8;

  /**
   * Mapping-Enabled 16-bits int. Can hold nullptr value. &id:avena::data::type::Int16;
   */
  typedef avena::data::type::Int16 Int16;

  /**
   * Mapping-Enabled 16-bits unsigned int. Can hold nullptr value. &id:avena::data::type::UInt16;
   */
  typedef avena::data::type::UInt16 UInt16;

  /**
   * Mapping-Enabled 32-bits int. Can hold nullptr value. &id:avena::data::type::Int32;
   */
  typedef avena::data::type::Int32 Int32;

  /**
   * Mapping-Enabled 32-bits unsigned int. Can hold nullptr value. &id:avena::data::type::UInt32;
   */
  typedef avena::data::type::UInt32 UInt32;

  /**
   * Mapping-Enabled 64-bits int. Can hold nullptr value. &id:avena::data::type::Int64;
   */
  typedef avena::data::type::Int64 Int64;

  /**
   * Mapping-Enabled 64-bits unsigned int. Can hold nullptr value. &id:avena::data::type::UInt64;
   */
  typedef avena::data::type::UInt64 UInt64;

  /**
   * Mapping-Enabled 32-bits float. Can hold nullptr value. &id:avena::data::type::Float32;
   */
  typedef avena::data::type::Float32 Float32;

  /**
   * Mapping-Enabled 64-bits float (double). Can hold nullptr value. &id:avena::data::type::Float64;
   */
  typedef avena::data::type::Float64 Float64;

  /**
   * Mapping-Enabled Boolean. Can hold nullptr value. &id:avena::data::type::Boolean;
   */
  typedef avena::data::type::Boolean Boolean;

  /**
   * Base class for all Object-like Mapping-enabled structures. &id:avena::data::type::BaseObject;
   */
  typedef avena::data::type::BaseObject BaseObject;

  /**
   * Base class for all DTO objects. &id:avena::data::type::DTO;
   */
  typedef avena::data::type::DTO DTO;

  /**
   * Mapping-Enabled DTO Object. &id:avena::data::type::DTOWrapper;
   */
  template <class T>
  using Object = avena::data::type::DTOWrapper<T>;

  /**
   * Mapping-Enabled Enum. &id:avena::data::type::Enum;
   */
  template <class T>
  using Enum = avena::data::type::Enum<T>;

  /**
   * Mapping-Enabled Vector. &id:avena::data::type::Vector;
   */
  template <class T>
  using Vector = avena::data::type::Vector<T>;

  /**
   * Abstract Vector.
   */
  typedef avena::data::type::AbstractVector AbstractVector;

  /**
   * Mapping-Enabled List. &id:avena::data::type::List;
   */
  template <class T>
  using List = avena::data::type::List<T>;

  /**
   * Abstract List.
   */
  typedef avena::data::type::AbstractList AbstractList;

  /**
   * Mapping-Enabled UnorderedSet. &id:avena::data::type::UnorderedSet;
   */
  template <class T>
  using UnorderedSet = avena::data::type::UnorderedSet<T>;

  /**
   * Abstract UnorderedSet.
   */
  typedef avena::data::type::AbstractUnorderedSet AbstractUnorderedSet;

  /**
   * Mapping-Enabled PairList<Key, Value>. &id:avena::data::type::PairList;
   */
  template <class Key, class Value>
  using PairList = avena::data::type::PairList<Key, Value>;

  /**
   * Mapping-Enabled PairList<String, Value>. &id:avena::data::type::PairList;
   */
  template <class Value>
  using Fields = avena::PairList<String, Value>;

  /**
   * Abstract Fields
   */
  typedef Fields<avena::Void> AbstractFields;

  /**
   * Mapping-Enabled UnorderedMap<Key, Value>. &id:avena::data::type::UnorderedMap;.
   */
  template <class Key, class Value>
  using UnorderedMap = avena::data::type::UnorderedMap<Key, Value>;

  /**
   * Mapping-Enabled UnorderedMap<String, Value>. &id:avena::data::type::UnorderedMap;.
   */
  template <class Value>
  using UnorderedFields = avena::UnorderedMap<String, Value>;

  /**
   * Abstract UnorderedFields
   */
  typedef UnorderedFields<avena::Void> AbstractUnorderedFields;
  
}

#endif /* oatpp_Types_hpp */
