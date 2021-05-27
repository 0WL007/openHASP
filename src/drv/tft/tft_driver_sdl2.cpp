/* MIT License - Copyright (c) 2019-2021 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#if defined(WINDOWS) || defined(POSIX)

#include "lvgl.h"
#include <SDL2/SDL.h>

#include "display/monitor.h"
#include "indev/mouse.h"

#include "drv/tft/tft_driver.h"
#include "tft_driver_sdl2.h"

#include "dev/device.h"
#include "hasp_debug.h"

namespace dev {

/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void* data)
{
    (void)data;

    while(1) {
        SDL_Delay(5);   /*Sleep for 5 millisecond*/
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}

void TftSdl::init(int w, int h)
{

// Workaround for sdl2 `-m32` crash
// https://bugs.launchpad.net/ubuntu/+source/libsdl2/+bug/1775067/comments/7
#ifndef WIN32
    setenv("DBUS_FATAL_WARNINGS", "0", 1);
#endif

    /* Add a display
     * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init(w, h); // (MONITOR_HOR_RES, MONITOR_VER_RES);
    monitor_title(haspDevice.get_hostname());

    /* Add the mouse as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    mouse_init();

    /* Tick init.
     * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about how much time were elapsed
     * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);
}
void TftSdl::show_info()
{
    SDL_version linked;
    SDL_GetVersion(&linked);
    LOG_VERBOSE(TAG_TFT, F("Driver     : SDL2"));
    LOG_VERBOSE(TAG_TFT, F("SDL Version: v%d.%d.%d"), linked.major, linked.minor, linked.patch);
}

void TftSdl::splashscreen()
{
    // tft.fillScreen(TFT_DARKCYAN);
    // int x = (tft.width() - logoWidth) / 2;
    // int y = (tft.height() - logoHeight) / 2;
    // tft.drawXBitmap(x, y, bootscreen, logoWidth, logoHeight, TFT_WHITE);
}
void TftSdl::set_rotation(uint8_t rotation)
{}
void TftSdl::set_invert(bool invert)
{}
void TftSdl::flush_pixels(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
    monitor_flush(disp, area, color_p);
}
bool TftSdl::is_driver_pin(uint8_t pin)
{
    return false;
}
const char* TftSdl::get_tft_model()
{
    return "SDL2";
}

} // namespace dev

dev::TftSdl haspTft;

#endif // WINDOWS || POSIX