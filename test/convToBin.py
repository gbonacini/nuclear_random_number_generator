#!/usr/bin/env python3

# -----------------------------------------------------------------
# GeigerRng - A Random Number Generator using an Arduino Geiger Shield
# Copyright (C) 2022  Gabriele Bonacini
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
# -----------------------------------------------------------------

import sys
import binascii

if len(sys.argv) != 3:
   print("Syntax: {} <input_file> <output_file>\n".format(sys.argv[0]))
   sys.exit(1)

with open(sys.argv[2], "wb") as outputFile:
   with open(sys.argv[1]) as inputFile:
       for hexValues in iter(lambda: inputFile.read(4), ''):
            if len(hexValues) == 4:
                le = '{}{}{}{}'.format(hexValues[2], hexValues[3], hexValues[0], hexValues[1])
                binValue = binascii.unhexlify(le)
                outputFile.write(binValue)
            else:
                print("Discarded Odd Ending (no padding permitted): {}\n".format(hexValues))
             
   
