/*
 * config.h
 *
 *  Created on: 4 Jul. 2018
 *      Author: vlad
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdbool.h>

typedef struct config_st config;

config *config_new();
char *config_parse_cmd_line(config *, int, char *[]);
int config_get_opt_limit(config *);
char *config_get_workspace_name(config *);
char *config_get_def_file_name(config *);
bool config_is_verbose(config *);
void config_destroy(config *);

#endif /* CONFIG_H_ */
