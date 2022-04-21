cmake_minimum_required(VERSION 3.21)
project(diploma_project)

set(CMAKE_CXX_STANDARD 14)

add_executable(yzip yzip.cpp version/version_etc.cpp error/error.cpp getfile/get_file.cpp file_handle/file_handle.cpp
        bitfile/bitfile.cpp lzw/lzw.cpp lzw/lzw.h file_info_struct.h
        endswith/endswith.cpp endswith/endswith.h encryption/encryption.cpp encryption/encryption.h
        package/get_cur_time.cpp package/get_cur_time.h package/mode_to_letters.cpp package/mode_to_letters.h package/pack.cpp package/pack.h
        is_file_exist/is_file_exist.cpp is_file_exist/is_file_exist.h size_h/size_h.cpp size_h/size_h.h verbose_info/verbose_info.cpp verbose_info/verbose_info.h
        file_handle/file_handle.cpp file_handle/file_handle.h encryption/aes_128_cbc.cpp encryption/aes_128_cbc.h)
target_link_libraries(yzip crypto)
