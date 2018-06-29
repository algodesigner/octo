/*
 * dconsumer.h
 *
 *  Created on: 10 Jun. 2018
 *      Author: vlad
 */

#ifndef DCONSUMER_H_
#define DCONSUMER_H_

struct dconsumer {
	void *instance;
	void (*add_project)(void *, const char *);
	void (*add_workspace)(void *, const char *, const char *);
	void (*add_workspace_project)(void *, const char *, const char *);
};

#endif /* DCONSUMER_H_ */
