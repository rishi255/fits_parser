cmake_minimum_required(VERSION 3.10)

# set the project name
project(FITS_Parser)

set(HEADER_DIRECTORY /source/headers)

set(SOURCE source/main.cpp source/fits_parser.cpp source/card.cpp)
set(HEADER_FILES ${HEADER_DIRECTORY}/card.hpp ${HEADER_DIRECTORY}/fits_parser.hpp)
# add the executable
add_executable(
    ${PROJECT_NAME} 
    ${SOURCE} 
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${HEADER_DIRECTORY}
)