/*
 * universe.h
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include "logger.h"

typedef struct universe_st universe;

universe *universe_new(logger *, const char *);
void universe_accept(universe *, void *,
		void (*)(void *, const char *, const char *, const char *));
void universe_destroy(universe *);

#endif /* UNIVERSE_H_ */
