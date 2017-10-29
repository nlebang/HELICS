/*
Copyright (C) 2017, Battelle Memorial Institute
All rights reserved.

This software was co-developed by Pacific Northwest National Laboratory, operated by the Battelle Memorial
Institute; the National Renewable Energy Laboratory, operated by the Alliance for Sustainable Energy, LLC; and the
Lawrence Livermore National Laboratory, operated by Lawrence Livermore National Security, LLC.

*/
#include <boost/test/unit_test.hpp>

#include <boost/asio/ip/udp.hpp>
#include "helics/core/ActionMessage.h"
#include "helics/core/BrokerFactory.h"
#include "helics/core/CoreFactory.h"
#include "helics/core/core-types.h"
#include "helics/core/core.h"
#include "helics/core/udp/UdpBroker.h"
#include "helics/core/udp/UdpComms.h"
#include "helics/core/udp/UdpCore.h"
#include "helics/common/AsioServiceManager.h"

//#include "boost/process.hpp"
#include <future>

BOOST_AUTO_TEST_SUITE (UDPCore_tests)

using helics::Core;
using boost::asio::ip::udp;

/*
BOOST_AUTO_TEST_CASE (zmqComms_broker_test)
{
    std::atomic<int> counter{0};
    std::string host = "tcp://127.0.0.1";
    helics::ZmqComms comm (host, host);

    auto ctx = zmqContextManager::getContextPointer ();
    zmq::socket_t repSocket (ctx->getContext (), ZMQ_REP);
    repSocket.bind ("tcp://127.0.0.1:23405");

    comm.setCallback ([&counter](helics::ActionMessage m) { ++counter; });
    comm.setBrokerPorts (23405);
    comm.setName ("tests");
    auto confut = std::async (std::launch::async, [&comm]() { return comm.connect (); });

    zmq::message_t rxmsg;

    repSocket.recv (&rxmsg);

    BOOST_CHECK_GT (rxmsg.size (), 32);

    helics::ActionMessage rM (static_cast<char *> (rxmsg.data ()), rxmsg.size ());
    BOOST_CHECK (rM.action () == helics::action_message_def::action_t::cmd_protocol);
    rM.index = DISCONNECT;
    repSocket.send (rM.to_string ());
    auto connected = confut.get ();
    BOOST_CHECK (!connected);
}
*/
/** test the request set class with various scenarios*/
/*
BOOST_AUTO_TEST_CASE (zmqRequestSet_test1)
{
    std::string host = "tcp://127.0.0.1";

    helics::ZmqRequestSets reqset;

    auto ctx = zmqContextManager::getContextPointer ();
    zmq::socket_t repSocket1 (ctx->getContext (), ZMQ_REP);
    repSocket1.bind ("tcp://127.0.0.1:23405");
    zmq::socket_t repSocket2 (ctx->getContext (), ZMQ_REP);
    repSocket2.bind ("tcp://127.0.0.1:23406");
    zmq::socket_t repSocket3 (ctx->getContext (), ZMQ_REP);
    repSocket3.bind ("tcp://127.0.0.1:23407");

    reqset.addRoutes (1, "tcp://127.0.0.1:23405");
    reqset.addRoutes (2, "tcp://127.0.0.1:23406");
    reqset.addRoutes (3, "tcp://127.0.0.1:23407");

    helics::ActionMessage M (helics::CMD_IGNORE);
    M.index = 1;

    reqset.transmit (1, M);
    BOOST_CHECK (reqset.waiting ());

    zmq::message_t msg;
    repSocket1.recv (&msg);

    repSocket1.send (msg);
    // should still be waiting
    BOOST_CHECK (reqset.waiting ());
    auto msgCnt = reqset.checkForMessages ();
    BOOST_CHECK (!reqset.waiting ());
    BOOST_CHECK_EQUAL (msgCnt, 1);

    auto M2 = reqset.getMessage ();

    BOOST_CHECK (M2->action () == helics::CMD_IGNORE);

    // send two messages
    reqset.transmit (2, M);
    reqset.transmit (2, M);
    BOOST_CHECK (reqset.waiting ());

    repSocket2.recv (&msg);

    repSocket2.send (msg);
    reqset.checkForMessages ();
    BOOST_CHECK (reqset.waiting ());
    repSocket2.recv (&msg);

    repSocket2.send (msg);
    reqset.checkForMessages ();
    BOOST_CHECK (!reqset.waiting ());

    BOOST_CHECK (reqset.hasMessages ());
}
*/
/** test the request set class with various scenarios*/
/*
BOOST_AUTO_TEST_CASE (zmqRequestSet_test2)
{
    std::string host = "tcp://127.0.0.1";

    helics::ZmqRequestSets reqset;

    auto ctx = zmqContextManager::getContextPointer ();
    zmq::socket_t repSocket1 (ctx->getContext (), ZMQ_REP);
    repSocket1.bind ("tcp://127.0.0.1:23405");
    zmq::socket_t repSocket2 (ctx->getContext (), ZMQ_REP);
    repSocket2.bind ("tcp://127.0.0.1:23406");
    zmq::socket_t repSocket3 (ctx->getContext (), ZMQ_REP);
    repSocket3.bind ("tcp://127.0.0.1:23407");

    reqset.addRoutes (1, "tcp://127.0.0.1:23405");
    reqset.addRoutes (2, "tcp://127.0.0.1:23406");
    reqset.addRoutes (3, "tcp://127.0.0.1:23407");

    helics::ActionMessage M (helics::CMD_IGNORE);
    M.index = 1;

    reqset.transmit (1, M);
    reqset.transmit (2, M);
    reqset.transmit (3, M);
    zmq::message_t msg;
    repSocket1.recv (&msg);

    repSocket1.send (msg);

    repSocket2.recv (&msg);

    repSocket2.send (msg);
    repSocket3.recv (&msg);

    repSocket3.send (msg);
    // make sure the check receives all messages
    reqset.checkForMessages ();
    BOOST_CHECK (!reqset.waiting ());
    reqset.transmit (1, M);
    reqset.transmit (2, M);
    reqset.transmit (3, M);
    reqset.transmit (1, M);
    reqset.transmit (2, M);
    reqset.transmit (3, M);
    reqset.transmit (1, M);
    reqset.transmit (2, M);
    reqset.transmit (3, M);

    repSocket1.recv (&msg);

    repSocket1.send (msg);

    repSocket2.recv (&msg);

    repSocket2.send (msg);
    repSocket3.recv (&msg);

    repSocket3.send (msg);
    BOOST_CHECK_EQUAL (reqset.checkForMessages (), 6);
}
*/

