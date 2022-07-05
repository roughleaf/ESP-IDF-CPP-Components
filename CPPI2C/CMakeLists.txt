set(SOURCES src/CPPI2C/cppi2c.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include)