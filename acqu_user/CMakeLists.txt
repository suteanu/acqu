message(STATUS "Configuring acqu_user...")

# acqu user incs have been collected in previous subprojects
include_directories("root/src" ${ACQU_USER_INCS})

set(MOD "UserRoot")
ROOT_PREPARE_LIB("root/src" "root/src" "root/src/UserLinkDef.h" "root/src/UserDict.cc" SRCS EXESRCS)
ADD_CUSTOM_TARGET("lib${MOD}Dict" DEPENDS "root/src/UserDict.cc")

add_library("lib${MOD}" SHARED ${SRCS})
add_dependencies("lib${MOD}" "lib${MOD}Dict")
target_link_libraries("lib${MOD}" ${ROOT_LIBRARIES} ${ACQU_USER_LIBS})
set_target_properties("lib${MOD}" PROPERTIES LIBRARY_OUTPUT_NAME ${MOD})


set(MOD "MCUserRoot")
ROOT_PREPARE_LIB("root/src" "root/src" "root/src/MCUserLinkDef.h" "root/src/MCDict.cc" SRCS EXESRCS)
ADD_CUSTOM_TARGET("lib${MOD}Dict" DEPENDS "root/src/MCDict.cc")

add_library("lib${MOD}" SHARED ${SRCS})
add_dependencies("lib${MOD}" "lib${MOD}Dict")
target_link_libraries("lib${MOD}" ${ROOT_LIBRARIES} ${ACQU_USER_LIBS})
set_target_properties("lib${MOD}" PROPERTIES LIBRARY_OUTPUT_NAME ${MOD})

# the above defined additional "Dict" targets
# prevents rootcint going crazy only because building Dict's
# twice the same time in the same directory in parallel make


# build finally the two executables which many people need desperately ^-^
set(MOD "AcquRoot")
add_executable(${MOD} "root/src/Main${MOD}.cc")
target_link_libraries(${MOD} libMCUserRoot libUserRoot)

set(MOD "AcquMC")
add_executable(${MOD} "root/src/Main${MOD}.cc")
target_link_libraries(${MOD} libMCUserRoot libUserRoot)

