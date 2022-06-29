set(SOURCES src/CPPLEDC/cppledc.cpp
            src/CPPLEDC/cppledctimer.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS . include)