set(SRC_FILES
    "main.c"
    "epd_display/epd_display.c"  
)

set(REQ_COMPONENTS
    spiffs
    esp_driver_sdspi
    fatfs
    json
    nvs_flash
    esp_wifi
)

idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS "."
    REQUIRES ${REQ_COMPONENTS}
)
