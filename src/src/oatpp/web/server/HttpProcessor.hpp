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

#ifndef oatpp_web_server_HttpProcessor_hpp
#define oatpp_web_server_HttpProcessor_hpp

#include "./HttpRouter.hpp"

#include "./interceptor/RequestInterceptor.hpp"
#include "./interceptor/ResponseInterceptor.hpp"
#include "./handler/ErrorHandler.hpp"

#include "oatpp/web/protocol/http/encoding/ProviderCollection.hpp"

#include "oatpp/web/protocol/http/incoming/RequestHeadersReader.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"

#include "oatpp/web/protocol/http/outgoing/Response.hpp"
#include "oatpp/web/protocol/http/utils/CommunicationUtils.hpp"

#include "oatpp/data/stream/StreamBufferedProxy.hpp"
#include "oatpp/async/Processor.hpp"

namespace avena { namespace web { namespace server {

/**
 * HttpProcessor. Helper class to handle HTTP processing.
 */
class HttpProcessor {
public:
  typedef std::list<std::shared_ptr<web::server::interceptor::RequestInterceptor>> RequestInterceptors;
  typedef std::list<std::shared_ptr<web::server::interceptor::ResponseInterceptor>> ResponseInterceptors;
  typedef web::protocol::http::incoming::RequestHeadersReader RequestHeadersReader;
  typedef protocol::http::utils::CommunicationUtils::ConnectionState ConnectionState;

  HttpProcessor() = default;

public:

  /**
   * Resource config per connection.
   */
  struct Config {

    /**
     * Buffer used to read headers in request. Initial size of the buffer.
     */
    v_buff_size headersInBufferInitial = 2048;

    /**
     * Buffer used to write headers in response. Initial size of the buffer.
     */
    v_buff_size headersOutBufferInitial = 2048;

    /**
     * Size of the chunk used for iterative-read of headers.
     */
    v_buff_size headersReaderChunkSize = 2048;

    /**
     * Maximum allowed size of requests headers. The overall size of all headers in the request.
     */
    v_buff_size headersReaderMaxSize = 4096;

  };

public:

  /**
   * Collection of components needed to serve http-connection.
   */
  struct Components {

    /**
     * Constructor.
     * @param pRouter
     * @param pContentEncodingProviders
     * @param pBodyDecoder
     * @param pErrorHandler
     * @param pRequestInterceptors
     * @param pConfig
     */
    Components(const std::shared_ptr<HttpRouter>& pRouter,
               const std::shared_ptr<protocol::http::encoding::ProviderCollection>& pContentEncodingProviders,
               const std::shared_ptr<const avena::web::protocol::http::incoming::BodyDecoder>& pBodyDecoder,
               const std::shared_ptr<handler::ErrorHandler>& pErrorHandler,
               const RequestInterceptors& pRequestInterceptors,
               const ResponseInterceptors& pResponseInterceptors,
               const std::shared_ptr<Config>& pConfig);

    /**
     * Constructor.
     * @param pRouter
     */
    Components(const std::shared_ptr<HttpRouter>& pRouter);

    /**
     * Constructor.
     * @param pRouter
     * @param pConfig
     */
    Components(const std::shared_ptr<HttpRouter>& pRouter, const std::shared_ptr<Config>& pConfig);

    /**
     * Router to route incoming requests. &id:avena::web::server::HttpRouter;.
     */
    std::shared_ptr<HttpRouter> router;

    /**
     * Content-encoding providers. &id:avena::web::protocol::encoding::ProviderCollection;.
     */
    std::shared_ptr<protocol::http::encoding::ProviderCollection> contentEncodingProviders;

    /**
     * Body decoder. &id:avena::web::protocol::http::incoming::BodyDecoder;.
     */
    std::shared_ptr<const avena::web::protocol::http::incoming::BodyDecoder> bodyDecoder;

    /**
     * Error handler. &id:avena::web::server::handler::ErrorHandler;.
     */
    std::shared_ptr<handler::ErrorHandler> errorHandler;

    /**
     * Collection of request interceptors. &id:avena::web::server::interceptor::RequestInterceptor;.
     */
    RequestInterceptors requestInterceptors;

    /**
     * Collection of request interceptors. &id:avena::web::server::interceptor::ResponseInterceptor;.
     */
    ResponseInterceptors responseInterceptors;

