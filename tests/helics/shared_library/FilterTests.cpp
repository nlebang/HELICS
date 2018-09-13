/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/

#include "ctestFixtures.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <future>
/** these test cases test out the message federates
 */

namespace bdata = boost::unit_test::data;
namespace utf = boost::unit_test;

BOOST_FIXTURE_TEST_SUITE (filter_tests, FederateTestFixture, *utf::label("ci"))


/** test registration of filters*/


BOOST_DATA_TEST_CASE (message_filter_registration, bdata::make (core_types), core_type)
{
    auto broker = AddBroker (core_type, 2);
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, helics_time_zero, "filter");
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, helics_time_zero, "message");

    auto fFed = GetFederateAt (0);
    auto mFed = GetFederateAt (1);

    helicsFederateRegisterGlobalEndpoint (mFed, "port1","",&err);
    helicsFederateRegisterGlobalEndpoint (mFed, "port2", NULL,&err);
    BOOST_CHECK_EQUAL (err.error_code, helics_ok);
    CE(auto f1 = helicsFederateRegisterFilter (fFed, helics_filtertype_custom, "filter1",&err));
    CE(helicsFilterAddSourceTarget(f1, "port1",&err));
    BOOST_CHECK (f1 != NULL);
    CE(auto f2 = helicsFederateRegisterFilter (fFed, helics_filtertype_custom, "filter2",&err));
    CE(helicsFilterAddDestinationTarget(f2, "port2",&err));
    BOOST_CHECK (f2 != f1);
    CE(auto ep1 = helicsFederateRegisterEndpoint (fFed, "fout", "",&err));
    BOOST_CHECK (ep1 != NULL);
    CE(auto f3 = helicsFederateRegisterFilter (fFed, helics_filtertype_custom, "",&err));
    helicsFilterAddSourceTarget(f3, "filter0/fout",nullptr);
    BOOST_CHECK (f3 != f2);
    CE(helicsFederateFinalize (mFed,&err));
    CE(helicsFederateFinalize (fFed,&err));
    CE(federate_state state=helicsFederateGetState (fFed, &err));
    BOOST_CHECK (state == helics_state_finalize);
}

/** test a filter operator
The filter operator delays the message by 2.5 seconds meaning it should arrive by 3 sec into the simulation
*/


BOOST_DATA_TEST_CASE (message_filter_function, bdata::make (core_types), core_type)
{
    auto broker = AddBroker (core_type, 2);
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, 1.0, "filter");
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, 1.0, "message");

    auto fFed = GetFederateAt (0);
    auto mFed = GetFederateAt (1);

    auto p1 = helicsFederateRegisterGlobalEndpoint (mFed, "port1", nullptr,&err);
    auto p2 = helicsFederateRegisterGlobalEndpoint (mFed, "port2", "",&err);
    BOOST_CHECK_EQUAL (err.error_code, helics_ok);

    auto f1 = helicsFederateRegisterFilter (fFed, helics_filtertype_delay, "filter1",&err);
    BOOST_CHECK_EQUAL (err.error_code, helics_ok);
    CE(helicsFilterAddSourceTarget(f1, "port1",&err));
    BOOST_CHECK (f1 != NULL);
    CE(helicsFilterSet (f1, "delay", 2.5,&err));

    CE(helicsFederateEnterExecutingModeAsync (fFed,&err));
    CE(helicsFederateEnterExecutingMode (mFed,&err));
    CE(helicsFederateEnterExecutingModeComplete (fFed,&err));

    CE (federate_state state=helicsFederateGetState (fFed, &err));
    BOOST_CHECK (state == helics_state_execution);
    std::string data (500, 'a');
    CE(helicsEndpointSendMessageRaw (p1, "port2", data.c_str (), static_cast<int> (data.size ()), &err));

    CE(helicsFederateRequestTimeAsync (mFed, 1.0,&err));
    CE(helicsFederateRequestTime(fFed, 1.0,&err));
    CE(helicsFederateRequestTimeComplete (mFed,&err));

    auto res = helicsFederateHasMessage (mFed);
    BOOST_CHECK (!res);

    CE(helicsFederateRequestTimeAsync (mFed, 2.0,&err));
    CE(helicsFederateRequestTime(fFed, 2.0,&err));
    CE(helicsFederateRequestTimeComplete (mFed, &err));
    BOOST_REQUIRE (!helicsEndpointHasMessage (p2));

    CE(helicsFederateRequestTimeAsync (fFed, 3.0,&err));
    CE(helicsFederateRequestTime(mFed, 3.0,&err));

    BOOST_REQUIRE (helicsEndpointHasMessage (p2));

    auto m2 = helicsEndpointGetMessage (p2);
    BOOST_CHECK_EQUAL (m2.source, "port1");
    BOOST_CHECK_EQUAL (m2.original_source, "port1");
    BOOST_CHECK_EQUAL (m2.dest, "port2");
    BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data.size ()));
    BOOST_CHECK_EQUAL (m2.time, 2.5);

    CE(helicsFederateRequestTime(mFed, 3.0,&err));
    CE(helicsFederateRequestTimeComplete (fFed, &err));
    CE(helicsFederateFinalize (mFed,&err));
    CE(helicsFederateFinalize (fFed,&err));
    CE(state=helicsFederateGetState (fFed, &err));
    BOOST_CHECK (state == helics_state_finalize);
}

