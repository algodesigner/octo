/*
 * ls.h
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */

#ifndef LS_H_
#define LS_H_

/*
 * Traverses the directory entries and calles the specified function when an
 * entry of directory type is encountered.
 */
void ls(const char *, void (*)(struct dirent *));

#endif /* LS_H_ */
