#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "sort.h"
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

int colno;

// First argument smaller than second < 0
// Compare the values in the column number specified
static int
comprec(const void *p1, const void *p2) {
  unsigned int *arg1 = ((rec_dataptr_t *)p1) -> data_ptr;
  unsigned int *arg2 = ((rec_dataptr_t *)p2) -> data_ptr;
  unsigned int colno1 = ((rec_dataptr_t *)p1) -> data_ints;
  unsigned int colno2 = ((rec_dataptr_t *)p2) -> data_ints;

  if ((colno1 - 1) < colno) {
    if ((colno2 - 1) < colno) {
      return (arg1[colno1-1] > arg2[colno2 - 1]);
    } else {
    return (arg1[colno1-1] > arg2[colno]);
    }
  }
  if ((colno2 - 1) < colno) {
    return (arg1[colno] > arg2[colno2 -1]);
  }
  return (arg1[colno] > arg2[colno]);
}

void
usage() {
  fprintf(stderr, "Usage: ./varsort -i inputfile -o outputfile [-c column]\n");
  exit(1);
}

int
main(int argc, char *argv[]) {
  // arguments
  colno = 0;
  char *outfile = "no/such/file";
  char *infile = "no/such/file";

  char *arg1 = argv[1];
  // check for number of arguments
  if (argc < 5 || argc > 7 || arg1[0] != '-') usage ();

  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "i:o:c:")) != -1) {
    switch (c) {
    case 'i':
      infile = strdup(optarg);
      break;
    case 'o':
      outfile = strdup(optarg);
      break;
    case 'c':
      colno = atoi(optarg);
      if (colno < 0) {
        fprintf(stderr, "Error: Column should be a non-negative integer\n");
        exit(1);
      }
      break;
    default:
      usage();
    }
  }

  // open input file
  int inf = open(infile, O_RDONLY);

  // open output file
  int outf = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);

  // Error checking
  if (inf < 0) {
    fprintf(stderr, "Error: Cannot open file %s\n", infile);
    exit(1);
  }

  if (outf < 0) {
    fprintf(stderr, "Error: Cannot open file %s\n", outfile);
    exit(1);
  }

  // read the input file
  int recordsleft;
  int rc;

  rc = read(inf, &recordsleft, sizeof(recordsleft));
  // Error checking for read
  if (rc != sizeof(recordsleft)) {
    perror("read");
    exit(1);
  }

  // Create data structures
  rec_dataptr_t *head = NULL;
  head = malloc(recordsleft*sizeof(rec_dataptr_t));

  for (int k = 0 ; k < recordsleft; k++) {
    rec_dataptr_t r;

    // Read the fixed-sized portion of record: size of data
    rc = read(inf, &r, sizeof(rec_nodata_t));
    if (rc != sizeof(rec_nodata_t)) {
      perror("read");
      exit(1);
    }

    r.data_ptr = (unsigned int *) malloc(r.data_ints*sizeof(unsigned int));
    if (r.data_ptr == NULL) {
      fprintf(stderr, "Error: Cannot allocate memory\n");
      exit(1);
    }

    // Read the variable portion of the record
    for (int i = 0; i < r.data_ints; i++) {
    rc = read(inf, &r.data_ptr[i], sizeof(unsigned int));
    if (rc !=  sizeof(unsigned int)) {
      perror("read");
      exit(1);
      }
    }

    head[r.index] = r;
  }

  (void) close(inf);

  // SORT
  qsort(head, recordsleft, sizeof(rec_dataptr_t), comprec);

  // Write to file
  int wc = write(outf, &recordsleft, sizeof(recordsleft));
  if (wc != sizeof(recordsleft)) {
    perror("write");
    exit(1);
  }

  for (int i = 0; i < recordsleft; i++) {
    rec_dataptr_t r = head[i];
    wc = write(outf, &r, sizeof(rec_nodata_t));
    for (int j = 0; j < r.data_ints; j++) {
       wc = write(outf, &r.data_ptr[j], sizeof(unsigned int));
    }
    free(r.data_ptr);
  }

  free(head);
  free(infile);
  free(outfile);
  (void) close(outf);
}
