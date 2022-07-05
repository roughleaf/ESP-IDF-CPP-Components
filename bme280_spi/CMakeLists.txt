set(SOURCES src/bme280_spi/bme280_spi.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include
                    REQUIRES bme280_common CPPSPI)