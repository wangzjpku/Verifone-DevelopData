
#ifndef    G_C
#define    G_C        1
#define    G_S        2
#define    G_I        3
#define    G_L        4
typedef    struct
{
	void    *addr;
	int     type;
}	g_data;
#endif

g_data    gvardata[] = {
/*   0    */    { (void *)r_acct_num, G_S },
/*   1    */    { (void *)r_exp_date, G_S },
/*   2    */    { (void *)r_amount, G_S },
/*   3    */    { (void *)r_merchant, G_S },
/*   4    */    { (void *)r_date, G_S },
/*   5    */    { (void *)r_response, G_S },
/*   6    */    { (void *)r_cardholder, G_S },
/*   7    */    { (void *)r_trans_num, G_S }
        };
