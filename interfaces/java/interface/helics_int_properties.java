/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

public final class helics_int_properties {
  public final static helics_int_properties helics_int_property_max_iterations = new helics_int_properties("helics_int_property_max_iterations", helicsJNI.helics_int_property_max_iterations_get());
  public final static helics_int_properties helics_int_property_log_level = new helics_int_properties("helics_int_property_log_level", helicsJNI.helics_int_property_log_level_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static helics_int_properties swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + helics_int_properties.class + " with value " + swigValue);
  }

  private helics_int_properties(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private helics_int_properties(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private helics_int_properties(String swigName, helics_int_properties swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static helics_int_properties[] swigValues = { helics_int_property_max_iterations, helics_int_property_log_level };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

