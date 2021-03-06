/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2014 The libLTE Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution.
 *
 * \section LICENSE
 *
 * This file is part of the libLTE library.
 *
 * libLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */


#ifndef MEXUTILS_
#define MEXUTILS_


#include <stdbool.h>

#ifdef UNDEF_BOOL
#undef bool
#endif

#include "mex.h"

#include "liblte/config.h"

typedef _Complex float cf_t; 

LIBLTE_API bool mexutils_isScalar(const mxArray *ptr);

LIBLTE_API int mexutils_read_cell(const mxArray *ptr, 
                                  lte_cell_t *cell);

LIBLTE_API char *mexutils_get_char_struct(const mxArray *ptr, 
                                          const char *field_name); 

LIBLTE_API int mexutils_read_uint32_struct(const mxArray *ptr, 
                                           const char *field_name, 
                                           uint32_t *value); 

LIBLTE_API int mexutils_write_f(float *buffer, 
                                mxArray **ptr, 
                                uint32_t nr, 
                                uint32_t nc);

LIBLTE_API int mexutils_write_cf(cf_t *buffer, 
                                 mxArray **ptr, 
                                 uint32_t nr, 
                                 uint32_t nc);

LIBLTE_API int mexutils_write_uint8(uint8_t *buffer, 
                                    mxArray **ptr, 
                                    uint32_t nr, 
                                    uint32_t nc);

LIBLTE_API int mexutils_write_int(int *buffer, 
                                  mxArray **ptr, 
                                  uint32_t nr, 
                                  uint32_t nc);

LIBLTE_API int mexutils_read_f(const mxArray *ptr, 
                               float **buffer);

LIBLTE_API int mexutils_read_cf(const mxArray *ptr, 
                                cf_t **buffer);
  
#endif
