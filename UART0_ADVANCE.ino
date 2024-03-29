// used for refrence of pointers http://www.cs.fsu.edu/~myers/cgs4406/notes/pointers.html

// In this library you can print "Single characters" , "Strings" and "Numbers" using the function   => usart_print(data)
// If you want to write data to some sensor you can use the function                                => usart_write(data)
// If you want to Receive some data you can use the function                                        => usart_receive()
// If you want to wait for the Rx buffer to get filled you can use function                         => usart_available()
// To initialize the usart you can use the function                                                 => usart_init(baud rate)

#include<avr/io.h>
#include<util/delay.h>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
function : usart_init(uint16_t)
Input    : The Required Baud rate is passed as an argument in the function
Output   : Nothing

The Function is used to initialize the "Baud Rate", "Select Number OF Data Bits (UCSZn1 & UCSZn0)", "Usart Mode (Synchronous or asynchronous) ",
"Stop bits". Also the Receiver and Transmitter are also enabled in this function.

 */
void usart_init(uint16_t baud)
{
uint16_t BAUD = ((16000000/(baud))/16)-1;   // using baud formulae =>[ Desired Baud rate = (Fosc/(x+1))  ], where x = BAUD used in the equation
 
 UBRR0L = BAUD;                             // SETTING BAUD RATE loading data in low byte Baud Rate Register
 UBRR0H = BAUD>>8;                          // shifting the data 8 bit right so as to Store MSB in High byte Baud rate Register
 
 UCSR0C |= (_BV(UCSZ01)|_BV(UCSZ00));      //SELECT NUMBER OF DATA BITS (UCSZn1 & UCSZn0),SELECTING USART MODE,PARITY MODE & NUMBER OF STOP BITS 
                                           // AS  DEFAULTS i.e. BY LEAVING THE BITS AS 0 ONLY.
  
// UCSR0B |= (_BV(RXCIE0)|_BV(TXCIE0)); //ENABLING INTERRUPTS

 UCSR0B |= (_BV(RXEN0)|_BV(TXEN0));       //ENABLE TX & RX (RXENn & TXENn)
 
  
}

/*
 * Function : usart_transmit(3 arguments)  // 3 argument means => 1 argument in each usart_transmit() function as we have 3 usart_transmit function
 *            So for this function, concept of Function overloading is used i.e. 'having function with same name but different arguments.' 
 *            
 *            
 *            
 * => The purpose of Function overloading here is that usart_transmit() should be able to transmit "character" , "String" and "numbers"  all 3.
 * 
 * INPUTs : 1. Character, this will take character as input and will tranmit that character
 *          2. Character array (or we can say String), this will take the address of first element of the String as an argument
 *             Note: We can also use pointer instead of array (i.e. usart_transmit(char *str) instead of usart_transmit(char str[])
 *                   both work similarly as both pass address in themselves)
 *          3. Integers (or we can say Numbers), this will take Integer as input and then will convert that number into "Sting" and then it will          
 *             transmit.
 * 
 * OUTPUT : Nothing            
 * 
 * Working : These functions are used to "print" the data i.e. anything entered is converted into string or character if its not a character
 *           or string already (e.g. usart_print(345) => this will first be converted into characters [if more than one number than in "String" 
 *           or we can say "Array of Characters" and then loaded into the UDR0] e.g. if usart_print('c') => it will be directly loaded into UDR0.  
 *           PURPOSE : The main use of this function is to display data ,say on 'Serial Monitor' as they accept only "characters". 
 *         
 *    =>     Firstly it waits for the UDRE0 bit to be set in UCSR0A RGISTER i.e. it waits for UDR (data register) to get empty 
 *           and then data passed as argument is loaded into UDR0 Register.
 */

/////////////////////////////////////////////// // This function is used to print single character // ////////////////////////////////////////////

void usart_print(char num)                  
{
 /*                                           ALL IS RUBISH READ THE NEXT NOTE
    NOTE I MADE A MISTKAE HERE -> while(UDRE!=1) , I WAS ACTUALLY GIVING A TRUE CONDITION i.e. WAIT UNTIL UDRE0 IS IS NOT EQUAL TO 1(UDRE != 1
    it just means UDRE == 0, i.e WAIT UNTIL DATA REGISTER IS NON EMPTY,:) "so Data Register UDRE0 was getting filled to when it was NON EMPTY") 
    SO THIS CONDITION WAS GETTING BYPASSED SO "DONT DO IT AGAIN!!!!! DON'T EVER PASS A TRUE CONDITION IN WHILE LOOP IF YOU HAVE TO WAIT FOR SOME 
    CONDITION TO OCCUR!!!!!!!!!!!" 

  Note: here also I made a mistake UDRE0 can never be equal to 1 because it is defined as #define UDRE0 5 in avr/io.h all of the above is rubbish
        while(UDRE0==1); =>while(5==1) which can never be true so this condition will be bypassed.
  */      
  
  while(!(UCSR0A & _BV(UDRE0)));// WAITING FOR DATA REGISTER(UDR) TO BE EMPTY BY MONITORING REGISTER EMPTY FLAG(UDREn) 
                                //1-> Data Register Empty & 0->Data Register Non empty
 
  UDR0 = num;// LOADING THE DATA FROM VARIABLE INTO  UDRE 

}

//////////////////// // This Function is used to print String (address of first element of string gets passed as the /////////////////////////// 
/////////////////// //  argument when we pass the String i.e usart_print("Hello"), here address of 'H' get passed. ///////////////////////////

