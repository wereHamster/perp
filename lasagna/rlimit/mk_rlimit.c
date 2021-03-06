/* mk_rlimit.c
** generate tables for resource limits defined for platform
** wcm, 2009.09.17 - 2010.12.01
** ===
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/resource.h>

/* struct rlimit_def: */
#include "rlimit_private.h"

const char progname[] = "mk_rlimit";

/*
** all resource limits defined APUE/2e, fig 7.15 (p. 204):
** note: RLIMIT_AS and RLIMIT_VMEM are synonymous
*/
static struct rlimit_def rlimit_defs[] = {
#ifdef RLIMIT_AS
  {RLIMIT_AS, "RLIMIT_AS", "maximum total available memory (bytes)"},
#ifndef RLIMIT_VMEM
  /* provide RLIMIT_VMEM as synonym: */
  {RLIMIT_AS, "RLIMIT_VMEM", "maximum total available memory (bytes)"},
#endif
#endif
#ifdef RLIMIT_CORE
  {RLIMIT_CORE, "RLIMIT_CORE", "maximum size core file (bytes)"},
#endif
#ifdef RLIMIT_CPU
  {RLIMIT_CPU, "RLIMIT_CPU", "maximum cpu time per process (seconds)"},
#endif
#ifdef RLIMIT_DATA
  {RLIMIT_DATA, "RLIMIT_DATA", "maximum data segment per process (bytes)"},
#endif
#ifdef RLIMIT_FSIZE
  {RLIMIT_FSIZE, "RLIMIT_FSIZE", "maximum file size that may be created (bytes)"},
#endif
#ifdef RLIMIT_LOCKS
  {RLIMIT_LOCKS, "RLIMIT_LOCKS", "maximum number of file locks that may be held (locks)"},
#endif
#ifdef RLIMIT_MEMLOCK
  {RLIMIT_MEMLOCK, "RLIMIT_MEMLOCK", "maximum memory a process may mlock() (bytes)"},
#endif
#ifdef RLIMIT_NOFILE
  {RLIMIT_NOFILE, "RLIMIT_NOFILE", "maximum number of open files per process (files)"},
#endif
#ifdef RLIMIT_NPROC
  {RLIMIT_NPROC, "RLIMIT_NPROC", "maximum number of child processes per user id (processes)"},
#endif
#ifdef RLIMIT_RSS
  {RLIMIT_RSS, "RLIMIT_RSS", "maximum resident set size (bytes)"},
#endif
#ifdef RLIMIT_SBSIZE
  {RLIMIT_SBSIZE, "RLIMIT_SBSIZE", "maximum size of socket buffers (bytes)"},
#endif
#ifdef RLIMIT_STACK
  {RLIMIT_STACK, "RLIMIT_STACK", "maximum size of stack segment per process (bytes)"},
#endif
#ifdef RLIMIT_VMEM
  {RLIMIT_VMEM, "RLIMIT_VMEM", "maximum total available memory (bytes)"},
#ifndef RLIMIT_AS
  {RLIMIT_VMEM, "RLIMIT_AS", "maximum total available memory (bytes)"},
#endif
#endif
};


static
int rlimit_compare(const void *p1, const void *p2)
{
  struct rlimit_def *rd1 = (struct rlimit_def *)p1;
  int                r1 = rd1->r;
  struct rlimit_def *rd2 = (struct rlimit_def *)p2;
  int                r2 = rd2->r;

  if(r1 < r2) return -1;
  if(r1 == r2) return 0;
  /* else: */
  return 1;
}


static
size_t rlimit_init(void)
{
  size_t items = (sizeof rlimit_defs / sizeof(struct rlimit_def));

  qsort(rlimit_defs, items, sizeof(struct rlimit_def), &rlimit_compare);

  return items;
}


static
void
do_rlimit(void)
{
  size_t  rlimit_items = 0;
  size_t  i;

  rlimit_items = rlimit_init();
  fprintf(stderr, "%s: sorted %zu items in rlimit table\n", progname, rlimit_items);

  printf(
      "/* rlimit_defs.c.in\n"
      "** automatically generated by %s (%s) \n"
      "*/\n\n", progname, __FILE__ );

  printf(
      "/*\n"
      "** %zu sorted rlimit_def entries in rlimit_defs[] table:\n"
      "*/\n", rlimit_items);

  printf(
    "static const struct rlimit_def rlimit_defs[] = {\n");

  for(i = 0; i < rlimit_items; ++i){
      printf("  {%d, \"%s\", \"%s\"},\n", rlimit_defs[i].r,
                                          rlimit_defs[i].name,
                                          rlimit_defs[i].mesg);
  }

  printf("};\n\n");

  printf(
    "/* eof (rlimit_defs.c.in) */\n");

  return;
}



int
main(int argc, char *argv[])
{
  /* silence compiler warning unused parameters (nv): */
  (void)argc;
  (void)argv;

  do_rlimit();
  return 0;
}


/* eof (mk_rlimit.c) */
