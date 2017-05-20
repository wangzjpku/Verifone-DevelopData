#include <stdlib.h>
#include <string.h>
#include <svc.h>
#include <svctxo.h>
#include <stdio.h>
#include <aclconio.h>
#include <aclstr.h>
#include <message.h>
#include "count.txt"
#include <Applidl.h>



void doCountDown(void);

//Globals defined

unsigned long idlecount=0;
unsigned long slowpollcount=0;
unsigned long fastpollcount=0;

char szKeyMap[MAX_ALPNUM_KEYS][CHAR_PER_KEY]={
           "0- +Q",   "1Z.AB",   "2CDEF",  "3GHIJ",  "4KLMN",  "5OPRS",
           "6TUVW",   "7XY*,",   "8'\"#=",  "9:$~^",  "{}[]\\","&%()!"};


char r_acct_num[23];
char r_exp_date[5];
char r_amount[10];
char r_merchant[20];
char r_date[10];
char r_response[22];

void     idle_menu_prompt(void);
void     manager_menu_prompt(void);
short    idle_exit (short);
short    inFKeyResponder(short);
short    inGKeyResponder(short);
short    in1KeyResponder(short);
short    in2KeyResponder(short);
short    inCancelKeyResponder(short);
short    inEnterKeyResponder(short);
short    inEndTable(short);
short    slow_poll(void);
short    fast_poll(void);
short    appl_idle_loop(void);	
short    inActivateEvtResponder(void);
short    inDeActivateEvtResponder(void);

void idle_menu_prompt()
{
    memset(r_acct_num,0x0,sizeof(r_acct_num));
    memset(r_exp_date,0x0,sizeof(r_exp_date));
    memset(r_amount,0x0,sizeof(r_amount));
    memset(r_merchant,0x0,sizeof(r_merchant));
    memset(r_date,0x0,sizeof(r_date));
    memset(r_response,0x0,sizeof(r_response));

	window(1,1,21,8);
	clrscr();
    write_at("Press 1 for Sale", 16, 1, 1);
	write_at("Press 2 for Print", 17, 1, 3);

}

void manager_menu_prompt()
{

	window(1,1,21,8);
	clrscr();
    write_at("Setup", 5, 16, 3);
	write_at("Reports", 7, 14, 5);
	write_at("Press Cancel to exit", 20, 1, 8);
}

// event handlers
short idle_exit (short state)
{
	window(1,1,21,8);
    clrscr();
    return(BRANCH_EXIT);              
}


short    inFKeyResponder(short state)
{

	window(1,1,21,8);
    clrscr();
    write_at("In Set Up", 9, 12, 1);
	write_at("Press Cancel to exit", 20, 1, 8);
	return(BRANCH_EXIT); 
}

short    inGKeyResponder(short state)
{

	window(1,1,21,8);
    clrscr();
    write_at("In Reports", 10, 11, 1);
	write_at("Press Cancel to exit", 20, 1, 8);
	return(BRANCH_EXIT); 
}


short    in1KeyResponder(short state)
{
    window(1,1,21,8);
    clrscr();
    write_at("In Sale", 7, 14, 1);
    SVC_WAIT(3000);
    idle_menu_prompt();
    return(BRANCH_EXIT);              

}


short    in2KeyResponder(short state)
{
	window(1,1,21,8);
    clrscr();
    write_at("In Print", 8, 13, 1);
    SVC_WAIT(3000);
    idle_menu_prompt();
    return(BRANCH_EXIT);              

}

short    inCancelKeyResponder(short state)
{
   idle_menu_prompt();
   return(BRANCH_EXIT);              

}

short    inEnterKeyResponder(short state)
{
    
    manager_menu_prompt();
    return(BRANCH_EXIT);              

}

short    inEndTable(short state)
{
   return(BRANCH_EXIT);              

}

short slow_poll(void)
{
	++slowpollcount;
	return BRANCH_EXIT;
}

short fast_poll(void)
{
	++fastpollcount;
	return BRANCH_EXIT;
}

