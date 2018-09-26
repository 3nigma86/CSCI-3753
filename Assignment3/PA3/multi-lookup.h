#ifndef MULTI_LOOKUP_H
#define MULTI_LOOKUP_H

// libraries as in lookup.c and pthread_hello_world.c
#include <pthread.h>		// pthreads!
#include <stdlib.h>		
#include <stdio.h>		// for file I/O, stderr
#include <errno.h>		// for errno

// necessary functions to include:
#include "util.h"		// for DNS lookup
#include "queue.h"		// queue for hostnames.

// global variables as in lookup.c
#define MINARGS 3
#define USAGE "<inputFilePath> <inputFilePath> ... <outputFilePath>"
#define SBUFSIZE 20
#define INPUTFS "%1024s"
#define MAX_RESOLVER_THREADS 10
#define MAX_REQUESTER_THREADS 5
#define MAX_IP_LENGTH INET6_ADDRSTRLEN


// function to push hostnames into the queue.
void* readFiles(char* filename);


// function to perform DNS lookup.
void* dns();


// main
int main(int argc, char* argv[]);

#endif