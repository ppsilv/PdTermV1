# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/PdTermV1_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/PdTermV1_autogen.dir/ParseCache.txt"
  "PdTermV1_autogen"
  )
endif()
