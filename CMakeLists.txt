CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(fake-hwclock)
ADD_EXECUTABLE(fake-hwclock fake-hwclock.c)
INSTALL(FILES fake-hwclock DESTINATION bin)
