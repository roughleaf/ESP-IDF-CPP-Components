set(SOURCES src/bme280_i2c/bme280_i2c.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include
                    REQUIRES bme280_common CPPI2C)