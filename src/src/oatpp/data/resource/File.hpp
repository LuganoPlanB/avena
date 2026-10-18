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

#ifndef oatpp_data_resource_File_hpp
#define oatpp_data_resource_File_hpp

#include "./Resource.hpp"
#include "oatpp/data/stream/Stream.hpp"

namespace avena { namespace data { namespace resource {

/**
 * File.
 * @extends - &id:avena::data::Resource;.
 */
class File : public Resource {
private:
  
  struct FileHandle {

    avena::String fileName;

    FileHandle(const avena::String& fullFileName)
      : fileName(fullFileName)
    {}

  };

public:
  static avena::String concatDirAndName(const avena::String& dir, const avena::String& filename);
private:
  std::shared_ptr<FileHandle> m_handle;
public:

  /**
   * Default constructor.
   */
  File() = default;

  /**
   * Constructor.
   * @param fullFilename
   */
  File(const avena::String& fullFilename);

  /**
   * Constructor.
   * @param directory
   * @param filename
   */
  File(const avena::String& directory, const avena::String& filename);

  /**
   * Open output stream to a file. <br>
   * *Note: stream also captures file-handle. The file object won't be deleted until the stream is deleted.*
   * @return - `std::shared_ptr` to &id:avena::data::stream::OutputStream;.
   */
  std::shared_ptr<data::stream::OutputStream> openOutputStream() override;

  /**
   * Open input stream to a temporary file. <br>
   * *Note: stream also captures file-handle. The file won't be deleted until the stream is deleted.*
   * @return - `std::shared_ptr` &id:avena::data::stream::InputStream;.
   */
  std::shared_ptr<data::stream::InputStream> openInputStream() override;

  /**
   * Not applicable.
   * @return - always returns `nullptr`.
   */
  avena::String getInMemoryData() override;

  /**
   * Not applicable.
   * @return - always returns `-1`.
   */
  v_int64 getKnownSize() override;

  /**
   * Get location where temporary data is stored.
   * @return - `&id:avena::String;`.
   */
  avena::String getLocation() override;

};

}}}

#endif //oatpp_data_resource_File_hpp