short appl_idle_loop (void)	// Application specific idle loop processing
{
// Application specific idle processing
   	char time[20];
   	int retVal;
    retVal=read_clock(time);

    return BRANCH_EXIT;
}

short inActivateEvtResponder(void)
{
	//Appln processing for Activate event
    return 1;
}

short inDeActivateEvtResponder(void)
{
	//Appln processing for Deactivate event
    return 1;
}


PF_TABLE appl_table[] =
{
    #define FT_EXIT                             0
    idle_exit,
    #define    FT_EVENT_FKEY                    1
    inFKeyResponder,
    #define    FT_EVENT_GKEY                    2
    inGKeyResponder,
    #define    FT_EVENT_1KEY                    3
    in1KeyResponder,
    #define    FT_EVENT_2KEY                    4
    in2KeyResponder,
    #define    FT_EVENT_CANCEL_KEY              5
    inCancelKeyResponder,
    #define    FT_EVENT_ENTER_KEY               6
    inEnterKeyResponder,
	#define    FT_ENDTBL                        7
    inEndTable,
    (PF_TABLE) END_TABLE
};

BRANCH_TBL_ENTRY  Manager_tbl1[] =
{
    { KEY_f,        FT_EVENT_FKEY,            Manager_tbl1    },
    { KEY_g,        FT_EVENT_GKEY,            Manager_tbl1    },
    { KEY_CANCEL,   FT_EVENT_CANCEL_KEY,      idle_table      },
	{ END_TABLE,    FT_ENDTBL,                Manager_tbl1    }
};


/**************************************************************************/
/*                    Main Idle State Branch Table                        */
/*            (this table is required for every application)              */
/**************************************************************************/
BRANCH_TBL_ENTRY idle_table[] =
{
    { KEY1,         FT_EVENT_1KEY,            idle_table    },
    { KEY2,         FT_EVENT_2KEY,            idle_table    },
    { KEY_CR,       FT_EVENT_ENTER_KEY,       Manager_tbl1  },
	{ SLOW_POLL_IN,	0,                        idle_table    },
	{ FAST_POLL_IN,	0,                        idle_table    },
    { END_TABLE,    FT_ENDTBL,                idle_table    }
};



int main (int argc, char *argv[])
{
	int hConsole = 0;
	hConsole = open("/dev/console",0);
	clrscr();
	set_cursor(1);

    msg_select_file("count.msg");

	write_at("HELLO!!",7,1,1);
    write_at("This is a sample",16,1,3);
    write_at("for Verix ",10,1,4);
    write_at("in TXO Mode",11,1,5);

    clrscr();
	appl_idle_set_idle_loop_time(50L);
    idle_menu_prompt();
   	aie_main(idle_table, appl_table, appl_idle_loop, fast_poll, slow_poll,inActivateEvtResponder,inDeActivateEvtResponder);

	close(hConsole);
	return 0;
}

void doCountDown()
{
    char szMsg[22],buff[21];

    int iStartCount=0; 
    memset(buff,0x0,sizeof(buff));
    set_cursor(0);

    memset(szMsg, 0, sizeof(szMsg));
    msg_get(STARTMSG, szMsg);
    write_at(szMsg,strlen(szMsg),1,1);

	window(1,2,21,2);
	gotoxy(1,1);

    getkbd_entry(0,"",buff,(unsigned int) 100000,
					(unsigned int) NUMERIC,szKeyMap,sizeof(szKeyMap), 2, 1);
    iStartCount = strn2int(buff,2);

    window(1,1,21,8);
    clrscr();

    while(1)
    {
        if (0==iStartCount)
        {
            clrscr();
            memset(szMsg, 0, sizeof(szMsg));
            msg_get(FINAL, szMsg);
            write_at(szMsg,strlen(szMsg),1,1);
            error_tone();
            SVC_WAIT(200);
            error_tone();
            SVC_WAIT(200);
            error_tone();
            break;
        }

        memset(szMsg, 0, sizeof(szMsg));
        sprintf(szMsg,"COUNTDOWN %02d",iStartCount);
        write_at(szMsg,strlen(szMsg),1,1);
        normal_tone();
        SVC_WAIT(1000);
        iStartCount --;
    }

}
