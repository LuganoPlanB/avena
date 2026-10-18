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

#ifndef oatpp_orm_Executor_hpp
#define oatpp_orm_Executor_hpp

#include "Connection.hpp"
#include "QueryResult.hpp"

#include "oatpp/data/mapping/TypeResolver.hpp"
#include "oatpp/data/type/Type.hpp"
#include "oatpp/data/share/MemoryLabel.hpp"
#include "oatpp/data/share/StringTemplate.hpp"

namespace avena { namespace orm {

/**
 * Database executor.
 */
class Executor {
public:
  typedef avena::data::share::StringTemplate StringTemplate;
  typedef std::unordered_map<data::share::StringKeyLabel, const avena::Type*> ParamsTypeMap;
protected:
  std::shared_ptr<data::mapping::TypeResolver> m_defaultTypeResolver;
public:

  /**
   * Default constructor.
   */
  Executor();

  /**
   * Default virtual destructor.
   */
  virtual ~Executor() = default;

  /**
   * Get default type resolver.
   * @return
   */
  std::shared_ptr<const data::mapping::TypeResolver> getDefaultTypeResolver();

  /**
   * Create new type resolver.
   * @return
   */
  virtual std::shared_ptr<data::mapping::TypeResolver> createTypeResolver() = 0;

  /**
   * Get database connection.
   * @return
   */
  virtual provider::ResourceHandle<Connection> getConnection() = 0;

  /**
   * Parse query template.
   * @param name - template name.
   * @param text - template text.
   * @param paramsTypeMap - template parameter types.
   * @param prepare - `true` if the query should use prepared statement, `false` otherwise.
   * @return - &id:avena::data::share::StringTemplate;.
   */
  virtual StringTemplate parseQueryTemplate(const avena::String& name,
                                            const avena::String& text,
                                            const ParamsTypeMap& paramsTypeMap,
                                            bool prepare = false) = 0;

  /**
   * Execute database query using a query template.
   * @param queryTemplate - a query template obtained in a prior call to &l:Executor::parseQueryTemplate (); method.
   * @param params - query parameters.
   * @param enabledInterpretations - enabled type interpretations.
   * @param connection - database connection.
   * @return - &id:avena::orm::QueryResult;.
   */
  virtual std::shared_ptr<QueryResult> execute(const StringTemplate& queryTemplate,
                                               const std::unordered_map<avena::String, avena::Void>& params,
                                               const std::shared_ptr<const data::mapping::TypeResolver>& typeResolver = nullptr,
                                               const provider::ResourceHandle<Connection>& connection = nullptr) = 0;

  /**
   * Execute an arbitrary database query. <br>
   * In its default implementation it'll call execute with the null-named query template.
   * The query template will be created by a call to `parseQueryTemplate(nullptr, query, {}, false)`.
   * @param query - query text.
   * @param params - query parameters.
   * @param enabledInterpretations - enabled type interpretations.
   * @param connection - database connection.
   * @return - &id:avena::orm::QueryResult;.
   */
  virtual std::shared_ptr<QueryResult> execute(const avena::String& query,
                                               const std::unordered_map<avena::String, avena::Void>& params,
                                               const std::shared_ptr<const data::mapping::TypeResolver>& typeResolver = nullptr,
                                               const provider::ResourceHandle<Connection>& connection = nullptr);

  /**
   * Begin database transaction. Should NOT be used directly. Use &id:avena::orm::Transaction; instead.
   * @param connection - database connection.
   * @return - &id:avena::orm::QueryResult;.
   */
  virtual std::shared_ptr<QueryResult> begin(const provider::ResourceHandle<Connection>& connection = nullptr) = 0;

  /**
   * Commit database transaction. Should NOT be used directly. Use &id:avena::orm::Transaction; instead.
   * @param connection
   * @return - &id:avena::orm::QueryResult;.
   */
  virtual std::shared_ptr<QueryResult> commit(const provider::ResourceHandle<Connection>& connection) = 0;

  /**
   * Rollback database transaction. Should NOT be used directly. Use &id:avena::orm::Transaction; instead.
   * @param connection
   * @return - &id:avena::orm::QueryResult;.
   */
  virtual std::shared_ptr<QueryResult> rollback(const provider::ResourceHandle<Connection>& connection) = 0;

  /**
   * Get current database schema version.
   * @param suffix - suffix of schema version control table name.
   * @param connection - database connection.
   * @return - schema version.
   */
  virtual v_int64 getSchemaVersion(const avena::String& suffix = nullptr,
                                   const provider::ResourceHandle<Connection>& connection = nullptr) = 0;

  /**
   * Run schema migration script. Should NOT be used directly. Use &id:avena::orm::SchemaMigration; instead.
   * @param script - script text.
   * @param newVersion - schema version corresponding to this script.
   * @param suffix - suffix of schema version control table name.
   * @param connection - database connection.
   */
  virtual void migrateSchema(const avena::String& script,
                             v_int64 newVersion,
                             const avena::String& suffix = nullptr,
                             const provider::ResourceHandle<Connection>& connection = nullptr) = 0;

};

}}

#endif // oatpp_orm_Executor_hpp
