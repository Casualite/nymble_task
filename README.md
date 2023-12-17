# NYMBLE EF TASK
Code written in C on VS code to run on an arduino uno r3 (atmega328p). Since there were less vs code extensions avr boards, makefile was written to ease compilation, upload and cleaning. avr-gcc used as the compiler and avr dude used to upload code to uno.
## Board side code
### UART INITIALIZATION
- Internal clock disabled before with **fuse bits** i.e **CKSEL[3:0]=0000**
![image](https://github.com/Casualite/nymble_task/assets/92442612/ab537e8c-859e-42b4-b082-7c5c0f900f06)
- **USART** is initailized, passing expected **baud rate** as parameter, 2400 taked as asked. **UBRR** bits set after calculating ubrr. Clock used for calculating ubrr is **16MHz**
![image](https://github.com/Casualite/nymble_task/assets/92442612/16a707e9-1142-4614-bfc8-ea69eab074be)

- Appropriate bits changed in **UCSR0C** to enable sync/async operation, further **8 bit** dataframe used for UART operations
![image](https://github.com/Casualite/nymble_task/assets/92442612/ca2b9152-bf6b-4cf0-b3e2-f154ce18763c)

-  Vector **UCSR0B** modified to enable TX/RX and their respective interupts,specifically **UCSR0B[7:6]=11 UCSR0B[4:3]=11**
![image](https://github.com/Casualite/nymble_task/assets/92442612/29210d97-e208-4ec2-964e-faee2f66c5fa)
![image](https://github.com/Casualite/nymble_task/assets/92442612/d0afb9e5-c301-45ed-b3c2-20952d81b8ce)

-  No parity checks done

### UART TX/RX
- **BUFFER** was created of size "1KB". This was used to store all bites in the string
![image](https://github.com/Casualite/nymble_task/assets/92442612/81f8d5a4-5eea-42a0-8174-3639965c5ec4)

- Interupt in **USART_RX_vect**(from **UCSR0A**) used to receive data from a function that follows datasheet. Then this is added to the buffer until a special character is reached, after which it starts transmitting data. I was not able to transmit '\0' hence used '$' as end of string character. This allows for variable number of characters to be sent, not specific to example
![image](https://github.com/Casualite/nymble_task/assets/92442612/45286b68-38d9-43b3-8b19-1a631c3f49a8)

- As mentioned in the last point, once end character is obtained, TX starts. Tx is done in two ways:
    1. Using a function which **takes the string as argument**, which inturn calls another routine, which follows whats given in the datasheet
![image](https://github.com/Casualite/nymble_task/assets/92442612/aaf5ceeb-0a43-4d55-8a14-685da7b194b6)
    1. Triggering **USART_TX_vect** interupt from **USART_RX_vect** interupt, both are in **UCSR0A**. What follows is recursive inturpts after each Tx is complete, until all characters are read and transmitted, including the end of sentence character
> [!WARNING]
> Use only one at a time, using both will cause a lot of wrong **USART Transmit Complete** interupts to trigger, **leading to a many more wrong triggers at an exponential rate**.

## PC Side
- Code is written in **python**
- Uses **pyserial** to establish connection with arduino 
- The String is encoded in **utf-8** and written to the arduino
- **timeit** used for timer.Timer is started , and once every **N** bits are reached, it calculates the elapsed time using **datetime.timedelta**. N=10 in this case.
![image](https://github.com/Casualite/nymble_task/assets/92442612/0e19d1a8-b4b1-43e0-80f7-b0e0f74780b3)
- Meanwhile the transmitted string is collected into an empty string and displayed at the end. This was done to prevent displaying both simultaneously. Also an observation made was that, **print statements cause PC and board to desynchronize, leading to errors like missing characters or completely changed characters**
- output:


  ![image](https://github.com/Casualite/nymble_task/assets/92442612/68374660-f9b9-4ee5-b430-641a455699d6)



  



