/******************************************************************************
 * Student Name    :  Dallas Hall
 * RMIT Student ID :  s3461243
 * COURSE CODE     :  CPT220 / OLA00
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 2, 2019.
 *****************************************************************************/
#include "shared.h"
#include <errno.h>
#include <string.h>

#ifndef HELPERS_H
#define HELPERS_H

/**
 * you may add your own data structures for the helpers module here
 **/

/**
 * end of students data structures.
 **/

/**
 * public functions available from this module - feel free to add more of your
 * own
 **/
char *
strdup(const char *);

char *
fold(const char[]);

int rollDice(void);

#endif
