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
#include <acldev.h>
#include <printer.h>
#include <formater.h>
#include "..\include\gvars.h"
#include "..\include\applic.h"
#include <xmodem.h>




//Globals defined

int ghConsole = 0, ghCard=0;
char gCardSwiped='N';

unsigned long idlecount=0;
unsigned long slowpollcount=0;
unsigned long fastpollcount=0;

char szKeyMap[MAX_ALPNUM_KEYS][CHAR_PER_KEY]={
           "0- +Q",   "1Z.AB",   "2CDEF",  "3GHIJ",  "4KLMN",  "5OPRS",
           "6TUVW",   "7XY*,",   "8'\"#=",  "9:$~^",  "{}[]\\","&%()!"};

char card_buff[CARD_SIZE];


FORMATER     p3700Lab;

unsigned char TxBuffPkt[100];
unsigned char RxBuffPkt[100];

// Function Prototypes

void doCountDown(void);
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
short    inAmountEntry(void);
short    inCardResponder(short);
short    inExpdateEntry(void);
short    inCardEntry(void);
void     Print(void);
short    inGetHostResponse(void);
short    inReadTime(void);
short    inGetMID(char *data_buf);
short    inReadMerchant(void);
int      p3700_direct (FORMATER* , int);
void     AsmPkt(void);
void     DsmPkt(void);
int      get_zp_phonenum(char *);





int p3700_direct (FORMATER* formater, int init_time_out)
{
   formater->output_close = p3700_close;
   formater->output_print = p3700_print;
   formater->output_mode =  p3700_mode;
   formater->direct = 1;
   formater->max_buffers = 2;
   return p3700_init(formater->h_comm_port, (short)init_time_out);
}


