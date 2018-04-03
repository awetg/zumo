#include "project.h"

/* Don't remove the functions below */
int _write(int file, char *ptr, int len)
{
    (void)file; /* Parameter is not used, suppress unused argument warning */
	int n;
	for(n = 0; n < len; n++) {
        if(*ptr == '\n') UART_1_PutChar('\r');
		UART_1_PutChar(*ptr++);
	}
	return len;
}

int _read (int file, char *ptr, int count)
{
    int chs = 0;
    char ch;
    static char buf[128];
    static unsigned int cnt = 0;
    static unsigned int head = 0;
    static unsigned int tail = 0;
    static unsigned int lf = 0;
 
    (void)file; /* Parameter is not used, suppress unused argument warning */
    while(count > 0) {
        ch = UART_1_GetChar();
        if(ch != 0) {
            UART_1_PutChar(ch);
            if(ch == '\r') { /* convert CR to LF for processing, putty sends CR when Enter is pressed */
                ch = '\n';
                UART_1_PutChar(ch);
            }
            if(ch == 127) { /* backspace handling */
                if(cnt > 0) {
                    cnt--;
                    head = (head - 1) & 0x7F;
                }
            }
            else { /* other characters: store and check for LF */
                buf[head] = ch;
                head = (head + 1) & 0x7F;
                cnt++;
                if(ch == '\n' || cnt >= 128) lf++;
            }
        }
        if(lf > 0) {
            if(cnt > 0) {
                if(buf[tail] == '\n') lf--;
                *ptr++ = buf[tail];
                tail = (tail + 1) & 0x7F;
                cnt--;
                chs++;
                count--;
            }
            else {
                lf = 0;
            }
        }
    }
    return chs;
}