    /**
     * Resource allocation config. &l:HttpProcessor::Config;.
     */
    std::shared_ptr<Config> config;

  };

private:

  struct ProcessingResources {

    ProcessingResources(const std::shared_ptr<Components>& pComponents,
                        const provider::ResourceHandle<avena::data::stream::IOStream>& pConnection);

    std::shared_ptr<Components> components;
    provider::ResourceHandle<avena::data::stream::IOStream> connection;
    avena::data::stream::BufferOutputStream headersInBuffer;
    avena::data::stream::BufferOutputStream headersOutBuffer;
    RequestHeadersReader headersReader;
    std::shared_ptr<avena::data::stream::InputStreamBufferedProxy> inStream;

  };

  static
  std::shared_ptr<protocol::http::outgoing::Response>
  processNextRequest(ProcessingResources& resources,
                     const std::shared_ptr<protocol::http::incoming::Request>& request,
                     ConnectionState& connectionState);
  static ConnectionState processNextRequest(ProcessingResources& resources);

public:

  /**
   * Listener of the connection processing task.
   */
  class TaskProcessingListener {
  public:
    virtual void onTaskStart(const provider::ResourceHandle<data::stream::IOStream>& connection) = 0;
    virtual void onTaskEnd(const provider::ResourceHandle<data::stream::IOStream>& connection) = 0;
  };

public:

  /**
   * Connection serving task. <br>
   * Usege example: <br>
   * `std::thread thread(&HttpProcessor::Task::run, HttpProcessor::Task(components, connection));`
   */
  class Task : public base::Countable {
  private:
    std::shared_ptr<Components> m_components;
    provider::ResourceHandle<avena::data::stream::IOStream> m_connection;
    TaskProcessingListener* m_taskListener;
  public:

    /**
     * Constructor.
     * @param components - &l:HttpProcessor::Components;.
     * @param connection - &id:avena::data::stream::IOStream;.
     */
    Task(const std::shared_ptr<Components>& components,
         const provider::ResourceHandle<avena::data::stream::IOStream>& connection,
         TaskProcessingListener* taskListener);

    Task(const Task&) = delete;
    Task &operator=(const Task&) = delete;

    /**
     * Move-Constructor to correclty count tasks;
     */
     Task(Task &&other);

     /**
      * Move-Assignment to correctly count tasks.
      * @param t
      * @return
      */
    Task &operator=(Task &&other);

    /**
     * Destructor, needed for counting.
     */
    ~Task() override;

  public:

    /**
     * Run loop.
     */
    void run();

  };
  
public:

  /**
   * Connection serving coroutiner - &id:avena::async::Coroutine;.
   */
  class Coroutine : public avena::async::Coroutine<HttpProcessor::Coroutine> {
  private:
    std::shared_ptr<Components> m_components;
    provider::ResourceHandle<avena::data::stream::IOStream> m_connection;
    avena::data::stream::BufferOutputStream m_headersInBuffer;
    RequestHeadersReader m_headersReader;
    std::shared_ptr<avena::data::stream::BufferOutputStream> m_headersOutBuffer;
    std::shared_ptr<avena::data::stream::InputStreamBufferedProxy> m_inStream;
    ConnectionState m_connectionState;
  private:
    avena::web::server::HttpRouter::BranchRouter::Route m_currentRoute;
    std::shared_ptr<protocol::http::incoming::Request> m_currentRequest;
    std::shared_ptr<protocol::http::outgoing::Response> m_currentResponse;
    TaskProcessingListener* m_taskListener;
  private:
    bool m_shouldInterceptResponse;
  public:

    /**
     * Constructor.
     * @param components - &l:HttpProcessor::Components;.
     * @param connection - &id:avena::data::stream::IOStream;.
     */
    Coroutine(const std::shared_ptr<Components>& components,
              const provider::ResourceHandle<avena::data::stream::IOStream>& connection,
              TaskProcessingListener* taskListener);

    ~Coroutine() override;

    Action act() override;

    Action parseHeaders();
    
    Action onHeadersParsed(const RequestHeadersReader::Result& headersReadResult);
    
    Action onRequestFormed();
    Action onResponse(const std::shared_ptr<protocol::http::outgoing::Response>& response);
    Action onResponseFormed();
    Action onRequestDone();
    
    Action handleError(Error* error) override;
    
  };
  
};
  
}}}

#endif /* oatpp_web_server_HttpProcessor_hpp */
