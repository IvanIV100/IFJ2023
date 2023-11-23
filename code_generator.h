#include "symtable.h"
#include "scanner.h"
#include "parser.c"
#include "parser.h"

// code_generator.h

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "symtable.h"
#include "parser.h"

#define MAX_VAR 256
#define MAX_LIT 8192