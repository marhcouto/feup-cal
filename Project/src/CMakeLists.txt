cmake_minimum_required(VERSION 3.17)
project(Entrega2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Google tests
set(GOOGLE_TEST_DIRECTORY lib/googletest-master)
add_subdirectory(${GOOGLE_TEST_DIRECTORY})

# Build GraphViewer
set(GRAPHVIEWERCPP_PATH lib/GraphViewerCpp)
add_subdirectory(${GRAPHVIEWERCPP_PATH})
include_directories(${GRAPHVIEWERCPP_PATH}/include)

add_executable(Entrega2 main.cpp FileHandling/FileHandling.cpp E-Stafetas/Vehicle.cpp E-Stafetas/Driver.cpp E-Stafetas/Order.cpp E-Stafetas/Client.cpp E-Stafetas/Company.cpp Algorithms/Graph.cpp GraphViewer/GVMaker.cpp Menu/Menu.cpp) # Main
add_executable(Tests tests.cpp Tests/Algorithms_test.cpp Tests/FileHandling_test.cpp Tests/Speed_test.cpp FileHandling/FileHandling.cpp Tests/Interface_test.cpp Algorithms/Graph.cpp E-Stafetas/Vehicle.cpp E-Stafetas/Driver.cpp E-Stafetas/Order.cpp E-Stafetas/Client.cpp E-Stafetas/Company.cpp) # Tests

target_link_libraries(Tests gtest_main gmock_main)
target_link_libraries(Entrega2 graphviewer)