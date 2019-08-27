/*
 * C.E.A. IRFU/SIS/LDISC
 *
 * $Id$
 *
 * who       when      what
 * --------  --------  ----------------------------------------------
 * agaget  15/04/16  created
 */

program LaunchSeq

#include <string.h>

%%#include <string.h>
%%#include <time.h> 
%%#include <stdlib.h>
%%#include <stdio.h>



/* start/stop/pause button */
int startButton;
assign startButton to "{section}-{subsection}:StartSequence";
monitor startButton;
/* start/stop/pause button */
int startSeq;
assign startSeq to "{section}-{subsection}:Conditionning-01:Rdy-Seq";
monitor startSeq;

char pathPV[256];
assign pathPV to "{section}-{subsection}:DirectorySave";
monitor pathPV;

char pathHome[256];
assign pathHome to "{section}-{subsection}:DirectorySaveHome";
monitor pathHome;


/* Variable cycle */
int nbPalier=0;
int finCycle=0;

foreign ssId;
%{
	int EndsWith(const char *str, const char *suffix)
	{
	    if (!str || !suffix)
		return 0;
	    size_t lenstr = strlen(str);
	    size_t lensuffix = strlen(suffix);
	    if (lensuffix >  lenstr)
		return 0;
	    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
	}
}%
				
ss ss1
{
    state idle
    {
	when (startButton == 1)
	{      
		startSeq=1;
		pvPut(startSeq);
		
	} state DebutCycle
    }
    
    state DebutCycle
    {
	when (startButton == 0)
	{ 
		
		startSeq=0;
		pvPut(startSeq);
	}state idle


    }


   

}

