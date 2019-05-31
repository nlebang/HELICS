/*
Copyright © 2017-2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once
#include <atomic>
#include <memory>

/** namespace for the global variable in tripwire*/
namespace tripwire
{
/** the actual tripwire type*/
using triplineType = std::shared_ptr<std::atomic<bool>>;

/** singleton class containing the actual trip line*/
class TripWire
{
  private:
    /** get the tripwire*/
    static triplineType getline ();

    friend class TripWireDetector;
    friend class TripWireTrigger;
};

/** class to check if a trip line was tripped*/
class TripWireDetector
{
  public:
    TripWireDetector ();
    /** check if the line was tripped*/
    bool isTripped () const noexcept;

  private:
    std::shared_ptr<const std::atomic<bool>> lineDetector;  //!< const pointer to the tripwire
};

/** class to trigger a tripline on destruction */
class TripWireTrigger
{
  public:
    /** default constructor*/
    TripWireTrigger ();
    /** destructor*/
    ~TripWireTrigger ();
    /** move constructor*/
    TripWireTrigger (TripWireTrigger &&twt) = default;
    /** deleted copy constructor*/
    TripWireTrigger (const TripWireTrigger &twt) = delete;
    /** move assignment*/
    TripWireTrigger &operator= (TripWireTrigger &&twt) = default;
    /** deleted copy assignment*/
    TripWireTrigger &operator= (const TripWireTrigger &twt) = delete;

  private:
    triplineType lineTrigger;  //!< the tripwire
};
}  // namespace tripwire