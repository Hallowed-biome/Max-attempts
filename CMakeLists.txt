cmake_minimum_required(VERSION 3.25)

# Nome do teu projeto (pode ser o mesmo do mod)
project(AttemptsSelector)

# Busca e configura o SDK do Geode automaticamente na nuvem
find_package(Geode REQUIRED)

# Define que o teu código principal está na pasta src/main.cpp
add_geode_mod(
    ${PROJECT_NAME}
    VERSION 1.0.0
    CONTAINS_RESOURCES
    src/main.cpp
)

# Vincula as bibliotecas padrão do Geode ao teu mod
target_link_libraries(${PROJECT_NAME} EACH_GEODE_LINK)
