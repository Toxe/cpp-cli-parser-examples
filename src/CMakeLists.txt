set(ALL_TARGETS
    argparse
    cli11
)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(ALL_TARGETS ${ALL_TARGETS} clipp)
endif()

foreach(target ${ALL_TARGETS})
    add_executable(${target} ${target}.cpp)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_features(${target} PUBLIC cxx_std_20)
    target_compile_options(${target} PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
    target_link_libraries(${target} PRIVATE ${SANITIZER_FLAGS})
endforeach()

target_link_libraries(argparse PRIVATE argparse::argparse fmt::fmt spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(cli11 PRIVATE CLI11::CLI11 fmt::fmt spdlog::spdlog spdlog::spdlog_header_only)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(clipp PRIVATE clipp::clipp fmt::fmt spdlog::spdlog spdlog::spdlog_header_only)
endif()
