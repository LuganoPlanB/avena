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

#ifndef oatpp_orm_SchemaMigration_hpp
#define oatpp_orm_SchemaMigration_hpp

#include "Executor.hpp"

namespace avena { namespace orm {

/**
 * Database schema migration helper.
 */
class SchemaMigration {
private:

  static constexpr v_int32 SOURCE_TEXT = 0;
  static constexpr v_int32 SOURCE_FILE = 1;

  struct Source {
    v_int64 version;
    v_int32 type;
    avena::String param;
  };

private:
  base::ObjectHandle<Executor> m_executor;
  avena::String m_suffix;
  std::vector<Source> m_scripts;
public:

  /**
   * Constructor.
   * @param executor - &id:avena::orm::Executor;.
   * @param suffix - suffix of schema version control table.
   */
  SchemaMigration(const base::ObjectHandle<Executor>& executor, const avena::String& suffix = nullptr);

  /**
   * Default virtual destructor.
   */
  virtual ~SchemaMigration() = default;

  /**
   * Add migration script as text.
   * @param version - schema version corresponding to this script.
   * @param script - script text.
   */
  void addText(v_int64 version, const avena::String& script);

  /**
   * Add migration script file.
   * @param version - schema version corresponding to this script.
   * @param script - path to script file.
   */
  void addFile(v_int64 version, const avena::String& filename);

  /**
   * Run database schema migration.
   */
  void migrate();

  /**
   * Get current database schema version.
   * @return - schema version.
   */
  v_int64 getSchemaVersion();

};

}}

#endif // oatpp_orm_SchemaMigration_hpp
