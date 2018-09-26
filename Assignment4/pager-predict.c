/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "simulator.h"


void Pattern1(int proc, int page);
void Pattern2(int proc);
void Pattern3(int proc, int page);
void Pattern4(int proc, int page);
void clearPages(int proc, int page);

void Pattern1(int proc,int page){
	if(page == 9 || page == 10 || page == 11){
        pageout(proc, 0);
        pageout(proc, page-1);
        pagein(proc, page);
        pagein(proc, page+1);
        pagein(proc, page+2);
    }
    
    else if(page == 12){
        pageout(proc, page-1);
        pagein(proc, page);
        pagein(proc, 13);
        pagein(proc, 9);
        pagein(proc, 0);
    }else if(page == 13){
        pageout(proc, page-1);
        pagein(proc, page);
        pagein(proc, 9);
        pagein(proc, 10);
    }
    
    else if(page == 0){
        
        for(int i=4; i<20; i++){
            pageout(proc, i);
        }
        
        pagein(proc, page);
        pagein(proc, page+1);
        pagein(proc, page+2);
        
        
    }
    
    else if(page>0 && page<9){
        pageout(proc, page-1);
        pagein(proc, page);
        pagein(proc, page+1);
        pagein(proc, page+2);
    }
}

void Pattern2(int proc){
    for(int i=0; i<4; i++){
        pagein(proc, i);
    }
    for(int i=4; i<20; i++){
        pageout(proc, i);
    }
}

void Pattern3(int proc,int page){
    
    if(page < 7){
        pageout(proc, page-1);
        pagein(proc, page);
        pagein(proc, page+1);
    }
    
    else if(page == 7){
        pageout(proc, 6);
        pagein(proc, 7);
        pagein(proc, 8);
        
    }
    
    else if(page == 8){
        pageout(proc, 7);
        pagein(proc, 0);
    }
    
    else if(page>8){
        pageout(proc, page-1);
        pagein(proc, page);
        pagein(proc, page+1);
        pagein(proc, 0);
    }
}

void Pattern4(int proc, int page){
    
    if(page == 0){
        
        for(int i=4; i<15; i++){
            pageout(proc, i);
        }
        
        for(int i=0; i<4; i++){
            pagein(proc, i);
        }
    }
	
	else if(page == 1){
        pageout(proc, 0);
        pagein(proc, 1);
        pagein(proc, 2);
        pagein(proc, 3);
    }
	
	else if(page == 2){
        pageout(proc, 1);
        pagein(proc, 2);
        pagein(proc, 3);
        pagein(proc, 10);
    }

	else if(page == 3){
        pageout(proc, 2);
        pagein(proc, 3);
        pagein(proc, 4);
        pagein(proc, 10);
        pagein(proc, 11);
    }
    
    else if(page == 4){
        pageout(proc, 3);
        pageout(proc, 10);
        pageout(proc, 11);
        pagein(proc, 4);
        pagein(proc, 5);
        pagein(proc, 6);
        pagein(proc, 7);
    }
    
    else if(page == 5){
        pageout(proc, 4);
        pagein(proc, 5);
        pagein(proc, 6);
        pagein(proc, 7);
        pagein(proc, 8);
    }
    
    else if(page == 6){
        pageout(proc, 5);
        pagein(proc, 9);
        pagein(proc, 6);
        pagein(proc, 7);
        pagein(proc, 8);
    }
    
    else if(page == 7){
        pageout(proc, 6);
        pagein(proc, 9);
        pagein(proc, 10);
        pagein(proc, 7);
        pagein(proc, 8);
    }
    
    else if(page == 8){
        pageout(proc, 7);
        pagein(proc, 9);
        pagein(proc, 10);
        pagein(proc, 11);
        pagein(proc, 8);
        pagein(proc, 0);
    }
    
    else if(page == 9){
        pageout(proc, 8);
        pagein(proc, 10);
        pagein(proc, 11);
        pagein(proc, 0);
        pagein(proc, 9);
    }
    
    else if(page == 10){
        
        for(int i=2; i<10; i++){
            pageout(proc, i);
        }
        pagein(proc, 10);
        pagein(proc, 11);
        pagein(proc, 0);
        pagein(proc, 1);
    }
    
    else if(page == 11){
        pageout(proc, 10);
        pagein(proc, 11);
        pagein(proc, 0);
        pagein(proc, 1);
        pagein(proc, 12);
    }
    
    else if(page == 12){
        pagein(proc, 13);
        pagein(proc, 9);
    }
    
    else if(page == 13){
        pagein(proc, 9);
    }
}

