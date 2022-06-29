set(SOURCES src/CPPANALOG/cppadc.cpp
            src/CPPANALOG/cppdac.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS . include
                    REQUIRES "esp_adc_cal")