void idle_menu_prompt()
{
    memset(r_acct_num,0x0,sizeof(r_acct_num));
    memset(r_exp_date,0x0,sizeof(r_exp_date));
    memset(r_amount,0x0,sizeof(r_amount));
    memset(r_merchant,0x0,sizeof(r_merchant));
    memset(r_date,0x0,sizeof(r_date));
    memset(r_response,0x0,sizeof(r_response));
    memset(r_cardholder,0x0,sizeof(r_cardholder));

    gCardSwiped='N';
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

void AsmPkt(void)
{
    memset(TxBuffPkt,0x0,sizeof(TxBuffPkt));
    strcpy((char*)TxBuffPkt,r_acct_num);
    strcat((char*)TxBuffPkt,r_exp_date);
}

void DsmPkt(void)
{
    strcpy(r_response,(char*)RxBuffPkt);
}

int get_zp_phonenum( char *sz_phonebuff )
{
    char szptr[50];
	int retVal;

    retVal = (short)get_env("*ZP", (char *)szptr, 5);
	
    if(szptr != NULL) {
        strncpy( sz_phonebuff, szptr, 5 );
    }
    else {
        window(1,8,21,8);
        clrscr();
        write_at("*ZP IS BAD. Fix It", 18, 1, 8);
        strcpy(sz_phonebuff,"\0");
    }
	szptr[4] ='\0';
	sz_phonebuff[4] = '\0';
    if(szptr != NULL)
        return (0);
    return (-1);
}

short inGetHostResponse()
{
	int retVal;
	int handle = -1;
	char dlStr[20]="101";
	int iwrite;

    memset(RxBuffPkt,0x0,sizeof(RxBuffPkt));
    memset(TxBuffPkt,0x0,sizeof(TxBuffPkt));

    AsmPkt();

    retVal =  xmdm_open( &handle, NULL, -1,50,Rt_2400,Fmt_A7E1+Fmt_DTR);
    xmdm_flush(handle);

  	if ((retVal = get_zp_phonenum(dlStr)) != 0)
       return 0;


    retVal = xmdm_get_line_dial( handle, dlStr, &iwrite, -1, 100);

    SVC_WAIT(3000);

    if (retVal == CONNECT_2400)
	{
        retVal = xmdm_send_data( handle, (char*)TxBuffPkt, strlen((char*)TxBuffPkt),300);
        SVC_WAIT(400);
        retVal = xmdm_receive_data(handle, (char*)RxBuffPkt, 0, 10,300);
       	retVal = xmdm_hangup( handle, -1, 0);
        SVC_WAIT(300);
	    retVal = xmdm_close(handle,0,0);

    }    

    DsmPkt();   


    strcpy(r_trans_num,"1234");
    return 0;
}

short    inAmountEntry(void)
{
	int ret_val;
    char data_buf[20];

    window(1,1,21,8);
    clrscr();

    write_at("Enter Amount",12,1,1);
    window(1,2,21,8);

    gotoxy(1,1);

    ret_val = getkbd_entry (0, "", data_buf, (unsigned int) 100000,
					(unsigned int) AMOUNT , szKeyMap,sizeof(szKeyMap),2,10000L,100L);

    strcpy(r_amount,data_buf);

    return 0;
}

short    inCardResponder(short state)
{
    short shRet=0;
    struct TRACK parsed;

    shRet=read(ghCard, card_buff, sizeof(card_buff));
    if(shRet>=0)
    {
        shRet=card_parse(card_buff, &parsed, "12");
        gCardSwiped='Y';
    }
    if ((shRet == 1) || (shRet == 2))
    {
        strcpy(r_acct_num, parsed.acct);
        strcpy(r_exp_date, parsed.exp);
        strcpy(r_cardholder, parsed.name);
    }
    clrscr();
    write_at(r_acct_num,strlen(r_acct_num),1,1);
    SVC_WAIT(3000);
    inAmountEntry();
    inReadTime();
    inReadMerchant();
    inGetHostResponse();
    Print();
    idle_menu_prompt();
    return(BRANCH_EXIT);
}

short    inReadTime(void)
{
	int h_clock=0, yr, mth, day, hr, min, sec, wday;
    char data_buf[10];
    clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
    sprintf(data_buf,"%02d %02d %02d", mth, day, (yr%100));
    data_buf[2]='/'; 
    data_buf[5]='/';
    strcpy(r_date,data_buf);
    return 0;
}

short inGetMID(char *data_buf)
{
   	int ret_val;

    window(1,1,21,2);
    clrscr();

    write_at("Enter Merchant ID",17,1,1);

    window(1,2,21,8);

    gotoxy(1,1);

    ret_val=SVC_KEY_TXT(data_buf, NUMERIC,20, 4,(char*)szKeyMap,sizeof(szKeyMap));
    
    return 0;

}

short    inReadMerchant(void)
{
    char data_buf[22];

    memset(data_buf,0x0,sizeof(data_buf));
    get_env("#MID",data_buf,21);

    if (0==strlen(data_buf))
    {
        inGetMID(data_buf);
        put_env("#MID",data_buf,(size_t)strlen(data_buf));
    }
    strcpy(r_merchant,data_buf);
    return 0;
}

short    inExpdateEntry(void)
{
	int ret_val;
    char data_buf[5];

    window(1,1,21,8);
    clrscr();

    write_at("Enter Expiry date",17,1,1);
    gotoxy(1,2);

    ret_val=SVC_KEY_TXT(data_buf, NUMERIC,4, 4,(char*)szKeyMap,sizeof(szKeyMap));
    strcpy(r_exp_date,ctons1((unsigned char*) data_buf));
    return 0;
}

short    inCardEntry(void)
{
	int ret_val;
    struct TRACK parsed;
    window(1,1,21,8);
    clrscr();

    write_at("Enter Card Number",17,1,1);
    window(1,2,21,8);
    gotoxy(1,1);

    ret_val = key_card_entry(0, ghCard, card_buff, 0,  (unsigned int)100000,  23,  15, "", 0,(char*)szKeyMap, sizeof(szKeyMap));

    if (0x32==ret_val)
    {
        gCardSwiped='Y';
        ret_val=card_parse(card_buff, &parsed, "12");
        if ((ret_val == 1) || (ret_val == 2))
        {
            strcpy(r_acct_num, parsed.acct);
            strcpy(r_exp_date, parsed.exp);
            strcpy(r_cardholder, parsed.name);
        }
    }
    else
    {
        strcpy(r_acct_num,card_buff);
        inExpdateEntry();
    }

    clrscr();
    write_at(r_acct_num,strlen(r_acct_num),1,1);
    SVC_WAIT(3000);
    inAmountEntry();
    inReadTime();
    inReadMerchant();
    inGetHostResponse();
    Print();
    return 0;
}

short    in1KeyResponder(short state)
{
    window(1,1,21,8);
    clrscr();
    //write_at("In Sale", 7, 14, 1);
    //SVC_WAIT(3000);

    inCardEntry();
  
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
    #define    FT_EVENT_CARD                    7
    inCardResponder,
	#define    FT_ENDTBL                        8
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
    { APPL_CARD,    FT_EVENT_CARD,            idle_table    },
	{ SLOW_POLL_IN,	0,                        idle_table    },
	{ FAST_POLL_IN,	0,                        idle_table    },
    { END_TABLE,    FT_ENDTBL,                idle_table    }
};



int main (int argc, char *argv[])
{
    ghConsole = open(DEV_CONSOLE, 0);
    ghCard = open(DEV_CARD, 0);

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

	close(ghConsole);
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

void Print(void)
{
    short handle;

    int Val=0;
    int errline=0;
    int linestoprint=0;
  	unsigned long	condition=0;



    open_block_t parm;

    handle = open("/dev/com4", 0);
	memset(&parm,0,sizeof(parm));

	parm.rate      = Rt_19200; // 3300 ITP is always set to 19200 baud
	parm.format    = Fmt_A8N1 | Fmt_auto ; // 3300 ITP is always set at 8N1
	parm.protocol  = P_char_mode;
    parm.parameter = 0;

	set_opn_blk( handle, &parm);

	SVC_WAIT(200);
	p3700_init(handle, 6);

	SVC_WAIT(100);
	
	formater_main(gvardata) ;
    Val=formater_open(handle, &p3700Lab, "Receipt.frm", p3700_direct, 12);
    Val=0;

    if ('Y'==gCardSwiped)
        condition = 0x00000001;
    else
        condition = 0x00000002;
 
    linestoprint=1;

    while( formater_line(&p3700Lab, linestoprint, TO_END, 1, condition, &errline) < 0 )
    {
    	linestoprint = errline;
    }
    
    SVC_WAIT(1000);
    formater_close(&p3700Lab);
}
