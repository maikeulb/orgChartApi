cmake_minimum_required(VERSION 3.5)
project(org_chart_test CXX)

add_executable(${PROJECT_NAME} test_main.cc test_controllers.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE drogon)

ParseAndAddDrogonTests(${PROJECT_NAME})
