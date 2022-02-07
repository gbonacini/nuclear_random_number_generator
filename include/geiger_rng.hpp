// -----------------------------------------------------------------
// GeigerRng - A Random Number Generator using an Arduino Geiger Shield
// Copyright (C) 2022  Gabriele Bonacini
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
// -----------------------------------------------------------------

#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>

namespace geigerrng {

    class GeigerRng{
         public:
            void detect(void)                                        noexcept;
            static GeigerRng* getInstance(unsigned int speed=9600)   noexcept;

         private:
            const unsigned int         THRESHOLD[5]         = { 45, 95, 200, 400, 600 };
            static const unsigned int  GEIGER_PIN           = 2,
                                       MIN_RESULT           = 0,
                                       MAX_RESULT           = 15,
                                       INVALID_RESULT       = MAX_RESULT + 1,
                                       MSEC_FOR_MINUTE      = 60000,
                                       POLLING_RATE         = 10000,
                                       PRINT_RATE           = 2000,
                                       POLLING_SLOTS        = MSEC_FOR_MINUTE / POLLING_RATE;
            static const char          VALUES[MAX_RESULT+1];

            static_assert( MIN_RESULT <  MAX_RESULT ); 
            static_assert( MAX_RESULT < INVALID_RESULT ); 
            static_assert( MSEC_FOR_MINUTE == 60000 ); 
            static_assert( POLLING_RATE > 0 && POLLING_RATE <= MSEC_FOR_MINUTE ); 

            // Conversion factor for this tube - CPM to uSV/h
            const float            TUBE_CONV_FACTOR = 0.00812;

            static GeigerRng*      instance;

            unsigned int           ledArray[5],
                                   consoleSpeed; 
            int                    geiger_input;
            static long            count,
                                   genCount,
                                   lastCount;
            long                   countPerMinute,
                                   timePrevious,
                                   timePreviousMeasure,
                                   timePreviousPrint,
                                   time,
                                   countPrevious;
            float                  radiationValue;
            static unsigned short  roulette,
                                   lastRnd;

            LiquidCrystal          lcd;

            explicit GeigerRng(unsigned int speed)       noexcept;
            void ledVar(int value)                       noexcept;
            static void increaseCount(void)              noexcept;
    };

}
