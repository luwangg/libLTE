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


lte_cell_t cell = {
  6,            // nof_prb
  1,            // nof_ports
  1000,         // cell_id
  CPNORM,       // cyclic prefix
  R_1,          // PHICH resources      
  PHICH_NORM    // PHICH length
};

phich_resources_t phich_res = R_1;
phich_length_t phich_length = PHICH_NORM;

void usage(char *prog) {
  printf("Usage: %s [cpvgel]\n", prog);
  printf("\t-c cell id [Default %d]\n", cell.id);
  printf("\t-p cell.nof_ports [Default %d]\n", cell.nof_ports);
  printf("\t-n cell.nof_prb [Default %d]\n", cell.nof_prb);
  printf("\t-g phich ng factor: 1/6, 1/2, 1, 2 [Default 1]\n");
  printf("\t-e phich extended length [Default normal]\n");
  printf("\t-l extended cyclic prefix [Default normal]\n");
  printf("\t-v [set verbose to debug, default none]\n");
}

void parse_args(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "cpnvgel")) != -1) {
    switch(opt) {
    case 'p':
      cell.nof_ports = atoi(argv[optind]);
      break;
    case 'n':
      cell.nof_prb = atoi(argv[optind]);
      break;
    case 'c':
      cell.id = atoi(argv[optind]);
      break;
    case 'g':
      if (!strcmp(argv[optind], "1/6")) {
        phich_res = R_1_6;
      } else if (!strcmp(argv[optind], "1/2")) {
        phich_res = R_1_2;
      } else if (!strcmp(argv[optind], "1")) {
        phich_res = R_1;
      } else if (!strcmp(argv[optind], "2")) {
        phich_res = R_2;
      } else {
        fprintf(stderr, "Invalid phich ng factor %s. Setting to default.\n", argv[optind]);
      }
      break;
    case 'e':
      phich_length = PHICH_EXT;
      break;
    case 'l':
      cell.cp = CPEXT;
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
  phich_t phich;
  regs_t regs;
  int i, j;
  cf_t *ce[MAX_PORTS];
  int nof_re;
  cf_t *slot_symbols[MAX_PORTS];
  uint8_t ack[50][PHICH_NORM_NSEQUENCES], ack_rx;
  uint32_t nsf, distance;
  int cid, max_cid;
  uint32_t ngroup, nseq, max_nseq;

  parse_args(argc,argv);

  max_nseq = CP_ISNORM(cell.cp)?PHICH_NORM_NSEQUENCES:PHICH_EXT_NSEQUENCES;

  nof_re = CPNORM_NSYMB * cell.nof_prb * RE_X_RB;

  /* init memory */
  for (i=0;i<MAX_PORTS;i++) {
    ce[i] = malloc(sizeof(cf_t) * nof_re);
    if (!ce[i]) {
      perror("malloc");
      exit(-1);
    }
    for (j=0;j<nof_re;j++) {
      ce[i][j] = 1;
    }
    slot_symbols[i] = malloc(sizeof(cf_t) * nof_re);
    if (!slot_symbols[i]) {
      perror("malloc");
      exit(-1);
    }
  }

  if (cell.id == 1000) {
    cid = 0;
    max_cid = 503;
  } else {
    cid = cell.id;
    max_cid = cell.id;
  }
  while(cid <= max_cid) {
    cell.id = cid;
    
    printf("Testing CellID=%d...\n", cid);

    if (regs_init(&regs, cell)) {
      fprintf(stderr, "Error initiating regs\n");
      exit(-1);
    }

    if (phich_init(&phich, &regs, cell)) {
      fprintf(stderr, "Error creating PBCH object\n");
      exit(-1);
    }

    for (nsf=0;nsf<10;nsf++) {

      phich_reset(&phich, slot_symbols);

      /* Transmit all PHICH groups and sequence numbers */
      for (ngroup=0;ngroup<phich_ngroups(&phich);ngroup++) {
        for (nseq=0;nseq<max_nseq;nseq++) {

          ack[ngroup][nseq] = rand()%2;

          phich_encode(&phich, ack[ngroup][nseq], ngroup, nseq, nsf, slot_symbols);
        }
      }
      /* combine outputs */
      for (i=1;i<cell.nof_ports;i++) {
        for (j=0;j<nof_re;j++) {
          slot_symbols[0][j] += slot_symbols[i][j];
        }
      }

      /* Receive all PHICH groups and sequence numbers */
      for (ngroup=0;ngroup<phich_ngroups(&phich);ngroup++) {
        for (nseq=0;nseq<max_nseq;nseq++) {

          if (phich_decode(&phich, slot_symbols[0], ce, 0, ngroup, nseq, nsf, &ack_rx, &distance)<0) {
            printf("Error decoding ACK\n");
            exit(-1);
          }
          INFO("%d/%d, ack_tx: %d, ack_rx: %d, ns: %d, distance: %d\n",
              ngroup, nseq, ack[ngroup][nseq], ack_rx, nsf, distance);
          if (ack[ngroup][nseq] != ack_rx) {
            printf("Invalid received ACK: %d!=%d\n", ack[ngroup][nseq], ack_rx);
            exit(-1);
          }
          if (distance) {
            printf("Error\n");
            exit(-1);
          }
        }
      }
    }
    phich_free(&phich);
    regs_free(&regs);
    cid++;
  }

  for (i=0;i<MAX_PORTS;i++) {
    free(ce[i]);
    free(slot_symbols[i]);
  }
  printf("OK\n");
  exit(0);
}