BOOST_AUTO_TEST_CASE (udpComms_broker_test_transmit)
{
    std::atomic<int> counter{0};
    std::string host = "localhost";
    helics::UdpComms comm (host,host);

    auto srv = AsioServiceManager::getServicePointer ();

    udp::socket rxSocket(AsioServiceManager::getService(), udp::endpoint(udp::v4(), 23901));

    BOOST_CHECK(rxSocket.is_open());
    comm.setCallback ([&counter](helics::ActionMessage m) { ++counter; });
    comm.setBrokerPort (23901);
    comm.setPortNumber (23903);
    comm.setName ("tests");
    bool connected = comm.connect ();
    BOOST_REQUIRE (connected);
    comm.transmit (0, helics::CMD_IGNORE);
    
    std::vector<char> data(1024);

    udp::endpoint remote_endpoint;
    boost::system::error_code error;
   auto len=rxSocket.receive_from(boost::asio::buffer(data),
        remote_endpoint, 0, error);


    BOOST_CHECK_GT (len, 32);
    helics::ActionMessage rM (data.data(),len);
    BOOST_CHECK (rM.action () == helics::action_message_def::action_t::cmd_ignore);
    comm.disconnect ();
}


BOOST_AUTO_TEST_CASE (zmqComms_rx_test)
{
    std::atomic<int> counter{0};
    helics::ActionMessage act;
    std::string host = "localhost";
    helics::UdpComms comm(host, host);

    auto srv = AsioServiceManager::getServicePointer();

    udp::resolver resolver(AsioServiceManager::getService());
    udp::socket rxSocket(AsioServiceManager::getService(), udp::endpoint(udp::v4(), 23901));

    BOOST_CHECK(rxSocket.is_open());
    comm.setCallback([&counter, &act](helics::ActionMessage m) {
        ++counter;
        act = m;
    });
    comm.setBrokerPort(23901);
    comm.setPortNumber(23903);
    comm.setName("tests");

   
    comm.setCallback ([&counter, &act](helics::ActionMessage m) {
        ++counter;
        act = m;
    });

    bool connected = comm.connect ();
    BOOST_REQUIRE (connected);

    udp::resolver::query queryNew(udp::v4(),"localhost", "23903");

    auto txendpoint = *resolver.resolve(queryNew);

    helics::ActionMessage cmd (helics::CMD_ACK);
    std::string buffer = cmd.to_string ();
    
    auto cnt = rxSocket.send_to (boost::asio::buffer(buffer), txendpoint);
    BOOST_REQUIRE_EQUAL (cnt, buffer.size ());
   

    std::this_thread::sleep_for (std::chrono::milliseconds (200));
    BOOST_REQUIRE_EQUAL (counter, 1);
    BOOST_CHECK (act.action () == helics::action_message_def::action_t::cmd_ack);
    comm.disconnect ();
}


