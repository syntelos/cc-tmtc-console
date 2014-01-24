/*
 * ISS Live Qt
 * Copyright (C) 2014, John Pritchard, Syntelos
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * (LGPL and GPL) as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL
 * and GPL for more details.
 *
 * You should have received a copy of the LGPL and GPL along with this
 * program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "S1000006.h"

const S1000006 S1000006::I;

S1000006::S1000006()
    : ISSLSchematic("S1000006","CRONUS","STR_S_MDM","event","64","enum","0=Off|1=On|3=Failed","Starboard Thermal Radiator (STR) Multiplexer/Demultiplexer (MDM) on-off status","Starboard Thermal Radiator (STR) Multiplexer/Demultiplexer (MDM) on-off status")
{
}

