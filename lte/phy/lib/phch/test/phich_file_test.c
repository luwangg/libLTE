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
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "liblte/phy/phy.h"

char *input_file_name = NULL;
char *matlab_file_name = NULL;

lte_cell_t cell = {
  50,           // cell.nof_prb
  2,            // cell.nof_ports
  150,          // cell.id
  CPNORM,       // cyclic prefix
  R_1,          // PHICH resources      
  PHICH_NORM    // PHICH length
};

int flen;
int nof_ctrl_symbols = 1;
int numsubframe = 0;

FILE *fmatlab = NULL;

filesource_t fsrc;
cf_t *input_buffer, *fft_buffer, *ce[MAX_PORTS];
phich_t phich;
regs_t regs;
lte_fft_t fft;
chest_dl_t chest;

void usage(char *prog) {
  printf("Usage: %s [vcoe] -i input_file\n", prog);
  printf("\t-o output matlab file name [Default Disabled]\n");
  printf("\t-c cell.id [Default %d]\n", cell.id);
  printf("\t-p cell.nof_ports [Default %d]\n", cell.nof_ports);
  printf("\t-n cell.nof_prb [Default %d]\n", cell.nof_prb);
  printf("\t-f nof control symbols [Default %d]\n", nof_ctrl_symbols);
  printf("\t-g phich ng factor: 1/6, 1/2, 1, 2 [Default 1]\n");
  printf("\t-e phich extended length [Default normal]\n");
  printf("\t-l extended cyclic prefix [Default normal]\n");
  printf("\t-v [set verbose to debug, default none]\n");
}

void parse_args(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "iovcenpfgl")) != -1) {
    switch(opt) {
    case 'i':
      input_file_name = argv[optind];
      break;
    case 'o':
      matlab_file_name = argv[optind];
      break;
    case 'c':
      cell.id = atoi(argv[optind]);
      break;
    case 'f':
      nof_ctrl_symbols = atoi(argv[optind]);
      break;
    case 'g':
      if (!strcmp(argv[optind], "1/6")) {
        cell.phich_resources = R_1_6;
      } else if (!strcmp(argv[optind], "1/2")) {
        cell.phich_resources = R_1_2;
      } else if (!strcmp(argv[optind], "1")) {
        cell.phich_resources = R_1;
      } else if (!strcmp(argv[optind], "2")) {
        cell.phich_resources = R_2;
      } else {
        fprintf(stderr, "Invalid phich ng factor %s. Setting to default.\n", argv[optind]);
      }
      break;
    case 'e':
      cell.phich_length = PHICH_EXT;
      break;
    case 'n':
      cell.nof_prb = atoi(argv[optind]);
      break;
    case 'p':
      cell.nof_ports = atoi(argv[optind]);
      break;
    case 'v':
      verbose++;
      break;
    case 'l':
      cell.cp = CPEXT;
      break;
    default:
      usage(argv[0]);
      exit(-1);
    }
  }
  if (!input_file_name) {
    usage(argv[0]);
    exit(-1);
  }
}

int base_init() {
  int i;

  if (filesource_init(&fsrc, input_file_name, COMPLEX_FLOAT_BIN)) {
    fprintf(stderr, "Error opening file %s\n", input_file_name);
    exit(-1);
  }

  if (matlab_file_name) {
    fmatlab = fopen(matlab_file_name, "w");
    if (!fmatlab) {
      perror("fopen");
      return -1;
    }
  } else {
    fmatlab = NULL;
  }

  flen = SLOT_LEN(lte_symbol_sz(cell.nof_prb));

  input_buffer = malloc(flen * sizeof(cf_t));
  if (!input_buffer) {
    perror("malloc");
    exit(-1);
  }

  fft_buffer = malloc(SF_LEN_RE(cell.nof_prb, cell.cp) * sizeof(cf_t));
  if (!fft_buffer) {
    perror("malloc");
    return -1;
  }

  for (i=0;i<MAX_PORTS;i++) {
    ce[i] = malloc(SF_LEN_RE(cell.nof_prb, cell.cp) * sizeof(cf_t));
    if (!ce[i]) {
      perror("malloc");
      return -1;
    }
  }

  if (chest_dl_init(&chest, cell)) {
    fprintf(stderr, "Error initializing equalizer\n");
    return -1;
  }

  if (lte_fft_init(&fft, cell.cp, cell.nof_prb)) {
    fprintf(stderr, "Error initializing FFT\n");
    return -1;
  }

  if (regs_init(&regs, cell)) {
    fprintf(stderr, "Error initiating regs\n");
    return -1;
  }

  if (phich_init(&phich, &regs, cell)) {
    fprintf(stderr, "Error creating PBCH object\n");
    return -1;
  }

  DEBUG("Memory init OK\n",0);
  return 0;
}

void base_free() {
  int i;

  filesource_free(&fsrc);
  if (fmatlab) {
    fclose(fmatlab);
  }

  free(input_buffer);
  free(fft_buffer);

  filesource_free(&fsrc);
  for (i=0;i<MAX_PORTS;i++) {
    free(ce[i]);
  }
  chest_dl_free(&chest);
  lte_fft_free(&fft);

  phich_free(&phich);
  regs_free(&regs);
}

int main(int argc, char **argv) {
  uint32_t distance;
  int n;
  uint32_t ngroup, nseq, max_nseq;
  uint8_t ack_rx;

  if (argc < 3) {
    usage(argv[0]);
    exit(-1);
  }

  parse_args(argc,argv);

  max_nseq = CP_ISNORM(cell.cp)?PHICH_NORM_NSEQUENCES:PHICH_EXT_NSEQUENCES;

  if (base_init()) {
    fprintf(stderr, "Error initializing memory\n");
    exit(-1);
  }

  n = filesource_read(&fsrc, input_buffer, flen);

  lte_fft_run_sf(&fft, input_buffer, fft_buffer);

  if (fmatlab) {
    fprintf(fmatlab, "infft=");
    vec_fprint_c(fmatlab, input_buffer, flen);
    fprintf(fmatlab, ";\n");

    fprintf(fmatlab, "outfft=");
    vec_fprint_c(fmatlab, fft_buffer, CP_NSYMB(cell.cp) * cell.nof_prb * RE_X_RB);
    fprintf(fmatlab, ";\n");
  }

  /* Get channel estimates for each port */
  chest_dl_estimate(&chest, fft_buffer, ce, 0);

  INFO("Decoding PHICH\n", 0);

  /* Receive all PHICH groups and sequence numbers */
  for (ngroup=0;ngroup<phich_ngroups(&phich);ngroup++) {
    for (nseq=0;nseq<max_nseq;nseq++) {

      if (phich_decode(&phich, fft_buffer, ce, chest_dl_get_noise_estimate(&chest), ngroup, nseq, numsubframe, &ack_rx, &distance)<0) {
        printf("Error decoding ACK\n");
        exit(-1);
      }

      INFO("%d/%d, ack_rx: %d, ns: %d, distance: %d\n",
          ngroup, nseq, ack_rx, numsubframe, distance);
    }
  }

  base_free();

  if (n < 0) {
    fprintf(stderr, "Error decoding phich\n");
    exit(-1);
  } else if (n == 0) {
    printf("Could not decode phich\n");
    exit(-1);
  } else {
    exit(0);
  }
}
