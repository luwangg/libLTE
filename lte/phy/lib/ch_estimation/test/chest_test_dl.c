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

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <complex.h>

#include "liblte/phy/phy.h"

lte_cell_t cell = {
  6,            // nof_prb
  1,    // nof_ports
  1000,         // cell_id
  CPNORM        // cyclic prefix
};

char *output_matlab = NULL;

void usage(char *prog) {
  printf("Usage: %s [recov]\n", prog);

  printf("\t-r nof_prb [Default %d]\n", cell.nof_prb);
  printf("\t-e extended cyclic prefix [Default normal]\n");

  printf("\t-c cell_id (1000 tests all). [Default %d]\n", cell.id);

  printf("\t-o output matlab file [Default %s]\n",output_matlab?output_matlab:"None");
  printf("\t-v increase verbosity\n");
}

void parse_args(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "recov")) != -1) {
    switch(opt) {
    case 'r':
      cell.nof_prb = atoi(argv[optind]);
      break;
    case 'e':
      cell.cp = CPEXT;
      break;
    case 'c':
      cell.id = atoi(argv[optind]);
      break;
    case 'o':
      output_matlab = argv[optind];
      break;
    case 'v':
      verbose++;
      break;
    default:
      usage(argv[0]);
      exit(-1);
    }
  }
}


int main(int argc, char **argv) {
  chest_dl_t est;
  precoding_t cheq; 
  cf_t *input = NULL, *ce = NULL, *h = NULL, *output = NULL;
  int i, j, n_port, sf_idx, cid, num_re;
  int ret = -1;
  int max_cid;
  FILE *fmatlab = NULL;
  
  parse_args(argc,argv);

  if (output_matlab) {
    fmatlab=fopen(output_matlab, "w");
    if (!fmatlab) {
      perror("fopen");
      goto do_exit;
    }
  }

  num_re = 2 * cell.nof_prb * RE_X_RB * CP_NSYMB(cell.cp);

  input = malloc(num_re * sizeof(cf_t));
  if (!input) {
    perror("malloc");
    goto do_exit;
  }
  output = malloc(num_re * sizeof(cf_t));
  if (!output) {
    perror("malloc");
    goto do_exit;
  }
  h = malloc(num_re * sizeof(cf_t));
  if (!h) {
    perror("malloc");
    goto do_exit;
  }
  ce = malloc(num_re * sizeof(cf_t));
  if (!ce) {
    perror("malloc");
    goto do_exit;
  }

  if (cell.id == 1000) {
    cid = 0;
    max_cid = 504;
  } else {
    cid = cell.id;
    max_cid = cell.id;
  }
  
  precoding_init(&cheq, num_re);

  while(cid <= max_cid) {
    cell.id = cid; 
    if (chest_dl_init(&est, cell)) {
      fprintf(stderr, "Error initializing equalizer\n");
      goto do_exit;
    }

    for (sf_idx=0;sf_idx<1;sf_idx++) {
      for (n_port=0;n_port<cell.nof_ports;n_port++) {

        bzero(input, sizeof(cf_t) * num_re);
        for (i=0;i<num_re;i++) {
          input[i] = 0.5-rand()/RAND_MAX+I*(0.5-rand()/RAND_MAX);
        }

        bzero(ce, sizeof(cf_t) * num_re);
        bzero(h, sizeof(cf_t) * num_re);

        refsignal_cs_put_sf(cell, n_port, 
                            est.csr_signal.pilots[n_port/2][sf_idx], input);

        for (i=0;i<2*CP_NSYMB(cell.cp);i++) {
          for (j=0;j<cell.nof_prb * RE_X_RB;j++) {
            float x = -1+(float) i/CP_NSYMB(cell.cp) + cosf(2 * M_PI * (float) j/cell.nof_prb/RE_X_RB);
            h[i*cell.nof_prb * RE_X_RB+j] = (3+x) * cexpf(I * x);
            input[i*cell.nof_prb * RE_X_RB+j] *= h[i*cell.nof_prb * RE_X_RB+j];            
          }
        }

        struct timeval t[3];
        gettimeofday(&t[1], NULL);
        for (int j=0;j<100;j++) {
          chest_dl_estimate_port(&est, input, ce, sf_idx, n_port);          
        }
        gettimeofday(&t[2], NULL);
        get_time_interval(t);
        printf("CHEST: %f us\n", (float) t[0].tv_usec/100);
        
        gettimeofday(&t[1], NULL);
        for (int j=0;j<100;j++) {
          predecoding_single(&cheq, input, ce, output, num_re, 0);
        }
        gettimeofday(&t[2], NULL);
        get_time_interval(t);
        printf("CHEQ-ZF: %f us\n", (float) t[0].tv_usec/100);

        float mse = 0;
        for (i=0;i<num_re;i++) {
          mse += cabsf(input[i]-output[i]);
        }
        mse /= num_re;
        printf("MSE: %f\n", mse);

        gettimeofday(&t[1], NULL);
        for (int j=0;j<100;j++) {
          predecoding_single(&cheq, input, ce, output, num_re, chest_dl_get_noise_estimate(&est));
        }
        gettimeofday(&t[2], NULL);
        get_time_interval(t);
        printf("CHEQ-MMSE: %f us\n", (float) t[0].tv_usec/100);
        
        mse = 0;
        for (i=0;i<num_re;i++) {
          mse += cabsf(input[i]-output[i]);
        }
        mse /= num_re;
        printf("MSE: %f\n", mse);

        if (mse > 1.7) {
          goto do_exit;
        }
        
        if (fmatlab) {
          fprintf(fmatlab, "input=");
          vec_fprint_c(fmatlab, input, num_re);
          fprintf(fmatlab, ";\n");
          fprintf(fmatlab, "h=");
          vec_fprint_c(fmatlab, h, num_re);
          fprintf(fmatlab, ";\n");
          fprintf(fmatlab, "ce=");
          vec_fprint_c(fmatlab, ce, num_re);
          fprintf(fmatlab, ";\n");
        }
      }
    }
    chest_dl_free(&est);
    cid+=10;
    INFO("cid=%d\n", cid);
  }


  ret = 0;

do_exit:

  precoding_free(&cheq);

  if (output) {
    free(output);
  }
  if (ce) {
    free(ce);
  }
  if (input) {
    free(input);
  }
  if (h) {
    free(h);
  }

  if (!ret) {
    printf("OK\n");
  } else {
    printf("Error at cid=%d, slot=%d, port=%d\n",cid, sf_idx, n_port);
  }

  exit(ret);
}
