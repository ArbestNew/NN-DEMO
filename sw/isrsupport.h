/* isrsupport.c */
unsigned disableIRQ(void);
unsigned restoreIRQ(unsigned oldCPSR);
unsigned enableIRQ(void);
unsigned disableFIQ(void);
unsigned restoreFIQ(unsigned oldCPSR);
unsigned enableFIQ(void);