BOOST_AUTO_TEST_CASE (zmqComm_transmit_through)
{
    std::atomic<int> counter{0};
    std::atomic<int> counter2{0};
    helics::ActionMessage act;
    helics::ActionMessage act2;

    std::string host = "localhost";
    helics::UdpComms comm (host, host);
    helics::UdpComms comm2 (host, "");

    comm.setBrokerPort(23901);
    comm.setName ("tests");
    comm2.setName ("test2");
    comm2.setPortNumber (23901);
    comm.setPortNumber (23908);

    comm.setCallback ([&counter, &act](helics::ActionMessage m) {
        ++counter;
        act = m;
    });
    comm2.setCallback ([&counter2, &act2](helics::ActionMessage m) {
        ++counter2;
        act2 = m;
    });

    // need to launch the connection commands at the same time since they depend on eachother in this case
    auto connected_fut = std::async (std::launch::async, [&comm] { return comm.connect (); });

    bool connected = comm2.connect ();
    BOOST_REQUIRE (connected);
    connected = connected_fut.get ();
    BOOST_REQUIRE (connected);

    comm.transmit (0, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    if (counter2 != 1)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds (500));
    }
    BOOST_REQUIRE_EQUAL (counter2, 1);
    BOOST_CHECK (act2.action () == helics::action_message_def::action_t::cmd_ack);

    comm.disconnect ();
    comm2.disconnect ();
}


