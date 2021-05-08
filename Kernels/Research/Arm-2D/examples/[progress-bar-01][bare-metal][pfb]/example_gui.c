/*
 * Copyright (c) 2009-2021 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*============================ INCLUDES ======================================*/
#include <stdio.h>
#include "platform.h"
#include "example_gui.h"
#include "./controls/controls.h"
#include <time.h>

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define arm_2d_layer(__TILE_ADDR, __TRANS, __X, __Y, ...)                       \
    {                                                                           \
        .ptTile = (__TILE_ADDR),                                                \
        .tRegion.tLocation.iX = (__X),                                          \
        .tRegion.tLocation.iY = (__Y),                                          \
        .chTransparency = (__TRANS),                                            \
        __VA_ARGS__                                                             \
    }

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

static volatile uint32_t s_wSystemTimeInMs = 0;
//static volatile bool s_bTimeout = false;
extern void platform_1ms_event_handler(void);

static volatile uint_fast16_t s_hwDemoProgress = 0;

void platform_1ms_event_handler(void)
{
    s_wSystemTimeInMs++; 
    if (!(s_wSystemTimeInMs & (_BV(4) - 1))) {
        //! every 256 ms
        
        srand(clock()); //!< update rand seed
        
        s_hwDemoProgress += ((uint64_t)rand() * 5ll) / (uint64_t)RAND_MAX;
        //s_chDemoProgress++;
        if (s_hwDemoProgress >= 1000) {
            s_hwDemoProgress = 1000;
        }
        
    }
}

void example_gui_init(void)
{
    controls_init();
    
}


void example_gui_do_events(void)
{

}

__WEAK 
void example_gui_on_refresh_evt_handler(const arm_2d_tile_t *ptFrameBuffer)
{
     ARM_2D_UNUSED(ptFrameBuffer);
}

void example_gui_refresh(const arm_2d_tile_t *ptFrameBuffer, bool bIsNewFrame)
{
    static uint_fast16_t s_hwProgress = 0;
    arm_2d_rgb16_fill_colour(ptFrameBuffer, NULL, GLCD_COLOR_BLACK);
    
    if (bIsNewFrame) {
        s_hwProgress = s_hwDemoProgress;
    }
    
    progress_bar_simple_show(ptFrameBuffer, s_hwProgress);
    
    example_gui_on_refresh_evt_handler(ptFrameBuffer);
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


