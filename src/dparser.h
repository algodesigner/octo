/*
 * dparser.h
 *
 * A workspace definition file grammatic parser class.
 *
 *  Created on: 7 Jun. 2018
 *      Author: Vlad
 */

#ifndef DPARSER_H_
#define DPARSER_H_

#include "dconsumer.h"
#include "logger.h"

typedef struct dparser_st dparser;

dparser *dpaser_new(logger *, const struct dconsumer *);
void dparser_proc_char(dparser *, int);
void dparser_destroy(dparser *);

#endif /* DPARSER_H_ */
