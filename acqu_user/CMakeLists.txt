message(STATUS "Configuring acqu_user...")

# acqu user incs have been collected in previous subprojects
include_directories("root/src" "root/src-mc" ${ACQU_USER_INCS})

set(MOD "UserRoot")
ROOT_PREPARE_LIB("root/src" "root/src" "root/src/UserLinkDef.h" "root/src/UserDict.cc" SRCS EXESRCS)
add_library("lib${MOD}" SHARED ${SRCS})
target_link_libraries("lib${MOD}" ${ROOT_LIBRARIES} ${ACQU_USER_LIBS})
set_target_properties("lib${MOD}" PROPERTIES LIBRARY_OUTPUT_NAME ${MOD})


set(MOD "MCUserRoot")
ROOT_PREPARE_LIB("root/src-mc" "root/src-mc" "root/src-mc/MCUserLinkDef.h" "root/src-mc/MCDict.cc" SRCS EXESRCS)
add_library("lib${MOD}" SHARED ${SRCS})
target_link_libraries("lib${MOD}" ${ROOT_LIBRARIES} libAcquMC libAcquRoot libAcquDAQ)
set_target_properties("lib${MOD}" PROPERTIES LIBRARY_OUTPUT_NAME ${MOD})

# the above defined additional "Dict" targets
# prevents rootcint going crazy only because building Dict's
# twice the same time in the same directory in parallel make


# build finally the two executables which many people need desperately ^-^
set(MOD "AcquRoot")
add_executable(${MOD} "root/src/Main${MOD}.cc")
target_link_libraries(${MOD} libUserRoot)

set(MOD "AcquMC")
add_executable(${MOD} "root/src-mc/Main${MOD}.cc")
target_link_libraries(${MOD} libMCUserRoot)

