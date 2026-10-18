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

#include "File.hpp"

#include "oatpp/data/stream/FileStream.hpp"

namespace avena { namespace data { namespace resource {

avena::String File::concatDirAndName(const avena::String& dir, const avena::String& filename) {
  if(dir && dir->size() > 0) {
    auto lastChar = dir->data()[dir->size() - 1];
    if(lastChar != '/' && lastChar != '\\') {
      return dir + "/" + filename;
    }
    return dir + filename;
  }
  return filename;
}

File::File(const avena::String& fullFileName)
  : m_handle(std::make_shared<FileHandle>(fullFileName))
{}

File::File(const avena::String& tmpDirectory, const avena::String& tmpFileName)
  : m_handle(std::make_shared<FileHandle>(concatDirAndName(tmpDirectory, tmpFileName)))
{}

std::shared_ptr<data::stream::OutputStream> File::openOutputStream() {
  if(m_handle) {
    return std::make_shared<data::stream::FileOutputStream>(m_handle->fileName->c_str(), "wb", m_handle);
  }
  throw std::runtime_error("[avena::data::resource::File::openOutputStream()]: Error. FileHandle is NOT initialized.");
}

std::shared_ptr<data::stream::InputStream> File::openInputStream() {
  if(m_handle) {
    return std::make_shared<data::stream::FileInputStream>(m_handle->fileName->c_str(), m_handle);
  }
  throw std::runtime_error("[avena::data::resource::File::openInputStream()]: Error. FileHandle is NOT initialized.");
}

avena::String File::getInMemoryData() {
  return nullptr;
}

v_int64 File::getKnownSize() {
  return -1;
}

avena::String File::getLocation() {
  if(m_handle) {
    return m_handle->fileName;
  }
  return nullptr;
}

}}}
