cmake_minimum_required(VERSION 3.25)

project(max_attempts)

find_package(Geode REQUIRED)

setup_geode_mod(max_attempts)

# Seus códigos C++ (ajuste o caminho se não estiver na pasta src)
target_sources(max_attempts PRIVATE src/main.cpp)
