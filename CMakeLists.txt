cmake_minimum_required(VERSION 2.8)

add_executable (fake-hwclock
        fake-hwclock.c
)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/fake-hwclock DESTINATION bin)

