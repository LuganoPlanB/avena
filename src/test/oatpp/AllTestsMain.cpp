// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.

#include "oatpp/web/ClientRetryTest.hpp"
#include "oatpp/web/FullTest.hpp"
#include "oatpp/web/FullAsyncTest.hpp"
#include "oatpp/web/FullAsyncClientTest.hpp"
#include "oatpp/web/PipelineTest.hpp"
#include "oatpp/web/PipelineAsyncTest.hpp"
#include "oatpp/web/protocol/http/encoding/ChunkedTest.hpp"
#include "oatpp/web/server/api/ApiControllerTest.hpp"
#include "oatpp/web/server/handler/AuthorizationHandlerTest.hpp"
#include "oatpp/web/server/HttpRouterTest.hpp"
#include "oatpp/web/server/ServerStopTest.hpp"
#include "oatpp/web/mime/multipart/StatefulParserTest.hpp"
#include "oatpp/web/mime/ContentMappersTest.hpp"

#include "oatpp/network/virtual_/PipeTest.hpp"
#include "oatpp/network/virtual_/InterfaceTest.hpp"
#include "oatpp/network/UrlTest.hpp"
#include "oatpp/network/ConnectionPoolTest.hpp"
#include "oatpp/network/monitor/ConnectionMonitorTest.hpp"

#include "oatpp/json/DeserializerTest.hpp"
#include "oatpp/json/DTOMapperPerfTest.hpp"
#include "oatpp/json/DTOMapperTest.hpp"
#include "oatpp/json/EnumTest.hpp"
#include "oatpp/json/BooleanTest.hpp"
#include "oatpp/json/UnorderedSetTest.hpp"

#include "oatpp/encoding/Base64Test.hpp"
#include "oatpp/encoding/HexTest.hpp"
#include "oatpp/encoding/UnicodeTest.hpp"
#include "oatpp/encoding/UrlTest.hpp"

#include "oatpp/utils/parser/CaretTest.hpp"
#include "oatpp/provider/PoolTest.hpp"
#include "oatpp/provider/PoolTemplateTest.hpp"
#include "oatpp/async/ConditionVariableTest.hpp"
#include "oatpp/async/LockTest.hpp"

#include "oatpp/data/type/UnorderedMapTest.hpp"
#include "oatpp/data/type/PairListTest.hpp"
#include "oatpp/data/type/VectorTest.hpp"
#include "oatpp/data/type/UnorderedSetTest.hpp"
#include "oatpp/data/type/ListTest.hpp"
#include "oatpp/data/type/ObjectTest.hpp"
#include "oatpp/data/type/StringTest.hpp"
#include "oatpp/data/type/PrimitiveTest.hpp"
#include "oatpp/data/type/ObjectWrapperTest.hpp"
#include "oatpp/data/type/TypeTest.hpp"
#include "oatpp/data/type/AnyTest.hpp"
#include "oatpp/data/type/EnumTest.hpp"
#include "oatpp/data/type/InterpretationTest.hpp"
#include "oatpp/data/mapping/TypeResolverTest.hpp"

#include "oatpp/data/resource/InMemoryDataTest.hpp"

#include "oatpp/data/stream/BufferStreamTest.hpp"

#include "oatpp/data/mapping/TreeTest.hpp"
#include "oatpp/data/mapping/ObjectToTreeMapperTest.hpp"
#include "oatpp/data/mapping/TreeToObjectMapperTest.hpp"
#include "oatpp/data/mapping/ObjectRemapperTest.hpp"

#include "oatpp/data/share/LazyStringMapTest.hpp"
#include "oatpp/data/share/StringTemplateTest.hpp"
#include "oatpp/data/share/MemoryLabelTest.hpp"
#include "oatpp/data/buffer/ProcessorTest.hpp"

#include "oatpp/base/CommandLineArgumentsTest.hpp"
#include "oatpp/base/LogTest.hpp"

#include "oatpp/LoggerTest.hpp"

#include "oatpp/async/Coroutine.hpp"

#include "oatpp/data/mapping/Tree.hpp"

#include "oatpp/Environment.hpp"

#include <iostream>
#include <mutex>

