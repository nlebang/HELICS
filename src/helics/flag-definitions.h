/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/

/*defines left in this code as it is used in the shared library*/
#ifndef _HELICS_FLAG_DEFINITIONS_
#define _HELICS_FLAG_DEFINITIONS_
#pragma once
/** @file
@details define statements for flag codes
*/

/** flag indicating that a federate is observe only*/
#define OBSERVER_FLAG 0
/** flag indicating that a federate can only return requested times*/
#define UNINTERRUPTIBLE_FLAG 1
/** flag indicating that a federate can be interrupted*/
#define INTERRUPTIBLE_FLAG 2
/** flag indicating that a federate/interface is a signal generator only*/
#define SOURCE_ONLY_FLAG 4
/** flag indicating a federate/interface should only transmit values if they have changed(binary equivalence)*/
#define ONLY_TRANSMIT_ON_CHANGE_FLAG 6
/** flag indicating a federate/interface should only trigger an update if a value has changed (binary
 * equivalence)*/
#define ONLY_UPDATE_ON_CHANGE_FLAG 8
/** flag indicating a federate should only grant time if all other federates have already passed the requested
 * time*/
#define WAIT_FOR_CURRENT_TIME_UPDATE_FLAG 10

/** flag indicating that a federate has rollback capability*/
#define ROLLBACK_FLAG 12
/** flag indicating that a federate performs forward computation and does internal rollback*/
#define FORWARD_COMPUTE_FLAG 14

/** flag indicating that a federate needs to run in real time*/
#define REALTIME_FLAG 16

/** used to delay a core from entering initialization mode even if it would otherwise be ready*/
#define DELAY_INIT_ENTRY 45
/** used to clear the DELAY_INIT_ENTRY flag in cores*/
#define ENABLE_INIT_ENTRY 47

/* error code definitions */

#define ERROR_CODE_REGISTRATION_FAILURE (-1)
#define ERROR_CODE_CONNECTION_FAILURE (-2)
#define ERROR_CODE_INVALID_OBJECT (-3)
#define ERROR_CODE_INVALID_ARGUMENT (-4)
#define ERROR_CODE_DISCARD (-5)
#define ERROR_CODE_SYSTEM_FAILURE (-6)
#define ERROR_CODE_INVALID_STATE_TRANSITION (-9)
#define ERROR_CODE_INVALID_FUNCTION_CALL (-10)
#define ERROR_CODE_EXECUTION_FAILURE (-14)

/** property definitions for time properties **/
#define TIME_DELTA_PROPERTY 137
#define PERIOD_PROPERTY 140
#define OFFSET_PROPERTY 141
#define RT_LAG_PROPERTY 143
#define RT_LEAD_PROPERTY 144
#define RT_TOLERANCE_PROPERTY 145
#define INPUT_DELAY_PROPERTY 148
#define OUTPUT_DELAY_PROPERTY 150

/** property definitions for integer properties **/
#define MAX_ITERATIONS_PROPERTY 259
#define LOG_LEVEL_PROPERTY 271

/** handle option flags*/
#define CONNECTION_REQUIRED_OPTION 397
#define CONNECTION_OPTIONAL_OPTION 402
#define SINGLE_CONNECTION_ONLY_OPTION 407
#define MULTIPLE_CONNECTIONS_ALLOWED_OPTION 409

#endif
