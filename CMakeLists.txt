ADD_DEFINITIONS(-g -O0 -W -Wall 
    -DMUDUO_STD_STRING 
    -D_REENTRANT 
    -D_FILE_OFFSET_BITS=64 
    -DAC_HAS_INFO 
    -DAC_HAS_WARNING 
    -DAC_HAS_ERROR 
    -DAC_HAS_CRITICAL 
    -DHAVE_CXX_STDHEADERS 
    -Wno-deprecated 
    ${CMAKE_CXX_FLAGS} 
    -std=c++11)

include_directories( ./ 
    /home/yinkangxi/mylib/include)

link_directories(
   ./ 
   /usr/local/lib
   /home/yinkangxi/mylib/lib/muduo)

add_library(simple_protostream ReadStream.cc WriteStream.cc Codec.cc)
target_link_libraries(simple_protostream muduo_net)

add_executable(test_client client.cc)
target_link_libraries(test_client muduo_net simple_protostream muduo_base pthread)

add_executable(test_server server.cc)
target_link_libraries(test_server muduo_net simple_protostream muduo_base pthread)
