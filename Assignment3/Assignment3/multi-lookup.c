
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include "util.h"
#include "multi-lookup.h"

		 
//mutex locks
pthread_mutex_t arrayLock;	
pthread_mutex_t requestorLock;		
pthread_mutex_t resolverLock;



char hostname[SBUFSIZE][SBUFSIZE];   
char firstipstr[INET6_ADDRSTRLEN];   
char inputFiles[10][SBUFSIZE];        
char* outputFile; 
char *argt[30];                  	
int filesComplete = 0;	             					
int reqthreads, resthreads;          
int reqstart, resstart = 0;
int *numInput;
int counter = 0;
int resthreadcounter, reqthreadcounter;
bool requestorsReturn = false;
struct timeval begin, end;          	                	
FILE* inputfp = NULL;               
FILE* results = NULL;
FILE* serviced = NULL;
FILE* performance=NULL;
                                        

int main(int argc, char* argv[]){ 
	int i;
	int new;
	int f = argc -5;
	numInput = &f;
	bool requestorsReturn = false;
	gettimeofday(&begin, NULL);


	pthread_mutex_init(&arrayLock, NULL);            
	pthread_mutex_init(&requestorLock,NULL);            
	pthread_mutex_init(&resolverLock,NULL);
	
        pthread_t reqthread[reqthreads];
	pthread_t resthread[resthreads];

	
	reqthreads = strtol(argv[1],argt ,10);
	resthreads = strtol(argv[2],argt, 10);

	
	if (argc < MINARGS){                                                         
		fprintf(stderr, "Not enough arguments: %d", (argc-1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return(EXIT_FAILURE);
	}
	
	for (i = 0; i < (argc-5); i++){                       
		strncpy(inputFiles[i], argv[i+5], sizeof(hostname[i]));
	}
	
	serviced = fopen("serviced.txt","w");         
		if(!serviced){
			fprintf(stderr, "Error: No serviced.txt found.\n");
			return 1;
		}

	if(reqthreads < 6){	
	for (i = 0; i < reqthreads; i++){                                                      
		int producer = pthread_create(&reqthread[i], NULL,(void*) readFiles, argv[i+5]); 
		counter++;    
		if (producer){
			fprintf(stderr, "ERROR: return code from pthread_create() is %d\n", producer);
			}
		}
	}
	else{
		new = reqthreads - 5;
		for (i = 0; i < 5; i++){                                                      
		int producer = pthread_create(&reqthread[i], NULL,(void*) readFiles, argv[i+5]); 
		counter++;      
		if (producer){
			fprintf(stderr, "ERROR: return code from pthread_create() is %d\n", producer);
			}
		}
		for (i = 0; i < new; i++){                                                      
		int producer = pthread_create(&reqthread[i], NULL,(void*) readFiles, argv[i+5]);  
		counter++;     
		if (producer){
			fprintf(stderr, "ERROR: return code from pthread_create() is %d\n", producer);
			}
		}
	}
	
	
	


	for (i = 0; i < resthreads; i++){                                                    
		int consumer = pthread_create(&resthread[i], NULL,(void*) dns, argv[4]) ;         
		if(consumer){                                                                
			fprintf(stderr, "ERROR: return code from pthread_create() is %d\n", consumer);  
		}
	}
	
	/*******************************************Don't go past this until everything in dns and readFiles works and returns properly everything here is just*******************/
	/*****************************************//*waiting for all the threads to finish, killing the threads, and all kinds of clean up work after all the files are read *****/


	for (i = 0; i < reqthreads; i++){              
		pthread_join(reqthread[i], NULL); 
	}
	
	printf("Joined all requestor threads\n");
	requestorsReturn = true;
	printf("%d\n",requestorsReturn);
	for (i = 0; i < resthreads; i++){           
		pthread_join(resthread[i], NULL);
	}
	printf("Joined all resolver threads\n");

	
	
	for(i = 0;i < 5; i++){
		hostname[i][0] = '\0';
	}
	gettimeofday(&end, NULL);
	double t = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
	
	printf("%d Files complete with total run time of %f seconds.\n",argc-5,t);
	
	
	
	fclose(serviced);
	fclose(results);      

	pthread_mutex_destroy(&arrayLock);             
	pthread_mutex_destroy(&requestorLock);
	pthread_mutex_destroy(&resolverLock);
	return EXIT_SUCCESS;

}			

void* readFiles(char* filename){                 	
	int threadComplete = 0;
	int i;
	int hostnamecount = 0; 
	bool finished = false;
	char filePath[SBUFSIZE];
	reqthreadcounter++;
	struct timeval beginthread, endthread;
	printf("beginning of readFiles\n");
	gettimeofday(&beginthread, NULL);

	while(1){ 
		start:
		strncpy(filePath, "input/", sizeof("input/"));
		strcat(filePath, filename);
		finished =false;


		inputfp = fopen(filePath, "r");               
		if(!inputfp){
			fprintf(stderr, "Error: Bogus input file path.\n");
			return NULL;
		}

		while(!finished){
			usleep(rand()%100);
			pthread_mutex_lock(&arrayLock);
			for(i = 0; i < 20; i++){
				if(!feof(inputfp)){
				/*pthread_mutex_lock(&arrayLock);*/
					if(hostname[i][0] == '\0'){
						fscanf(inputfp,"%s",hostname[i]); 
						printf("REQ added %s\n",hostname[i]);
						hostnamecount++;
						pthread_mutex_unlock(&arrayLock);		
					}
				}

				if(feof(inputfp)){
					pthread_mutex_lock(&requestorLock);
					printf("Thread %ld hit the end of file %s\n",syscall( __NR_gettid ),filename); 
					finished = true;
					filesComplete++;
					threadComplete++;
					fclose(inputfp);
					fprintf(serviced, "LOG :Thread %ld serviced %d files.\n\n",syscall( __NR_gettid ),threadComplete);
					for(i = 0; i < *numInput;i++){
						if (strcmp(filename, inputFiles[i]) == 0){   
							inputFiles[i][0] = '\0';            
							printf("%s\n",inputFiles[i]);
							break;
						}
					}

					if (filesComplete < *numInput){
						printf("Files complete < Input\n");
						for(i = 0; i < *numInput;i++){                    
							if (inputFiles[i][0] != '\0'){
								printf("Thread %ld Looking for new file : %s\n",syscall( __NR_gettid ),inputFiles[i]);
								strncpy(filename, inputFiles[i], sizeof(inputFiles[i]));
								pthread_mutex_unlock(&requestorLock);
								goto start;
							}
						}
					}
					else{
						pthread_mutex_unlock(&requestorLock);
						goto label;
					}
				}
				pthread_mutex_unlock(&arrayLock);
			}
			/*pthread_mutex_unlock(&arrayLock);*/
		}
	}			
	label:
	pthread_mutex_lock(&requestorLock);		
	printf("Right before returning from readFiles\n");
	gettimeofday(&endthread, NULL);
	double s = (endthread.tv_sec - beginthread.tv_sec) + ((endthread.tv_usec - beginthread.tv_usec)/1000000.0);
	performance = fopen("performance.txt", "a+");
	fprintf(performance,"\n\nWith number of requestor threads = %d and Number of resolver thread = %d...\nID of requestor thread = %ld\nHostnames processed = %d\nTotal runtime = %f seconds",reqthreadcounter,resthreadcounter,syscall( __NR_gettid ), hostnamecount,s);
	fclose(performance);
	counter--;
	pthread_mutex_unlock(&requestorLock);
	return NULL;
}

void* dns(char* filename) {      
	int i;
	int hostnamecount = 0; 
	struct timeval resbegin, resend;
	gettimeofday(&resbegin, NULL);
	resthreadcounter++;                         
	
	results = fopen(filename, "w");
	if(!results){
		fprintf(stderr, "Error: Bogus output file path.\n");   
		return (NULL);
	}

	/*while (counter != 0){*/
	while(1){
		pthread_mutex_lock(&arrayLock);
		if(hostname[0][0] != '\0'){
			if (dnslookup(hostname[0], firstipstr, sizeof(firstipstr))==UTIL_FAILURE){            
				fprintf(stderr, "Error: DNS lookup failure on hostname: %s\n", hostname[0]);   
				strncpy(firstipstr, "", sizeof(firstipstr));                                   
				fprintf(results, "%s,%s\n", hostname[0],firstipstr);
				                           
			}	
			else{	
				fprintf(results, "%s,%s,%d\n", hostname[0],firstipstr,hostnamecount);
				printf("RESOLVER : %s, %s\n",hostname[0],firstipstr);
				
			}	
			
			for(i = 0; i < 19 ; i++){
				printf("%s\n\n",hostname[i]);
				strncpy(hostname[i], hostname[i+1], sizeof(hostname[i]));
			}
			hostname[19][0] = '\0';
			pthread_mutex_lock(&resolverLock);
			hostnamecount++;
			pthread_mutex_unlock(&resolverLock);
			printf("counter is %d\n",counter);
			pthread_mutex_unlock(&arrayLock);
		}
		if((counter == 0)&&(hostname[0][0] == '\0')){
			pthread_mutex_unlock(&arrayLock);
			break;
		}
		pthread_mutex_unlock(&arrayLock);
		continue;
			
	}
	pthread_mutex_lock(&resolverLock);
	printf("You made it past the looping in DNS\n");
        gettimeofday(&resend, NULL);                          
	double s = (resend.tv_sec - resbegin.tv_sec) + ((resend.tv_usec - resbegin.tv_usec)/1000000.0);	                   
	performance = fopen("performance.txt", "a+");
	fprintf(performance,"\n\nWith number of requestor threads = %d and Number of resolver thread = %d...\nID of resolver thread = %ld\nHostnames processed = %d\nTotal runtime = %f seconds",reqthreadcounter,resthreadcounter,syscall( __NR_gettid ), hostnamecount,s);
	fclose(performance);
	printf("Right before DNS return\n");
	pthread_mutex_unlock(&resolverLock);
	return NULL;
}

