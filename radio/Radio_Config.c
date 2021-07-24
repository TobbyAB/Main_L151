#include "Radio.h"
#include "AX5043.h"
#include "Radio_Config.h"

#define DBG_TAG "radio"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

const uint16_t RegisterVaule[][2]=
{
    { REG_AX5043_MODULATION,          0x08},
    { REG_AX5043_ENCODING,            0x00},
    { REG_AX5043_FRAMING,             0x26},
    { REG_AX5043_PINFUNCSYSCLK,       0x01},
    { REG_AX5043_PINFUNCDCLK,         0x01},
    { REG_AX5043_PINFUNCDATA,         0x01},
    { REG_AX5043_PINFUNCANTSEL,       0x04},
    { REG_AX5043_PINFUNCPWRAMP,       0x00},
    { REG_AX5043_WAKEUPXOEARLY,       0x01},
    { REG_AX5043_IFFREQ1,             0x01},
    { REG_AX5043_IFFREQ0,             0xE4},
    { REG_AX5043_DECIMATION,          0x16},
    { REG_AX5043_RXDATARATE2,         0x00},
    { REG_AX5043_RXDATARATE1,         0x3D},
    { REG_AX5043_RXDATARATE0,         0x8D},
    { REG_AX5043_MAXDROFFSET2,        0x00},
    { REG_AX5043_MAXDROFFSET1,        0x00},
    { REG_AX5043_MAXDROFFSET0,        0x00},
    { REG_AX5043_MAXRFOFFSET2,        0x80},
    { REG_AX5043_MAXRFOFFSET1,        0x02},
    { REG_AX5043_MAXRFOFFSET0,        0x31},
    { REG_AX5043_FSKDMAX1,            0x00},
    { REG_AX5043_FSKDMAX0,            0xA6},
    { REG_AX5043_FSKDMIN1,            0xFF},
    { REG_AX5043_FSKDMIN0,            0x5A},
    { REG_AX5043_AMPLFILTER,          0x00},
    { REG_AX5043_RXPARAMSETS,         0xF4},
    { REG_AX5043_AGCGAIN0,            0xC5},
    { REG_AX5043_AGCTARGET0,          0x84},
    { REG_AX5043_TIMEGAIN0,           0xF8},
    { REG_AX5043_DRGAIN0,             0xF2},
    { REG_AX5043_PHASEGAIN0,          0xC3},
    { REG_AX5043_FREQUENCYGAINA0,     0x0F},
    { REG_AX5043_FREQUENCYGAINB0,     0x1F},
    { REG_AX5043_FREQUENCYGAINC0,     0x08},
    { REG_AX5043_FREQUENCYGAIND0,     0x08},
    { REG_AX5043_AMPLITUDEGAIN0,      0x06},
    { REG_AX5043_FREQDEV10,           0x00},
    { REG_AX5043_FREQDEV00,           0x00},
    { REG_AX5043_BBOFFSRES0,          0x00},
    { REG_AX5043_AGCGAIN1,            0xC5},
    { REG_AX5043_AGCTARGET1,          0x84},
    { REG_AX5043_AGCAHYST1,           0x00},
    { REG_AX5043_AGCMINMAX1,          0x00},
    { REG_AX5043_TIMEGAIN1,           0xF6},
    { REG_AX5043_DRGAIN1,             0xF1},
    { REG_AX5043_PHASEGAIN1,          0xC3},
    { REG_AX5043_FREQUENCYGAINA1,     0x0F},
    { REG_AX5043_FREQUENCYGAINB1,     0x1F},
    { REG_AX5043_FREQUENCYGAINC1,     0x08},
    { REG_AX5043_FREQUENCYGAIND1,     0x08},
    { REG_AX5043_AMPLITUDEGAIN1,      0x06},
    { REG_AX5043_FREQDEV11,           0x00},
    { REG_AX5043_FREQDEV01,           0x3E},
    { REG_AX5043_FOURFSK1,            0x16},
    { REG_AX5043_BBOFFSRES1,          0x00},
    { REG_AX5043_AGCGAIN3,            0xFF},
    { REG_AX5043_AGCTARGET3,          0x84},
    { REG_AX5043_AGCAHYST3,           0x00},
    { REG_AX5043_AGCMINMAX3,          0x00},
    { REG_AX5043_TIMEGAIN3,           0xF5},
    { REG_AX5043_DRGAIN3,             0xF0},
    { REG_AX5043_PHASEGAIN3,          0xC3},
    { REG_AX5043_FREQUENCYGAINA3,     0x0F},
    { REG_AX5043_FREQUENCYGAINB3,     0x1F},
    { REG_AX5043_FREQUENCYGAINC3,     0x0C},
    { REG_AX5043_FREQUENCYGAIND3,     0x0C},
    { REG_AX5043_AMPLITUDEGAIN3,      0x06},
    { REG_AX5043_FREQDEV13,           0x00},
    { REG_AX5043_FREQDEV03,           0x3E},
    { REG_AX5043_FOURFSK3,            0x16},
    { REG_AX5043_BBOFFSRES3,          0x00},
    { REG_AX5043_MODCFGF,             0x00},
    { REG_AX5043_FSKDEV2,             0x00},
    { REG_AX5043_FSKDEV1,             0x04},
    { REG_AX5043_FSKDEV0,             0x2F},
    { REG_AX5043_MODCFGA,             0x05},
    { REG_AX5043_TXRATE2,             0x00},
    { REG_AX5043_TXRATE1,             0x0C},
    { REG_AX5043_TXRATE0,             0x19},
    { REG_AX5043_TXPWRCOEFFB1,        0x07},
    { REG_AX5043_TXPWRCOEFFB0,        0x00},
    { REG_AX5043_PLLVCOI,             0x99},
    { REG_AX5043_PLLRNGCLK,           0x04},
    { REG_AX5043_BBTUNE,              0x0F},
    { REG_AX5043_BBOFFSCAP,           0x77},
    { REG_AX5043_PKTADDRCFG,          0x00},
    { REG_AX5043_PKTLENCFG,           0x80},
    { REG_AX5043_PKTLENOFFSET,        0x00},
    { REG_AX5043_PKTMAXLEN,           0xC8},
    { REG_AX5043_MATCH0PAT3,          0xAA},
    { REG_AX5043_MATCH0PAT2,          0xCC},
    { REG_AX5043_MATCH0PAT1,          0xAA},
    { REG_AX5043_MATCH0PAT0,          0xCC},
    { REG_AX5043_MATCH0LEN,           0x9F},
    { REG_AX5043_MATCH0MAX,           0x1F},
    { REG_AX5043_MATCH1PAT1,          0x55},
    { REG_AX5043_MATCH1PAT0,          0x55},
    { REG_AX5043_MATCH1LEN,           0x8A},
    { REG_AX5043_MATCH1MAX,           0x0A},
    { REG_AX5043_TMGTXBOOST,          0x3E},
    { REG_AX5043_TMGTXSETTLE,         0x31},
    { REG_AX5043_TMGRXBOOST,          0x3E},
    { REG_AX5043_TMGRXSETTLE,         0x31},
    { REG_AX5043_TMGRXOFFSACQ,        0x00},
    { REG_AX5043_TMGRXCOARSEAGC,      0x7F},
    { REG_AX5043_TMGRXRSSI,           0x03},
    { REG_AX5043_TMGRXPREAMBLE2,      0x35},
    { REG_AX5043_RSSIABSTHR,          0xE0},
    { REG_AX5043_BGNDRSSITHR,         0x00},
    { REG_AX5043_PKTCHUNKSIZE,        0x0D},
    { REG_AX5043_PKTACCEPTFLAGS,      0x00},
    { REG_AX5043_DACVALUE1,           0x00},
    { REG_AX5043_DACVALUE0,           0x00},
    { REG_AX5043_DACCONFIG,           0x00},
    { REG_AX5043_REF,                 0x03},
    { REG_AX5043_0xF1C,               0x07},
    { REG_AX5043_0xF21,               0x68},
    { REG_AX5043_0xF22,               0xFF},
    { REG_AX5043_0xF23,               0x84},
    { REG_AX5043_0xF26,               0x98},
    { REG_AX5043_0xF34,               0x28},
    { REG_AX5043_0xF35,               0x11},
    { REG_AX5043_0xF44,               0x25},
    { AX5043_Addr_END,     AX5043_Vule_END}
};