BOOST_AUTO_TEST_CASE (zmqComm_transmit_add_route)
{
    std::atomic<int> counter{0};
    std::atomic<int> counter2{0};
    std::atomic<int> counter3{0};

    std::string host = "localhost";
    helics::UdpComms comm (host, host);
    helics::UdpComms comm2 (host, "");
    helics::UdpComms comm3 (host, host);

    comm.setBrokerPort (23901);
    comm.setName ("tests");
    comm2.setName ("broker");
    comm3.setName ("test3");
    comm3.setBrokerPort (23901);

    comm2.setPortNumber (23901);
    comm.setPortNumber (23905);
    comm3.setPortNumber (23920);

    helics::ActionMessage act;
    helics::ActionMessage act2;
    helics::ActionMessage act3;

    comm.setCallback ([&counter, &act](helics::ActionMessage m) {
        ++counter;
        act = m;
    });
    comm2.setCallback ([&counter2, &act2](helics::ActionMessage m) {
        ++counter2;
        act2 = m;
    });
    comm3.setCallback ([&counter3, &act3](helics::ActionMessage m) {
        ++counter3;
        act3 = m;
    });

    // need to launch the connection commands at the same time since they depend on eachother in this case
    // auto connected_fut = std::async(std::launch::async, [&comm] {return comm.connect(); });

    bool connected = comm2.connect ();
    BOOST_REQUIRE (connected);
    // connected = connected_fut.get();
    connected = comm.connect ();
    BOOST_REQUIRE (connected);
    connected = comm3.connect ();

    comm.transmit (0, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    BOOST_REQUIRE_EQUAL (counter2, 1);
    BOOST_CHECK (act2.action () == helics::action_message_def::action_t::cmd_ack);

    comm3.transmit (0, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    BOOST_REQUIRE_EQUAL (counter2, 2);
    BOOST_CHECK (act2.action () == helics::action_message_def::action_t::cmd_ack);

    comm2.addRoute (3, comm3.getAddress ());

    comm2.transmit (3, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    if (counter3 != 1)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds (250));
    }
    BOOST_REQUIRE_EQUAL (counter3, 1);
    BOOST_CHECK (act3.action () == helics::action_message_def::action_t::cmd_ack);

    comm2.addRoute (4, comm.getAddress ());

    comm2.transmit (4, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    BOOST_REQUIRE_EQUAL (counter, 1);
    BOOST_CHECK (act.action () == helics::action_message_def::action_t::cmd_ack);

    comm.disconnect ();
    comm2.disconnect ();
    comm3.disconnect ();
}

/*
BOOST_AUTO_TEST_CASE (zmqCore_initialization_test)
{
    std::string initializationString =
      "1 --brokerport=23405 --repport=23410 --local_interface=tcp://127.0.0.1 --name=core1";
    auto core = helics::CoreFactory::create (helics::core_type::ZMQ, initializationString);

    BOOST_REQUIRE (core != nullptr);
    BOOST_CHECK (core->isInitialized ());
    auto ctx = zmqContextManager::getContextPointer ();
    zmq::socket_t repSocket (ctx->getContext (), ZMQ_REP);
    repSocket.bind ("tcp://127.0.0.1:23405");

    zmq::socket_t pullSocket (ctx->getContext (), ZMQ_PULL);
    pullSocket.bind ("tcp://127.0.0.1:23406");
    bool connected = core->connect ();
    BOOST_REQUIRE (connected);

    zmq::message_t rxmsg;

    repSocket.recv (&rxmsg);

    BOOST_CHECK_GT (rxmsg.size (), 32);
    helics::ActionMessage rM (static_cast<char *> (rxmsg.data ()), rxmsg.size ());

    BOOST_CHECK_EQUAL (rM.name, "core1");
    BOOST_CHECK (rM.action () == helics::action_message_def::action_t::cmd_reg_broker);
    helics::ActionMessage resp (helics::CMD_PRIORITY_ACK);
    repSocket.send (resp.to_string ());

    core->disconnect ();
    // boost::interprocess::message_queue::remove("testbroker");
}
*/
/** test case checks default values and makes sure they all mesh together
also tests the automatic port determination for cores
*/
/*
BOOST_AUTO_TEST_CASE (zmqCore_core_broker_default_test)
{
    std::string initializationString = "1";

    auto broker = helics::BrokerFactory::create (helics::core_type::ZMQ, initializationString);

    auto core = helics::CoreFactory::create (helics::core_type::ZMQ, initializationString);
    bool connected = broker->isConnected ();
    BOOST_CHECK (connected);
    connected = core->connect ();
    BOOST_CHECK (connected);

    auto ccore = static_cast<helics::ZmqCore *> (core.get ());
    // this will test the automatic port allocation
    BOOST_CHECK_EQUAL (ccore->getAddress (), "tcp://127.0.0.1:23500;tcp://127.0.0.1:23501");
    core->disconnect ();
    broker->disconnect ();
}
*/
BOOST_AUTO_TEST_SUITE_END ()