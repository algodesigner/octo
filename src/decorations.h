/*
 * decorations.h
 * ANSI Colour and style definitions.
 *
 *  Created on: 18 Jul. 2018
 *      Author: Vlad
 */

#ifndef DECORATIONS_H_
#define DECORATIONS_H_

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RED_BR     "\x1b[31;1m"
#define ANSI_COLOR_GREEN_BR   "\x1b[32;1m"
#define ANSI_COLOR_YELLOW_BR  "\x1b[33;1m"
#define ANSI_COLOR_BLUE_BR    "\x1b[34;1m"
#define ANSI_COLOR_MAGENTA_BR "\x1b[35;1m"
#define ANSI_COLOR_CYAN_BR    "\x1b[36;1m"

#define ANSI_COLOR_RESET   "\x1b[0m"

#define ANSI_STYLE_BOLD      "\x1b[1m"
#define ANSI_STYLE_UNDERLINE "\x1b[4m"
#define ANSI_STYLE_REVERSED  "\x1b[7m"

#endif /* DECORATIONS_H_ */