/** test a filter operator
The filter operator delays the message by 2.5 seconds meaning it should arrive by 3 sec into the simulation
*/
BOOST_DATA_TEST_CASE (message_filter_function_two_stage, bdata::make (core_types), core_type)
{
    auto broker = AddBroker (core_type, 3);
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, 1.0, "filter");
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, 1.0, "filter2");
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, 1.0, "message");

    auto fFed = GetFederateAt (0);
    auto fFed2 = GetFederateAt (1);
    auto mFed = GetFederateAt (2);

    auto p1 = helicsFederateRegisterGlobalEndpoint (mFed, "port1", "",&err);
    auto p2 = helicsFederateRegisterGlobalEndpoint (mFed, "port2", "",&err);
    BOOST_CHECK_EQUAL (err.error_code, helics_ok);

    auto f1 = helicsFederateRegisterFilter (fFed, helics_filtertype_delay, "filter1",&err);
    BOOST_CHECK_EQUAL (err.error_code, helics_ok);
    CE(helicsFilterAddSourceTarget(f1, "port1",&err));

    BOOST_CHECK (f1 != NULL);
    CE(helicsFilterSet (f1, "delay", 1.25,&err));

    CE (auto f2 = helicsFederateRegisterFilter (fFed, helics_filtertype_delay, "filter2", &err));
    CE(helicsFilterAddSourceTarget(f1, "port1",&err));
    BOOST_CHECK (f2 != NULL);
    CE(helicsFilterSet (f2, "delay", 1.25,&err));

    CE(helicsFederateEnterExecutingModeAsync (fFed,&err));
    CE(helicsFederateEnterExecutingModeAsync (fFed2,&err));
    CE(helicsFederateEnterExecutingMode (mFed,&err));
    CE(helicsFederateEnterExecutingModeComplete (fFed,&err));
    CE(helicsFederateEnterExecutingModeComplete (fFed2,&err));

    CE (federate_state state=helicsFederateGetState (fFed, &err));
    BOOST_CHECK (state == helics_state_execution);
    std::string data (500, 'a');
    CE (helicsEndpointSendMessageRaw (p1, "port2", data.c_str (), static_cast<int> (data.size ()), &err));

    CE(helicsFederateRequestTimeAsync (mFed, .0,&err));
    CE(helicsFederateRequestTimeAsync (fFed, 1.0,&err));
    CE(helicsFederateRequestTime(fFed2, 1.0,&err));
    CE(helicsFederateRequestTimeComplete (mFed,&err));
    CE(helicsFederateRequestTimeComplete (fFed,&err));
    auto res = helicsFederateHasMessage (mFed);
    BOOST_CHECK (!res);

    CE(helicsFederateRequestTimeAsync (mFed, .0,&err));
    CE(helicsFederateRequestTimeAsync (fFed2, 2.0,&err));
    CE(helicsFederateRequestTime(fFed, 2.0,&err));
    CE(helicsFederateRequestTimeComplete (mFed,&err));
    CE(helicsFederateRequestTimeComplete (fFed2,&err));
    BOOST_REQUIRE (!helicsEndpointHasMessage (p2));

    CE(helicsFederateRequestTimeAsync (fFed, 3.0,&err));
    CE(helicsFederateRequestTimeAsync (fFed2, 3.0,&err));
    CE(helicsFederateRequestTime(mFed, 3.0,&err));
    if (!helicsEndpointHasMessage (p2))
    {
        printf ("missing message\n");
    }
    BOOST_REQUIRE (helicsEndpointHasMessage (p2));

    auto m2 = helicsEndpointGetMessage (p2);
    BOOST_CHECK_EQUAL (m2.source, "port1");
    BOOST_CHECK_EQUAL (m2.original_source, "port1");
    BOOST_CHECK_EQUAL (m2.dest, "port2");
    BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data.size ()));
    BOOST_CHECK_EQUAL (m2.time, 2.5);

    CE(helicsFederateRequestTimeComplete (fFed,&err));
    CE(helicsFederateRequestTimeComplete (fFed2,&err));
    CE(helicsFederateFinalize (mFed,&err));
    CE(helicsFederateFinalize (fFed,&err));
    CE(helicsFederateFinalize (fFed2,&err));
    CE(state=helicsFederateGetState (fFed,&err));
    BOOST_CHECK (state == helics_state_finalize);
}

