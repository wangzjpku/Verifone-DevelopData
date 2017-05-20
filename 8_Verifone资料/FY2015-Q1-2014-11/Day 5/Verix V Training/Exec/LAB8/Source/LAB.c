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




//Globals defined
#define LOGOFILE	"blogo.lgo"

int ghConsole = 0, ghCard=0, printer_h=0;

unsigned long idlecount=0;
unsigned long slowpollcount=0;
unsigned long fastpollcount=0;

char szKeyMap[MAX_ALPNUM_KEYS][CHAR_PER_KEY]={
           "0- +Q",   "1Z.AB",   "2CDEF",  "3GHIJ",  "4KLMN",  "5OPRS",
           "6TUVW",   "7XY*,",   "8'\"#=",  "9:$~^",  "{}[]\\","&%()!"};

char card_buff[CARD_SIZE];

char r_acct_num[23];
char r_exp_date[5];
char r_amount[10];
char r_merchant[20];
char r_date[10];
char r_response[22];

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
void     load_logo_to_printer(void);



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

short inGetHostResponse()
{
    strcpy(r_response,"Approved");
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

    return ret_val;
}

short    inCardResponder(short state)
{
    short shRet=0;
    struct TRACK parsed;

    shRet=read(ghCard, card_buff, sizeof(card_buff));
    if(shRet>=0)
    {
        shRet=card_parse(card_buff, &parsed, "12");
    }
    if ((shRet == 1) || (shRet == 2))
    {
        strcpy(r_acct_num, parsed.acct);
        strcpy(r_exp_date, parsed.exp);

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
    
    return ret_val;

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
    return ret_val;
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
        ret_val=card_parse(card_buff, &parsed, "12");
        if ((ret_val == 1) || (ret_val == 2))
        {
            strcpy(r_acct_num, parsed.acct);
            strcpy(r_exp_date, parsed.exp);
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
    return ret_val;
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

    read_clock(time);

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

	load_logo_to_printer();

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
void printer_init()
{
	open_block_t parm;

    printer_h = open("/dev/com4", 0);
	memset(&parm,0,sizeof(parm));

	parm.rate      = Rt_19200; // 3300 ITP is always set to 19200 baud
	parm.format    = Fmt_A8N1 | Fmt_auto |Fmt_RTS; // 3300 ITP is always set at 8N1
	parm.protocol  = P_char_mode;
    parm.parameter = 0;

	set_opn_blk( printer_h, &parm);

	SVC_WAIT(200);
	p3700_init(printer_h, 6);

	SVC_WAIT(100);
}
void printer_close()
{
	p3700_close(printer_h);
}
void load_logo_to_printer()
{
	int		fp;

	fp=open(LOGOFILE,O_RDONLY);
	if(fp==-1){//File Doesn't Exist
		clrscr();
		set_font("");
		display_at(1,2,"LOGO File Missing, Reload Please!",CLR_LINE);
		get_char();
		SVC_RESTART("");
		return ;
	}

	printer_init();

	write_at("Downloading logo...",19,1,2);
	fp = open(LOGOFILE, O_RDONLY);
	if (p3700_dnld_graphic_file(printer_h,fp)!=0)
		write_at("ER",2,20,2);
	else
		write_at("OK",2,20,2);

	close(fp);

	printer_close();
}
void Print()
{
    int retVal;
    unsigned char	printBuf[256 + 1];

	printer_init();

	p3700_select_font(printer_h,1,0);

	p3700_print_graphic(printer_h,0,90);
	
    memset(printBuf, 0, sizeof(printBuf));
	printBuf[0] = PRINT_NORM;

    sprintf((char *)&printBuf[1],"\n%s\n",r_merchant);
	retVal = p3700_print( printer_h, printBuf );
    
    sprintf((char *)&printBuf[1],"DATE : %s\n",r_date);
	retVal = p3700_print( printer_h, printBuf );

    sprintf((char *)&printBuf[1],"A/C num: %s\n",r_acct_num);
	retVal = p3700_print( printer_h, printBuf );

	printBuf[0] = DBL_HEIGHT;
    sprintf((char *)&printBuf[1],"Amount: %s\n",r_amount);
	retVal = p3700_print( printer_h, printBuf );

	printBuf[0] = PRINT_NORM;
    strcpy((char *)&printBuf[1],"CARD TYPE:  VISA         TRANS TYPE:  SALE\n");
 	retVal = p3700_print( printer_h, printBuf );

    sprintf((char *)&printBuf[1],"Approval Code: %s\n",r_response);
	retVal = p3700_print( printer_h, printBuf );
    
    SVC_WAIT(500);
	printer_close();
}

