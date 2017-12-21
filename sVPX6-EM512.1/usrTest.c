#if 0
UINT32 guiIntNum = 0;

VOID intISR(VOID)
{
    guiIntNum++;
}

#define EPIC_INT_NUM   1

VOID testInt(UINT32 uiMode)
{
    intOpenInit(EPIC_INT_NUM,uiMode,intISR,NULL);
    
    intMaskEnable(EPIC_INT_NUM);
    
    switch(uiMode)
    {
        case EPIC_MODE_LOW_LEV:
            intSetMode(EPIC_INT_NUM,EPIC_MODE_LOW_LEV);
            break;
        case EPIC_MODE_HIGH_LEV:
            intSetMode(EPIC_INT_NUM,EPIC_MODE_HIGH_LEV);
            break;
        case EPIC_MODE_LOW_EDG:
            intSetMode(EPIC_INT_NUM,EPIC_MODE_LOW_EDG);
            break;
        case EPIC_MODE_HIGH_EDG:
            intSetMode(EPIC_INT_NUM,EPIC_MODE_HIGH_EDG);
            break;        
    }
}
#endif

