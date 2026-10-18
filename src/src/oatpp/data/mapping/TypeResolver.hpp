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

#ifndef oatpp_data_mapping_TypeResolver_hpp
#define oatpp_data_mapping_TypeResolver_hpp

#include "oatpp/Types.hpp"

namespace avena { namespace data { namespace mapping {

/**
 * Helper class to work with oatpp types.
 */
class TypeResolver {
public:

  /**
   * Local resolution cache used to reduce number of type interpretation iterations.
   */
  struct Cache {
    /**
     * types map.
     */
    std::unordered_map<const avena::Type*, const avena::Type*> types;

    /**
     * values by type map.
     */
    std::unordered_map<const avena::Type*, std::unordered_map<avena::Void, avena::Void>> values;
  };

private:

  const avena::Type* findPropertyType(const avena::Type* baseType,
                                      const std::vector<std::string>& path,
                                      v_uint32 pathPosition,
                                      Cache& cache) const;

  avena::Void findPropertyValue(const avena::Void& baseObject,
                                const std::vector<std::string>& path,
                                v_uint32 pathPosition,
                                Cache& cache) const;

private:
  std::vector<bool> m_knownClasses;
  std::vector<std::string> m_enabledInterpretations;
public:

  /**
   * Default constructor.
   */
  TypeResolver();

  /**
   * Virtual destructor.
   */
  virtual ~TypeResolver() = default;

  /**
   * Set if the type class is considered known/unknown
   * @param classId
   * @param isKnown
   */
  void setKnownClass(const avena::ClassId& classId, bool isKnown);

  /**
   * Set all mentioned type classes as known.
   * @param knownClasses
   */
  void addKnownClasses(const std::vector<avena::ClassId>& knownClasses);

  /**
   * Check if type class is known.
   * @param classId
   * @return
   */
  bool isKnownClass(const avena::ClassId& classId) const;

  /**
   * Check if type is known.
   * @param type
   * @return
   */
  bool isKnownType(const avena::Type* type) const;

  /**
   * Set enabled type interpretations.
   * @param interpretations
   */
  void setEnabledInterpretations(const std::vector<std::string>& interpretations);

  /**
   * Get enabled type interpretations.
   * @return
   */
  const std::vector<std::string>& getEnabledInterpretations() const;

  /**
   * Resolve unknown type according to enabled interpretations.
   * @param type - type to resolve.
   * @param cache - local cache.
   * @return
   */
  const avena::Type* resolveType(const avena::Type* type, Cache& cache) const;

  /**
   * Resolve unknown value according to enabled interpretations.
   * @param value - value to resolve.
   * @param cache - local cache.
   * @return
   */
  avena::Void resolveValue(const avena::Void& value, Cache& cache) const;

  /**
   * Traverse object property tree resolving unknown types according to enabled interpretations.
   * @param objectType - base object type.
   * @param path - vector of property names.
   * @param cache - local cache.
   * @return - &id:avena::Type;. `nullptr` - if couldn't resolve.
   */
  const avena::Type* resolveObjectPropertyType(const avena::Type* objectType,
                                              const std::vector<std::string>& path,
                                              Cache& cache) const;

  /**
   * Traverse object property tree resolving unknown value types according to enabled interpretations.
   * @param object - base object.
   * @param path - vector of property names.
   * @param cache - local cache.
   * @return - value as &id:avena::Void;. The `valueType` will be set to resolved type
   * or to `avena::Void::Class::getType()` if couldn't resolve.
   */
  avena::Void resolveObjectPropertyValue(const avena::Void& object,
                                         const std::vector<std::string>& path,
                                         Cache& cache) const;

};

}}}

#endif // oatpp_data_mapping_TypeResolver_hpp