namespace {

void runTests() {

  avena::Environment::printCompilationConfig();

  OATPP_LOGd("Tests", "avena::String size={}", sizeof(avena::String))

  OATPP_LOGd("Tests", "avena::String size={}", sizeof(avena::String))
  OATPP_LOGd("Tests", "std::string size={}", sizeof(std::string))
  OATPP_LOGd("Tests", "Vector size={}", sizeof(std::vector<int>))
  OATPP_LOGd("Tests", "Map size={}", sizeof(std::unordered_map<avena::String, avena::String>))
  OATPP_LOGd("Tests", "Tree size={}", sizeof(avena::data::mapping::Tree))

  OATPP_LOGd("Tests", "coroutine handle size={}", sizeof(avena::async::CoroutineHandle))
  OATPP_LOGd("Tests", "coroutine size={}", sizeof(avena::async::AbstractCoroutine))
  OATPP_LOGd("Tests", "action size={}", sizeof(avena::async::Action))
  OATPP_LOGd("Tests", "class count={}", avena::data::type::ClassId::getClassCount())

  auto names = avena::data::type::ClassId::getRegisteredClassNames();
  v_int32 i = 0;
  for(auto& name : names) {
    OATPP_LOGd("CLASS", "{} --> '{}'", i, name)
    i ++;
  }

  OATPP_RUN_TEST(avena::test::LoggerTest);
  OATPP_RUN_TEST(avena::base::CommandLineArgumentsTest);
  OATPP_RUN_TEST(avena::base::LogTest);

  OATPP_RUN_TEST(avena::data::share::MemoryLabelTest);
  OATPP_RUN_TEST(avena::data::share::LazyStringMapTest);
  OATPP_RUN_TEST(avena::data::share::StringTemplateTest);

  OATPP_RUN_TEST(avena::data::buffer::ProcessorTest);
  OATPP_RUN_TEST(avena::data::stream::BufferStreamTest);

  OATPP_RUN_TEST(avena::data::mapping::TreeTest);
  OATPP_RUN_TEST(avena::data::mapping::ObjectToTreeMapperTest);
  OATPP_RUN_TEST(avena::data::mapping::TreeToObjectMapperTest);
  OATPP_RUN_TEST(avena::data::mapping::ObjectRemapperTest);

  OATPP_RUN_TEST(avena::data::type::ObjectWrapperTest);
  OATPP_RUN_TEST(avena::data::type::TypeTest);

  OATPP_RUN_TEST(avena::data::type::StringTest);

  OATPP_RUN_TEST(avena::data::type::PrimitiveTest);
  OATPP_RUN_TEST(avena::data::type::ListTest);
  OATPP_RUN_TEST(avena::data::type::VectorTest);
  OATPP_RUN_TEST(avena::data::type::UnorderedSetTest);
  OATPP_RUN_TEST(avena::data::type::PairListTest);
  OATPP_RUN_TEST(avena::data::type::UnorderedMapTest);
  OATPP_RUN_TEST(avena::data::type::AnyTest);
  OATPP_RUN_TEST(avena::data::type::EnumTest);

  OATPP_RUN_TEST(avena::data::type::ObjectTest);

  OATPP_RUN_TEST(avena::data::type::InterpretationTest);
  OATPP_RUN_TEST(avena::data::mapping::TypeResolverTest);

  OATPP_RUN_TEST(avena::data::resource::InMemoryDataTest);

  OATPP_RUN_TEST(avena::async::ConditionVariableTest);
  OATPP_RUN_TEST(avena::async::LockTest);

  OATPP_RUN_TEST(avena::utils::parser::CaretTest);

  OATPP_RUN_TEST(avena::provider::PoolTest);
  OATPP_RUN_TEST(avena::provider::PoolTemplateTest);

  OATPP_RUN_TEST(avena::json::EnumTest);
  OATPP_RUN_TEST(avena::json::BooleanTest);

  OATPP_RUN_TEST(avena::json::UnorderedSetTest);

  OATPP_RUN_TEST(avena::json::DeserializerTest);

  OATPP_RUN_TEST(avena::json::DTOMapperPerfTest);

  OATPP_RUN_TEST(avena::json::DTOMapperTest);
  OATPP_RUN_TEST(avena::test::encoding::Base64Test);
  OATPP_RUN_TEST(avena::encoding::HexTest);
  OATPP_RUN_TEST(avena::test::encoding::UnicodeTest);
  OATPP_RUN_TEST(avena::test::encoding::UrlTest);

  OATPP_RUN_TEST(avena::test::network::UrlTest);
  OATPP_RUN_TEST(avena::test::network::ConnectionPoolTest);
  OATPP_RUN_TEST(avena::test::network::monitor::ConnectionMonitorTest);
  OATPP_RUN_TEST(avena::test::network::virtual_::PipeTest);
  OATPP_RUN_TEST(avena::test::network::virtual_::InterfaceTest);

  OATPP_RUN_TEST(avena::test::web::protocol::http::encoding::ChunkedTest);

  OATPP_RUN_TEST(avena::test::web::mime::multipart::StatefulParserTest);
  OATPP_RUN_TEST(avena::web::mime::ContentMappersTest);

  OATPP_RUN_TEST(avena::test::web::server::HttpRouterTest);
  OATPP_RUN_TEST(avena::test::web::server::api::ApiControllerTest);
  OATPP_RUN_TEST(avena::test::web::server::handler::AuthorizationHandlerTest);

  {

    avena::test::web::server::ServerStopTest test_virtual(0);
    test_virtual.run();

    avena::test::web::server::ServerStopTest test_port(8000);
    test_port.run();

  }

  {

    avena::test::web::PipelineTest test_virtual(0, 3000);
    test_virtual.run();

    avena::test::web::PipelineTest test_port(8000, 3000);
    test_port.run();

  }

  {

    avena::test::web::PipelineAsyncTest test_virtual(0, 3000);
    test_virtual.run();

    avena::test::web::PipelineAsyncTest test_port(8000, 3000);
    test_port.run();

  }

  {

    avena::test::web::FullTest test_virtual(0, 1000);
    test_virtual.run();

    avena::test::web::FullTest test_port(8000, 5);
    test_port.run();

  }

  {

    avena::test::web::FullAsyncTest test_virtual(0, 1000);
    test_virtual.run();

    avena::test::web::FullAsyncTest test_port(8000, 5);
    test_port.run();

  }

  {

    avena::test::web::FullAsyncClientTest test_virtual(0, 1000);
    test_virtual.run(20);

    avena::test::web::FullAsyncClientTest test_port(8000, 5);
    test_port.run(1);

  }

  {

    avena::test::web::ClientRetryTest test_virtual(0);
    test_virtual.run();

    avena::test::web::ClientRetryTest test_port(8000);
    test_port.run();

  }

}

}

int main() {

  avena::Environment::init();

  runTests();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << avena::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << avena::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(avena::Environment::getObjectsCount() == 0)

  avena::Environment::destroy();

  return 0;
}

