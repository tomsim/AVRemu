////////////////////////////////////////////////////////////////////////////////
// atxmegaAU.cpp
// ATxmega128A4U / 64A4U / 32A4U / 16A4U
////////////////////////////////////////////////////////////////////////////////

#include "avr.h"
#include "instr.h"

namespace AVR
{
  ATxmegaAU::ATxmegaAU(std::size_t programSize, std::size_t dataSize, std::size_t eepromSize)
    : Mcu(programSize, false, 0x1000, 0x2000, dataSize, eepromSize),
      _usartC0("USARTC0"), _usartC1("USARTC1"), _usartD0("USARTD0"), _usartD1("USARTD1"), _usartE0("USARTE0")
  {
    _isXMega = true ;

    const Instruction *instructions[]
    {
      &instrADD, &instrADC, &instrADIW, &instrSUB, &instrSUBI, &instrSBC, &instrSBCI, &instrSBIW, &instrAND, &instrANDI,
      &instrOR, &instrORI, &instrEOR, &instrCOM, &instrNEG, &instrINC, &instrDEC, &instrMUL, &instrMULS, &instrMULSU,
      &instrFMUL, &instrFMULS, &instrFMULSU, &instrDES,

      &instrRJMP, &instrIJMP, &instrEIJMP, &instrJMP, &instrRCALL, &instrICALL, &instrEICALL, &instrCALL, &instrRET,
      &instrRETI, &instrCPSE, &instrCP, &instrCPC, &instrCPI, &instrSBRC, &instrSBRS, &instrSBIC, &instrSBIS,
      &instrBRBS, &instrBRBC,

      &instrMOV, &instrMOVW, &instrLDI, &instrLDS, &instrLDx1, &instrLDx2, &instrLDx3, &instrLDy1, &instrLDy2,
      &instrLDy3, &instrLDy4, &instrLDz1, &instrLDz2, &instrLDz3, &instrLDz4, &instrSTS, &instrSTx1, &instrSTx2,
      &instrSTx3, &instrSTy1, &instrSTy2, &instrSTy3, &instrSTy4, &instrSTz1, &instrSTz2, &instrSTz3, &instrSTz4,
      &instrLPM1, &instrLPM2, &instrLPM3, &instrELPM1, &instrELPM2, &instrELPM3, &instrSPM1, &instrSPM2, &instrIN,
      &instrOUT, &instrPUSH, &instrPOP, &instrXCH, &instrLAS, &instrLAC, &instrLAT,

      &instrLSR, &instrROR, &instrASR, &instrSWAP, &instrBSET, &instrBCLR, &instrSBI, &instrCBI, &instrBST,
      &instrBLD,

      &instrBREAK, &instrNOP, &instrSLEEP, &instrWDR,
    } ;
    for (const Instruction* iInstr: instructions)
      AddInstruction(iInstr) ;

    _knownProgramAddresses = std::vector<Mcu::KnownProgramAddress>
      {
        { 0x000, "RESET"           , "RESET" },
        { 0x002, "OSCF_INT"        , "Crystal oscillator failure interrupt (NMI)" },
        { 0x004, "PORTC_INT0"      , "Port C interrupt 0" },
        { 0x006, "PORTC_INT1"      , "Port C interrupt 1" },
        { 0x008, "PORTR_INT0"      , "Port R interrupt 0" },
        { 0x00a, "PORTR_INT1"      , "Port R interrupt 1" },
        { 0x00C, "DMA_INT_CH0"     , "DMA controller channel 0 interrupt" },
        { 0x00E, "DMA_INT_CH1"     , "DMA controller channel 1 interrupt" },
        { 0x010, "DMA_INT_CH2"     , "DMA controller channel 2 interrupt" },
        { 0x012, "DMA_INT_CH3"     , "DMA controller channel 3 interrupt" },
        { 0x014, "RTC_INT_OVF"     , "Real time counter overflow interrupt" },
        { 0x016, "RTC_INT_COMP"    , "Real time counter compare interrupt" },
        { 0x018, "TWIC_INT_SLAVE"  , "Two-Wire Interface on Port C slave interrupt" },
        { 0x01A, "TWIC_INT_MASTER" , "Two-Wire Interface on Port C master interrupt" },
        { 0x01C, "TCC0_INT_OVF"    , "Timer/counter 0 on port C interrupt" },
        { 0x01E, "TCC0_INT_ERR"    , "Timer/counter 0 on port C interrupt" },
        { 0x020, "TCC0_INT_CCA"    , "Timer/counter 0 on port C interrupt" },
        { 0x022, "TCC0_INT_CCB"    , "Timer/counter 0 on port C interrupt" },
        { 0x024, "TCC0_INT_CCC"    , "Timer/counter 0 on port C interrupt" },
        { 0x026, "TCC0_INT_CCD"    , "Timer/counter 0 on port C interrupt" },
        { 0x028, "TCC1_INT_OVF"    , "Timer/counter 1 on port C interrupt" },
        { 0x02A, "TCC1_INT_ERR"    , "Timer/counter 1 on port C interrupt" },
        { 0x02C, "TCC1_INT_CCA"    , "Timer/counter 1 on port C interrupt" },
        { 0x02E, "TCC1_INT_CCB"    , "Timer/counter 1 on port C interrupt" },
        { 0x030, "SPIC_INT"        , "SPI on port C interrupt" },
        { 0x032, "USARTC0_INT_RXC" , "USART 0 on port C receive complete interrupt" },
        { 0x034, "USARTC0_INT_DRE" , "USART 0 on port C data register empty interrupt" },
        { 0x036, "USARTC0_INT_TXC" , "USART 0 on port C transmit complete interrupt" },
        { 0x038, "USARTC1_INT_RXC" , "USART 1 on port C receive complete interrupt" },
        { 0x03A, "USARTC1_INT_DRE" , "USART 1 on port C data register empty interrupt" },
        { 0x03C, "USARTC1_INT_TXC" , "USART 1 on port C transmit complete interrupt" },
        { 0x03E, "AES_INT"         , "AES interrupt" },
        { 0x040, "NVM_INT_EE"      , "Nonvolatile Memory EEPROM interrupt" },
        { 0x042, "NVM_INT_SPM"     , "Nonvolatile Memory SPM interrupt" },
        { 0x044, "PORTB_INT0"      , "Port B interrupt 0" },
        { 0x046, "PORTB_INT1"      , "Port B interrupt 1" },
        { 0x056, "PORTE_INT0"      , "Port E interrupt 0" },
        { 0x058, "PORTE_INT1"      , "Port E interrupt 1" },
        { 0x05A, "TWIE_INT_SLAVE"  , "Two-wire Interface on Port E slave interrupt" },
        { 0x05C, "TWIE_INT_MASTER" , "Two-wire Interface on Port E master interrupt" },
        { 0x05E, "TCE0_INT_OVF"    , "Timer/counter 0 on port E interrupt" },
        { 0x060, "TCE0_INT_ERR"    , "Timer/counter 0 on port E interrupt" },
        { 0x062, "TCE0_INT_CCA"    , "Timer/counter 0 on port E interrupt" },
        { 0x064, "TCE0_INT_CCB"    , "Timer/counter 0 on port E interrupt" },
        { 0x066, "TCE0_INT_CCC"    , "Timer/counter 0 on port E interrupt" },
        { 0x068, "TCE0_INT_CCD"    , "Timer/counter 0 on port E interrupt" },
        { 0x06A, "TCE1_INT_OVF"    , "Timer/counter 1 on port E interrupt" },
        { 0x06C, "TCE1_INT_ERR"    , "Timer/counter 1 on port E interrupt" },
        { 0x06E, "TCE1_INT_CCA"    , "Timer/counter 1 on port E interrupt" },
        { 0x070, "TCE1_INT_CCB"    , "Timer/counter 1 on port E interrupt" },
        { 0x074, "USARTE0_INT_RXC" , "USART 0 on port E receive complete interrupt" },
        { 0x076, "USARTE0_INT_DRE" , "USART 0 on port E data register empty interrupt" },
        { 0x078, "USARTE0_INT_TXC" , "USART 0 on port E transmit complete interrupt" },
        { 0x080, "PORTD_INT0"      , "Port D interrupt 0" },
        { 0x082, "PORTD_INT1"      , "Port D interrupt 1" },
        { 0x084, "PORTA_INT0"      , "Port A interrupt 0" },
        { 0x086, "PORTA_INT1"      , "Port A interrupt 1" },
        { 0x088, "ACA_INT_COMP0"   , "Analog Comparator 0 on Port A interrupt" },
        { 0x08A, "ACA_INT_COMP1"   , "Analog Comparator 0 on Port A interrupt" },
        { 0x08C, "ACA_INT_WINDOW"  , "Analog Comparator window on Port A interrupt" },
        { 0x08E, "ADCA_INT_CH0"    , "Analog to Digital Converter channel 0 on Port A interrupt base" },
        { 0x090, "ADCA_INT_CH1"    , "Analog to Digital Converter channel 1 on Port A interrupt base" },
        { 0x092, "ADCA_INT_CH2"    , "Analog to Digital Converter channel 2 on Port A interrupt base" },
        { 0x094, "ADCA_INT_CH3"    , "Analog to Digital Converter channel 3 on Port A interrupt base" },
        { 0x09A, "TCD0_INT_OVF"    , "Timer/counter 0 on port D interrupt" },
        { 0x09C, "TCD0_INT_ERR"    , "Timer/counter 0 on port D interrupt" },
        { 0x09E, "TCD0_INT_CCA"    , "Timer/counter 0 on port D interrupt" },
        { 0x0A0, "TCD0_INT_CCB"    , "Timer/counter 0 on port D interrupt" },
        { 0x0A2, "TCD0_INT_CCC"    , "Timer/counter 0 on port D interrupt" },
        { 0x0A4, "TCD0_INT_CCD"    , "Timer/counter 0 on port D interrupt" },
        { 0x0A6, "TCD1_INT_OVF"    , "Timer/counter 1 on port D interrupt" },
        { 0x0A8, "TCD1_INT_ERR"    , "Timer/counter 1 on port D interrupt" },
        { 0x0AA, "TCD1_INT_CCA"    , "Timer/counter 1 on port D interrupt" },
        { 0x0AC, "TCD1_INT_CCB"    , "Timer/counter 1 on port D interrupt" },
        { 0x0AE, "SPID_INT"        , "SPI on port D interrupt" },
        { 0x0B0, "USARTD0_INT_RXC" , "USART 0 on port D receive complete interrupt" },
        { 0x0B2, "USARTD0_INT_DRE" , "USART 0 on port D data register empty interrupt" },
        { 0x0B4, "USARTD0_INT_TXC" , "USART 0 on port D transmit complete interrupt" },
        { 0x0B6, "USARTD1_INT_RXC" , "USART 1 on port D receive complete interrupt" },
        { 0x0B8, "USARTD1_INT_DRE" , "USART 1 on port D data register empty interrupt" },
        { 0x0BA, "USARTD1_INT_TXC" , "USART 1 on port D transmit complete interrupt" },
        { 0x0FA, "USB_INT_BUSEVENT", "USB on port D SOF, suspend, resume, bus reset, CRC, underflow, overflow, and stall error interrupts" },
        { 0x0FC, "USB_INT_TRNCOMPL", "USB on port D Transaction complete interrupt" },
      } ;

    std::vector<std::pair<uint32_t, Io::Register*>> ioRegs
    {
      { 0x0000, new IoRegisterNotImplemented("GPIOR0") }, // General Purpose IO Registers
      { 0x0001, new IoRegisterNotImplemented("GPIOR1") },
      { 0x0002, new IoRegisterNotImplemented("GPIOR2") },
      { 0x0003, new IoRegisterNotImplemented("GPIOR3") },
      { 0x0004, new IoRegisterNotImplemented("GPIOR4") },
      { 0x0005, new IoRegisterNotImplemented("GPIOR5") },
      { 0x0006, new IoRegisterNotImplemented("GPIOR6") },
      { 0x0007, new IoRegisterNotImplemented("GPIOR7") },
      { 0x0008, new IoRegisterNotImplemented("GPIOR8") },
      { 0x0009, new IoRegisterNotImplemented("GPIOR9") },
      { 0x000a, new IoRegisterNotImplemented("GPIOR10") },
      { 0x000b, new IoRegisterNotImplemented("GPIOR11") },
      { 0x000c, new IoRegisterNotImplemented("GPIOR12") },
      { 0x000d, new IoRegisterNotImplemented("GPIOR13") },
      { 0x000e, new IoRegisterNotImplemented("GPIOR14") },
      { 0x000f, new IoRegisterNotImplemented("GPIOR15") },
        
      { 0x0010, new IoRegisterNotImplemented("VPORT0_DIR") }, // Virtual Port 0
      { 0x0011, new IoRegisterNotImplemented("VPORT0_OUT") },
      { 0x0012, new IoRegisterNotImplemented("VPORT0_IN") },
      { 0x0013, new IoRegisterNotImplemented("VPORT0_INTFLAGS") },
        
      { 0x0014, new IoRegisterNotImplemented("VPORT1_DIR") }, // Virtual Port 1
      { 0x0015, new IoRegisterNotImplemented("VPORT1_OUT") },
      { 0x0016, new IoRegisterNotImplemented("VPORT1_IN") },
      { 0x0017, new IoRegisterNotImplemented("VPORT1_INTFLAGS") },
        
      { 0x0018, new IoRegisterNotImplemented("VPORT2_DIR") }, // Virtual Port 2
      { 0x0019, new IoRegisterNotImplemented("VPORT2_OUT") },
      { 0x001a, new IoRegisterNotImplemented("VPORT2_IN") },
      { 0x001b, new IoRegisterNotImplemented("VPORT2_INTFLAGS") },
        
      { 0x001c, new IoRegisterNotImplemented("VPORT3_DIR") }, // Virtual Port 3
      { 0x001d, new IoRegisterNotImplemented("VPORT3_OUT") },
      { 0x001e, new IoRegisterNotImplemented("VPORT3_IN") },
      { 0x001f, new IoRegisterNotImplemented("VPORT3_INTFLAGS") },
                
      { 0x0034, new IoRegisterNotImplemented("CPU_CCP") }, // CPU
      { 0x0038, new IoRegisterNotImplemented("CPU_RAMPD") },
      { 0x0039, new IoRegisterNotImplemented("CPU_RAMPX") },
      { 0x003a, new IoRegisterNotImplemented("CPU_RAMPY") },
      { 0x003b, new IoRegisterNotImplemented("CPU_RAMPZ") },
      { 0x003c, new IoRegisterNotImplemented("CPU_EIND") },
      //{ 0x003d, new IoRegisterNotImplemented("CPU_SPL") },
      //{ 0x003e, new IoRegisterNotImplemented("CPU_SPH") },
      //{ 0x003f, new IoRegisterNotImplemented("CPU_SREG") },
        
      { 0x0040, new IoRegisterNotImplemented("CLK_CTRL") }, // Clock Control
      { 0x0042, new IoRegisterNotImplemented("CLK_PSCTRL") },
      { 0x0043, new IoRegisterNotImplemented("CLK_LOCK") },
      { 0x0044, new IoRegisterNotImplemented("CLK_RTCCTRL") },
      { 0x0045, new IoRegisterNotImplemented("CLK_USBSCTRL") },
        
      { 0x0048, new IoRegisterNotImplemented("SLEEP_CTRL") }, // Sleep Controller
        
      { 0x0050, new IoRegisterNotImplemented("OSC_CTRL") }, // Oscillator Control
      { 0x0051, new IoRegisterNotImplemented("OSC_STATUS", 0x1f) },
      { 0x0052, new IoRegisterNotImplemented("OSC_XOSCCTRL") },
      { 0x0053, new IoRegisterNotImplemented("OSC_XOSCFAIL") },
      { 0x0054, new IoRegisterNotImplemented("OSC_RC32KCAL") },
      { 0x0055, new IoRegisterNotImplemented("OSC_PLLCTRL") },
      { 0x0056, new IoRegisterNotImplemented("OSC_DFLLCTRL") },

      { 0x0060, new IoRegisterNotImplemented("DFLLRC32M_CTRL") }, // DFLL for the 32MHz Internal RC Oscillator
      { 0x0062, new IoRegisterNotImplemented("DFLLRC32M_CALA") },
      { 0x0063, new IoRegisterNotImplemented("DFLLRC32M_CALB") },
      { 0x0065, new IoRegisterNotImplemented("DFLLRC32M_COMP1") },
      { 0x0066, new IoRegisterNotImplemented("DFLLRC32M_COMP2") },
        
      { 0x0068, new IoRegisterNotImplemented("DFLLRC2M_CTRL") }, // DFLL for the 2MHz RC Oscillator
      { 0x006a, new IoRegisterNotImplemented("DFLLRC2M_CALA") },
      { 0x006b, new IoRegisterNotImplemented("DFLLRC2M_CALB") },
      { 0x006d, new IoRegisterNotImplemented("DFLLRC2M_COMP1") },
      { 0x006e, new IoRegisterNotImplemented("DFLLRC2M_COMP2") },
        
      { 0x0070, new IoRegisterNotImplemented("PR_GEN") }, // Power Reduction
      { 0x0071, new IoRegisterNotImplemented("PR_PA") },
      { 0x0072, new IoRegisterNotImplemented("PR_PB") },
      { 0x0073, new IoRegisterNotImplemented("PR_PC") },
      { 0x0074, new IoRegisterNotImplemented("PR_PD") },
      { 0x0075, new IoRegisterNotImplemented("PR_PE") },
      { 0x0076, new IoRegisterNotImplemented("PR_PF") },

      { 0x0078, new IoRegisterNotImplemented("RST_STATUS") }, // Reset Controller
      { 0x0079, new IoRegisterNotImplemented("RST_CTRL") },
        
      { 0x0080, new IoRegisterNotImplemented("WDT_CTRL") }, // Watch-Dog Timer
      { 0x0081, new IoRegisterNotImplemented("WDT_WINCTRL") },
      { 0x0082, new IoRegisterNotImplemented("WDT_STATUS") },
      { 0x0090, new IoRegisterNotImplemented("MCU_DEVID0") }, // MCU Control
      { 0x0091, new IoRegisterNotImplemented("MCU_DEVID1") },
      { 0x0092, new IoRegisterNotImplemented("MCU_DEVID2") },
      { 0x0094, new IoRegisterNotImplemented("MCU_JTAGUID") },
      { 0x0096, new IoRegisterNotImplemented("MCU_MCUCR") },
      { 0x0097, new IoRegisterNotImplemented("MCU_ANAINIT") },
      { 0x0098, new IoRegisterNotImplemented("MCU_EVSYSLOCK") },
      { 0x0099, new IoRegisterNotImplemented("MCU_AWEXLOCK") },

      { 0x00A0, new IoRegisterNotImplemented("PMIC_STATUS") }, // Programmable Multilevel Interrupt Controller
      { 0x00A1, new IoRegisterNotImplemented("PMIC_INTPRI") },
      { 0x00A2, new IoRegisterNotImplemented("PMIC_CTRL") },

      { 0x00B0, new IoRegisterNotImplemented("PORTCFG_MPCMASK") }, // Port Configuration
      { 0x00B2, new IoRegisterNotImplemented("PORTCFG_VPCTRLA") },
      { 0x00B3, new IoRegisterNotImplemented("PORTCFG_VPCTRLB") },
      { 0x00B4, new IoRegisterNotImplemented("PORTCFG_CLKEVOUT") },
      { 0x00B5, new IoRegisterNotImplemented("PORTCFG_EBIOUT") },
      { 0x00B6, new IoRegisterNotImplemented("PORTCFG_EVCTRL") },

      { 0x00C0, new IoRegisterNotImplemented("AES_CTRL") }, // AES Module
      { 0x00C1, new IoRegisterNotImplemented("AES_STATUS") },
      { 0x00C2, new IoRegisterNotImplemented("AES_STATE") },
      { 0x00C3, new IoRegisterNotImplemented("AES_KEY") },
      { 0x00C4, new IoRegisterNotImplemented("AES_INTCTRL") },

      { 0x00D0, new IoRegisterNotImplemented("CRC_CTRL") }, // CRC Module
      { 0x00D1, new IoRegisterNotImplemented("CRC_STATUS") },
      { 0x00D3, new IoRegisterNotImplemented("CRC_DATAIN") },
      { 0x00D4, new IoRegisterNotImplemented("CRC_CHECKSUM0") },
      { 0x00D5, new IoRegisterNotImplemented("CRC_CHECKSUM1") },
      { 0x00D6, new IoRegisterNotImplemented("CRC_CHECKSUM2") },
      { 0x00D7, new IoRegisterNotImplemented("CRC_CHECKSUM3") },

      { 0x0100, new IoRegisterNotImplemented("DMA_CTRL") }, // DMA Module
      { 0x0103, new IoRegisterNotImplemented("DMA_INTFLAGS") },
      { 0x0104, new IoRegisterNotImplemented("DMA_STATUS") },
      { 0x0106, new IoRegisterNotImplemented("DMA_TEMPL") },
      { 0x0107, new IoRegisterNotImplemented("DMA_TEMPH") },
      { 0x0110, new IoRegisterNotImplemented("DMA_CH0_CTRLA") },
      { 0x0111, new IoRegisterNotImplemented("DMA_CH0_CTRLB") },
      { 0x0112, new IoRegisterNotImplemented("DMA_CH0_ADDCTRL") },
      { 0x0113, new IoRegisterNotImplemented("DMA_CH0_TRIGSRC") },
      { 0x0114, new IoRegisterNotImplemented("DMA_CH0_TRFCNTL") },
      { 0x0115, new IoRegisterNotImplemented("DMA_CH0_TRFCNTH") },
      { 0x0116, new IoRegisterNotImplemented("DMA_CH0_REPCNT") },
      { 0x0118, new IoRegisterNotImplemented("DMA_CH0_SRCADDR0") },
      { 0x0119, new IoRegisterNotImplemented("DMA_CH0_SRCADDR1") },
      { 0x011a, new IoRegisterNotImplemented("DMA_CH0_SRCADDR2") },
      { 0x011c, new IoRegisterNotImplemented("DMA_CH0_DESTADDR0") },
      { 0x011d, new IoRegisterNotImplemented("DMA_CH0_DESTADDR1") },
      { 0x011e, new IoRegisterNotImplemented("DMA_CH0_DESTADDR2") },
      { 0x0120, new IoRegisterNotImplemented("DMA_CH1_CTRLA") },
      { 0x0121, new IoRegisterNotImplemented("DMA_CH1_CTRLB") },
      { 0x0122, new IoRegisterNotImplemented("DMA_CH1_ADDCTRL") },
      { 0x0123, new IoRegisterNotImplemented("DMA_CH1_TRIGSRC") },
      { 0x0124, new IoRegisterNotImplemented("DMA_CH1_TRFCNTL") },
      { 0x0125, new IoRegisterNotImplemented("DMA_CH1_TRFCNTH") },
      { 0x0126, new IoRegisterNotImplemented("DMA_CH1_REPCNT") },
      { 0x0128, new IoRegisterNotImplemented("DMA_CH1_SRCADDR0") },
      { 0x0129, new IoRegisterNotImplemented("DMA_CH1_SRCADDR1") },
      { 0x012a, new IoRegisterNotImplemented("DMA_CH1_SRCADDR2") },
      { 0x012c, new IoRegisterNotImplemented("DMA_CH1_DESTADDR0") },
      { 0x012d, new IoRegisterNotImplemented("DMA_CH1_DESTADDR1") },
      { 0x012e, new IoRegisterNotImplemented("DMA_CH1_DESTADDR2") },
      { 0x0130, new IoRegisterNotImplemented("DMA_CH2_CTRLA") },
      { 0x0131, new IoRegisterNotImplemented("DMA_CH2_CTRLB") },
      { 0x0132, new IoRegisterNotImplemented("DMA_CH2_ADDCTRL") },
      { 0x0133, new IoRegisterNotImplemented("DMA_CH2_TRIGSRC") },
      { 0x0134, new IoRegisterNotImplemented("DMA_CH2_TRFCNTL") },
      { 0x0135, new IoRegisterNotImplemented("DMA_CH2_TRFCNTH") },
      { 0x0136, new IoRegisterNotImplemented("DMA_CH2_REPCNT") },
      { 0x0138, new IoRegisterNotImplemented("DMA_CH2_SRCADDR0") },
      { 0x0139, new IoRegisterNotImplemented("DMA_CH2_SRCADDR1") },
      { 0x013a, new IoRegisterNotImplemented("DMA_CH2_SRCADDR2") },
      { 0x013c, new IoRegisterNotImplemented("DMA_CH2_DESTADDR0") },
      { 0x013d, new IoRegisterNotImplemented("DMA_CH2_DESTADDR1") },
      { 0x013e, new IoRegisterNotImplemented("DMA_CH2_DESTADDR2") },
      { 0x0140, new IoRegisterNotImplemented("DMA_CH3_CTRLA") },
      { 0x0141, new IoRegisterNotImplemented("DMA_CH3_CTRLB") },
      { 0x0142, new IoRegisterNotImplemented("DMA_CH3_ADDCTRL") },
      { 0x0143, new IoRegisterNotImplemented("DMA_CH3_TRIGSRC") },
      { 0x0144, new IoRegisterNotImplemented("DMA_CH3_TRFCNTL") },
      { 0x0145, new IoRegisterNotImplemented("DMA_CH3_TRFCNTH") },
      { 0x0146, new IoRegisterNotImplemented("DMA_CH3_REPCNT") },
      { 0x0148, new IoRegisterNotImplemented("DMA_CH3_SRCADDR0") },
      { 0x0149, new IoRegisterNotImplemented("DMA_CH3_SRCADDR1") },
      { 0x014a, new IoRegisterNotImplemented("DMA_CH3_SRCADDR2") },
      { 0x014c, new IoRegisterNotImplemented("DMA_CH3_DESTADDR0") },
      { 0x014d, new IoRegisterNotImplemented("DMA_CH3_DESTADDR1") },
      { 0x014e, new IoRegisterNotImplemented("DMA_CH3_DESTADDR2") },

      { 0x0180, new IoRegisterNotImplemented("EVSYS_CH0MUX") }, // Event System
      { 0x0181, new IoRegisterNotImplemented("EVSYS_CH1MUX") },
      { 0x0182, new IoRegisterNotImplemented("EVSYS_CH2MUX") },
      { 0x0183, new IoRegisterNotImplemented("EVSYS_CH3MUX") },
      { 0x0184, new IoRegisterNotImplemented("EVSYS_CH4MUX") },
      { 0x0185, new IoRegisterNotImplemented("EVSYS_CH5MUX") },
      { 0x0186, new IoRegisterNotImplemented("EVSYS_CH6MUX") },
      { 0x0187, new IoRegisterNotImplemented("EVSYS_CH7MUX") },
      { 0x0188, new IoRegisterNotImplemented("EVSYS_CH0CTRL") },
      { 0x0189, new IoRegisterNotImplemented("EVSYS_CH1CTRL") },
      { 0x018a, new IoRegisterNotImplemented("EVSYS_CH2CTRL") },
      { 0x018b, new IoRegisterNotImplemented("EVSYS_CH3CTRL") },
      { 0x018c, new IoRegisterNotImplemented("EVSYS_CH4CTRL") },
      { 0x018d, new IoRegisterNotImplemented("EVSYS_CH5CTRL") },
      { 0x018e, new IoRegisterNotImplemented("EVSYS_CH6CTRL") },
      { 0x018f, new IoRegisterNotImplemented("EVSYS_CH7CTRL") },
      { 0x0190, new IoRegisterNotImplemented("EVSYS_STROBE") },
      { 0x0191, new IoRegisterNotImplemented("EVSYS_DATA") },

      { 0x01C0, new IoRegisterNotImplemented("NVM_ADDR0") }, // Non Volatile Memory (NVM) Controller
      { 0x01C1, new IoRegisterNotImplemented("NVM_ADDR1") },
      { 0x01C2, new IoRegisterNotImplemented("NVM_ADDR2") },
      { 0x01C4, new IoRegisterNotImplemented("NVM_DATA0") },
      { 0x01C5, new IoRegisterNotImplemented("NVM_DATA1") },
      { 0x01C6, new IoRegisterNotImplemented("NVM_DATA2") },
      { 0x01Ca, new IoRegisterNotImplemented("NVM_CMD") },
      { 0x01Cb, new IoRegisterNotImplemented("NVM_CTRLA") },
      { 0x01Cc, new IoRegisterNotImplemented("NVM_CTRLB") },
      { 0x01Cd, new IoRegisterNotImplemented("NVM_INTCTRL") },
      { 0x01Cf, new IoRegisterNotImplemented("NVM_STATUS") },
      { 0x01D0, new IoRegisterNotImplemented("NVM_LOCKBITS") },

      { 0x0200, new IoRegisterNotImplemented("ADCA_CTRLA") }, // Analog to Digital Converter on port A
      { 0x0201, new IoRegisterNotImplemented("ADCA_CTRLB") },
      { 0x0202, new IoRegisterNotImplemented("ADCA_REFCTRL") },
      { 0x0203, new IoRegisterNotImplemented("ADCA_EVCTRL") },
      { 0x0204, new IoRegisterNotImplemented("ADCA_PRESCALER") },
      { 0x0205, new IoRegisterNotImplemented("ADCA_INTFLAGS") },
      { 0x0206, new IoRegisterNotImplemented("ADCA_TEMP") },
      { 0x020c, new IoRegisterNotImplemented("ADCA_CALL") },
      { 0x020d, new IoRegisterNotImplemented("ADCA_CALH") },
      { 0x0210, new IoRegisterNotImplemented("ADCA_CH0RESL") },
      { 0x0211, new IoRegisterNotImplemented("ADCA_CH0RESH") },
      { 0x0212, new IoRegisterNotImplemented("ADCA_CH1RESL") },
      { 0x0213, new IoRegisterNotImplemented("ADCA_CH1RESH") },
      { 0x0214, new IoRegisterNotImplemented("ADCA_CH2RESL") },
      { 0x0215, new IoRegisterNotImplemented("ADCA_CH2RESH") },
      { 0x0216, new IoRegisterNotImplemented("ADCA_CH3RESL") },
      { 0x0217, new IoRegisterNotImplemented("ADCA_CH3RESH") },
      { 0x0218, new IoRegisterNotImplemented("ADCA_CMPL") },
      { 0x0219, new IoRegisterNotImplemented("ADCA_CMPH") },
      { 0x0220, new IoRegisterNotImplemented("ADCA_CH0_CTRL") },
      { 0x0221, new IoRegisterNotImplemented("ADCA_CH0_MUXCTRL") },
      { 0x0222, new IoRegisterNotImplemented("ADCA_CH0_INTCTRL") },
      { 0x0223, new IoRegisterNotImplemented("ADCA_CH0_INTFLAGS") },
      { 0x0224, new IoRegisterNotImplemented("ADCA_CH0_RESL") },
      { 0x0225, new IoRegisterNotImplemented("ADCA_CH0_RESH") },
      { 0x0226, new IoRegisterNotImplemented("ADCA_CH0_SCAN") },
      { 0x0228, new IoRegisterNotImplemented("ADCA_CH1_CTRL") },
      { 0x0229, new IoRegisterNotImplemented("ADCA_CH1_MUXCTRL") },
      { 0x022a, new IoRegisterNotImplemented("ADCA_CH1_INTCTRL") },
      { 0x022b, new IoRegisterNotImplemented("ADCA_CH1_INTFLAGS") },
      { 0x022c, new IoRegisterNotImplemented("ADCA_CH1_RESL") },
      { 0x022d, new IoRegisterNotImplemented("ADCA_CH1_RESH") },
      { 0x022e, new IoRegisterNotImplemented("ADCA_CH1_SCAN") },
      { 0x0230, new IoRegisterNotImplemented("ADCA_CH2_CTRL") },
      { 0x0231, new IoRegisterNotImplemented("ADCA_CH2_MUXCTRL") },
      { 0x0232, new IoRegisterNotImplemented("ADCA_CH2_INTCTRL") },
      { 0x0233, new IoRegisterNotImplemented("ADCA_CH2_INTFLAGS") },
      { 0x0234, new IoRegisterNotImplemented("ADCA_CH2_RESL") },
      { 0x0235, new IoRegisterNotImplemented("ADCA_CH2_RESH") },
      { 0x0236, new IoRegisterNotImplemented("ADCA_CH2_SCAN") },
      { 0x0238, new IoRegisterNotImplemented("ADCA_CH3_CTRL") },
      { 0x0239, new IoRegisterNotImplemented("ADCA_CH3_MUXCTRL") },
      { 0x023a, new IoRegisterNotImplemented("ADCA_CH3_INTCTRL") },
      { 0x023b, new IoRegisterNotImplemented("ADCA_CH3_INTFLAGS") },
      { 0x023c, new IoRegisterNotImplemented("ADCA_CH3_RESL") },
      { 0x023d, new IoRegisterNotImplemented("ADCA_CH3_RESH") },
      { 0x023e, new IoRegisterNotImplemented("ADCA_CH3_SCAN") },
        
      { 0x0380, new IoRegisterNotImplemented("ACA_AC0CTRL") }, // Analog Comparator pair on port A
      { 0x0381, new IoRegisterNotImplemented("ACA_AC1CTRL") },
      { 0x0382, new IoRegisterNotImplemented("ACA_AC0MUXCTRL") },
      { 0x0383, new IoRegisterNotImplemented("ACA_AC1MUXCTRL") },
      { 0x0384, new IoRegisterNotImplemented("ACA_CTRLA") },
      { 0x0385, new IoRegisterNotImplemented("ACA_CTRLB") },
      { 0x0386, new IoRegisterNotImplemented("ACA_WINCTRL") },
      { 0x0387, new IoRegisterNotImplemented("ACA_STATUS") },
      { 0x0388, new IoRegisterNotImplemented("ACA_CURRCTRL") },
      { 0x0389, new IoRegisterNotImplemented("ACA_CURRCALIB") },

      { 0x0400, new IoRegisterNotImplemented("RTC_CTRL") }, // Real Time Counter
      { 0x0401, new IoRegisterNotImplemented("RTC_STATUS") },
      { 0x0402, new IoRegisterNotImplemented("RTC_INTCTRL") },
      { 0x0403, new IoRegisterNotImplemented("RTC_INTFLAGS") },
      { 0x0404, new IoRegisterNotImplemented("RTC_TEMP") },
      { 0x0408, new IoRegisterNotImplemented("RTC_CNTL") },
      { 0x0409, new IoRegisterNotImplemented("RTC_CNTH") },
      { 0x040a, new IoRegisterNotImplemented("RTC_PERL") },
      { 0x040b, new IoRegisterNotImplemented("RTC_PERH") },
      { 0x040c, new IoRegisterNotImplemented("RTC_COMPL") },
      { 0x040d, new IoRegisterNotImplemented("RTC_COMPH") },

      { 0x0480, new IoRegisterNotImplemented("TWIC_CTRL") }, // Two Wire Interface on port C
      { 0x0481, new IoRegisterNotImplemented("TWIC_MASTER_CTRLA") },
      { 0x0482, new IoRegisterNotImplemented("TWIC_MASTER_CTRLB") },
      { 0x0483, new IoRegisterNotImplemented("TWIC_MASTER_CTRLC") },
      { 0x0484, new IoRegisterNotImplemented("TWIC_MASTER_STATUS") },
      { 0x0485, new IoRegisterNotImplemented("TWIC_MASTER_BAUD") },
      { 0x0486, new IoRegisterNotImplemented("TWIC_MASTER_ADDR") },
      { 0x0487, new IoRegisterNotImplemented("TWIC_MASTER_DATA") },
      { 0x0488, new IoRegisterNotImplemented("TWIC_SLAVE_CTRLA") },
      { 0x0489, new IoRegisterNotImplemented("TWIC_SLAVE_CTRLB") },
      { 0x048a, new IoRegisterNotImplemented("TWIC_SLAVE_STATUS") },
      { 0x048b, new IoRegisterNotImplemented("TWIC_SLAVE_ADDR") },
      { 0x048c, new IoRegisterNotImplemented("TWIC_SLAVE_DATA") },
      { 0x048d, new IoRegisterNotImplemented("TWIC_SLAVE_ADDRMASK") },

      { 0x04a0, new IoRegisterNotImplemented("TWIE_CTRL") }, // Two Wire Interface on port E
      { 0x04a1, new IoRegisterNotImplemented("TWIE_MASTER_CTRLA") },
      { 0x04a2, new IoRegisterNotImplemented("TWIE_MASTER_CTRLB") },
      { 0x04a3, new IoRegisterNotImplemented("TWIE_MASTER_CTRLC") },
      { 0x04a4, new IoRegisterNotImplemented("TWIE_MASTER_STATUS") },
      { 0x04a5, new IoRegisterNotImplemented("TWIE_MASTER_BAUD") },
      { 0x04a6, new IoRegisterNotImplemented("TWIE_MASTER_ADDR") },
      { 0x04a7, new IoRegisterNotImplemented("TWIE_MASTER_DATA") },
      { 0x04a8, new IoRegisterNotImplemented("TWIE_SLAVE_CTRLA") },
      { 0x04a9, new IoRegisterNotImplemented("TWIE_SLAVE_CTRLB") },
      { 0x04aa, new IoRegisterNotImplemented("TWIE_SLAVE_STATUS") },
      { 0x04ab, new IoRegisterNotImplemented("TWIE_SLAVE_ADDR") },
      { 0x04ac, new IoRegisterNotImplemented("TWIE_SLAVE_DATA") },
      { 0x04ad, new IoRegisterNotImplemented("TWIE_SLAVE_ADDRMASK") },

      { 0x04C0, new IoRegisterNotImplemented("USB_FRAMENUML") }, // Universal Serial Bus Interface
      { 0x04C1, new IoRegisterNotImplemented("USB_FRAMENUMH") },

      { 0x0600, new IoRegisterNotImplemented("PORTA_DIR") }, // Port A
      { 0x0601, new IoRegisterNotImplemented("PORTA_DIRSET") },
      { 0x0602, new IoRegisterNotImplemented("PORTA_DIRCLR") },
      { 0x0603, new IoRegisterNotImplemented("PORTA_DIRTGL") },
      { 0x0604, new IoRegisterNotImplemented("PORTA_OUT") },
      { 0x0605, new IoRegisterNotImplemented("PORTA_OUTSET") },
      { 0x0606, new IoRegisterNotImplemented("PORTA_OUTCLR") },
      { 0x0607, new IoRegisterNotImplemented("PORTA_OUTTGL") },
      { 0x0608, new IoRegisterNotImplemented("PORTA_IN") },
      { 0x0609, new IoRegisterNotImplemented("PORTA_INTCTRL") },
      { 0x060a, new IoRegisterNotImplemented("PORTA_INT0MASK") },
      { 0x060b, new IoRegisterNotImplemented("PORTA_INT1MASK") },
      { 0x060c, new IoRegisterNotImplemented("PORTA_INTFLAGS") },
      { 0x060e, new IoRegisterNotImplemented("PORTA_REMAP") },
      { 0x0610, new IoRegisterNotImplemented("PORTA_PIN0CTRL") },
      { 0x0611, new IoRegisterNotImplemented("PORTA_PIN1CTRL") },
      { 0x0612, new IoRegisterNotImplemented("PORTA_PIN2CTRL") },
      { 0x0613, new IoRegisterNotImplemented("PORTA_PIN3CTRL") },
      { 0x0614, new IoRegisterNotImplemented("PORTA_PIN4CTRL") },
      { 0x0615, new IoRegisterNotImplemented("PORTA_PIN5CTRL") },
      { 0x0616, new IoRegisterNotImplemented("PORTA_PIN6CTRL") },
      { 0x0617, new IoRegisterNotImplemented("PORTA_PIN7CTRL") },

      { 0x0620, new IoRegisterNotImplemented("PORTB_DIR") }, // Port B
      { 0x0621, new IoRegisterNotImplemented("PORTB_DIRSET") },
      { 0x0622, new IoRegisterNotImplemented("PORTB_DIRCLR") },
      { 0x0623, new IoRegisterNotImplemented("PORTB_DIRTGL") },
      { 0x0624, new IoRegisterNotImplemented("PORTB_OUT") },
      { 0x0625, new IoRegisterNotImplemented("PORTB_OUTSET") },
      { 0x0626, new IoRegisterNotImplemented("PORTB_OUTCLR") },
      { 0x0627, new IoRegisterNotImplemented("PORTB_OUTTGL") },
      { 0x0628, new IoRegisterNotImplemented("PORTB_IN") },
      { 0x0629, new IoRegisterNotImplemented("PORTB_INTCTRL") },
      { 0x062a, new IoRegisterNotImplemented("PORTB_INT0MASK") },
      { 0x062b, new IoRegisterNotImplemented("PORTB_INT1MASK") },
      { 0x062c, new IoRegisterNotImplemented("PORTB_INTFLAGS") },
      { 0x062e, new IoRegisterNotImplemented("PORTB_REMAP") },
      { 0x0630, new IoRegisterNotImplemented("PORTB_PIN0CTRL") },
      { 0x0631, new IoRegisterNotImplemented("PORTB_PIN1CTRL") },
      { 0x0632, new IoRegisterNotImplemented("PORTB_PIN2CTRL") },
      { 0x0633, new IoRegisterNotImplemented("PORTB_PIN3CTRL") },
      { 0x0634, new IoRegisterNotImplemented("PORTB_PIN4CTRL") },
      { 0x0635, new IoRegisterNotImplemented("PORTB_PIN5CTRL") },
      { 0x0636, new IoRegisterNotImplemented("PORTB_PIN6CTRL") },
      { 0x0637, new IoRegisterNotImplemented("PORTB_PIN7CTRL") },

      { 0x0640, new IoRegisterNotImplemented("PORTC_DIR") }, // Port C
      { 0x0641, new IoRegisterNotImplemented("PORTC_DIRSET") },
      { 0x0642, new IoRegisterNotImplemented("PORTC_DIRCLR") },
      { 0x0643, new IoRegisterNotImplemented("PORTC_DIRTGL") },
      { 0x0644, new IoRegisterNotImplemented("PORTC_OUT") },
      { 0x0645, new IoRegisterNotImplemented("PORTC_OUTSET") },
      { 0x0646, new IoRegisterNotImplemented("PORTC_OUTCLR") },
      { 0x0647, new IoRegisterNotImplemented("PORTC_OUTTGL") },
      { 0x0648, new IoRegisterNotImplemented("PORTC_IN") },
      { 0x0649, new IoRegisterNotImplemented("PORTC_INTCTRL") },
      { 0x064a, new IoRegisterNotImplemented("PORTC_INT0MASK") },
      { 0x064b, new IoRegisterNotImplemented("PORTC_INT1MASK") },
      { 0x064c, new IoRegisterNotImplemented("PORTC_INTFLAGS") },
      { 0x064e, new IoRegisterNotImplemented("PORTC_REMAP") },
      { 0x0650, new IoRegisterNotImplemented("PORTC_PIN0CTRL") },
      { 0x0651, new IoRegisterNotImplemented("PORTC_PIN1CTRL") },
      { 0x0652, new IoRegisterNotImplemented("PORTC_PIN2CTRL") },
      { 0x0653, new IoRegisterNotImplemented("PORTC_PIN3CTRL") },
      { 0x0654, new IoRegisterNotImplemented("PORTC_PIN4CTRL") },
      { 0x0655, new IoRegisterNotImplemented("PORTC_PIN5CTRL") },
      { 0x0656, new IoRegisterNotImplemented("PORTC_PIN6CTRL") },
      { 0x0657, new IoRegisterNotImplemented("PORTC_PIN7CTRL") },

      { 0x0660, new IoRegisterNotImplemented("PORTD_DIR") }, // Port D
      { 0x0661, new IoRegisterNotImplemented("PORTD_DIRSET") },
      { 0x0662, new IoRegisterNotImplemented("PORTD_DIRCLR") },
      { 0x0663, new IoRegisterNotImplemented("PORTD_DIRTGL") },
      { 0x0664, new IoRegisterNotImplemented("PORTD_OUT") },
      { 0x0665, new IoRegisterNotImplemented("PORTD_OUTSET") },
      { 0x0666, new IoRegisterNotImplemented("PORTD_OUTCLR") },
      { 0x0667, new IoRegisterNotImplemented("PORTD_OUTTGL") },
      { 0x0668, new IoRegisterNotImplemented("PORTD_IN") },
      { 0x0669, new IoRegisterNotImplemented("PORTD_INTCTRL") },
      { 0x066a, new IoRegisterNotImplemented("PORTD_INT0MASK") },
      { 0x066b, new IoRegisterNotImplemented("PORTD_INT1MASK") },
      { 0x066c, new IoRegisterNotImplemented("PORTD_INTFLAGS") },
      { 0x066e, new IoRegisterNotImplemented("PORTD_REMAP") },
      { 0x0670, new IoRegisterNotImplemented("PORTD_PIN0CTRL") },
      { 0x0671, new IoRegisterNotImplemented("PORTD_PIN1CTRL") },
      { 0x0672, new IoRegisterNotImplemented("PORTD_PIN2CTRL") },
      { 0x0673, new IoRegisterNotImplemented("PORTD_PIN3CTRL") },
      { 0x0674, new IoRegisterNotImplemented("PORTD_PIN4CTRL") },
      { 0x0675, new IoRegisterNotImplemented("PORTD_PIN5CTRL") },
      { 0x0676, new IoRegisterNotImplemented("PORTD_PIN6CTRL") },
      { 0x0677, new IoRegisterNotImplemented("PORTD_PIN7CTRL") },

      { 0x0680, new IoRegisterNotImplemented("PORTE_DIR") }, // Port E
      { 0x0681, new IoRegisterNotImplemented("PORTE_DIRSET") },
      { 0x0682, new IoRegisterNotImplemented("PORTE_DIRCLR") },
      { 0x0683, new IoRegisterNotImplemented("PORTE_DIRTGL") },
      { 0x0684, new IoRegisterNotImplemented("PORTE_OUT") },
      { 0x0685, new IoRegisterNotImplemented("PORTE_OUTSET") },
      { 0x0686, new IoRegisterNotImplemented("PORTE_OUTCLR") },
      { 0x0687, new IoRegisterNotImplemented("PORTE_OUTTGL") },
      { 0x0688, new IoRegisterNotImplemented("PORTE_IN") },
      { 0x0689, new IoRegisterNotImplemented("PORTE_INTCTRL") },
      { 0x068a, new IoRegisterNotImplemented("PORTE_INT0MASK") },
      { 0x068b, new IoRegisterNotImplemented("PORTE_INT1MASK") },
      { 0x068c, new IoRegisterNotImplemented("PORTE_INTFLAGS") },
      { 0x068e, new IoRegisterNotImplemented("PORTE_REMAP") },
      { 0x0690, new IoRegisterNotImplemented("PORTE_PIN0CTRL") },
      { 0x0691, new IoRegisterNotImplemented("PORTE_PIN1CTRL") },
      { 0x0692, new IoRegisterNotImplemented("PORTE_PIN2CTRL") },
      { 0x0693, new IoRegisterNotImplemented("PORTE_PIN3CTRL") },
      { 0x0694, new IoRegisterNotImplemented("PORTE_PIN4CTRL") },
      { 0x0695, new IoRegisterNotImplemented("PORTE_PIN5CTRL") },
      { 0x0696, new IoRegisterNotImplemented("PORTE_PIN6CTRL") },
      { 0x0697, new IoRegisterNotImplemented("PORTE_PIN7CTRL") },

      { 0x07E0, new IoRegisterNotImplemented("PORTR_DIR") }, // Port R
      { 0x07E1, new IoRegisterNotImplemented("PORTR_DIRSET") },
      { 0x07E2, new IoRegisterNotImplemented("PORTR_DIRCLR") },
      { 0x07E3, new IoRegisterNotImplemented("PORTR_DIRTGL") },
      { 0x07E4, new IoRegisterNotImplemented("PORTR_OUT") },
      { 0x07E5, new IoRegisterNotImplemented("PORTR_OUTSET") },
      { 0x07E6, new IoRegisterNotImplemented("PORTR_OUTCLR") },
      { 0x07E7, new IoRegisterNotImplemented("PORTR_OUTTGL") },
      { 0x07E8, new IoRegisterNotImplemented("PORTR_IN") },
      { 0x07E9, new IoRegisterNotImplemented("PORTR_INTCTRL") },
      { 0x07Ea, new IoRegisterNotImplemented("PORTR_INT0MASK") },
      { 0x07Eb, new IoRegisterNotImplemented("PORTR_INT1MASK") },
      { 0x07Ec, new IoRegisterNotImplemented("PORTR_INTFLAGS") },
      { 0x07Ee, new IoRegisterNotImplemented("PORTR_REMAP") },
      { 0x07F0, new IoRegisterNotImplemented("PORTR_PIN0CTRL") },
      { 0x07F1, new IoRegisterNotImplemented("PORTR_PIN1CTRL") },
      { 0x07F2, new IoRegisterNotImplemented("PORTR_PIN2CTRL") },
      { 0x07F3, new IoRegisterNotImplemented("PORTR_PIN3CTRL") },
      { 0x07F4, new IoRegisterNotImplemented("PORTR_PIN4CTRL") },
      { 0x07F5, new IoRegisterNotImplemented("PORTR_PIN5CTRL") },
      { 0x07F6, new IoRegisterNotImplemented("PORTR_PIN6CTRL") },
      { 0x07F7, new IoRegisterNotImplemented("PORTR_PIN7CTRL") },

      { 0x0800, new IoRegisterNotImplemented("TCC0_CTRLA") }, // Timer/Counter 0 on port C
      { 0x0801, new IoRegisterNotImplemented("TCC0_CTRLB") },
      { 0x0802, new IoRegisterNotImplemented("TCC0_CTRLC") },
      { 0x0803, new IoRegisterNotImplemented("TCC0_CTRLD") },
      { 0x0804, new IoRegisterNotImplemented("TCC0_CTRLE") },
      { 0x0806, new IoRegisterNotImplemented("TCC0_INTCTRLA") },
      { 0x0807, new IoRegisterNotImplemented("TCC0_INTCTRLB") },
      { 0x0808, new IoRegisterNotImplemented("TCC0_CTRLFCLR") },
      { 0x0809, new IoRegisterNotImplemented("TCC0_CTRLFSET") },
      { 0x080a, new IoRegisterNotImplemented("TCC0_CTRLGCLR") },
      { 0x080b, new IoRegisterNotImplemented("TCC0_CTRLGSET") },
      { 0x080c, new IoRegisterNotImplemented("TCC0_INTFLAGS") },
      { 0x080f, new IoRegisterNotImplemented("TCC0_TEMP") },
      { 0x0820, new IoRegisterNotImplemented("TCC0_CNTL") },
      { 0x0821, new IoRegisterNotImplemented("TCC0_CNTH") },
      { 0x0826, new IoRegisterNotImplemented("TCC0_PERL") },
      { 0x0827, new IoRegisterNotImplemented("TCC0_PERH") },
      { 0x0828, new IoRegisterNotImplemented("TCC0_CCAL") },
      { 0x0829, new IoRegisterNotImplemented("TCC0_CCAH") },
      { 0x082a, new IoRegisterNotImplemented("TCC0_CCBL") },
      { 0x082b, new IoRegisterNotImplemented("TCC0_CCBH") },
      { 0x082c, new IoRegisterNotImplemented("TCC0_CCCL") },
      { 0x082d, new IoRegisterNotImplemented("TCC0_CCCH") },
      { 0x082e, new IoRegisterNotImplemented("TCC0_CCDL") },
      { 0x082f, new IoRegisterNotImplemented("TCC0_CCDH") },
      { 0x0836, new IoRegisterNotImplemented("TCC0_PERBUFL") },
      { 0x0837, new IoRegisterNotImplemented("TCC0_PERBUFH") },
      { 0x0838, new IoRegisterNotImplemented("TCC0_CCABUFL") },
      { 0x0839, new IoRegisterNotImplemented("TCC0_CCABUFH") },
      { 0x083a, new IoRegisterNotImplemented("TCC0_CCBBUFL") },
      { 0x083b, new IoRegisterNotImplemented("TCC0_CCBBUFH") },
      { 0x083c, new IoRegisterNotImplemented("TCC0_CCCBUFL") },
      { 0x083d, new IoRegisterNotImplemented("TCC0_CCCBUFH") },
      { 0x083e, new IoRegisterNotImplemented("TCC0_CCDBUFL") },
      { 0x083f, new IoRegisterNotImplemented("TCC0_CCDBUFH") },

      { 0x0840, new IoRegisterNotImplemented("TCC1_CTRLA") }, // Timer/Counter 1 on port C
      { 0x0841, new IoRegisterNotImplemented("TCC1_CTRLB") },
      { 0x0842, new IoRegisterNotImplemented("TCC1_CTRLC") },
      { 0x0843, new IoRegisterNotImplemented("TCC1_CTRLD") },
      { 0x0844, new IoRegisterNotImplemented("TCC1_CTRLE") },
      { 0x0846, new IoRegisterNotImplemented("TCC1_INTCTRLA") },
      { 0x0847, new IoRegisterNotImplemented("TCC1_INTCTRLB") },
      { 0x0848, new IoRegisterNotImplemented("TCC1_CTRLFCLR") },
      { 0x0849, new IoRegisterNotImplemented("TCC1_CTRLFSET") },
      { 0x084a, new IoRegisterNotImplemented("TCC1_CTRLGCLR") },
      { 0x084b, new IoRegisterNotImplemented("TCC1_CTRLGSET") },
      { 0x084c, new IoRegisterNotImplemented("TCC1_INTFLAGS") },
      { 0x084f, new IoRegisterNotImplemented("TCC1_TEMP") },
      { 0x0860, new IoRegisterNotImplemented("TCC1_CNTL") },
      { 0x0861, new IoRegisterNotImplemented("TCC1_CNTH") },
      { 0x0866, new IoRegisterNotImplemented("TCC1_PERL") },
      { 0x0867, new IoRegisterNotImplemented("TCC1_PERH") },
      { 0x0868, new IoRegisterNotImplemented("TCC1_CCAL") },
      { 0x0869, new IoRegisterNotImplemented("TCC1_CCAH") },
      { 0x086a, new IoRegisterNotImplemented("TCC1_CCBL") },
      { 0x086b, new IoRegisterNotImplemented("TCC1_CCBH") },
      { 0x086c, new IoRegisterNotImplemented("TCC1_CCCL") },
      { 0x086d, new IoRegisterNotImplemented("TCC1_CCCH") },
      { 0x086e, new IoRegisterNotImplemented("TCC1_CCDL") },
      { 0x086f, new IoRegisterNotImplemented("TCC1_CCDH") },
      { 0x0876, new IoRegisterNotImplemented("TCC1_PERBUFL") },
      { 0x0877, new IoRegisterNotImplemented("TCC1_PERBUFH") },
      { 0x0878, new IoRegisterNotImplemented("TCC1_CCABUFL") },
      { 0x0879, new IoRegisterNotImplemented("TCC1_CCABUFH") },
      { 0x087a, new IoRegisterNotImplemented("TCC1_CCBBUFL") },
      { 0x087b, new IoRegisterNotImplemented("TCC1_CCBBUFH") },
      { 0x087c, new IoRegisterNotImplemented("TCC1_CCCBUFL") },
      { 0x087d, new IoRegisterNotImplemented("TCC1_CCCBUFH") },
      { 0x087e, new IoRegisterNotImplemented("TCC1_CCDBUFL") },
      { 0x087f, new IoRegisterNotImplemented("TCC1_CCDBUFH") },

      { 0x0880, new IoRegisterNotImplemented("AWEXC_CTRL") }, // Advanced Waveform Extension on port C
      { 0x0882, new IoRegisterNotImplemented("AWEXC_FDEMASK") },
      { 0x0883, new IoRegisterNotImplemented("AWEXC_FDCTRL") },
      { 0x0884, new IoRegisterNotImplemented("AWEXC_STATUS") },
      { 0x0886, new IoRegisterNotImplemented("AWEXC_DTBOTH") },
      { 0x0887, new IoRegisterNotImplemented("AWEXC_DTBOTHBUF") },
      { 0x0888, new IoRegisterNotImplemented("AWEXC_DTLS") },
      { 0x0889, new IoRegisterNotImplemented("AWEXC_DTHS") },
      { 0x088a, new IoRegisterNotImplemented("AWEXC_DTLSBUF") },
      { 0x088b, new IoRegisterNotImplemented("AWEXC_DTHSBUF") },
      { 0x088c, new IoRegisterNotImplemented("AWEXC_OUTOVEN") },

      { 0x0890, new IoRegisterNotImplemented("HIRESC_CTRLA") }, // High Resolution Extension on port C

      { 0x08A0, new IoXmegaUsart::Data     (_usartC0) }, // USART 0 on port C
      { 0x08A1, new IoXmegaUsart::Status   (_usartC0) },
      { 0x08A3, new IoXmegaUsart::CtrlA    (_usartC0) },
      { 0x08A4, new IoXmegaUsart::CtrlB    (_usartC0) },
      { 0x08A5, new IoXmegaUsart::CtrlC    (_usartC0) },
      { 0x08A6, new IoXmegaUsart::BaudCtrlA(_usartC0) },
      { 0x08A7, new IoXmegaUsart::BaudCtrlB(_usartC0) },

      { 0x08B0, new IoXmegaUsart::Data     (_usartC1) }, // USART 1 on port C
      { 0x08B1, new IoXmegaUsart::Status   (_usartC1) },                     
      { 0x08B3, new IoXmegaUsart::CtrlA    (_usartC1) },                     
      { 0x08B4, new IoXmegaUsart::CtrlB    (_usartC1) },                     
      { 0x08B5, new IoXmegaUsart::CtrlC    (_usartC1) },                     
      { 0x08B6, new IoXmegaUsart::BaudCtrlA(_usartC1) },                     
      { 0x08B7, new IoXmegaUsart::BaudCtrlB(_usartC1) },                     

      { 0x08C0, new IoRegisterNotImplemented("SPIC_CTRL") }, // Serial Peripheral Interface on port C
      { 0x08C1, new IoRegisterNotImplemented("SPIC_INTCTRL") },
      { 0x08C2, new IoRegisterNotImplemented("SPIC_STATUS") },
      { 0x08C3, new IoRegisterNotImplemented("SPIC_DATA") },

      { 0x08F8, new IoRegisterNotImplemented("IRCOM_CTRL") }, // Infrared Communication Module
      { 0x08F9, new IoRegisterNotImplemented("IRCOM_TXPLCTRL") },
      { 0x08Fa, new IoRegisterNotImplemented("IRCOM_RXPLCTRL") },

      { 0x0900, new IoRegisterNotImplemented("TCD0_CTRLA") }, // Timer/Counter 0 on port D
      { 0x0901, new IoRegisterNotImplemented("TCD0_CTRLB") },
      { 0x0902, new IoRegisterNotImplemented("TCD0_CTRLC") },
      { 0x0903, new IoRegisterNotImplemented("TCD0_CTRLD") },
      { 0x0904, new IoRegisterNotImplemented("TCD0_CTRLE") },
      { 0x0906, new IoRegisterNotImplemented("TCD0_INTCTRLA") },
      { 0x0907, new IoRegisterNotImplemented("TCD0_INTCTRLB") },
      { 0x0908, new IoRegisterNotImplemented("TCD0_CTRLFCLR") },
      { 0x0909, new IoRegisterNotImplemented("TCD0_CTRLFSET") },
      { 0x090a, new IoRegisterNotImplemented("TCD0_CTRLGCLR") },
      { 0x090b, new IoRegisterNotImplemented("TCD0_CTRLGSET") },
      { 0x090c, new IoRegisterNotImplemented("TCD0_INTFLAGS") },
      { 0x090f, new IoRegisterNotImplemented("TCD0_TEMP") },
      { 0x0920, new IoRegisterNotImplemented("TCD0_CNTL") },
      { 0x0921, new IoRegisterNotImplemented("TCD0_CNTH") },
      { 0x0926, new IoRegisterNotImplemented("TCD0_PERL") },
      { 0x0927, new IoRegisterNotImplemented("TCD0_PERH") },
      { 0x0928, new IoRegisterNotImplemented("TCD0_CCAL") },
      { 0x0929, new IoRegisterNotImplemented("TCD0_CCAH") },
      { 0x092a, new IoRegisterNotImplemented("TCD0_CCBL") },
      { 0x092b, new IoRegisterNotImplemented("TCD0_CCBH") },
      { 0x092c, new IoRegisterNotImplemented("TCD0_CCCL") },
      { 0x092d, new IoRegisterNotImplemented("TCD0_CCCH") },
      { 0x092e, new IoRegisterNotImplemented("TCD0_CCDL") },
      { 0x092f, new IoRegisterNotImplemented("TCD0_CCDH") },
      { 0x0936, new IoRegisterNotImplemented("TCD0_PERBUFL") },
      { 0x0937, new IoRegisterNotImplemented("TCD0_PERBUFH") },
      { 0x0938, new IoRegisterNotImplemented("TCD0_CCABUFL") },
      { 0x0939, new IoRegisterNotImplemented("TCD0_CCABUFH") },
      { 0x093a, new IoRegisterNotImplemented("TCD0_CCBBUFL") },
      { 0x093b, new IoRegisterNotImplemented("TCD0_CCBBUFH") },
      { 0x093c, new IoRegisterNotImplemented("TCD0_CCCBUFL") },
      { 0x093d, new IoRegisterNotImplemented("TCD0_CCCBUFH") },
      { 0x093e, new IoRegisterNotImplemented("TCD0_CCDBUFL") },
      { 0x093f, new IoRegisterNotImplemented("TCD0_CCDBUFH") },

      { 0x0940, new IoRegisterNotImplemented("TCD1_CTRLA") }, // Timer/Counter 1 on port D
      { 0x0941, new IoRegisterNotImplemented("TCD1_CTRLB") },
      { 0x0942, new IoRegisterNotImplemented("TCD1_CTRLC") },
      { 0x0943, new IoRegisterNotImplemented("TCD1_CTRLD") },
      { 0x0944, new IoRegisterNotImplemented("TCD1_CTRLE") },
      { 0x0946, new IoRegisterNotImplemented("TCD1_INTCTRLA") },
      { 0x0947, new IoRegisterNotImplemented("TCD1_INTCTRLB") },
      { 0x0948, new IoRegisterNotImplemented("TCD1_CTRLFCLR") },
      { 0x0949, new IoRegisterNotImplemented("TCD1_CTRLFSET") },
      { 0x094a, new IoRegisterNotImplemented("TCD1_CTRLGCLR") },
      { 0x094b, new IoRegisterNotImplemented("TCD1_CTRLGSET") },
      { 0x094c, new IoRegisterNotImplemented("TCD1_INTFLAGS") },
      { 0x094f, new IoRegisterNotImplemented("TCD1_TEMP") },
      { 0x0960, new IoRegisterNotImplemented("TCD1_CNTL") },
      { 0x0961, new IoRegisterNotImplemented("TCD1_CNTH") },
      { 0x0966, new IoRegisterNotImplemented("TCD1_PERL") },
      { 0x0967, new IoRegisterNotImplemented("TCD1_PERH") },
      { 0x0968, new IoRegisterNotImplemented("TCD1_CCAL") },
      { 0x0969, new IoRegisterNotImplemented("TCD1_CCAH") },
      { 0x096a, new IoRegisterNotImplemented("TCD1_CCBL") },
      { 0x096b, new IoRegisterNotImplemented("TCD1_CCBH") },
      { 0x096c, new IoRegisterNotImplemented("TCD1_CCCL") },
      { 0x096d, new IoRegisterNotImplemented("TCD1_CCCH") },
      { 0x096e, new IoRegisterNotImplemented("TCD1_CCDL") },
      { 0x096f, new IoRegisterNotImplemented("TCD1_CCDH") },
      { 0x0976, new IoRegisterNotImplemented("TCD1_PERBUFL") },
      { 0x0977, new IoRegisterNotImplemented("TCD1_PERBUFH") },
      { 0x0978, new IoRegisterNotImplemented("TCD1_CCABUFL") },
      { 0x0979, new IoRegisterNotImplemented("TCD1_CCABUFH") },
      { 0x097a, new IoRegisterNotImplemented("TCD1_CCBBUFL") },
      { 0x097b, new IoRegisterNotImplemented("TCD1_CCBBUFH") },
      { 0x097c, new IoRegisterNotImplemented("TCD1_CCCBUFL") },
      { 0x097d, new IoRegisterNotImplemented("TCD1_CCCBUFH") },
      { 0x097e, new IoRegisterNotImplemented("TCD1_CCDBUFL") },
      { 0x097f, new IoRegisterNotImplemented("TCD1_CCDBUFH") },

      { 0x0990, new IoRegisterNotImplemented("HIRESD_CTRLA") }, // High Resolution Extension on port D

      { 0x09A0, new IoXmegaUsart::Data     (_usartD0) }, // USART 0 on port D
      { 0x09A1, new IoXmegaUsart::Status   (_usartD0) },                     
      { 0x09A3, new IoXmegaUsart::CtrlA    (_usartD0) },                     
      { 0x09A4, new IoXmegaUsart::CtrlB    (_usartD0) },                     
      { 0x09A5, new IoXmegaUsart::CtrlC    (_usartD0) },                     
      { 0x09A6, new IoXmegaUsart::BaudCtrlA(_usartD0) },                     
      { 0x09A7, new IoXmegaUsart::BaudCtrlB(_usartD0) },                     

      { 0x09B0, new IoXmegaUsart::Data     (_usartD1) }, // USART 1 on port D
      { 0x09B1, new IoXmegaUsart::Status   (_usartD1) },                     
      { 0x09B3, new IoXmegaUsart::CtrlA    (_usartD1) },                     
      { 0x09B4, new IoXmegaUsart::CtrlB    (_usartD1) },                     
      { 0x09B5, new IoXmegaUsart::CtrlC    (_usartD1) },                     
      { 0x09B6, new IoXmegaUsart::BaudCtrlA(_usartD1) },                     
      { 0x09B7, new IoXmegaUsart::BaudCtrlB(_usartD1) },                     

      { 0x09C0, new IoRegisterNotImplemented("SPID_CTRL") }, // Serial Peripheral Interface on port D
      { 0x09C1, new IoRegisterNotImplemented("SPID_INTCTRL") },
      { 0x09C2, new IoRegisterNotImplemented("SPID_STATUS") },
      { 0x09C3, new IoRegisterNotImplemented("SPID_DATA") },

      { 0x0A00, new IoRegisterNotImplemented("TCE0_CTRLA") }, // Timer/Counter 0 on port E
      { 0x0A01, new IoRegisterNotImplemented("TCE0_CTRLB") },
      { 0x0A02, new IoRegisterNotImplemented("TCE0_CTRLC") },
      { 0x0A03, new IoRegisterNotImplemented("TCE0_CTRLD") },
      { 0x0A04, new IoRegisterNotImplemented("TCE0_CTRLE") },
      { 0x0A06, new IoRegisterNotImplemented("TCE0_INTCTRLA") },
      { 0x0A07, new IoRegisterNotImplemented("TCE0_INTCTRLB") },
      { 0x0A08, new IoRegisterNotImplemented("TCE0_CTRLFCLR") },
      { 0x0A09, new IoRegisterNotImplemented("TCE0_CTRLFSET") },
      { 0x0A0a, new IoRegisterNotImplemented("TCE0_CTRLGCLR") },
      { 0x0A0b, new IoRegisterNotImplemented("TCE0_CTRLGSET") },
      { 0x0A0c, new IoRegisterNotImplemented("TCE0_INTFLAGS") },
      { 0x0A0f, new IoRegisterNotImplemented("TCE0_TEMP") },
      { 0x0A20, new IoRegisterNotImplemented("TCE0_CNTL") },
      { 0x0A21, new IoRegisterNotImplemented("TCE0_CNTH") },
      { 0x0A26, new IoRegisterNotImplemented("TCE0_PERL") },
      { 0x0A27, new IoRegisterNotImplemented("TCE0_PERH") },
      { 0x0A28, new IoRegisterNotImplemented("TCE0_CCAL") },
      { 0x0A29, new IoRegisterNotImplemented("TCE0_CCAH") },
      { 0x0A2a, new IoRegisterNotImplemented("TCE0_CCBL") },
      { 0x0A2b, new IoRegisterNotImplemented("TCE0_CCBH") },
      { 0x0A2c, new IoRegisterNotImplemented("TCE0_CCCL") },
      { 0x0A2d, new IoRegisterNotImplemented("TCE0_CCCH") },
      { 0x0A2e, new IoRegisterNotImplemented("TCE0_CCDL") },
      { 0x0A2f, new IoRegisterNotImplemented("TCE0_CCDH") },
      { 0x0A36, new IoRegisterNotImplemented("TCE0_PERBUFL") },
      { 0x0A37, new IoRegisterNotImplemented("TCE0_PERBUFH") },
      { 0x0A38, new IoRegisterNotImplemented("TCE0_CCABUFL") },
      { 0x0A39, new IoRegisterNotImplemented("TCE0_CCABUFH") },
      { 0x0A3a, new IoRegisterNotImplemented("TCE0_CCBBUFL") },
      { 0x0A3b, new IoRegisterNotImplemented("TCE0_CCBBUFH") },
      { 0x0A3c, new IoRegisterNotImplemented("TCE0_CCCBUFL") },
      { 0x0A3d, new IoRegisterNotImplemented("TCE0_CCCBUFH") },
      { 0x0A3e, new IoRegisterNotImplemented("TCE0_CCDBUFL") },
      { 0x0A3f, new IoRegisterNotImplemented("TCE0_CCDBUFH") },

      { 0x0A80, new IoRegisterNotImplemented("AWEXE_CTRL") }, // Advanced Waveform Extension on port E
      { 0x0A82, new IoRegisterNotImplemented("AWEXE_FDEMASK") },
      { 0x0A83, new IoRegisterNotImplemented("AWEXE_FDCTRL") },
      { 0x0A84, new IoRegisterNotImplemented("AWEXE_STATUS") },
      { 0x0A86, new IoRegisterNotImplemented("AWEXE_DTBOTH") },
      { 0x0A87, new IoRegisterNotImplemented("AWEXE_DTBOTHBUF") },
      { 0x0A88, new IoRegisterNotImplemented("AWEXE_DTLS") },
      { 0x0A89, new IoRegisterNotImplemented("AWEXE_DTHS") },
      { 0x0A8a, new IoRegisterNotImplemented("AWEXE_DTLSBUF") },
      { 0x0A8b, new IoRegisterNotImplemented("AWEXE_DTHSBUF") },
      { 0x0A8c, new IoRegisterNotImplemented("AWEXE_OUTOVEN") },

      { 0x0A90, new IoRegisterNotImplemented("HIRESE_CTRLA") }, // High Resolution Extension on port E

      { 0x0AA0, new IoXmegaUsart::Data     (_usartE0) }, // USART 0 on port E
      { 0x0AA1, new IoXmegaUsart::Status   (_usartE0) },                     
      { 0x0AA3, new IoXmegaUsart::CtrlA    (_usartE0) },                     
      { 0x0AA4, new IoXmegaUsart::CtrlB    (_usartE0) },                     
      { 0x0AA5, new IoXmegaUsart::CtrlC    (_usartE0) },                     
      { 0x0AA6, new IoXmegaUsart::BaudCtrlA(_usartE0) },                     
      { 0x0AA7, new IoXmegaUsart::BaudCtrlB(_usartE0) },                     

      // DAC, Fuse, Signature?
    } ;
    for (const auto &iIoReg: ioRegs)
    {
      _io[iIoReg.first] = iIoReg.second ;
    }

    _io[0x3f] = new IoSREG::SREG(_sreg) ;
    _io[0x3e] = new IoSP::SPH(_sp) ;
    _io[0x3d] = new IoSP::SPL(_sp) ;
  }

  ATxmegaAU::~ATxmegaAU()
  {
  }

  ////////////////////////////////////////////////////////////////////////////////

  ATxmega128A4U::ATxmega128A4U() : ATxmegaAU(0x20000/2, 0x2000, 0x800)
  {
    _pcIs22Bit = true ;
  }
  ATxmega128A4U::~ATxmega128A4U()
  {
  }

  ////////////////////////////////////////////////////////////////////////////////

  ATxmega64A4U::ATxmega64A4U() : ATxmegaAU(0x10000/2, 0x1000, 0x800)
  {
  }
  ATxmega64A4U::~ATxmega64A4U()
  {
  }

  ////////////////////////////////////////////////////////////////////////////////

  ATxmega32A4U::ATxmega32A4U() : ATxmegaAU(0x8000/2, 0x1000, 0x400)
  {
  }
  ATxmega32A4U::~ATxmega32A4U()
  {
  }

  ////////////////////////////////////////////////////////////////////////////////

  ATxmega16A4U::ATxmega16A4U() : ATxmegaAU(0x4000/2, 0x800, 0x400)
  {
  }
  ATxmega16A4U::~ATxmega16A4U()
  {
  }

}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
