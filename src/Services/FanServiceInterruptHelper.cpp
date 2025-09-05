#include "Services/FanServiceInterruptHelper.h"
#include "Services/FanService.h"

void (*gpioISR[40])() = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11,
    isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21,
    isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31,
    isr32, isr33, isr34, isr35, isr36, isr37, isr38, isr39};

static void IRAM_ATTR isr0()
{
    if (FanService::instances[0])
        FanService::instances[0]->handlePulse();
}
static void IRAM_ATTR isr1()
{
    if (FanService::instances[1])
        FanService::instances[1]->handlePulse();
}
static void IRAM_ATTR isr2()
{
    if (FanService::instances[2])
        FanService::instances[2]->handlePulse();
}
static void IRAM_ATTR isr3()
{
    if (FanService::instances[3])
        FanService::instances[3]->handlePulse();
}
static void IRAM_ATTR isr4()
{
    if (FanService::instances[4])
        FanService::instances[4]->handlePulse();
}
static void IRAM_ATTR isr5()
{
    if (FanService::instances[5])
        FanService::instances[5]->handlePulse();
}
static void IRAM_ATTR isr6()
{
    if (FanService::instances[6])
        FanService::instances[6]->handlePulse();
}
static void IRAM_ATTR isr7()
{
    if (FanService::instances[7])
        FanService::instances[7]->handlePulse();
}
static void IRAM_ATTR isr8()
{
    if (FanService::instances[8])
        FanService::instances[8]->handlePulse();
}
static void IRAM_ATTR isr9()
{
    if (FanService::instances[9])
        FanService::instances[9]->handlePulse();
}
static void IRAM_ATTR isr10()
{
    if (FanService::instances[10])
        FanService::instances[10]->handlePulse();
}
static void IRAM_ATTR isr11()
{
    if (FanService::instances[11])
        FanService::instances[11]->handlePulse();
}
static void IRAM_ATTR isr12()
{
    if (FanService::instances[12])
        FanService::instances[12]->handlePulse();
}
static void IRAM_ATTR isr13()
{
    if (FanService::instances[13])
        FanService::instances[13]->handlePulse();
}
static void IRAM_ATTR isr14()
{
    if (FanService::instances[14])
        FanService::instances[14]->handlePulse();
}
static void IRAM_ATTR isr15()
{
    if (FanService::instances[15])
        FanService::instances[15]->handlePulse();
}
static void IRAM_ATTR isr16()
{
    if (FanService::instances[16])
        FanService::instances[16]->handlePulse();
}
static void IRAM_ATTR isr17()
{
    if (FanService::instances[17])
        FanService::instances[17]->handlePulse();
}
static void IRAM_ATTR isr18()
{
    if (FanService::instances[18])
        FanService::instances[18]->handlePulse();
}
static void IRAM_ATTR isr19()
{
    if (FanService::instances[19])
        FanService::instances[19]->handlePulse();
}
static void IRAM_ATTR isr20()
{
    if (FanService::instances[20])
        FanService::instances[20]->handlePulse();
}
static void IRAM_ATTR isr21()
{
    if (FanService::instances[21])
        FanService::instances[21]->handlePulse();
}
static void IRAM_ATTR isr22()
{
    if (FanService::instances[22])
        FanService::instances[22]->handlePulse();
}
static void IRAM_ATTR isr23()
{
    if (FanService::instances[23])
        FanService::instances[23]->handlePulse();
}
static void IRAM_ATTR isr24()
{
    if (FanService::instances[24])
        FanService::instances[24]->handlePulse();
}
static void IRAM_ATTR isr25()
{
    if (FanService::instances[25])
        FanService::instances[25]->handlePulse();
}
static void IRAM_ATTR isr26()
{
    if (FanService::instances[26])
        FanService::instances[26]->handlePulse();
}
static void IRAM_ATTR isr27()
{
    if (FanService::instances[27])
        FanService::instances[27]->handlePulse();
}
static void IRAM_ATTR isr28()
{
    if (FanService::instances[28])
        FanService::instances[28]->handlePulse();
}
static void IRAM_ATTR isr29()
{
    if (FanService::instances[29])
        FanService::instances[29]->handlePulse();
}
static void IRAM_ATTR isr30()
{
    if (FanService::instances[30])
        FanService::instances[30]->handlePulse();
}
static void IRAM_ATTR isr31()
{
    if (FanService::instances[31])
        FanService::instances[31]->handlePulse();
}
static void IRAM_ATTR isr32()
{
    if (FanService::instances[32])
        FanService::instances[32]->handlePulse();
}
static void IRAM_ATTR isr33()
{
    if (FanService::instances[33])
        FanService::instances[33]->handlePulse();
}
static void IRAM_ATTR isr34()
{
    if (FanService::instances[34])
        FanService::instances[34]->handlePulse();
}
static void IRAM_ATTR isr35()
{
    if (FanService::instances[35])
        FanService::instances[35]->handlePulse();
}
static void IRAM_ATTR isr36()
{
    if (FanService::instances[36])
        FanService::instances[36]->handlePulse();
}
static void IRAM_ATTR isr37()
{
    if (FanService::instances[37])
        FanService::instances[37]->handlePulse();
}
static void IRAM_ATTR isr38()
{
    if (FanService::instances[38])
        FanService::instances[38]->handlePulse();
}
static void IRAM_ATTR isr39()
{
    if (FanService::instances[39])
        FanService::instances[39]->handlePulse();
}