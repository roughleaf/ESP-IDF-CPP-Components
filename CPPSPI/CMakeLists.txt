set(SOURCES src/CPPSPI/cppspi.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include)