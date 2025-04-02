####
# FPrime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
# UT_SOURCE_FILES: list of source files for unit tests
#
# More information in the F´ CMake API documentation:
# https://fprime.jpl.nasa.gov/latest/documentation/reference
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/TextEventPacketizer.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/TextEventPacketizer.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/TextEventPacket.cpp"
)

# Uncomment and add any modules that this component depends on, else
# they might not be available when cmake tries to build this component.
#
# Module names are derived from the path from the nearest project/library/framework
# root when not specifically overridden by the developer. i.e. The module defined by
# `Ref/SignalGen/CMakeLists.txt` will be named `Ref_SignalGen`.  `Ref/SignalGen`
# is an acceptable alternative and will be internally converted to `Ref_SignalGen`.
#
# set(MOD_DEPS
#   MyPackage_MyOtherModule
# )
set(MOD_DEPS
    Fw/Com
    Fw/Log
)

register_fprime_module()


### Unit Tests ###
# set(UT_SOURCE_FILES
#   "${CMAKE_CURRENT_LIST_DIR}/TextEventPacketizer.fpp"
#   "${CMAKE_CURRENT_LIST_DIR}/test/ut/TextEventPacketizerTestMain.cpp"
#   "${CMAKE_CURRENT_LIST_DIR}/test/ut/TextEventPacketizerTester.cpp"
#   "${CMAKE_CURRENT_LIST_DIR}/test/ut/TextEventPacket.cpp"
# )
# set(UT_MOD_DEPS
#   Fw/Com
#   STest
# )
# set(UT_AUTO_HELPERS ON)
# register_fprime_ut()