void usart_print(char str[])                       
{                                             
  uint8_t len = strlen(str);                  // Storing the length of the string in variable 'len' with the help of 'strlen' function
  // usart_transmit(len);
  for (int i=0;i<len;i++)                     //Runnig the loop for i=0 to i< length of the string  
  {
    usart_print(str[i]);                      // Calling the usart_transmit(char num) function(i.e. the transmit function with character as argument)
  }
}

// NOTE: This function doesnot work properly, only 1st integer gets printed e.g. usart_print(76) => only 7 gets printed.

///////////////////////// This function is used to print single  Numbers or we can say Integers // //////////////////////////////////////////////

void usart_print(int num)                   
{
  char string[100];                                 // Converting the integer to String refer github ThingsToRemember Repo to learn about it. 
  
   int len =0;

    int i, rem, n;  
    n = num;
    while (n != 0 )
    {
        len++;
        n /= 10;
    }
    
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        string[len - (i + 1)] = rem + '0';
    }
    string[len] = '\0';
 

  while(!(UCSR0A & _BV(UDRE0)));// WAITING FOR DATA REGISTER(UDR) TO BE EMPTY BY MONITORING REGISTER EMPTY FLAG(UDREn) 
                                //1-> Data Register Empty & 0->Data Register Non empty
  for(int i=0;i<len;i++)
  {
  UDR0 = string[i];// LOADING THE DATA FROM VARIABLE INTO  UDRE 
  _delay_ms(10);        // NOTE : This delay is VERY IMPORTANT without this the whole number doesnot get printed , some data gets lost 
}                   //        The transmitter buffer requires some time befor getting Re-filled
}

/*
 * Function : usart_available()
 * INPUT    : Nothing
 * OUTPUT   : Nothing
 * 
 * Working : This function just wait for the Receive buffer to get filled with the data. When the data is Received completely it will
 *           pass to next instruction
 */


/*                       "Explanation of the meaning of => while(!(UCSR0A & _BV(RXC0))"

                                   "     _BV(RXC0) => 1<<RXC0 => 1<<7 => 0b10000000     "
  NOTE :1. while(!(UCSR0A & _BV(RXC0)) -> Take it as while(!(False)). Why so????? Because when RXC(Receive complete flag) is not set then the value 
          of (UCSR0A & _BV(RXC0)) = 0(i.e. 0b0xxxxxxx & 0b10000000 = 0b00000000 = 0 => FALSE) so," !(UCSR0A & _BV(RXC0)) =>  !(FALSE) => TRUE" 
          SO "while(!(UCSR0A)&_BV(RXC0))" can be written as while(TRUE),

        2. Now when RXC0 flag becomes set(i.e. 1) then (UCSR0A & _BV(RXC0)) = 10000000 (i.e0b1xxxxxxxx & 0b10000000 = 0b10000000) SO, 
          (UCSR0A & _BV(RXC0) can be considered as TRUE because value of (UCSR0A & _BV(RXC0) > 0 so,"" !(UCSR0A & _BV(RXC0)) =>  !(TRUE) => FALSE"
  Now when while(!(UCSR0A)&_BV(RXC0)) in this case becomes "while(FALSE)" therefore the while loop breaks 

*/

uint8_t usart_available()
{
  while(!(UCSR0A & _BV(RXC0))); // WAIT UNTIL THE RECEIVE BUFFER IS NOT FILLED WITH DATA "RXC0= 1 -> means that there is 
                                // new data in rx buffer that is not read yet
  
}

/*
 * Function : usart_write(int)
 * INPUT    : takes Integer as argument
 * OUTPUT   : Nothing
 * 
 * Working : This function is used to "write" the data to some sensor. The data is sent as it is without changing unlike usart_print() 
 *           in print() the function is coverted into string.
 *           So firstly the function waits for the data Register(UDR0) to get empty, then it writes the data to UDR0 (data Register)
 */

 
void usart_write(int num)
{
   while(!(UCSR0A & _BV(UDRE0)));// WAITING FOR DATA REGISTER(UDR) TO BE EMPTY BY MONITORING REGISTER EMPTY FLAG(UDREn) 
                                //1-> Data Register Empty & 0->Data Register Non empty
 
    UDR0 = num;// LOADING THE DATA FROM VARIABLE INTO  UDRE 
}



/*
 * Function : usart_receive()
 * INPUT    : Nothing
 * OUTPUT   : return the received data (UDR0)
 * 
 * Working : This function waits for the receive buffer to completely get filled . After that the data us received completely in 
 *           Rx buffer it is automatically passed to UDR0 register (data Register). So this function returns UDR0.
 */
char usart_receive()
{
  
  while(!(UCSR0A & _BV(RXC0))); // WAIT UNTIL THE RECEIVE BUFFER IS NOT FILLED WITH DATA "RXC0= 1 -> means that there is
                                // new data in rx buffer that is not read yet
  return UDR0;                  // RETURN THE VALUE STORED IN (UDRE0) DATA REGISTER 
}

void setup()
{
 
}
void loop()
{
    
}
/* PROBLEMS SOLVED
  1. single character was not getting printed by function "usart_transmit_str() 
  solution : Used the concept of function overloading, see in function usart_transmit for refrence

  PROBLEM UNSOLVED
  1.// problem is to accept any number e.g. 123 given as input to get printed at output , here when we pass usart_transmit(73) only 7 comes on
  serial monitor.

  */
