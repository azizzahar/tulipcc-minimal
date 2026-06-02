set(IDF_TARGET esp32s3)

set(MICROPY_PY_TINYUSB ON)

set(BOARD_DEFINITION1 AMYSYNTH_N16R8)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    ../esp32s3/boards/sdkconfig.tulip
    boards/AMYSYNTH_N16R8/sdkconfig.board
)
