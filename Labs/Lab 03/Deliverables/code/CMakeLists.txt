cmake_minimum_required(VERSION 4.0)
project(project-name LANGUAGES CXX)
# SET PROJECT NAME HERE

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(fmt REQUIRED)

# get all files in src directory
file(GLOB_RECURSE FILES_FOLDERS
    LIST_DIRECTORIES true
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*"
)

set(SOURCE_FILES "")
set(HEADER_FILES "")
set(FOLDERS "")

foreach(item IN LISTS FILES_FOLDERS)

    if(IS_DIRECTORY "${item}")
        list(APPEND FOLDERS "${item}")

    elseif(item MATCHES "\\.cpp$")
        list(APPEND SOURCE_FILES "${item}")

    elseif(item MATCHES "\\.hpp$")
        list(APPEND HEADER_FILES "${item}")

    endif()

endforeach()

# Separate main.cpp from library sources

set(MAIN_FILE "")
set(LIBRARY_SOURCES "")

foreach(file IN LISTS SOURCE_FILES)

    get_filename_component(filename "${file}" NAME)

    if(filename STREQUAL "main.cpp")
        set(MAIN_FILE "${file}")
    else()
        list(APPEND LIBRARY_SOURCES "${file}")
    endif()
endforeach()

# log all detected files in console
message(STATUS "Source files:")
foreach(file IN LISTS SOURCE_FILES)
    message(STATUS "  ${file}")
endforeach()

message(STATUS "Header files:")
foreach(file IN LISTS HEADER_FILES)
    message(STATUS "  ${file}")
endforeach()

# log all detected folders in console
message(STATUS "Folders:")
foreach(dir IN LISTS FOLDERS)
    message(STATUS "  ${dir}")
endforeach()


# library
# everything except main.cpp goes here.
add_library(project_lib
    ${LIBRARY_SOURCES}
    ${HEADER_FILES}
)

target_include_directories(
    project_lib
    PUBLIC
        ${FOLDERS}
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/external/included"
)

# installed library
target_link_libraries(
    project_lib
    PUBLIC
    # fmt::fmt
)

# only main.cpp for exec
add_executable(
    main
    ${MAIN_FILE}
)

target_link_libraries(
    main
    PRIVATE
        project_lib
)

# assets

add_custom_target(
    copy_assets ALL

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "${CMAKE_CURRENT_BINARY_DIR}/assets"
    COMMENT "Copying asset files..."
)

add_dependencies(main copy_assets)

# tests

include(CTest)

if(BUILD_TESTING)

    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    # all .cpp files in tests/
    file(GLOB_RECURSE TEST_FILES
        CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/tests/*.cpp"
    )

    message(STATUS "Test files:")
    foreach(file IN LISTS TEST_FILES)
        message(STATUS "  ${file}")
    endforeach()

    add_executable(
        project_tests
        ${TEST_FILES}
    )

    target_link_libraries(
        project_tests
        PRIVATE
            project_lib
            GTest::gtest_main
    )

    include(GoogleTest)

    gtest_discover_tests(project_tests)

endif()
