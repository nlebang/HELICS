function v = helics_time_property_period()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1432107635);
  end
  v = vInitialized;
end
