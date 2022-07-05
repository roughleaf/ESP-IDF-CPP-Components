set(SOURCES src/bme280_common/bme280_common.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include)