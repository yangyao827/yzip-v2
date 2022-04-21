//
// Created by yangyao on 2022/3/10.
//
#pragma once
#ifndef DIPLOMA_PROJECT_SYSTEM_H
#define DIPLOMA_PROJECT_SYSTEM_H
#endif //DIPLOMA_PROJECT_SYSTEM_H
#define proper_name(x) x

#include "/usr/include/bash/include/gettext.h"

#define _(msgid) gettext (msgid)
#define N_(msgid) msgid

#define emit_try_help() \
  do \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               program_name); \
    } \
  while (0)


#include <limits.h>

enum {
    GETOPT_HELP_CHAR = (CHAR_MIN - 2),
    GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

#define GETOPT_HELP_OPTION_DECL \
  "help", no_argument, NULL, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, NULL, GETOPT_VERSION_CHAR

#define case_GETOPT_HELP_CHAR            \
  case GETOPT_HELP_CHAR:            \
    usage (EXIT_SUCCESS);            \
    break;

#define case_GETOPT_VERSION_CHAR(ProgramName, Authors)            \
  case GETOPT_VERSION_CHAR:                        \
    version_etc ( ProgramName, CODE_TIME,VERSION, Authors);                    \
    exit (EXIT_SUCCESS);                        \
    break;