/** test two filter operators
The filter operator delays the message by 2.5 seconds meaning it should arrive by 3 sec into the simulation
*/


BOOST_DATA_TEST_CASE (message_filter_function2, bdata::make (core_types), core_type)
{
    auto broker = AddBroker (core_type, 2);
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, 1.0, "filter");
    AddFederates (helicsCreateMessageFederate, core_type, 1, broker, 1.0, "message");

    auto fFed = GetFederateAt (0);
    auto mFed = GetFederateAt (1);

    auto p1 = helicsFederateRegisterGlobalEndpoint (mFed, "port1", "",&err);
    auto p2 = helicsFederateRegisterGlobalEndpoint (mFed, "port2", "",&err);
    BOOST_CHECK_EQUAL (err.error_code, helics_ok);

   CE( auto f1 = helicsFederateRegisterFilter (fFed, helics_filtertype_delay, "filter1",&err));
    helicsFilterAddSourceTarget(f1, "port1",nullptr);
    BOOST_CHECK (f1 != NULL);
    CE(helicsFilterSet (f1, "delay", 2.5,&err));

    CE(auto f2 = helicsFederateRegisterFilter (fFed, helics_filtertype_delay,"filter2",&err));
    helicsFilterAddSourceTarget (f2, "port2", nullptr);
    BOOST_CHECK (f2 != NULL);
    CE(helicsFilterSet (f2, "delay", 2.5,&err));

    CE(helicsFederateEnterExecutingModeAsync (fFed,&err));
    CE(helicsFederateEnterExecutingMode (mFed,&err));
    CE(helicsFederateEnterExecutingModeComplete (fFed,&err));

    CE (federate_state state=helicsFederateGetState (fFed, &err));
    BOOST_CHECK (state == helics_state_execution);
    std::string data (500, 'a');
    CE (helicsEndpointSendMessageRaw (p1, "port2", data.c_str (), static_cast<int> (data.size ()), &err));

    CE(helicsFederateRequestTimeAsync (mFed, 1.0,&err));
    CE(helicsFederateRequestTime(fFed, 1.0,&err));
    CE(helicsFederateRequestTimeComplete (mFed,&err));

    auto res = helicsFederateHasMessage (mFed);
    BOOST_CHECK (!res);
    CE (helicsEndpointSendMessageRaw (p2, "port1", data.c_str (), static_cast<int> (data.size ()), &err));
    CE(helicsFederateRequestTimeAsync (mFed, 2.0,&err));
    CE(helicsFederateRequestTime(fFed, 2.0,&err));
    CE(helicsFederateRequestTimeComplete (mFed,&err));
    BOOST_REQUIRE (!helicsEndpointHasMessage (p2));
    // there may be something wrong here yet but this test isn't the one to find it and
    // this may prevent spurious errors for now.
    std::this_thread::yield ();
    CE(helicsFederateRequestTime(mFed, 3.0,&err));

    BOOST_REQUIRE (helicsEndpointHasMessage (p2));

    auto m2 = helicsEndpointGetMessage (p2);
    BOOST_CHECK_EQUAL (m2.source, "port1");
    BOOST_CHECK_EQUAL (m2.original_source, "port1");
    BOOST_CHECK_EQUAL (m2.dest, "port2");
    BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data.size ()));
    BOOST_CHECK_EQUAL (m2.time, 2.5);

    BOOST_CHECK (!helicsEndpointHasMessage (p1));
    CE(helicsFederateRequestTime(mFed, 4.0,&err));
    BOOST_CHECK (helicsEndpointHasMessage (p1));
    CE(helicsFederateFinalize (mFed,&err));
    CE(helicsFederateFinalize (fFed,&err));
    CE(state=helicsFederateGetState (fFed,&err));
    BOOST_CHECK (state == helics_state_finalize);
}

