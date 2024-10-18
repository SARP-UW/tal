/**
 * This file is part of the Titan Flight Computer Project
 * Copyright (c) 2024 UW SARP
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * @file common/runtime/debug.h
 * @authors Aaron McBride
 * @brief Runtime debug utilites.
 */

#pragma once

#if defined(__cplusplus)
  extern "C" {
#endif

  #if defined(TI_DEBUG)

    bool ti_assert(const bool cond, const char* msg);
    void ti_bkpt(void);
    bool ti_bkpt_if(const bool cond);

  #elif

    #define ti_assert(cond, msg) ((void)msg, (bool)cond)
    #define ti_bkpt()
    #define ti_bkpt_if(cond) ((bool)cond)

  #endif

#if defined(__cplusplus)
  }
#endif