const uint16_t TXMODE_REG[][2]=
{
    { REG_AX5043_PLLLOOP,     0x0B},
    { REG_AX5043_PLLCPI,      0x10},
    { REG_AX5043_PLLVCODIV,   0x24},
    { REG_AX5043_PLLVCOI,     0x99},
    { REG_AX5043_XTALCAP,     0x00},
    { REG_AX5043_0xF00,       0x0F},
    { REG_AX5043_REF,         0x03},
    { REG_AX5043_0xF18,       0x06}
};

const uint16_t RXMODE_REG[][2]=
{
    { REG_AX5043_PLLLOOP,    0x0B},
    { REG_AX5043_PLLCPI,     0x10},
    { REG_AX5043_PLLVCODIV,  0x24},
    { REG_AX5043_PLLVCOI,    0x99},
    { REG_AX5043_XTALCAP,    0x0C},
    { REG_AX5043_0xF00,      0x0F},
    { REG_AX5043_REF,        0x03},
    { REG_AX5043_0xF18,      0x02}
};

void WirelessBitRateConfigure(uint8_t BitRate0)
{
    SpiWriteLongAddressRegister(REG_AX5043_IFFREQ1  , 0x01 );
    SpiWriteLongAddressRegister(REG_AX5043_IFFREQ0  , 0xE4  );
    SpiWriteLongAddressRegister(REG_AX5043_DECIMATION , 0x16  );
    SpiWriteLongAddressRegister(REG_AX5043_RXDATARATE2 , 0x00 );
    SpiWriteLongAddressRegister(REG_AX5043_RXDATARATE1 , 0x3D );
    SpiWriteLongAddressRegister(REG_AX5043_RXDATARATE0 , 0x8D  );
    SpiWriteLongAddressRegister(REG_AX5043_MAXRFOFFSET1 , 0x02);
    SpiWriteLongAddressRegister(REG_AX5043_MAXRFOFFSET0 , 0x31);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDMAX1,0x00);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDMAX0,0xA6);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDMIN1,0xFF);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDMIN0,0x5A);

    SpiWriteLongAddressRegister(REG_AX5043_AGCGAIN0  , 0xC5);
    SpiWriteLongAddressRegister(REG_AX5043_AGCTARGET0 , 0x84);
    SpiWriteLongAddressRegister(REG_AX5043_TIMEGAIN0  , 0xF8);
    SpiWriteLongAddressRegister(REG_AX5043_DRGAIN0  , 0xF2);
    SpiWriteLongAddressRegister(REG_AX5043_PHASEGAIN0  , 0xC3);
    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAINC0, 0x09);
    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAIND0, 0x09);
    SpiWriteLongAddressRegister(REG_AX5043_AGCGAIN1   , 0xC5);
    SpiWriteLongAddressRegister(REG_AX5043_TIMEGAIN1   , 0xF6);
    SpiWriteLongAddressRegister(REG_AX5043_DRGAIN1   , 0xF1 );
    SpiWriteLongAddressRegister(REG_AX5043_PHASEGAIN1  , 0xc3);
    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAINC1, 0x09);
    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAIND1, 0x09);

    SpiWriteLongAddressRegister(REG_AX5043_TIMEGAIN3   , 0xF5);
    SpiWriteLongAddressRegister(REG_AX5043_DRGAIN3,0xF0);
    SpiWriteLongAddressRegister(REG_AX5043_PHASEGAIN3  , 0xC3);

    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAINC3, 0x0C);
    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAIND3, 0x0C);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDEV2   , 0x00  );
    SpiWriteLongAddressRegister(REG_AX5043_FSKDEV1   , 0x04  );
    SpiWriteLongAddressRegister(REG_AX5043_FSKDEV0   , 0x08  );
    SpiWriteLongAddressRegister(REG_AX5043_MODCFGA   , 0x05  );
    SpiWriteLongAddressRegister(REG_AX5043_TXRATE2   , 0x00  );
    SpiWriteLongAddressRegister(REG_AX5043_TXRATE1   , 0x0C );
    SpiWriteLongAddressRegister(REG_AX5043_TXRATE0   , 0x19 );
    SpiWriteLongAddressRegister(REG_AX5043_BBTUNE   , 0x0F  );
    SpiWriteLongAddressRegister(REG_AX5043_RSSIREFERENCE   , 0xFA);
}

