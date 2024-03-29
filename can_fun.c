/*CAN Function program*/
unsigned int MSG[10];

void can_config(void)
{
  VPBDIV=2;                   //PClk @30Mhz
  /*Configure P0.23---TXD2, P0.24--RXD2, P0.25--RXD1 and PIN10*/
  PINSEL1|=(1<<14)|(1<<16)|(1<<18);
  PINSEL1&=~((1<<15)|(1<<17)|(1<<19));

  /*CAN Clock @10Mhz, BRP[9:0]=2+1, TSEG1[19:16]=15+1, TSEG2[22:20]=3+1*/
  C2MOD=(1<<0);									 //disable CAN2
  C2BTR=(2<<0)|(15<<16)|(3<<20);                 //BRP=2, TSEG1=15,TSEG2=3
  C2MOD=0;                                       //enable CAN2

  C1MOD=(1<<0);                                 //disable CAN1
  C1BTR=(2<<0)|(15<<16)|(3<<20);                //BRP=2, TSEG1=15, TESG2=3
  C1MOD=0;                                      //enable CAN1      

}

void can_tx(unsigned int d1)
{
  /*CAN2 as TX*/
  while(!(C2GSR&(1<<2)));                  //monitor TX buffer is empty or not
  C2TID1=123;                              //load msg id to the reg
  C2TFI1=(1<<16);                           //load frame info DLC[19:16]=0001 to transmit 1byte of data,RTR=0,IDE=0
  C2TDA1=d1;                               //load  the argument or data to DATA reg A
  C2CMR=(1<<0)|(1<<5);                     //bit0-->enabled queued transmission and bit5-->select transmitter buffer 1  
  while(!(C2GSR&(1<<3)));                  //monitor transmission is completed or not
}

unsigned int can_rx(void)
{
  /*CAN1 as RX*/
  while(C1GSR&(1<<0));                    //monitor receiver buffer is empty or not 
  MSG[0]=C1RID;                           //load the msg ID to MSG[0]
  MSG[1]=C1RDA;                           //load the received data to MSG[1]
  C1CMR=(1<<2);                           //release receiver buffer

  return MSG[1];
}