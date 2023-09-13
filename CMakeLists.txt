cmake_minimum_required(VERSION 3.5)
project(flypen CXX)

# 设置MySQL Connector/C++的根目录路径

set(MYSQL_CONNECTOR_CPP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/mysql-connector")

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc mysql.cc user_controller.cc jwt_controller.cc MsgWebsock.cc)

# 链接MySQL Connector/C++库
# 包含MySQL Connector/C++的include目录
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${MYSQL_CONNECTOR_CPP_ROOT}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
)

# 链接MySQL Connector/C++库
# 包含MySQL Connector/C++的lib64目录
link_directories(${MYSQL_CONNECTOR_CPP_ROOT}/lib64)

# ##############################################################################
# 如果你在项目中本地包含了drogon源代码，请使用以下方法添加drogon
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# 并且注释掉以下几行
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon )
target_link_libraries(${PROJECT_NAME} PRIVATE -L${MYSQL_CONNECTOR_CPP_ROOT}/lib64 -lmysqlcppconn)

#link_directories(/home/lgy/c++kc/mysql-connector/lib64)
#
# ##############################################################################
if (CMAKE_CXX_STANDARD LESS 17)
    # 使用Boost来支持any、string_view和filesystem
    message(STATUS "use c++14")
    find_package(Boost 1.61.0 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR})
# 使用以下行来创建带有命名空间的视图
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# 使用以下行来创建带有前缀CHANGE_ME和路径命名空间的视图
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC})
# ##############################################################################
# 取消下面一行的注释以启用动态加载视图
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

add_subdirectory(test)
