/*
 * err_publisher.h
 *
 *  Created on: 14 Jul. 2018
 *      Author: Vlad
 */

#ifndef ERRPUBLISHER_H_
#define ERRPUBLISHER_H_

typedef struct err_publisher_st err_publisher;

err_publisher *err_publisher_new(void *, void (*)(void *, int, const char *));
void throw_err(err_publisher *, int, const char *, ...);
void err_publisher_destroy(err_publisher *);

#endif /* ERRPUBLISHER_H_ */
