cmake_minimum_required(VERSION 2.8)
project(fake-hwclock)
add_executable(fake-hwclock fake-hwclock.c)
install(TARGETS ${project_name} DESTINATION usr/local)