uint32_t WirelessFreqConfigure(uint8_t Freq2, uint8_t Freq1, uint8_t Freq0, uint8_t Channel_Num)
{
    uint32_t Freq_centr = 0;
    uint32_t FreqCentr_cal = 0;
    Freq_centr = Freq2;
    Freq_centr = Freq_centr << 8;
    Freq_centr += Freq1;
    Freq_centr = Freq_centr << 8;
    Freq_centr += Freq0;

    if (Freq_centr > 910000 || Freq_centr < 300000)
    {
        Freq_centr = 433000;
    }
    Freq_centr += CHANNEL_BW * Channel_Num;
    FreqCentr_cal = (uint32_t)((double)(Freq_centr/(double)(XTAL_FREQ/1000))*1024*1024*16);
    return FreqCentr_cal;
}

void WirelessTxPowerConfigure(uint8_t TxPower)
{
    switch (TxPower)
    {
        case 0:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x02);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x07);
            break;
        case 1:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x02);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x44);
            break;
        case 2:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x02);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x90);
            break;
        case 3:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x02);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0xEB);
            break;
        case 4:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x03);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x5E);
            break;
        case 5:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x03);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0xD6);
            break;
        case 6:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x04);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x06);
            break;
        case 7:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x04);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0xA9);
            break;
        case 8:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x05);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x7C);
            break;
        case 9:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x06);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x00);
            break;
        case 10:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x07);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x00);
            break;
        case 11:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x08);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x00);
            break;
        case 12:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x09);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0xD4);
            break;
        case 13:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x0C);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x00);
            break;
        case 14:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x0F);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x00);
        case 15:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x0F);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0xFF);
            break;
        default:
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB1, 0x02);
            SpiWriteLongAddressRegister(REG_AX5043_TXPWRCOEFFB0, 0x07);
            break;
    }
}
