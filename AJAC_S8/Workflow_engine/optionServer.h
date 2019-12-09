#ifndef __OPTIONSERVER_H__
#define __OPTIONSERVER_H__

extern char usersFile[100];

struct OptionArg
{
	char portNum [100];
	char File[100];
};

struct OptionArg check_arguments(int argc, char **argv);

#endif
