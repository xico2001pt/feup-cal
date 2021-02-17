# Add the source files of each exercise
file(GLOB CPP_FILES CONFIGURE_DEPENDS "*.cpp" "*/*.cpp")

add_executable(TP1
        main.cpp
        ${CPP_FILES} exercises.h)

target_link_libraries(TP1 gtest_main gmock_main)