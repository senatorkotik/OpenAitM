add_executable(OpenAitM "main.cpp" "common/conf.cpp" "common/conf.hpp")

target_include_directories(OpenAitM PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common")
target_link_libraries(OpenAitM PRIVATE Boost::asio Boost::beast spdlog::spdlog CLI11::CLI11 tomlplusplus::tomlplusplus)