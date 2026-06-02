# IDF 5.5.4 Compatibility Notes

## AMYSYNTH_N16R8 Board Build

This documents the IDF 5.5.4 compatibility fixes required to build the `AMYSYNTH_N16R8` board target against ESP-IDF v5.5.4.

### 1. `ldgen.cmake` — OBJECT_LIBRARY skipping

**File:** `/root/esp/esp-idf-v5.5.4/tools/cmake/ldgen.cmake` (IDF installation, line ~129)

IDF 5.5.4's linker script generator iterates over all link dependencies to collect library files. MicroPython's `micropy_extmod_btree` is built as a CMake `OBJECT_LIBRARY`. The IDF tool skipped `INTERFACE_LIBRARY` but not `OBJECT_LIBRARY`, causing a CMake error when trying to get `$<TARGET_FILE:micropy_extmod_btree>`.

**Fix:** Added `OR lib_type STREQUAL "OBJECT_LIBRARY"` to the skip condition.

### 2. mdns — IDF Component Registry blocked

**Files:** `tulip/amyboard/main/idf_component.yml`, `tulip/amyboard/esp32_common.cmake`, `tulip/amyboard/components/mdns/`

The `idf_component.yml` referenced `espressif/mdns: "~1.1.0"` from the IDF component registry. In this build environment, `components-file.espressif.com` returns 403.

**Fix:**
- Removed `espressif/mdns` from `main/idf_component.yml`
- Cloned `espressif/esp-protocols` from GitHub and copied `components/mdns` to `tulip/amyboard/components/mdns/`
- Added `mdns` to the `IDF_COMPONENTS` list in `esp32_common.cmake` so include dirs propagate

### 3. `machine_timer.c` — `timer_ll_set_clock_source` signature change

**File:** `micropython/ports/esp32/machine_timer.c` (line 158)

In IDF 5.5.4, `timer_ll_set_clock_source` changed its first argument from `timg_dev_t *hw` (pointer to peripheral registers) to `int group_id` (integer group index). All other `timer_ll_*` calls in the same file still use `self->hal_context.dev` and were unchanged.

**Fix:** Changed `self->hal_context.dev` → `self->group` for this one call.

### 4. `modespnow.c` — `esp_now_register_send_cb` callback type change

**File:** `micropython/ports/esp32/modespnow.c` (lines ~182, ~542)

In IDF 5.5.4, the `esp_now_send_cb_t` type changed from `void (*)(const uint8_t *mac_addr, ...)` to `void (*)(const esp_now_send_info_t *tx_info, ...)` where `esp_now_send_info_t` is `wifi_tx_info_t`.

**Fix:** Added `#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 0)` guards around both the forward declaration and the function definition, using the new `esp_now_send_info_t *` parameter type for IDF ≥5.5.0. The callback body was unchanged since it never used `mac_addr`.

---

## AMYSYNTH_N16R8 Hardware Configuration

- **MCU:** ESP32-S3, N16R8 (16 MB flash, 8 MB OPI PSRAM)
- **I2S (PCM5102 DAC):** BCLK=GPIO4, LRCLK=GPIO5, DOUT=GPIO6, no MCLK
- **UART MIDI in:** GPIO16 (UART1 RX)
- **USB:** TinyUSB CDC (REPL) + MIDI device class
- **No display, touch, WiFi runtime, BLE, or BBS**