BOOST_AUTO_TEST_CASE (message_clone_test)
{
    auto broker = AddBroker ("test", 3);
    AddFederates (helicsCreateMessageFederate, "test", 1, broker, 1.0, "source");
    AddFederates (helicsCreateMessageFederate, "test", 1, broker, 1.0, "dest");
    AddFederates (helicsCreateMessageFederate, "test", 1, broker, 1.0, "dest_clone");

    auto sFed = GetFederateAt (0);
    auto dFed = GetFederateAt (1);
    auto dcFed = GetFederateAt (2);

    auto p1 = helicsFederateRegisterGlobalEndpoint (sFed, "src", "", &err);
    auto p2 = helicsFederateRegisterGlobalEndpoint (dFed, "dest", "", &err);
    auto p3 = helicsFederateRegisterGlobalEndpoint (dcFed, "cm", "", &err);

    auto f1 = helicsFederateRegisterCloningFilter (dcFed, "cm", &err);
    BOOST_CHECK (err.error_code == helics_ok);
    CE(helicsFilterAddSourceTarget (f1, "src",&err));

    CE(helicsFederateEnterExecutingModeAsync (sFed,&err));
    CE(helicsFederateEnterExecutingModeAsync (dcFed,&err));
    CE(helicsFederateEnterExecutingMode (dFed,&err));
    CE(helicsFederateEnterExecutingModeComplete (sFed,&err));
    CE(helicsFederateEnterExecutingModeComplete (dcFed,&err));

    CE (federate_state state=helicsFederateGetState (sFed, &err));
    BOOST_CHECK (state == helics_state_execution);
    std::string data (500, 'a');
    CE (helicsEndpointSendMessageRaw (p1, "dest", data.c_str (), static_cast<int> (data.size ()), &err));

    CE(helicsFederateRequestTimeAsync (sFed, 1.0,&err));
    CE(helicsFederateRequestTimeAsync (dcFed, 1.0,&err));
    CE(helicsFederateRequestTime (dFed, 1.0,&err));
    CE(helicsFederateRequestTimeComplete (sFed,&err));
    CE(helicsFederateRequestTimeComplete (dcFed,&err));

    auto res = helicsFederateHasMessage (dFed);
    BOOST_CHECK (res);

    if (res)
    {
        auto m2 = helicsEndpointGetMessage (p2);
        BOOST_CHECK_EQUAL (m2.source, "src");
        BOOST_CHECK_EQUAL (m2.original_source, "src");
        BOOST_CHECK_EQUAL (m2.dest, "dest");
        BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data.size ()));
    }

    // now check the message clone
    res = helicsFederateHasMessage (dcFed);
    BOOST_CHECK (res);

    if (res)
    {
        auto m2 = helicsEndpointGetMessage (p3);
        BOOST_CHECK_EQUAL (m2.source, "src");
        BOOST_CHECK_EQUAL (m2.original_source, "src");
        BOOST_CHECK_EQUAL (m2.dest, "cm");
        BOOST_CHECK_EQUAL (m2.original_dest, "dest");
        BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data.size ()));
    }

    CE(helicsFederateFinalize (sFed,&err));
    CE(helicsFederateFinalize (dFed,&err));
    CE(helicsFederateFinalize (dcFed,&err));
    CE(state=helicsFederateGetState (sFed,&err));
    BOOST_CHECK (state == helics_state_finalize);
}

