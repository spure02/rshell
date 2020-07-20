CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY(googletest)

SET(CMAKE_CXX_STANDARD 11)
find_package(Boost)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  ADD_EXECUTABLE(test	
	src/tests/test.cpp
	src/Shell.cpp	
	src/Parser.cpp 
	src/cmd.cpp )

  ADD_EXECUTABLE(rshell	
	src/rshell.cpp
	src/Shell.cpp	
	src/Parser.cpp 
	src/cmd.cpp )



endif()



TARGET_LINK_LIBRARIES(test gtest)
target_link_libraries(test ${Boost_tokenizer})
TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
