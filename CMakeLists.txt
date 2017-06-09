cmake_minimum_required(VERSION 3.4.3)
project(boost.limits)

enable_testing()

add_compile_options(-std=c++14 -Wall)

include(ExternalProject)

find_package(Boost REQUIRED)

if (UNIX)
	set(SRC
		detail/limits_unix.cpp
	)
else ()
	message(FATAL_ERROR "Only Unix systems are supported at the moment.")
endif ()

add_library(boost.limits ${SRC})

add_executable(main main.cpp)
target_link_libraries(main boost.limits)

install(FILES boost_limits.hpp DESTINATION include)
install(TARGETS boost.limits DESTINATION lib)