void pageit(Pentry q[MAXPROCESSES]) {
    
    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];
    static int pageCount[MAXPROCESSES]; //Gets incremented every time a process moves to a new page
    static int pageMissCount[MAXPROCESSES]; //for keeping track of when page needed is not in memory
    static int pageFaultCount[MAXPROCESSES+1]; //for keeping track when page must be evicted
    static int oldpages[20][41];//for printing pages to screen
    static int last[40];//also just for printing the pages
    static int procPattern[MAXPROCESSES]; //keeps track of which the process type
    static bool procAssigned[MAXPROCESSES]; //for keeping track of when a process gets assigned to a type
    
    /* initialize static vars on first run */
    if(!initialized){
         for(int proctmp=0; proctmp < MAXPROCESSES; proctmp++){
            for(int pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
                timestamps[proctmp][pagetmp] = 0;
            }
            last[proctmp]=21;
        }
        initialized = 1;
    }
    
    /* Local vars*/
    int pc;
    int page;
    int previous;
    int oldpage;
    int oldest[3];
    bool found;
    static int proc;
    //static int lastPage;
    pc = q[proc].pc;                 // program counter for process
    page = pc/PAGESIZE;         // page the program counter needs
    
    if(last[proc] != page)
        previous = last[proc];  //this is for determining the type
    
 
    if(last[proc] != page && oldpages[proc][40] < 40){
        last[proc] = page;
        oldpages[proc][oldpages[proc][40]] = page;
        oldpages[proc][40]++;
        pageCount[proc]++;
    } 
    else if(oldpages[proc][40] == 40){
        last[proc] = page;
        pageCount[proc]++;
  
        
        oldpages[proc][40] = 41;
    }
    else if(last[proc] != page){
        last[proc] = page;
        pageFaultCount[20]++;
        pageCount[proc]++; 
    }
    
    
    //Assign type to processes
    if(pageCount[proc] == 5 && !procAssigned[proc]){
       
        if(page == 0){
            procPattern[proc] = 2;
            Pattern2(proc);
            procAssigned[proc] = true;
        }
        
        else if(page == 10){
            procPattern[proc] = 5;
            procAssigned[proc] = true;
        }
    }
    
    else if(pageCount[proc] == 10 && !procAssigned[proc]){
        
        if(page == 0 && previous == 8){
            procPattern[proc] = 4;
            procAssigned[proc] = true;
        }
    }
    
    else if(pageCount[proc] == 12 && !procAssigned[proc] && page == 0 && previous == 10){
        procPattern[proc] = 5;
        procAssigned[proc] = true;
        
    }
    
    else if(pageCount[proc] == 13 && !procAssigned[proc] && page == 0 && previous == 11){
        procPattern[proc] = 5;
        procAssigned[proc] = true;
    }
    
    else if(pageCount[proc] == 15 && !procAssigned[proc] && page == 9 && previous == 13){
        procPattern[proc] = 1;
        Pattern1(proc, page);
        procAssigned[proc] = true;
    }
    
    else if(pageCount[proc] == 15 && !procAssigned[proc] && page == 14)
        pageCount[proc] = 0;
    
    
    int pagesIn=0;
    for(int i=0; i<MAXPROCESSES; i++){
        for(int j=0; j<MAXPROCPAGES; j++){
            if(q[i].pages[j])
                pagesIn++;
        }
    }
    
    if( pagesIn > 99){
        //find the page with the oldest time stamp
        found = false;
        for(int proc_=0; proc_<MAXPROCESSES; proc_++) {
            
            for(oldpage=0; oldpage < q[proc_].npages; oldpage++) {
      
                if(!found && q[proc_].pages[oldpage]){
                    oldest[0] = proc_;
                    oldest[1] = oldpage;
                    oldest[2] = timestamps[proc_][oldpage];
                    found = true;
                    proc_ = 0;
                    oldpage = 0;
                    //loop through all pages in memory and find the oldest.
                } 
                
                else if(q[proc_].pages[oldpage] && timestamps[proc_][oldpage] < oldest[2]){
                    
                    oldest[0] = proc_;
                    oldest[1] = oldpage;
                    oldest[2] = timestamps[proc_][oldpage];
                }
            }
        }
        pageout(oldest[0],oldest[1]);
        
    }
    
    
    /* Is process active? */
    if(q[proc].active) {
        
        //run function for the process type
        if(procPattern[proc] == 1){
             Pattern1(proc, page);
        }
        else if(procPattern[proc] == 4){
            Pattern3(proc, page);
        }
        else if(procPattern[proc] == 5){
            Pattern4(proc, page);
        }
        else if(!procPattern[proc]){
            
            if(page<9){
                
                for(int i=0; i<page; i++){
                   pageout(proc, i);
                }
                pagein(proc, page);
                pagein(proc, page+1);
            }
            else if(page>8){
                for(int i=1; i<page; i++){
                    if(i!=9)
                        pageout(proc, i);
                }
                pagein(proc, page);
                pagein(proc, page+1);
                pagein(proc, 0);
            }
            else if(page == 14){
                pageout(proc, page-1);
                pagein(proc, 0);
            }
            
        }
        
        /* Is page swaped-out? */
        if(!q[proc].pages[page]) {
            pageMissCount[proc]++;//keeps track of when page is not loaded in memory
            
       
            
            if ((q[proc].pc % PAGESIZE) == 0){
                pagein(proc, page + 1);
            }
            
            /* Try to swap in */
            if(!pagein(proc,page)) {
                /* If swapping fails, swap out another page */
                
                pageFaultCount[proc]++; //keeps track of when page can't be loaded in memory because memory is full
                
                for(int i=0; i<20; i++){
                    for(int j=15; j<20; j++){
                        if(q[i].pages[j]){
                            pageout(i, j);
                        }
                    }
                }
                
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
                            proc_ = 0;
                            oldpage = 0;
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
                }
                
            } 
            
            else {
                timestamps[proc][page]=tick;
            }

        }
    }
    else{

        for(int i=0; i<20; i++){
            pageout(proc, i);
        }
    }
    timestamps[proc][page]=tick;
    /* advance time for next pageit iteration */
    tick++;
    //increment proc so the next time pageit() is called, it operates on the next process
    proc++;
    if (proc == 20)
        proc = 0;
    
}








