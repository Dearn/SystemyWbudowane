#ifndef ONE_WIRE_H
#define ONE_WIRE_H



//ROM commands
#define ONE_WIRE_CMD_READ_ROM           0x33
#define ONE_WIRE_CMD_MATCH_ROM          0x55
#define ONE_WIRE_CMD_SKIP_ROM           0xCC



void owInit(void);
char owPresence(void);
void owWrite(char dat);
char owRead(void);
char owCheckCrc(char *rom);
char owReadRom(char *rom);
void owStrongPullupOn(void);
void owStrongPullupOff(void);



#endif

