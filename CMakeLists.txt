cmake_minimum_required(VERSION 2.8)
project(fake-hwclock)
add_executable(fake-hwclock fake-hwclock.c)
install(programs fake-hwclock destination usr/local)