BOOST_AUTO_TEST_CASE (message_multi_clone_test)
{
    auto broker = AddBroker ("test", 4);
    AddFederates (helicsCreateMessageFederate, "test", 2, broker, 1.0, "source");
    AddFederates (helicsCreateMessageFederate, "test", 1, broker, 1.0, "dest");
    AddFederates (helicsCreateMessageFederate, "test", 1, broker, 1.0, "dest_clone");

    auto sFed = GetFederateAt (0);
    auto sFed2 = GetFederateAt (1);
    auto dFed = GetFederateAt (2);
    auto dcFed = GetFederateAt (3);

    auto p1 = helicsFederateRegisterGlobalEndpoint (sFed, "src", "",&err);
    BOOST_REQUIRE_EQUAL (err.error_code, 0);
    auto p2 = helicsFederateRegisterGlobalEndpoint (sFed2, "src2", "",&err);
    BOOST_REQUIRE_EQUAL (err.error_code, 0);
    auto p3 = helicsFederateRegisterGlobalEndpoint (dFed, "dest", "",&err);
    BOOST_REQUIRE_EQUAL (err.error_code, 0);
    auto p4 = helicsFederateRegisterGlobalEndpoint (dcFed, "cm", "",&err);
    BOOST_REQUIRE_EQUAL (err.error_code, 0);

    auto f1 = helicsFederateRegisterCloningFilter (dcFed, "cm",&err);
    BOOST_REQUIRE_EQUAL (err.error_code, 0);
    CE(helicsFilterAddSourceTarget (f1, "src",&err));
    CE(helicsFilterAddSourceTarget (f1, "src2",&err));

    CE(helicsFederateEnterExecutingModeAsync (sFed,&err));
    CE(helicsFederateEnterExecutingModeAsync (sFed2,&err));
    CE(helicsFederateEnterExecutingModeAsync (dcFed,&err));
    CE(helicsFederateEnterExecutingMode (dFed,&err));
    CE(helicsFederateEnterExecutingModeComplete (sFed,&err));
    CE(helicsFederateEnterExecutingModeComplete (sFed2,&err));
    CE(helicsFederateEnterExecutingModeComplete (dcFed,&err));

    CE(federate_state state=helicsFederateGetState (sFed,&err));
    BOOST_CHECK (state == helics_state_execution);
    std::string data (500, 'a');
    std::string data2 (400, 'b');
    CE (helicsEndpointSendMessageRaw (p1, "dest", data.c_str (), static_cast<int> (data.size ()), &err));
    CE (helicsEndpointSendMessageRaw (p2, "dest", data2.c_str (), static_cast<int> (data2.size ()), &err));

    CE(helicsFederateRequestTimeAsync (sFed, 1.0,&err));
    CE(helicsFederateRequestTimeAsync (sFed2, 1.0,&err));
    CE(helicsFederateRequestTimeAsync (dcFed, 1.0,&err));
    CE(helicsFederateRequestTime (dFed, 1.0,&err));
    CE(helicsFederateRequestTimeComplete (sFed,&err));
    CE(helicsFederateRequestTimeComplete (sFed2,&err));
    CE(helicsFederateRequestTimeComplete (dcFed,&err));

    auto mcnt = helicsEndpointPendingMessages (p3);
    BOOST_CHECK_EQUAL (mcnt, 2);
    auto res = helicsFederateHasMessage (dFed);
    BOOST_CHECK (res);

    if (res)
    {
        auto m2 = helicsEndpointGetMessage (p3);
        BOOST_CHECK_EQUAL (m2.source, "src");
        BOOST_CHECK_EQUAL (m2.original_source, "src");
        BOOST_CHECK_EQUAL (m2.dest, "dest");
        BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data.size ()));
        res = helicsFederateHasMessage (dFed);
        BOOST_CHECK (res);

        if (res)
        {
            m2 = helicsFederateGetMessage (dFed);
            BOOST_CHECK_EQUAL (m2.source, "src2");
            BOOST_CHECK_EQUAL (m2.original_source, "src2");
            BOOST_CHECK_EQUAL (m2.dest, "dest");
            BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data2.size ()));
        }
    }

    // now check the message clone
    mcnt = helicsEndpointPendingMessages (p4);
    BOOST_CHECK_EQUAL (mcnt, 2);
    res = helicsFederateHasMessage (dcFed);
    BOOST_CHECK (res);

    if (res)
    {
        auto m2 = helicsFederateGetMessage (dcFed);
        BOOST_CHECK_EQUAL (m2.source, "src");
        BOOST_CHECK_EQUAL (m2.original_source, "src");
        BOOST_CHECK_EQUAL (m2.dest, "cm");
        BOOST_CHECK_EQUAL (m2.original_dest, "dest");
        BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data.size ()));
        res = helicsFederateHasMessage (dcFed);
        BOOST_CHECK (res);

        if (res)
        {
            m2 = helicsFederateGetMessage (dcFed);
            BOOST_CHECK_EQUAL (m2.source, "src2");
            BOOST_CHECK_EQUAL (m2.original_source, "src2");
            BOOST_CHECK_EQUAL (m2.dest, "cm");
            BOOST_CHECK_EQUAL (m2.original_dest, "dest");
            BOOST_CHECK_EQUAL (m2.length, static_cast<int64_t> (data2.size ()));
        }
    }

    CE(helicsFederateFinalize (sFed,&err));
    CE(helicsFederateFinalize (sFed2,&err));
    CE(helicsFederateFinalize (dFed,&err));
    CE(helicsFederateFinalize (dcFed,&err));
    CE(state=helicsFederateGetState (sFed,&err));
    BOOST_CHECK (state == helics_state_finalize);
}

BOOST_AUTO_TEST_CASE (test_file_load)
{
    std::string filename = std::string (TEST_DIR) + "/test_files/example_filters.json";
    auto mFed = helicsCreateMessageFederateFromConfig (filename.c_str (),&err);

    const char *name=helicsFederateGetName (mFed);
    BOOST_CHECK_EQUAL (name, "filterFed");

    BOOST_CHECK_EQUAL (helicsFederateGetEndpointCount (mFed), 3);
    helicsFederateFinalize (mFed,nullptr);
    helicsFederateFree (mFed);
    // auto id = mFed.getEndpointId ("ept1");
    // BOOST_CHECK_EQUAL (mFed.getEndpointType (id), "genmessage");

    // BOOST_CHECK_EQUAL (mFed.filterObjectCount (), 3);

    // auto filt = mFed.getFilterObject (2);

    // auto cloneFilt = std::dynamic_pointer_cast<helics::CloningFilter> (filt);
    // BOOST_CHECK (cloneFilt);
    // mFed.disconnect ();
}
BOOST_AUTO_TEST_SUITE_END ()
