/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

public final class helics_error_types {
  public final static helics_error_types helics_ok = new helics_error_types("helics_ok", helicsJNI.helics_ok_get());
  public final static helics_error_types helics_error_registration_failure = new helics_error_types("helics_error_registration_failure", helicsJNI.helics_error_registration_failure_get());
  public final static helics_error_types helics_error_connection_failure = new helics_error_types("helics_error_connection_failure", helicsJNI.helics_error_connection_failure_get());
  public final static helics_error_types helics_error_invalid_object = new helics_error_types("helics_error_invalid_object", helicsJNI.helics_error_invalid_object_get());
  public final static helics_error_types helics_error_invalid_argument = new helics_error_types("helics_error_invalid_argument", helicsJNI.helics_error_invalid_argument_get());
  public final static helics_error_types helics_error_discard = new helics_error_types("helics_error_discard", helicsJNI.helics_error_discard_get());
  public final static helics_error_types helics_error_system_failure = new helics_error_types("helics_error_system_failure", helicsJNI.helics_error_system_failure_get());
  public final static helics_error_types helics_warning = new helics_error_types("helics_warning", helicsJNI.helics_warning_get());
  public final static helics_error_types helics_error_invalid_state_transition = new helics_error_types("helics_error_invalid_state_transition", helicsJNI.helics_error_invalid_state_transition_get());
  public final static helics_error_types helics_error_invalid_function_call = new helics_error_types("helics_error_invalid_function_call", helicsJNI.helics_error_invalid_function_call_get());
  public final static helics_error_types helics_error_execution_failure = new helics_error_types("helics_error_execution_failure", helicsJNI.helics_error_execution_failure_get());
  public final static helics_error_types helics_error_other = new helics_error_types("helics_error_other", helicsJNI.helics_error_other_get());
  public final static helics_error_types other_error_type = new helics_error_types("other_error_type", helicsJNI.other_error_type_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static helics_error_types swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + helics_error_types.class + " with value " + swigValue);
  }

  private helics_error_types(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private helics_error_types(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private helics_error_types(String swigName, helics_error_types swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static helics_error_types[] swigValues = { helics_ok, helics_error_registration_failure, helics_error_connection_failure, helics_error_invalid_object, helics_error_invalid_argument, helics_error_discard, helics_error_system_failure, helics_warning, helics_error_invalid_state_transition, helics_error_invalid_function_call, helics_error_execution_failure, helics_error_other, other_error_type };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

