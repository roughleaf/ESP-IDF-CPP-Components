set(SOURCES src/CPPGPIO/cppgpio_input.cpp
            src/CPPGPIO/cppgpio_output.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include)
