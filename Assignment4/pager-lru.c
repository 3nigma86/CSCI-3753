/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) { 
    
    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */
    int proctmp;
    int pagetmp;

    /* initialize static vars on first run */
    if(!initialized){
	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
		timestamps[proctmp][pagetmp] = 0; 
	    }
	}
	initialized = 1;
    }
    
    /* Local vars */
    int pc;
    int page;
    int oldpage;
    int oldest[3];
    bool found;
    static int proc;
    /* Is process active? */
    if(q[proc].active) {
        /* Dedicate all work to first active process*/
        pc = q[proc].pc;                 // program counter for process
        page = pc/PAGESIZE;         // page the program counter needs
        /* Is page swaped-out? */
        //printf("\nProcess %d is at %d which is page %d", proc, pc, page);
        if(!q[proc].pages[page]) {
            /* Try to swap in */
            if(!pagein(proc,page)) {
                /* If swapping fails, swap out another page */
                
                //find the page with the oldest time stamp
                found = false;
                for(int proc_=0; proc_<MAXPROCESSES; proc_++) {
                    for(oldpage=0; oldpage < q[proc_].npages; oldpage++) {
                        //find the first active page and set that as the current oldest.
                        if(!found && q[proc_].pages[oldpage]){
                            oldest[0] = proc_;
                            oldest[1] = oldpage;
                            oldest[2] = timestamps[proc_][oldpage];
                            found = true;
                            //loop through all pages in memory and find the oldest.
                        } else if(q[proc_].pages[oldpage] && timestamps[proc_][oldpage] < oldest[2]){
                            oldest[0] = proc_;
                            oldest[1] = oldpage;
                            oldest[2] = timestamps[proc_][oldpage];
                        }
                    }
                }
                /* Make sure page isn't one I want */
                if(oldest[0] != proc && oldest[1] != page) {
                    /* Try to swap-out */
                    pageout(oldest[0],oldest[1]);
                    pagein(proc,page);
                    timestamps[proc][page]=tick;
                    /* Break loop once swap-out starts*/
                }
            } else {
                timestamps[proc][page]=tick;
            }
        }
        //break;
    }
    /* advance time for next pageit iteration */
    tick++;
    //increment proc so the next time pageit() is called, it operates on the next process
    proc++;
    if (proc == 20)
        proc = 0;
    
}
