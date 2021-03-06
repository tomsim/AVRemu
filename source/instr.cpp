////////////////////////////////////////////////////////////////////////////////
// instr.cpp
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <algorithm>

#include "instr.h"

#pragma GCC diagnostic ignored "-Wparentheses"

namespace AVR
{
  ////////////////////////////////////////////////////////////////////////////////
  // Helper

  inline void xxxxxxxRRRRRxxxx(Command cmd, Command &r)
  {
    r = (cmd & 0b0000000111110000) >> 4 ;
  }
  inline void xxxxxxRxxxxxRRRR(Command cmd, Command &r)
  {
    r = ((cmd & 0b0000001000000000) >> 5) | (cmd & 0b0000000000001111) ;
  }
  inline void xxxxxxxxKKxxKKKK(Command cmd, Command &k)
  {
    k = ((cmd & 0b0000000011000000) >> 2) | (cmd & 0b0000000000001111) ;
  }
  inline void xxxxxxxxxxRRxxxx(Command cmd, Command &r)
  {
    r = ((cmd & 0b0000000000110000) >> 3) + 24 ;
  }
  inline void xxxxKKKKxxxxKKKK(Command cmd, Command &k)
  {
    k = ((cmd & 0b0000111100000000) >> 4) | (cmd & 0b0000000000001111) ;
  }
  inline void xxxxxxxxRRRRxxxx1(Command cmd, Command &r)
  {
    r = ((cmd & 0b0000000011110000) >> 4) + 16 ;
  }
  inline void xxxxxxxxRRRRxxxx2(Command cmd, Command &r)
  {
    r = ((cmd & 0b0000000011110000) >> 3) ; ;
  }
  inline void xxxxxxxxxxxxRRRR1(Command cmd, Command &r)
  {
    r = ((cmd & 0b0000000000001111)) + 16 ;
  }
  inline void xxxxxxxxxxxxRRRR2(Command cmd, Command &r)
  {
    r = ((cmd & 0b0000000000001111) << 1) ;
  }
  inline void xxxxxxxxxRRRxxxx(Command cmd, Command &r)
  {
    r = ((cmd & 0b0000000001110000) >> 4) + 16 ;
  }
  inline void xxxxxxxxxxxxxRRR(Command cmd, Command &r)
  {
    r = (cmd & 0b0000000000000111) + 16 ;
  }
  inline void xxxxxxxxKKKKxxxx(Command cmd, Command &k)
  {
    k = (cmd & 0b0000000011110000) >> 4 ;
  }
  inline void xxxxKKKKKKKKKKKK(Command cmd, Command &k)
  {
    k = (cmd & 0b0000111111111111) ;
    if (   k & 0b0000100000000000) // negative
      k    |= 0b1111000000000000 ;
  }
  inline void xxxxxxxKKKKKxxxK(Command cmd, Command &k)
  {
    k = ((cmd & 0b0000000111110000) >> 3) | (cmd & 0b0000000000000001) ;
  }
  inline void xxxxxxxxxxxxxBBB(Command cmd, Command &b)
  {
    b = (cmd & 0b0000000000000111) ;
  }
  inline void xxxxxxxxAAAAAxxx(Command cmd, Command &a)
  {
    a = (cmd & 0b0000000011111000) >> 3 ;
  }
  inline void xxxxxxKKKKKKKxxx(Command cmd, Command &k)
  {
    k = (cmd & 0b0000001111111000) >> 3 ;
    if (k & 0b0000000001000000) // negative
      k |= 0b1111111110000000 ;
  }
  inline void xxxxxxxxxxxxxSSS(Command cmd, Command &s)
  {
    s = (cmd & 0b0000000000000111) ;
  }
  inline void xxxxxxxxxSSSxxxx(Command cmd, Command &s)
  {
    s = (cmd & 0b0000000001110000) >> 4 ;
  }
  inline void xxxxxAAxxxxxAAAA(Command cmd, Command &a)
  {
    a = ((cmd & 0b0000011000000000) >> 5) | (cmd & 0b0000000000001111) ;
  }
  inline void xxQxQQxxxxxxxQQQ(Command cmd, Command &q)
  {
    q = ((cmd & 0b0010000000000000) >> 8) | ((cmd & 0b0000110000000000) >> 7) | ((cmd & 0b0000000000000111) >> 0) ;
  }

  ////////////////////////////////////////////////////////////////////////////////

  inline char sToFlag(Command s)
  {
    static char flags[] = "CZNVSHTI" ;
    return flags[s] ;
  }
  inline const char* sToBSET(Command s)
  {
    static const char *mnemonic[] = { "SEC", "SEZ", "SEN", "SEV", "SES", "SEH", "SET", "SEI" } ;
    return mnemonic[s] ;
  }
  inline const char* sToBCLR(Command s)
  {
    static const char *mnemonic[] = { "CLC", "CLZ", "CLN", "CLV", "CLS", "CLH", "CLT", "CLI" } ;
    return mnemonic[s] ;
  }
  inline const char* sToBRBS(Command s)
  {
    static const char *mnemonic[] = { "BRCS", "BREQ", "BRMI", "BRVS", "BRLT", "BRHS", "BRTS", "BRIE" } ;
    return mnemonic[s] ;
  }
  inline const char* sToBRBC(Command s)
  {
    static const char *mnemonic[] = { "BRCC", "BRNE", "BRPL", "BRVC", "BRGE", "BRHC", "BRTC", "BRID" } ;
    return mnemonic[s] ;
  }


  ////////////////////////////////////////////////////////////////////////////////

  std::string Disasm_xxxxxxxxxxxxxxxx(const Instruction &instr)
  {
    char buff[1024] ;
    sprintf(buff, "%-6s\t\t; %s", instr.Mnemonic().c_str(), instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxRDDDDDRRRR(const Instruction &instr, Command cmd)
  {
    Command r, d ;
    xxxxxxRxxxxxRRRR(cmd, r) ;
    xxxxxxxRRRRRxxxx(cmd, d) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, r%d\t\t; %s", instr.Mnemonic().c_str(), d, r, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxxKKDDKKKK(const Instruction &instr, Command cmd)
  {
    Command k, d ;
    xxxxxxxxKKxxKKKK(cmd, k) ;
    xxxxxxxxxxRRxxxx(cmd, d) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, 0x%02x\t\t; %d %s", instr.Mnemonic().c_str(), d, k, k, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxKKKKDDDDKKKK(const Instruction &instr, Command cmd)
  {
    Command k, d ;
    xxxxKKKKxxxxKKKK(cmd, k) ;
    xxxxxxxxRRRRxxxx1(cmd, d) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, 0x%02x\t\t; %d %s", instr.Mnemonic().c_str(), d, k, k, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxDDDDDxxxx(const Instruction &instr, Command cmd)
  {
    Command d ;
    xxxxxxxRRRRRxxxx(cmd, d) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d\t\t; %s", instr.Mnemonic().c_str(), d, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxxDDDDRRRR_MOVW(const Instruction &instr, Command cmd)
  {
    Command r, d ;
    xxxxxxxxRRRRxxxx2(cmd, d) ;
    xxxxxxxxxxxxRRRR2(cmd, r) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, r%d\t\t; %s", instr.Mnemonic().c_str(), d, r, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxxDDDDRRRR_MULS(const Instruction &instr, Command cmd)
  {
    Command r, d ;
    xxxxxxxxRRRRxxxx1(cmd, d) ;
    xxxxxxxxxxxxRRRR1(cmd, r) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, r%d\t\t; %s", instr.Mnemonic().c_str(), d, r, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxxxDDDxRRR(const Instruction &instr, Command cmd)
  {
    Command r, d ;
    xxxxxxxxxRRRxxxx(cmd, d) ;
    xxxxxxxxxxxxxRRR(cmd, r) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, r%d\t\t; %s", instr.Mnemonic().c_str(), d, r, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxxKKKKxxxx(const Instruction &instr, Command cmd)
  {
    Command k ;
    xxxxxxxxKKKKxxxx(cmd, k) ;
    char buff[1024] ;
    sprintf(buff, "%-6s %d\t\t; %s", instr.Mnemonic().c_str(), k, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxKKKKKKKKKKKK(const Instruction &instr, const Mcu &mcu, Command cmd)
  {
    Command k ;
    xxxxKKKKKKKKKKKK(cmd, k) ;
    uint32_t addr = (uint32_t)(mcu.PC()) + (int16_t)k ;
    char buff[1024] ;
    std::string label ;
    if (mcu.ProgAddrName(addr, label))
      sprintf(buff, "%-6s %s\t\t; %d 0x%05x %s", instr.Mnemonic().c_str(), label.c_str(), (int16_t)k, addr, instr.Description().c_str()) ;
    else
      sprintf(buff, "%-6s %d\t\t; 0x%05x %s", instr.Mnemonic().c_str(), (int16_t)k, addr, instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  void Xref_xxxxKKKKKKKKKKKK(const Instruction &instr, const Mcu &mcu, Command cmd, uint32_t &addr)
  {
    Command k ;
    xxxxKKKKKKKKKKKK(cmd, k) ;
    addr = (uint32_t)(mcu.PC()) + (int16_t)k ;
  }

  std::string Disasm_xxxxxxxKKKKKxxxKk16(const Instruction &instr, Mcu &mcu, Command cmd)
  {
    Command k ;
    xxxxxxxKKKKKxxxK(cmd, k) ;
    uint32_t addr = (((uint32_t)k) << 16) + mcu.ProgramNext() ;
    char buff[1024] ;
    std::string label ;
    if (mcu.ProgAddrName(addr, label))
      sprintf(buff, "%-6s %s\t\t; 0x%05x %s", instr.Mnemonic().c_str(), label.c_str(), addr, instr.Description().c_str()) ;
    else
      sprintf(buff, "%-6s 0x%05x\t\t; %s", instr.Mnemonic().c_str(), addr, instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  void Xref_xxxxxxxKKKKKxxxKk16(const Instruction &instr, Mcu &mcu, Command cmd, uint32_t &addr)
  {
    Command k ;
    xxxxxxxKKKKKxxxK(cmd, k) ;
    addr = (((uint32_t)k) << 16) + mcu.ProgramNext() ;
  }

  std::string Disasm_xxxxxxxRRRRRxBBB(const Instruction &instr, Command cmd)
  {
    Command r, b ;
    xxxxxxxRRRRRxxxx(cmd, r) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, %d\t\t; %s", instr.Mnemonic().c_str(), r, b, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxxAAAAABBB(const Instruction &instr, const Mcu &mcu, Command cmd)
  {
    Command a, b ;
    xxxxxxxxAAAAAxxx(cmd, a) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;
    std::string ioRegName ;
    mcu.IoName(a, ioRegName) ;
    char buff[1024] ;
    sprintf(buff, "%-6s %s, %d\t\t; 0x%02x %s", instr.Mnemonic().c_str(), ioRegName.c_str(), b, a, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxKKKKKKKSSS_BRBS(const Instruction &instr, const Mcu &mcu, Command cmd)
  {
    Command k, s ;
    xxxxxxKKKKKKKxxx(cmd, k) ;
    xxxxxxxxxxxxxSSS(cmd, s) ;
    uint32_t addr = (uint32_t)(mcu.PC()) + (int16_t)k ;
    char buff[1024] ;
    std::string label ;
    if (mcu.ProgAddrName(addr, label))
      sprintf(buff, "%-6s %s\t\t; %d 0x%05x %s", sToBRBS(s), label.c_str(), (int16_t)k, addr, instr.Description().c_str()) ;
    else
      sprintf(buff, "%-6s %d\t\t; 0x%05x %s", sToBRBS(s), (int16_t)k, addr, instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  std::string Disasm_xxxxxxKKKKKKKSSS_BRBC(const Instruction &instr, const Mcu &mcu, Command cmd)
  {
    Command k, s ;
    xxxxxxKKKKKKKxxx(cmd, k) ;
    xxxxxxxxxxxxxSSS(cmd, s) ;
    uint32_t addr = (uint32_t)(mcu.PC()) + (int16_t)k ;
    char buff[1024] ;
    std::string label ;
    if (mcu.ProgAddrName(addr, label))
      sprintf(buff, "%-6s %s\t\t; %d 0x%05x %s", sToBRBC(s), label.c_str(), (int16_t)k, addr, instr.Description().c_str()) ;
    else
      sprintf(buff, "%-6s %d\t\t; 0x%05x %s", sToBRBC(s), (int16_t)k, addr, instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  void Xref_xxxxxxKKKKKKKxxx(const Instruction &instr, const Mcu &mcu, Command cmd, uint32_t &addr)
  {
    Command k ;
    xxxxxxKKKKKKKxxx(cmd, k) ;
    addr = (uint32_t)(mcu.PC()) + (int16_t)k ;
  }

  std::string Disasm_xxxxxxKKKKKKKxxx(const Instruction &instr, const Mcu &mcu, Command cmd)
  {
    Command k ;
    xxxxxxKKKKKKKxxx(cmd, k) ;
    char buff[1024] ;
    sprintf(buff, "%-6s %d\t\t; 0x%05x %s", instr.Mnemonic().c_str(), (int16_t)k, (uint32_t)(mcu.PC()) + (int16_t)k, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxxxSSSxxxx_BSET(const Instruction &instr, Command cmd)
  {
    Command s ;
    xxxxxxxxxSSSxxxx(cmd, s) ;
    char buff[1024] ;
    sprintf(buff, "%-6s\t\t; %s", sToBSET(s), instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  std::string Disasm_xxxxxxxxxSSSxxxx_BCLR(const Instruction &instr, Command cmd)
  {
    Command s ;
    xxxxxxxxxSSSxxxx(cmd, s) ;
    char buff[1024] ;
    sprintf(buff, "%-6s\t\t; %s", sToBCLR(s), instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxDDDDDxBBB(const Instruction &instr, Command cmd)
  {
    Command d, b ;
    xxxxxxxRRRRRxxxx(cmd, d) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, %d\t\t; %s", instr.Mnemonic().c_str(), d, b, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxDDDDDxxxxk16(const Instruction &instr, Mcu &mcu, Command cmd)
  {
    Command d ;
    xxxxxxxRRRRRxxxx(cmd, d) ;
    uint32_t addr = mcu.ProgramNext() ;
    char buff[1024] ;
    std::string ioRegName ;
    if (mcu.IoName(addr, ioRegName))
      sprintf(buff, "%-6s r%d, %s\t\t; 0x%04x %s", instr.Mnemonic().c_str(), d, ioRegName.c_str(), addr, instr.Description().c_str()) ;
    else
      sprintf(buff, "%-6s r%d, 0x%04x\t\t; %s", instr.Mnemonic().c_str(), d, addr, instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  std::string Disasm_xxxxxxxRRRRRxxxxk16(const Instruction &instr, Mcu &mcu, Command cmd)
  {
    Command r ;
    xxxxxxxRRRRRxxxx(cmd, r) ;
    uint32_t addr = mcu.ProgramNext() ;
    char buff[1024] ;
    std::string ioRegName ;
    if (mcu.IoName(addr, ioRegName))
      sprintf(buff, "%-6s %s, r%d\t\t; 0x%04x %s", instr.Mnemonic().c_str(), ioRegName.c_str(), r, addr, instr.Description().c_str()) ;
    else
      sprintf(buff, "%-6s 0x%04x, r%d\t\t; %s", instr.Mnemonic().c_str(), addr, r, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxAADDDDDAAAA(const Instruction &instr, const Mcu &mcu, Command cmd)
  {
    Command d, a ;
    xxxxxAAxxxxxAAAA(cmd, a) ;
    xxxxxxxRRRRRxxxx(cmd, d) ;
    char buff[1024] ;
    std::string ioRegName  ;
    mcu.IoName(a, ioRegName) ;
    sprintf(buff, "%-6s r%d, %s\t\t; 0x%02x %s", instr.Mnemonic().c_str(), d, ioRegName.c_str(), a, instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  std::string Disasm_xxxxxAARRRRRAAAA(const Instruction &instr, const Mcu &mcu, Command cmd)
  {
    Command r, a ;
    xxxxxAAxxxxxAAAA(cmd, a) ;
    xxxxxxxRRRRRxxxx(cmd, r) ;
    char buff[1024] ;
    std::string ioRegName ;
    mcu.IoName(a, ioRegName) ;
    sprintf(buff, "%-6s %s, r%d\t\t; 0x%02x %s", instr.Mnemonic().c_str(), ioRegName.c_str(), r, a, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string Disasm_xxxxxxxDDDDDxxxx(const Instruction &instr, const Mcu &mcu, Command cmd, const char *offset)
  {
    Command d ;
    xxxxxxxRRRRRxxxx(cmd, d) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, %s\t\t; %s", instr.Mnemonic().c_str(), d, offset, instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  std::string Disasm_xxxxxxxRRRRRxxxx(const Instruction &instr, const Mcu &mcu, Command cmd, const char *offset)
  {
    Command r ;
    xxxxxxxRRRRRxxxx(cmd, r) ;
    char buff[1024] ;
    sprintf(buff, "%-6s %s, r%d\t\t; %s", instr.Mnemonic().c_str(), offset, r, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  std::string  Disasm_xxQxQQxDDDDDxQQQ(const Instruction &instr, const Mcu &mcu, Command cmd, const char *offset)
  {
    Command d, q ;
    xxxxxxxRRRRRxxxx(cmd, d) ;
    xxQxQQxxxxxxxQQQ(cmd, q) ;
    char buff[1024] ;
    sprintf(buff, "%-6s r%d, %s+%d\t\t; %s", instr.Mnemonic().c_str(), d, offset, q, instr.Description().c_str()) ;
    return std::string(buff) ;
  }
  std::string  Disasm_xxQxQQxRRRRRxQQQ(const Instruction &instr, const Mcu &mcu, Command cmd, const char *offset)
  {
    Command r, q ;
    xxxxxxxRRRRRxxxx(cmd, r) ;
    xxQxQQxxxxxxxQQQ(cmd, q) ;
    char buff[1024] ;
    sprintf(buff, "%-6s %s+%d, r%d\t\t; %s", instr.Mnemonic().c_str(), offset, q, r, instr.Description().c_str()) ;
    return std::string(buff) ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ADD
  InstrADD::InstrADD() : Instruction(0b0000110000000000, 0b1111110000000000, "ADD", "Add without Carry", false, false, false, false, false)
  {
  }

  InstrADD::~InstrADD()
  {
  }

  uint8_t InstrADD::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11000000 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd + rr ;
    uint8_t rHC = rd & rr | rr & ~r | ~r & rd ;
    uint8_t rV  = rd & rr & ~r | ~rd & ~rr & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrADD::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrADD::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ADC
  InstrADC::InstrADC() : Instruction(0b0001110000000000, 0b1111110000000000, "ADC", "Add with Carry", false, false, false, false, false)
  {
  }

  InstrADC::~InstrADC()
  {
  }

  uint8_t InstrADC::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg0 = mcu.GetSREG() ;
    uint8_t sreg = sreg0 & 0b11000000 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd + rr + (sreg0 & SREG::C) ;
    uint8_t rHC = rd & rr | rr & ~r | ~r & rd ;
    uint8_t rV  = rd & rr & ~r | ~rd & ~rr & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrADC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrADC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ADIW
  InstrADIW::InstrADIW() : Instruction(0b1001011000000000, 0b1111111100000000, "ADIW", "Add Immediate to Word", false, false, false, false, false)
  {
  }

  InstrADIW::~InstrADIW()
  {
  }

  uint8_t InstrADIW::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, nd ;
    xxxxxxxxKKxxKKKK(cmd, k) ;
    xxxxxxxxxxRRxxxx(cmd, nd) ;

    uint8_t  sreg = mcu.GetSREG() & 0b11100000 ;
    uint16_t rd   = mcu.RegW(nd) ;
    uint16_t r    = rd + k ;
    if (~rd & r & 0x8000)
      sreg |= SREG::V ;
    if (r & 0x8000)
      sreg |= SREG::N ;
    if (r == 0x0000)
      sreg |= SREG::Z ;
    if (~r & rd & 0x8000)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.RegW(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 2 ;
  }
  std::string InstrADIW::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxKKDDKKKK(*this, cmd) ;
  }
  XrefType InstrADIW::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SUB
  InstrSUB::InstrSUB() : Instruction(0b0001100000000000, 0b1111110000000000, "SUB", "Subtract without Carry", false, false, false, false, false)
  {
  }

  InstrSUB::~InstrSUB()
  {
  }

  uint8_t InstrSUB::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11000000 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd - rr ;
    uint8_t rHC = ~rd & rr | rr & r | r & ~rd ;
    uint8_t rV  = rd & ~rr & ~r | ~rd & rr & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrSUB::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrSUB::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SUBI
  InstrSUBI::InstrSUBI() : Instruction(0b0101000000000000, 0b1111000000000000, "SUBI", "Subtract Immediate", false, false, false, false, false)
  {
  }

  InstrSUBI::~InstrSUBI()
  {
  }

  uint8_t InstrSUBI::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, nd ;
    xxxxKKKKxxxxKKKK(cmd, k) ;
    xxxxxxxxRRRRxxxx1(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11000000 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd - k ;
    uint8_t rHC = ~rd & k | k & r | r & ~rd ;
    uint8_t rV  = rd & ~k & ~r | ~rd & k & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrSUBI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxKKKKDDDDKKKK(*this, cmd) ;
  }
  XrefType InstrSUBI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SBC
  InstrSBC::InstrSBC() : Instruction(0b0000100000000000, 0b1111110000000000, "SBC", "Subtract with Carry", false, false, false, false, false)
  {
  }

  InstrSBC::~InstrSBC()
  {
  }

  uint8_t InstrSBC::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg0 = mcu.GetSREG() ;
    uint8_t sreg = sreg0 & 0b11000000 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd - rr - (sreg0 & SREG::C) ;
    uint8_t rHC = ~rd & rr | rr & r | r & ~rd ;
    uint8_t rV  = rd & ~rr & ~r | ~rd & rr & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if ((r == 0x00) && (sreg0 && SREG::Z))
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrSBC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrSBC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SBCI
  InstrSBCI::InstrSBCI() : Instruction(0b0100000000000000, 0b1111000000000000, "SBCI", "Subtract Immediate with Carry", false, false, false, false, false)
  {
  }

  InstrSBCI::~InstrSBCI()
  {
  }

  uint8_t InstrSBCI::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, nd ;
    xxxxKKKKxxxxKKKK(cmd, k) ;
    xxxxxxxxRRRRxxxx1(cmd, nd) ;

    uint8_t sreg0 = mcu.GetSREG() ;
    uint8_t sreg = sreg0 & 0b11000000 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd - k - (sreg0 & SREG::C);
    uint8_t rHC = ~rd & k | k & r | r & ~rd ;
    uint8_t rV  = rd & ~k & ~r | ~rd & k & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if ((r == 0x00) && (sreg0 && SREG::Z))
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrSBCI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxKKKKDDDDKKKK(*this, cmd) ;
  }
  XrefType InstrSBCI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SBIW
  InstrSBIW::InstrSBIW() : Instruction(0b1001011100000000, 0b1111111100000000, "SBIW", "Subtract Immediate from Word", false, false, false, false, false)
  {
  }

  InstrSBIW::~InstrSBIW()
  {
  }

  uint8_t InstrSBIW::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, nd ;
    xxxxxxxxKKxxKKKK(cmd, k) ;
    xxxxxxxxxxRRxxxx(cmd, nd) ;

    uint8_t  sreg = mcu.GetSREG() & 0b11100000 ;
    uint16_t rd   = mcu.RegW(nd) ;
    uint16_t r    = rd - k ;
    if (~r & rd & 0x8000) // bug in spec
      sreg |= SREG::V ;
    if (r & 0x8000)
      sreg |= SREG::N ;
    if (r == 0x0000)
      sreg |= SREG::Z ;
    if (r & ~rd & 0x8000)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.RegW(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 2 ;
  }
  std::string InstrSBIW::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxKKDDKKKK(*this, cmd) ;
  }
  XrefType InstrSBIW::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // AND
  InstrAND::InstrAND() : Instruction(0b0010000000000000, 0b1111110000000000, "AND", "Logical AND", false, false, false, false, false)
  {
  }

  InstrAND::~InstrAND()
  {
  }

  uint8_t InstrAND::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100001 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd & rr ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrAND::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrAND::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ANDI
  InstrANDI::InstrANDI() : Instruction(0b0111000000000000, 0b1111000000000000, "ANDI", "Logical AND with Immediate", false, false, false, false, false)
  {
  }

  InstrANDI::~InstrANDI()
  {
  }

  uint8_t InstrANDI::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, nd ;
    xxxxKKKKxxxxKKKK(cmd, k) ;
    xxxxxxxxRRRRxxxx1(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100001 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd & k ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrANDI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxKKKKDDDDKKKK(*this, cmd) ;
  }
  XrefType InstrANDI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // OR
  InstrOR::InstrOR() : Instruction(0b0010100000000000, 0b1111110000000000, "OR", "Logical OR", false, false, false, false, false)
  {
  }

  InstrOR::~InstrOR()
  {
  }

  uint8_t InstrOR::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100001 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd | rr ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrOR::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrOR::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ORI
  InstrORI::InstrORI() : Instruction(0b0110000000000000, 0b1111000000000000, "ORI", "Logical OR with Immediate", false, false, false, false, false)
  {
  }

  InstrORI::~InstrORI()
  {
  }

  uint8_t InstrORI::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, nd ;
    xxxxKKKKxxxxKKKK(cmd, k) ;
    xxxxxxxxRRRRxxxx1(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100001 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd | k ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrORI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxKKKKDDDDKKKK(*this, cmd) ;
  }
  XrefType InstrORI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // EOR
  InstrEOR::InstrEOR() : Instruction(0b0010010000000000, 0b1111110000000000, "EOR", "Exclusive OR", false, false, false, false, false)
  {
  }

  InstrEOR::~InstrEOR()
  {
  }

  uint8_t InstrEOR::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100001 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd ^ rr ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrEOR::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrEOR::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // COM
  InstrCOM::InstrCOM() : Instruction(0b1001010000000000, 0b1111111000001111, "COM", "One's Complement", false, false, false, false, false)
  {
  }

  InstrCOM::~InstrCOM()
  {
  }

  uint8_t InstrCOM::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100000 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = 0xff - rd ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrCOM::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrCOM::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // NEG
  InstrNEG::InstrNEG() : Instruction(0b1001010000000001, 0b1111111000001111, "NEG", "Two's Complement", false, false, false, false, false)
  {
  }

  InstrNEG::~InstrNEG()
  {
  }

  uint8_t InstrNEG::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11000000 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = 0 - rd ;
    uint8_t rH = r | ~rd ;
    if (rH & 0x08)
      sreg |= SREG::H ;
    if (r == 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (r != 0x00)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrNEG::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrNEG::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // INC
  InstrINC::InstrINC() : Instruction(0b1001010000000011, 0b1111111000001111, "INC", "Increment", false, false, false, false, false)
  {
  }

  InstrINC::~InstrINC()
  {
  }

  uint8_t InstrINC::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100001 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd + 1 ;
    if (r == 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrINC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrINC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // DEC
  InstrDEC::InstrDEC() : Instruction(0b1001010000001010, 0b1111111000001111, "DEC", "Decrement", false, false, false, false, false)
  {
  }

  InstrDEC::~InstrDEC()
  {
  }

  uint8_t InstrDEC::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100001 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd - 1 ;
    if (r == 0x7f)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrDEC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrDEC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // MUL
  InstrMUL::InstrMUL() : Instruction(0b1001110000000000, 0b1111110000000000, "MUL", "Multiply Unsigned", false, false, false, false, false)
  {
  }

  InstrMUL::~InstrMUL()
  {
  }

  uint8_t InstrMUL::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11111100 ;
    uint16_t rr = mcu.Reg(nr) ;
    uint16_t rd = mcu.Reg(nd) ;
    uint16_t r = rd * rr ;
    if (r == 0x0000)
      sreg |= SREG::Z ;
    if (r & 0x8000)
      sreg |= SREG::C ;
    mcu.RegW(0, r) ;
    mcu.SetSREG(sreg) ;

    return 2 ;
  }
  std::string InstrMUL::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrMUL::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // MULS
  InstrMULS::InstrMULS() : Instruction(0b0000001000000000, 0b1111111100000000, "MULS", "Multiply Signed", false, false, false, false, false)
  {
  }

  InstrMULS::~InstrMULS()
  {
  }

  uint8_t InstrMULS::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxxxRRRRxxxx1(cmd, nd) ;
    xxxxxxxxxxxxRRRR1(cmd, nr) ;

    uint8_t sreg = mcu.GetSREG() & 0b11111100 ;
    int16_t rr = (int8_t)mcu.Reg(nr) ;
    int16_t rd = (int8_t)mcu.Reg(nd) ;
    int16_t r = rd * rr ;
    if (r == 0x0000)
      sreg |= SREG::Z ;
    if (r & 0x8000)
      sreg |= SREG::C ;
    mcu.RegW(0, (uint16_t)r) ;
    mcu.SetSREG(sreg) ;

    return 2 ;
  }
  std::string InstrMULS::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxDDDDRRRR_MULS(*this, cmd) ;
  }
  XrefType InstrMULS::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // MULSU
  InstrMULSU::InstrMULSU() : Instruction(0b0000001100000000, 0b1111111110001000, "MULSU", "Multiply Signed with Unsigned", false, false, false, false, false)
  {
  }

  InstrMULSU::~InstrMULSU()
  {
  }

  uint8_t InstrMULSU::Execute(Mcu &mcu, Command cmd) const
  {
    // todo exec InstrMULSU
    mcu.NotImplemented(*this) ;
    
    return 2 ;
  }
  std::string InstrMULSU::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxDDDxRRR(*this, cmd) ;
  }
  XrefType InstrMULSU::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // FMUL
  InstrFMUL::InstrFMUL() : Instruction(0b0000001100001000, 0b1111111110001000, "FMUL", "Fractional Multiply Unsigned", false, false, false, false, false)
  {
  }

  InstrFMUL::~InstrFMUL()
  {
  }

  uint8_t InstrFMUL::Execute(Mcu &mcu, Command cmd) const
  {
    // todo exec InstrFMUL
    mcu.NotImplemented(*this) ;

    return 2 ;
  }
  std::string InstrFMUL::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxDDDxRRR(*this, cmd) ;
  }
  XrefType InstrFMUL::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // FMULS
  InstrFMULS::InstrFMULS() : Instruction(0b0000001110000000, 0b1111111110001000, "FMULS", "Fractional Multiply Signed", false, false, false, false, false)
  {
  }

  InstrFMULS::~InstrFMULS()
  {
  }

  uint8_t InstrFMULS::Execute(Mcu &mcu, Command cmd) const
  {
    // todo exec InstrFMULS
    mcu.NotImplemented(*this) ;

    return 2 ;
  }
  std::string InstrFMULS::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxDDDxRRR(*this, cmd) ;
  }
  XrefType InstrFMULS::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // FMULSU
  InstrFMULSU::InstrFMULSU() : Instruction(0b0000001110001000, 0b1111111110001000, "FMULSU", "Fractional Multiply Signed with Unsigned", false, false, false, false, false)
  {
  }

  InstrFMULSU::~InstrFMULSU()
  {
  }

  uint8_t InstrFMULSU::Execute(Mcu &mcu, Command cmd) const
  {
    // todo exec InstrFMULSU
    mcu.NotImplemented(*this) ;

    return 2 ;
  }
  std::string InstrFMULSU::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxDDDxRRR(*this, cmd) ;
  }
  XrefType InstrFMULSU::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // DES
  InstrDES::InstrDES() : Instruction(0b1001010000001011, 0b1111111100001111, "DES", "Data Encryption Standard", false, false, false, false, false)
  {
  }

  InstrDES::~InstrDES()
  {
  }

  uint8_t InstrDES::Execute(Mcu &mcu, Command cmd) const
  {
    // todo exec InstrDES
    mcu.NotImplemented(*this) ;

    return 2 ; // todo ticks
  }
  std::string InstrDES::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxKKKKxxxx(*this, cmd) ;
  }
  XrefType InstrDES::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // RJMP
  InstrRJMP::InstrRJMP() : Instruction(0b1100000000000000, 0b1111000000000000, "RJMP", "Relative Jump", false, true, false, false, false)
  {
  }

  InstrRJMP::~InstrRJMP()
  {
  }

  uint8_t InstrRJMP::Execute(Mcu &mcu, Command cmd) const
  {
    Command k ;
    xxxxKKKKKKKKKKKK(cmd, k) ;
    mcu.PC() = mcu.PC() + (int16_t)k ;

    return 2 ;
  }
  std::string InstrRJMP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxKKKKKKKKKKKK(*this, mcu, cmd) ;
  }
  XrefType InstrRJMP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    Xref_xxxxKKKKKKKKKKKK(*this, mcu, cmd, addr) ;
    return XrefType::jmp ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // IJMP
  InstrIJMP::InstrIJMP() : Instruction(0b1001010000001001, 0b1111111111111111, "IJMP", "Indirect Jump", false, true, false, false, false)
  {
  }

  InstrIJMP::~InstrIJMP()
  {
  }

  uint8_t InstrIJMP::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.XrefAdd(XrefType::jmp, mcu.RegW(30), mcu.PC()-1) ;
    mcu.PC() = mcu.RegW(30) ;

    return 2 ;
  }
  std::string InstrIJMP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrIJMP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ; // todo information
  }

  ////////////////////////////////////////////////////////////////////////////////
  // EIJMP
  InstrEIJMP::InstrEIJMP() : Instruction(0b1001010000011001, 0b1111111111111111, "EIJMP", "Extended Indirect Jump", false, true, false, false, false)
  {
  }

  InstrEIJMP::~InstrEIJMP()
  {
  }

  uint8_t InstrEIJMP::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.XrefAdd(XrefType::jmp, mcu.GetEind() | mcu.RegW(30), mcu.PC()-1) ;
    mcu.PC() = mcu.GetEind() | mcu.RegW(30) ;

    return 2 ;
  }
  std::string InstrEIJMP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrEIJMP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ; // todo information
  }

  ////////////////////////////////////////////////////////////////////////////////
  // JMP
  InstrJMP::InstrJMP() : Instruction(0b1001010000001100, 0b1111111000001110, "JMP", "Jump", true, true, false, false, false)
  {
  }

  InstrJMP::~InstrJMP()
  {
  }

  uint8_t InstrJMP::Execute(Mcu &mcu, Command cmd) const
  {
    Command k ;
    xxxxxxxKKKKKxxxK(cmd, k) ;
    uint32_t addr = (((uint32_t)k) << 16) + mcu.ProgramNext() ;
    mcu.PC() = addr ;

    return 3 ;
  }
  std::string InstrJMP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxKKKKKxxxKk16(*this, mcu, cmd) ;
  }
  XrefType InstrJMP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    Xref_xxxxxxxKKKKKxxxKk16(*this, mcu, cmd, addr) ;
    return XrefType::jmp ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // RCALL
  InstrRCALL::InstrRCALL() : Instruction(0b1101000000000000, 0b1111000000000000, "RCALL", "Relative Call to Subroutine", false, false, false, true, false)
  {
  }

  InstrRCALL::~InstrRCALL()
  {
  }

  uint8_t InstrRCALL::Execute(Mcu &mcu, Command cmd) const
  {
    Command k ;
    xxxxKKKKKKKKKKKK(cmd, k) ;
    mcu.PushPC() ;
    mcu.PC() = mcu.PC() + (int16_t)k ;

    if (mcu.IsXmega())
      return mcu.PcIs22bit() ? 3 : 2 ;
    else if (mcu.IsTinyReduced())
      return 4 ;
    return mcu.PcIs22bit() ? 4 : 3 ;
  }
  std::string InstrRCALL::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxKKKKKKKKKKKK(*this, mcu, cmd) ;
  }
  XrefType InstrRCALL::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    Xref_xxxxKKKKKKKKKKKK(*this, mcu, cmd, addr) ;
    return XrefType::call ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ICALL
  InstrICALL::InstrICALL() : Instruction(0b1001010100001001, 0b1111111111111111, "ICALL", "Indirect Call to Subroutine", false, false, false, true, false)
  {
  }

  InstrICALL::~InstrICALL()
  {
  }

  uint8_t InstrICALL::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.XrefAdd(XrefType::call, mcu.RegW(30), mcu.PC()-1) ;
    mcu.PushPC() ;
    mcu.PC() = mcu.RegW(30) ;

    if (mcu.IsXmega())
      return mcu.PcIs22bit() ? 3 : 2 ;
    return mcu.PcIs22bit() ? 4 : 3 ;
  }
  std::string InstrICALL::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrICALL::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ; // todo information
  }

  ////////////////////////////////////////////////////////////////////////////////
  // EICALL
  InstrEICALL::InstrEICALL() : Instruction(0b1001010100011001, 0b1111111111111111, "EICALL", "Extended Indirect Call to Subroutine", false, false, false, true, false)
  {
  }

  InstrEICALL::~InstrEICALL()
  {
  }

  uint8_t InstrEICALL::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.XrefAdd(XrefType::call, mcu.GetEind() | mcu.RegW(30), mcu.PC()-1) ;
    mcu.PushPC() ;
    mcu.PC() = mcu.GetEind() | mcu.RegW(30) ;

    if (mcu.IsXmega())
      return 3 ;
    return 4 ;
  }
  std::string InstrEICALL::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrEICALL::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ; // todo information
  }

  ////////////////////////////////////////////////////////////////////////////////
  // CALL
  InstrCALL::InstrCALL() : Instruction(0b1001010000001110, 0b1111111000001110, "CALL", "Call to Subroutine", true, false, false, true, false)
  {
  }

  InstrCALL::~InstrCALL()
  {
  }

  uint8_t InstrCALL::Execute(Mcu &mcu, Command cmd) const
  {
    Command k ;
    xxxxxxxKKKKKxxxK(cmd, k) ;
    uint32_t addr = (((uint32_t)k) << 16) + mcu.ProgramNext() ;
    mcu.PushPC() ;
    mcu.PC() = addr ;

    if (mcu.IsXmega())
      return mcu.PcIs22bit() ? 4 : 3 ;
    return mcu.PcIs22bit() ? 5 : 4 ;
  }
  std::string InstrCALL::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxKKKKKxxxKk16(*this, mcu, cmd) ;
  }
  XrefType InstrCALL::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    Xref_xxxxxxxKKKKKxxxKk16(*this, mcu, cmd, addr) ;
    return XrefType::call ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // RET
  InstrRET::InstrRET() : Instruction(0b1001010100001000, 0b1111111111111111, "RET", "Return from Subroutine", false, false, false, false, true)
  {
  }

  InstrRET::~InstrRET()
  {
  }

  uint8_t InstrRET::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.PopPC() ;

    return mcu.PcIs22bit() ? 5 : 4 ;
  }
  std::string InstrRET::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrRET::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // RETI
  InstrRETI::InstrRETI() : Instruction(0b1001010100011000, 0b1111111111111111, "RETI", "Return from Interrupt", false, false, false, false, true)
  {
  }

  InstrRETI::~InstrRETI()
  {
  }

  uint8_t InstrRETI::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.PopPC() ;
    uint8_t sreg = mcu.GetSREG() ;
    sreg |= SREG::I ;
    mcu.SetSREG(sreg) ;

    return mcu.PcIs22bit() ? 5 : 4 ;
  }
  std::string InstrRETI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrRETI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // CPSE
  InstrCPSE::InstrCPSE() : Instruction(0b0001000000000000, 0b1111110000000000, "CPSE", "Compare Skip if Equal", false, false, true, false, false)
  {
  }

  InstrCPSE::~InstrCPSE()
  {
  }

  uint8_t InstrCPSE::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    if (rr == rd)
      return mcu.Skip() + 1 ;

    return 1 ;
  }
  std::string InstrCPSE::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrCPSE::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // CP
  InstrCP::InstrCP() : Instruction(0b0001010000000000, 0b1111110000000000, "CP", "Compare", false, false, false, false, false)
  {
  }

  InstrCP::~InstrCP()
  {
  }

  uint8_t InstrCP::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11000000 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd - rr ;
    uint8_t rHC = ~rd & rr | rr & r | r & ~rd ;
    uint8_t rV  = rd & ~rr & ~r | ~rd & rr & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrCP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrCP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // CPC
  InstrCPC::InstrCPC() : Instruction(0b0000010000000000, 0b1111110000000000, "CPC", "Compare with Carry", false, false, false, false, false)
  {
  }

  InstrCPC::~InstrCPC()
  {
  }

  uint8_t InstrCPC::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxRxxxxxRRRR(cmd, nr) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg0 = mcu.GetSREG() ;
    uint8_t sreg = sreg0 & 0b11000000 ;
    uint8_t rr = mcu.Reg(nr) ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd - rr - (sreg0 & SREG::C) ;
    uint8_t rHC = ~rd & rr | rr & r | r & ~rd ;
    uint8_t rV  = rd & ~rr & ~r | ~rd & rr & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if ((r == 0x00) && (sreg0 && SREG::Z))
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrCPC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrCPC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // CPI
  InstrCPI::InstrCPI() : Instruction(0b0011000000000000, 0b1111000000000000, "CPI", "Compare with Immediate", false, false, false, false, false)
  {
  }

  InstrCPI::~InstrCPI()
  {
  }

  uint8_t InstrCPI::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, nd ;
    xxxxKKKKxxxxKKKK(cmd, k) ;
    xxxxxxxxRRRRxxxx1(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11000000 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd - k ;
    uint8_t rHC = ~rd & k | k & r | r & ~rd ;
    uint8_t rV  = rd & ~k & ~r | ~rd & k & r ;
    if (rHC & 0x08)
      sreg |= SREG::H ;
    if (rV & 0x80)
      sreg |= SREG::V ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rHC & 0x80)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrCPI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxKKKKDDDDKKKK(*this, cmd) ;
  }
  XrefType InstrCPI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SBRC
  InstrSBRC::InstrSBRC() : Instruction(0b1111110000000000, 0b1111111000001000, "SBRC", "Skip if Bit in Register is Cleared", false, false, true, false, false)
  {
  }

  InstrSBRC::~InstrSBRC()
  {
  }

  uint8_t InstrSBRC::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, b ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;

    uint8_t r = mcu.Reg(nr) ;
    if (!(r & (1<<b)))
      return mcu.Skip() + 1 ;
    return 1 ;    
  }
  std::string InstrSBRC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxBBB(*this, cmd) ;
  }
  XrefType InstrSBRC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SBRS
  InstrSBRS::InstrSBRS() : Instruction(0b1111111000000000, 0b1111111000001000, "SBRS", "Skip if Bit in Register is Set", false, false, true, false, false)
  {
  }

  InstrSBRS::~InstrSBRS()
  {
  }

  uint8_t InstrSBRS::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, b ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;

    uint8_t r = mcu.Reg(nr) ;
    if (r & (1<<b))
      return mcu.Skip() + 1 ;
    return 1 ;
  }
  std::string InstrSBRS::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxBBB(*this, cmd) ;
  }
  XrefType InstrSBRS::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SBIC
  InstrSBIC::InstrSBIC() : Instruction(0b1001100100000000, 0b1111111100000000, "SBIC", "Skip if Bit in I/O Register is Cleared", false, false, true, false, false)
  {
  }

  InstrSBIC::~InstrSBIC()
  {
  }

  uint8_t InstrSBIC::Execute(Mcu &mcu, Command cmd) const
  {
    Command ni, b ;
    xxxxxxxxAAAAAxxx(cmd, ni) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;

    uint8_t i = mcu.Io(ni) ;
    if (!(i & (1<<b)))
      return mcu.Skip() + mcu.IsXmega() ? 2 : 1 ;
    return mcu.IsXmega() ? 2 : 1 ;
  }
  std::string InstrSBIC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxAAAAABBB(*this, mcu, cmd) ;
  }
  XrefType InstrSBIC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SBIS
  InstrSBIS::InstrSBIS() : Instruction(0b1001101100000000, 0b1111111100000000, "SBIS", "Skip if Bit in I/O Registerster is Set", false, false, true, false, false)
  {
  }

  InstrSBIS::~InstrSBIS()
  {
  }

  uint8_t InstrSBIS::Execute(Mcu &mcu, Command cmd) const
  {
    Command ni, b ;
    xxxxxxxxAAAAAxxx(cmd, ni) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;

    uint8_t i = mcu.Io(ni) ;
    if (i & (1<<b))
      return mcu.Skip() + mcu.IsXmega() ? 2 : 1 ;
    return mcu.IsXmega() ? 2 : 1 ;
  }
  std::string InstrSBIS::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxAAAAABBB(*this, mcu, cmd) ;
  }
  XrefType InstrSBIS::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // BRBS
  InstrBRBS::InstrBRBS() : Instruction(0b1111000000000000, 0b1111110000000000, "BRBS", "Branch if Bit in SREG is Set", false, false, true, false, false)
  {
  }

  InstrBRBS::~InstrBRBS()
  {
  }

  uint8_t InstrBRBS::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, s ;
    xxxxxxKKKKKKKxxx(cmd, k) ;
    xxxxxxxxxxxxxSSS(cmd, s) ;

    if (mcu.GetSREG() & (1<<s))
    {
      mcu.PC() = mcu.PC() + (int16_t)k ;
      return 2 ;
    }
    return 1 ;
  }
  std::string InstrBRBS::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxKKKKKKKSSS_BRBS(*this, mcu, cmd) ;
  }
  XrefType InstrBRBS::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    Xref_xxxxxxKKKKKKKxxx(*this, mcu, cmd, addr) ;
    return XrefType::jmp ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // BRBC
  InstrBRBC::InstrBRBC() : Instruction(0b1111010000000000, 0b1111110000000000, "BRBC", "Branch if Bit in SREG is Cleared", false, false, true, false, false)
  {
  }

  InstrBRBC::~InstrBRBC()
  {
  }

  uint8_t InstrBRBC::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, s ;
    xxxxxxKKKKKKKxxx(cmd, k) ;
    xxxxxxxxxxxxxSSS(cmd, s) ;

    if (!(mcu.GetSREG() & (1<<s)))
    {
      mcu.PC() = mcu.PC() + (int16_t)k ;
      return 2 ;
    }
    return 1 ;
  }
  std::string InstrBRBC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxKKKKKKKSSS_BRBC(*this, mcu, cmd) ;
  }
  XrefType InstrBRBC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    Xref_xxxxxxKKKKKKKxxx(*this, mcu, cmd, addr) ;
    return XrefType::jmp ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // MOV
  InstrMOV::InstrMOV() : Instruction(0b0010110000000000, 0b1111110000000000, "MOV", "Copy Register", false, false, false, false, false)
  {
  }

  InstrMOV::~InstrMOV()
  {
  }

  uint8_t InstrMOV::Execute(Mcu &mcu, Command cmd) const
  {
    Command r, d ;
    xxxxxxRxxxxxRRRR(cmd, r) ;
    xxxxxxxRRRRRxxxx(cmd, d) ;
    mcu.Reg(d, mcu.Reg(r)) ;

    return 1 ;
  }
  std::string InstrMOV::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxRDDDDDRRRR(*this, cmd) ;
  }
  XrefType InstrMOV::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // MOVW
  InstrMOVW::InstrMOVW() : Instruction(0b0000000100000000, 0b1111111100000000, "MOVW", "Copy Register Word", false, false, false, false, false)
  {
  }

  InstrMOVW::~InstrMOVW()
  {
  }

  uint8_t InstrMOVW::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, nd ;
    xxxxxxxxRRRRxxxx2(cmd, nd) ;
    xxxxxxxxxxxxRRRR2(cmd, nr) ;
    mcu.RegW(nd, mcu.RegW(nr)) ;

    return 1 ;
  }
  std::string InstrMOVW::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxDDDDRRRR_MOVW(*this, cmd) ;
  }
  XrefType InstrMOVW::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDI
  InstrLDI::InstrLDI() : Instruction(0b1110000000000000, 0b1111000000000000, "LDI", "Load Immediate", false, false, false, false, false)
  {
  }

  InstrLDI::~InstrLDI()
  {
  }

  uint8_t InstrLDI::Execute(Mcu &mcu, Command cmd) const
  {
    Command k, d ;
    xxxxKKKKxxxxKKKK(cmd, k) ;
    xxxxxxxxRRRRxxxx1(cmd, d) ;
    mcu.Reg(d, (uint8_t)k) ;

    return 1 ;
  }
  std::string InstrLDI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxKKKKDDDDKKKK(*this, cmd) ;
  }
  XrefType InstrLDI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDS
  InstrLDS::InstrLDS() : Instruction(0b1001000000000000, 0b1111111000001111, "LDS", "Load Direct from Data Space", true, false, false, false, false)
  {
  }

  InstrLDS::~InstrLDS()
  {
  }

  uint8_t InstrLDS::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t addr = mcu.GetRampD() | mcu.ProgramNext() ;
    mcu.Reg(nd, mcu.Data(addr)) ;

    return 2 ; // xmega?
  }
  std::string InstrLDS::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxxk16(*this, mcu, cmd) ;
  }
  XrefType InstrLDS::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDx1
  InstrLDx1::InstrLDx1() : Instruction(0b1001000000001100, 0b1111111000001111, "LD", "Load Indirect from Data Space using X", false, false, false, false, false)
  {
  }

  InstrLDx1::~InstrLDx1()
  {
  }

  uint8_t InstrLDx1::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t x = mcu.GetRampX() | mcu.RegW(26) ;
    mcu.Reg(nd, mcu.Data(x)) ;

    return 1 ;
  }
  std::string InstrLDx1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "X") ;
  }
  XrefType InstrLDx1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDx2
  InstrLDx2::InstrLDx2() : Instruction(0b1001000000001101, 0b1111111000001111, "LD", "Load Indirect from Data Space using X", false, false, false, false, false)
  {
  }

  InstrLDx2::~InstrLDx2()
  {
  }

  uint8_t InstrLDx2::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t x = mcu.GetRampX() | mcu.RegW(26) ;
    mcu.Reg(nd, mcu.Data(x++)) ;
    mcu.SetRampX(x) ;
    mcu.RegW(26, x) ;

    return mcu.IsXmega() ? 1 : 2 ;
  }
  std::string InstrLDx2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "X+") ;
  }
  XrefType InstrLDx2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDx3
  InstrLDx3::InstrLDx3() : Instruction(0b1001000000001110, 0b1111111000001111, "LD", "Load Indirect from Data Space using X", false, false, false, false, false)
  {
  }

  InstrLDx3::~InstrLDx3()
  {
  }

  uint8_t InstrLDx3::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t x = mcu.GetRampX() | mcu.RegW(26) ;
    mcu.Reg(nd, mcu.Data(--x)) ;
    mcu.SetRampX(x) ;
    mcu.RegW(26, x) ;

    return mcu.IsXmega() ? 2 : 3 ;
  }
  std::string InstrLDx3::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "-X") ;
  }
  XrefType InstrLDx3::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDy1
  InstrLDy1::InstrLDy1() : Instruction(0b1000000000001000, 0b1111111000001111, "LD", "Load Indirect from Data Space using Y", false, false, false, false, false)
  {
  }

  InstrLDy1::~InstrLDy1()
  {
  }

  uint8_t InstrLDy1::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t y = mcu.GetRampY() | mcu.RegW(28) ;
    mcu.Reg(nd, mcu.Data(y)) ;

    return 1 ;
  }
  std::string InstrLDy1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "Y") ;
  }
  XrefType InstrLDy1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDy2
  InstrLDy2::InstrLDy2() : Instruction(0b1001000000001001, 0b1111111000001111, "LD", "Load Indirect from Data Space using Y", false, false, false, false, false)
  {
  }

  InstrLDy2::~InstrLDy2()
  {
  }

  uint8_t InstrLDy2::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t y = mcu.GetRampY() | mcu.RegW(28) ;
    mcu.Reg(nd, mcu.Data(y++)) ;
    mcu.SetRampY(y) ;
    mcu.RegW(28, y) ;

    return mcu.IsXmega() ? 1 : 2 ;
  }
  std::string InstrLDy2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "Y+") ;
  }
  XrefType InstrLDy2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDy3
  InstrLDy3::InstrLDy3() : Instruction(0b1001000000001010, 0b1111111000001111, "LD", "Load Indirect from Data Space using Y", false, false, false, false, false)
  {
  }

  InstrLDy3::~InstrLDy3()
  {
  }

  uint8_t InstrLDy3::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t y = mcu.GetRampY() | mcu.RegW(28) ;
    mcu.Reg(nd, mcu.Data(--y)) ;
    mcu.SetRampY(y) ;
    mcu.RegW(28, y) ;

    return mcu.IsXmega() ? 2 : 3 ;
  }
  std::string InstrLDy3::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "-Y") ;
  }
  XrefType InstrLDy3::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDy4
  InstrLDy4::InstrLDy4() : Instruction(0b1000000000001000, 0b1101001000001000, "LD", "Load Indirect from Data Space using Y", false, false, false, false, false)
  {
  }

  InstrLDy4::~InstrLDy4()
  {
  }

  uint8_t InstrLDy4::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd, q ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    xxQxQQxxxxxxxQQQ(cmd, q) ;
    uint32_t y = mcu.GetRampY() | mcu.RegW(28) ;
    mcu.Reg(nd, mcu.Data(y+q)) ;

    return mcu.IsXmega() ? 2 : 3 ;
  }
  std::string InstrLDy4::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxQxQQxDDDDDxQQQ(*this, mcu, cmd, "Y") ;
  }
  XrefType InstrLDy4::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDz1
  InstrLDz1::InstrLDz1() : Instruction(0b1000000000000000, 0b1111111000001111, "LD", "Load Indirect from Data Space using Z", false, false, false, false, false)
  {
  }

  InstrLDz1::~InstrLDz1()
  {
  }

  uint8_t InstrLDz1::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.Reg(nd, mcu.Data(z)) ;

    return 1 ;
  }
  std::string InstrLDz1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "Z") ;
  }
  XrefType InstrLDz1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDz2
  InstrLDz2::InstrLDz2() : Instruction(0b1001000000000001, 0b1111111000001111, "LD", "Load Indirect from Data Space using Z", false, false, false, false, false)
  {
  }

  InstrLDz2::~InstrLDz2()
  {
  }

  uint8_t InstrLDz2::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.Reg(nd, mcu.Data(z++)) ;
    mcu.SetRampZ(z) ;
    mcu.RegW(30, z) ;

    return mcu.IsXmega() ? 1 : 2 ;
  }
  std::string InstrLDz2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "Z+") ;
  }
  XrefType InstrLDz2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDz3
  InstrLDz3::InstrLDz3() : Instruction(0b1001000000000010, 0b1111111000001111, "LD", "Load Indirect from Data Space using Z", false, false, false, false, false)
  {
  }

  InstrLDz3::~InstrLDz3()
  {
  }

  uint8_t InstrLDz3::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.Reg(nd, mcu.Data(--z)) ;
    mcu.SetRampZ(z) ;
    mcu.RegW(30, z) ;

    return mcu.IsXmega() ? 2 : 3 ;
  }
  std::string InstrLDz3::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "-Z") ;
  }
  XrefType InstrLDz3::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LDz4
  InstrLDz4::InstrLDz4() : Instruction(0b1000000000000000, 0b1101001000001000, "LD", "Load Indirect from Data Space using Z", false, false, false, false, false)
  {
  }

  InstrLDz4::~InstrLDz4()
  {
  }

  uint8_t InstrLDz4::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd, q ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    xxQxQQxxxxxxxQQQ(cmd, q) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.Reg(nd, mcu.Data(z+q)) ;

    return mcu.IsXmega() ? 2 : 3 ;
  }
  std::string InstrLDz4::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxQxQQxDDDDDxQQQ(*this, mcu, cmd, "Z") ;
  }
  XrefType InstrLDz4::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STS
  InstrSTS::InstrSTS() : Instruction(0b1001001000000000, 0b1111111000001111, "STS", "Store Direct to Data Space", true, false, false, false, false)
  {
  }

  InstrSTS::~InstrSTS()
  {
  }

  uint8_t InstrSTS::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    uint32_t addr = mcu.GetRampD() | mcu.ProgramNext() ;
    mcu.Data(addr, mcu.Reg(nd)) ;

    return 2 ;
  }
  std::string InstrSTS::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxxk16(*this, mcu, cmd) ;
  }
  XrefType InstrSTS::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STx1
  InstrSTx1::InstrSTx1() : Instruction(0b1001001000001100, 0b1111111000001111, "ST", "Store Indirect to Data Space using X", false, false, false, false, false)
  {
  }

  InstrSTx1::~InstrSTx1()
  {
  }

  uint8_t InstrSTx1::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t x = mcu.GetRampX() | mcu.RegW(26) ;
    mcu.Data(x, mcu.Reg(nr)) ;

    return (mcu.IsXmega() || mcu.IsTinyReduced()) ? 1 : 2 ;
  }
  std::string InstrSTx1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "X") ;
  }
  XrefType InstrSTx1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STx2
  InstrSTx2::InstrSTx2() : Instruction(0b1001001000001101, 0b1111111000001111, "ST", "Store Indirect to Data Space using X", false, false, false, false, false)
  {
  }

  InstrSTx2::~InstrSTx2()
  {
  }

  uint8_t InstrSTx2::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t x = mcu.GetRampX() | mcu.RegW(26) ;
    mcu.Data(x++, mcu.Reg(nr)) ;
    mcu.SetRampX(x) ;
    mcu.RegW(26, x) ;

    return (mcu.IsXmega() || mcu.IsTinyReduced()) ? 1 : 2 ;
  }
  std::string InstrSTx2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "X+") ;
  }
  XrefType InstrSTx2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STx3
  InstrSTx3::InstrSTx3() : Instruction(0b1001001000001110, 0b1111111000001111, "ST", "Store Indirect to Data Space using X", false, false, false, false, false)
  {
  }

  InstrSTx3::~InstrSTx3()
  {
  }

  uint8_t InstrSTx3::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t x = mcu.GetRampX() | mcu.RegW(26) ;
    mcu.Data(--x, mcu.Reg(nr)) ;
    mcu.SetRampX(x) ;
    mcu.RegW(26, x) ;

    return 2 ;
  }
  std::string InstrSTx3::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "-X") ;
  }
  XrefType InstrSTx3::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STy1
  InstrSTy1::InstrSTy1() : Instruction(0b1000001000001000, 0b1111111000001111, "ST", "Store Indirect to Data Space using Y", false, false, false, false, false)
  {
  }

  InstrSTy1::~InstrSTy1()
  {
  }

  uint8_t InstrSTy1::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t y = mcu.GetRampY() | mcu.RegW(28) ;
    mcu.Data(y, mcu.Reg(nr)) ;

    return (mcu.IsXmega() || mcu.IsTinyReduced()) ? 1 : 2 ;
  }
  std::string InstrSTy1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "Y") ;
  }
  XrefType InstrSTy1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STy2
  InstrSTy2::InstrSTy2() : Instruction(0b1001001000001001, 0b1111111000001111, "ST", "Store Indirect to Data Space using Y", false, false, false, false, false)
  {
  }

  InstrSTy2::~InstrSTy2()
  {
  }

  uint8_t InstrSTy2::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t y = mcu.GetRampY() | mcu.RegW(28) ;
    mcu.Data(y++, mcu.Reg(nr)) ;
    mcu.SetRampY(y) ;
    mcu.RegW(28, y) ;

    return (mcu.IsXmega() || mcu.IsTinyReduced()) ? 1 : 2 ;
  }
  std::string InstrSTy2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "Y+") ;
  }
  XrefType InstrSTy2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STy3
  InstrSTy3::InstrSTy3() : Instruction(0b1001001000001010, 0b1111111000001111, "ST", "Store Indirect to Data Space using Y", false, false, false, false, false)
  {
  }

  InstrSTy3::~InstrSTy3()
  {
  }

  uint8_t InstrSTy3::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t y = mcu.GetRampY() | mcu.RegW(28) ;
    mcu.Data(--y, mcu.Reg(nr)) ;
    mcu.SetRampY(y) ;
    mcu.RegW(28, y) ;

    return 2 ;
  }
  std::string InstrSTy3::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "-Y") ;
  }
  XrefType InstrSTy3::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STy4
  InstrSTy4::InstrSTy4() : Instruction(0b1000001000001000, 0b1101001000001000, "ST", "Store Indirect to Data Space using Y", false, false, false, false, false)
  {
  }

  InstrSTy4::~InstrSTy4()
  {
  }

  uint8_t InstrSTy4::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, q ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    xxQxQQxxxxxxxQQQ(cmd, q) ;
    uint32_t y = mcu.GetRampY() | mcu.RegW(28) ;
    mcu.Data(y+q, mcu.Reg(nr)) ;

    return 2 ;
  }
  std::string InstrSTy4::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxQxQQxRRRRRxQQQ(*this, mcu, cmd, "Y") ;
  }
  XrefType InstrSTy4::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STz1
  InstrSTz1::InstrSTz1() : Instruction(0b1000001000000000, 0b1111111000001111, "ST", "Store Indirect to Data Space using Z", false, false, false, false, false)
  {
  }

  InstrSTz1::~InstrSTz1()
  {
  }

  uint8_t InstrSTz1::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.Data(z, mcu.Reg(nr)) ;

    return (mcu.IsXmega() || mcu.IsTinyReduced()) ? 1 : 2 ;
  }
  std::string InstrSTz1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "Z") ;
  }
  XrefType InstrSTz1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STz2
  InstrSTz2::InstrSTz2() : Instruction(0b1001001000000001, 0b1111111000001111, "ST", "Store Indirect to Data Space using Z", false, false, false, false, false)
  {
  }

  InstrSTz2::~InstrSTz2()
  {
  }

  uint8_t InstrSTz2::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.Data(z++, mcu.Reg(nr)) ;
    mcu.SetRampZ(z) ;
    mcu.RegW(30, z) ;

    return (mcu.IsXmega() || mcu.IsTinyReduced()) ? 1 : 2 ;
  }
  std::string InstrSTz2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "Z+") ;
  }
  XrefType InstrSTz2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STz3
  InstrSTz3::InstrSTz3() : Instruction(0b1001001000000010, 0b1111111000001111, "ST", "Store Indirect to Data Space using Z", false, false, false, false, false)
  {
  }

  InstrSTz3::~InstrSTz3()
  {
  }

  uint8_t InstrSTz3::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.Data(--z, mcu.Reg(nr)) ;
    mcu.SetRampZ(z) ;
    mcu.RegW(30, z) ;

    return 2 ;
  }
  std::string InstrSTz3::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxRRRRRxxxx(*this, mcu, cmd, "-Z") ;
  }
  XrefType InstrSTz3::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // STz4
  InstrSTz4::InstrSTz4() : Instruction(0b1000001000000000, 0b1101001000001000, "ST", "Store Indirect to Data Space using Z", false, false, false, false, false)
  {
  }

  InstrSTz4::~InstrSTz4()
  {
  }

  uint8_t InstrSTz4::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, q ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    xxQxQQxxxxxxxQQQ(cmd, q) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.Data(z+q, mcu.Reg(nr)) ;

    return 2 ;
  }
  std::string InstrSTz4::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxQxQQxRRRRRxQQQ(*this, mcu, cmd, "Z") ;
  }
  XrefType InstrSTz4::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LPM1
  InstrLPM1::InstrLPM1() : Instruction(0b1001010111001000, 0b1111111111111111, "LPM", "Load Program Memory", false, false, false, false, false)
  {
  }

  InstrLPM1::~InstrLPM1()
  {
  }

  uint8_t InstrLPM1::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.XrefAdd(XrefType::data, mcu.RegW(30)>>1, mcu.PC()-1) ;
    Command z, p ;
    z = mcu.RegW(30) ;
    p = mcu.Program(z>>1) ;
    if (z&1)
      p >>= 8 ;
    mcu.Reg(0, (uint8_t)p) ;

    return 3 ;
  }
  std::string InstrLPM1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrLPM1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LPM2
  InstrLPM2::InstrLPM2() : Instruction(0b1001000000000100, 0b1111111000001111, "LPM", "Load Program Memory", false, false, false, false, false)
  {
  }

  InstrLPM2::~InstrLPM2()
  {
  }

  uint8_t InstrLPM2::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.XrefAdd(XrefType::data, mcu.RegW(30)>>1, mcu.PC()-1) ;
    Command z, nd, p ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    z = mcu.RegW(30) ;
    p = mcu.Program(z>>1) ;
    if (z&1)
      p >>= 8 ;
    mcu.Reg(nd, (uint8_t)p) ;

    return 3 ;
  }
  std::string InstrLPM2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "Z") ;
  }
  XrefType InstrLPM2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LPM3
  InstrLPM3::InstrLPM3() : Instruction(0b1001000000000101, 0b1111111000001111, "LPM", "Load Program Memory", false, false, false, false, false)
  {
  }

  InstrLPM3::~InstrLPM3()
  {
  }

  uint8_t InstrLPM3::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.XrefAdd(XrefType::data, mcu.RegW(30)>>1, mcu.PC()-1) ;
    Command z, nd, p ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    z = mcu.RegW(30) ;
    p = mcu.Program(z>>1) ;
    if (z&1)
      p >>= 8 ;
    mcu.Reg(nd, (uint8_t)p) ;
    mcu.RegW(30, z+1) ;

    return 3 ;
  }
  std::string InstrLPM3::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "Z+") ;
  }
  XrefType InstrLPM3::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ELPM1
  InstrELPM1::InstrELPM1() : Instruction(0b1001010111011000, 0b1111111111111111, "ELPM", "Extended Load Program Memory", false, false, false, false, false)
  {
  }

  InstrELPM1::~InstrELPM1()
  {
  }

  uint8_t InstrELPM1::Execute(Mcu &mcu, Command cmd) const
  {
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.XrefAdd(XrefType::data, z>>1, mcu.PC()-1) ;
    Command p ;
    p = mcu.Program(z>>1) ;
    if (z&1)
      p >>= 8 ;
    mcu.Reg(0, (uint8_t)p) ;

    return 3 ;
  }
  std::string InstrELPM1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrELPM1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ELPM2
  InstrELPM2::InstrELPM2() : Instruction(0b1001000000000110, 0b1111111000001111, "ELPM", "Extended Load Program Memory", false, false, false, false, false)
  {
  }

  InstrELPM2::~InstrELPM2()
  {
  }

  uint8_t InstrELPM2::Execute(Mcu &mcu, Command cmd) const
  {
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.XrefAdd(XrefType::data, z>>1, mcu.PC()-1) ;
    Command nd, p ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    p = mcu.Program(z>>1) ;
    if (z&1)
      p >>= 8 ;
    mcu.Reg(nd, (uint8_t)p) ;

    return 3 ;
  }
  std::string InstrELPM2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "Z") ;
  }
  XrefType InstrELPM2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ELPM3
  InstrELPM3::InstrELPM3() : Instruction(0b1001000000000111, 0b1111111000001111, "ELPM", "Extended Load Program Memory", false, false, false, false, false)
  {
  }

  InstrELPM3::~InstrELPM3()
  {
  }

  uint8_t InstrELPM3::Execute(Mcu &mcu, Command cmd) const
  {
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    mcu.XrefAdd(XrefType::data, z>>1, mcu.PC()-1) ;
    Command nd, p ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    p = mcu.Program(z>>1) ;
    if (z&1)
      p >>= 8 ;
    mcu.Reg(nd, (uint8_t)p) ;
    mcu.SetRampZ(z+1) ;
    mcu.RegW(30, z+1) ;

    return 3 ;
  }
  std::string InstrELPM3::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, mcu, cmd, "Z+") ;
  }
  XrefType InstrELPM3::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SPM1
  InstrSPM1::InstrSPM1() : Instruction(0b1001010111101000, 0b1111111111111111, "SPM", "Store Program Memory", false, false, false, false, false)
  {
  }

  InstrSPM1::~InstrSPM1()
  {
  }

  uint8_t InstrSPM1::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.NotImplemented(*this) ;
    return 99 ;
  }
  std::string InstrSPM1::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrSPM1::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ; // todo information
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SPM2
  InstrSPM2::InstrSPM2() : Instruction(0b1001010111111000, 0b1111111111111111, "SPM Z+", "Store Program Memory", false, false, false, false, false)
  {
  }

  InstrSPM2::~InstrSPM2()
  {
  }

  uint8_t InstrSPM2::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.NotImplemented(*this) ;
    return 99 ;
  }
  std::string InstrSPM2::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrSPM2::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ; // todo information
  }

  ////////////////////////////////////////////////////////////////////////////////
  // IN
  InstrIN::InstrIN() : Instruction(0b1011000000000000, 0b1111100000000000, "IN", "Load an I/O Location to Register", false, false, false, false, false)
  {
  }

  InstrIN::~InstrIN()
  {
  }

  uint8_t InstrIN::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd, a ;
    xxxxxAAxxxxxAAAA(cmd, a) ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    mcu.Reg(nd, mcu.Io(a)) ;

    return 1 ;
  }
  std::string InstrIN::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxAADDDDDAAAA(*this, mcu, cmd) ;
  }
  XrefType InstrIN::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // OUT
  InstrOUT::InstrOUT() : Instruction(0b1011100000000000, 0b1111100000000000, "OUT", "Store Register to I/O Location", false, false, false, false, false)
  {
  }

  InstrOUT::~InstrOUT()
  {
  }

  uint8_t InstrOUT::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, a ;
    xxxxxAAxxxxxAAAA(cmd, a) ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    mcu.Io(a, mcu.Reg(nr)) ;

    return 1 ;
  }
  std::string InstrOUT::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxAARRRRRAAAA(*this, mcu, cmd) ;
  }
  XrefType InstrOUT::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // PUSH
  InstrPUSH::InstrPUSH() : Instruction(0b1001001000001111, 0b1111111000001111, "PUSH", "Push Register on Stack", false, false, false, false, false)
  {
  }

  InstrPUSH::~InstrPUSH()
  {
  }

  uint8_t InstrPUSH::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    mcu.Push(mcu.Reg(nr)) ;

    return mcu.IsXmega() ? 1 : 2 ;
  }
  std::string InstrPUSH::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrPUSH::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // POP
  InstrPOP::InstrPOP() : Instruction(0b1001000000001111, 0b1111111000001111, "POP", "Pop Register from Stack", false, false, false, false, false)
  {
  }

  InstrPOP::~InstrPOP()
  {
  }

  uint8_t InstrPOP::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    mcu.Reg(nd, mcu.Pop()) ;

    return 2 ;
  }
  std::string InstrPOP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrPOP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // XCH
  InstrXCH::InstrXCH() : Instruction(0b1001001000000100, 0b1111111000001111, "XCH Z,", "Exchange Indirect Register and Data Space", false, false, false, false, false)
  {
  }

  InstrXCH::~InstrXCH()
  {
  }

  uint8_t InstrXCH::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t  rd = mcu.Reg(nd) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    uint8_t  r  = mcu.Data(z) ;
    mcu.Data(z, rd) ;
    mcu.Reg(nd, r) ;

    return 1 ;
  }
  std::string InstrXCH::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrXCH::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LAS
  InstrLAS::InstrLAS() : Instruction(0b1001001000000101, 0b1111111000001111, "LAS Z,", "Load and Set Indirect Register and Data Space", false, false, false, false, false)
  {
  }

  InstrLAS::~InstrLAS()
  {
  }

  uint8_t InstrLAS::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t  rd = mcu.Reg(nd) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    uint8_t  r  = mcu.Data(z) ;
    mcu.Data(z, rd | z) ;
    mcu.Reg(nd, r) ;

    return 1 ;
  }
  std::string InstrLAS::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrLAS::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LAC
  InstrLAC::InstrLAC() : Instruction(0b1001001000000110, 0b1111111000001111, "LAC Z,", "Load and Clear Indirect Register and Data Space", false, false, false, false, false)
  {
  }

  InstrLAC::~InstrLAC()
  {
  }

  uint8_t InstrLAC::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t  rd = mcu.Reg(nd) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    uint8_t  r  = mcu.Data(z) ;
    mcu.Data(z, ~rd & r) ;
    mcu.Reg(nd, r) ;

    return 1 ;
  }
  std::string InstrLAC::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrLAC::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LAT
  InstrLAT::InstrLAT() : Instruction(0b1001001000000111, 0b1111111000001111, "LAT Z,", "Load and Toggle Indirect Register and Data Space", false, false, false, false, false)
  {
  }

  InstrLAT::~InstrLAT()
  {
  }

  uint8_t InstrLAT::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t  rd = mcu.Reg(nd) ;
    uint32_t z = mcu.GetRampZ() | mcu.RegW(30) ;
    uint8_t  r  = mcu.Data(z) ;
    mcu.Data(z, rd ^ r) ;
    mcu.Reg(nd, r) ;

    return 1 ;
  }
  std::string InstrLAT::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrLAT::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // LSR
  InstrLSR::InstrLSR() : Instruction(0b1001010000000110, 0b1111111000001111, "LSR", "Logical Shift Right", false, false, false, false, false)
  {
  }

  InstrLSR::~InstrLSR()
  {
  }

  uint8_t InstrLSR::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100000 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd >> 1 ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rd & 0x01)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::C))
      sreg |= SREG::V ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrLSR::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrLSR::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ROR
  InstrROR::InstrROR() : Instruction(0b1001010000000111, 0b1111111000001111, "ROR", "Rotate Right through Carry", false, false, false, false, false)
  {
  }

  InstrROR::~InstrROR()
  {
  }

  uint8_t InstrROR::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg0 = mcu.GetSREG() ;
    uint8_t sreg = sreg0 & 0b11100000 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = rd >> 1 ;
    if (sreg0 && SREG::C)
      r |= 0x80 ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rd & 0x01)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::C))
      sreg |= SREG::V ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrROR::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrROR::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ASR
  InstrASR::InstrASR() : Instruction(0b1001010000000101, 0b1111111000001111, "ASR", "Arithmetic Shift Right", false, false, false, false, false)
  {
  }

  InstrASR::~InstrASR()
  {
  }

  uint8_t InstrASR::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t sreg = mcu.GetSREG() & 0b11100000 ;
    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = (uint8_t)((int8_t)rd >> 1) ;
    if (r & 0x80)
      sreg |= SREG::N ;
    if (r == 0x00)
      sreg |= SREG::Z ;
    if (rd & 0x01)
      sreg |= SREG::C ;
    if ((sreg && SREG::N) ^ (sreg && SREG::C))
      sreg |= SREG::V ;
    if ((sreg && SREG::N) ^ (sreg && SREG::V))
      sreg |= SREG::S ;
    mcu.Reg(nd, r) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrASR::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrASR::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SWAP
  InstrSWAP::InstrSWAP() : Instruction(0b1001010000000010, 0b1111111000001111, "SWAP", "Swap Nibbles", false, false, false, false, false)
  {
  }

  InstrSWAP::~InstrSWAP()
  {
  }

  uint8_t InstrSWAP::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;

    uint8_t rd = mcu.Reg(nd) ;
    uint8_t r = ((rd & 0xf0) >> 4) | ((rd & 0x0f) << 4) ;
    mcu.Reg(nd, r) ;

    return 1 ;
  }
  std::string InstrSWAP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxxxx(*this, cmd) ;
  }
  XrefType InstrSWAP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // BSET
  InstrBSET::InstrBSET() : Instruction(0b1001010000001000, 0b1111111110001111, "BSET", "Bit Set in SREG", false, false, false, false, false)
  {
  }

  InstrBSET::~InstrBSET()
  {
  }

  uint8_t InstrBSET::Execute(Mcu &mcu, Command cmd) const
  {
    Command s ;
    xxxxxxxxxSSSxxxx(cmd, s) ;

    uint8_t sreg = mcu.GetSREG() ;
    sreg |= 1 << s ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrBSET::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxSSSxxxx_BSET(*this, cmd) ;
  }
  XrefType InstrBSET::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // BCLR
  InstrBCLR::InstrBCLR() : Instruction(0b1001010010001000, 0b1111111110001111, "BCLR", "Bit Clear in SREG", false, false, false, false, false)
  {
  }

  InstrBCLR::~InstrBCLR()
  {
  }

  uint8_t InstrBCLR::Execute(Mcu &mcu, Command cmd) const
  {
    Command s ;
    xxxxxxxxxSSSxxxx(cmd, s) ;

    uint8_t sreg = mcu.GetSREG() ;
    sreg &= ~(1 << s) ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrBCLR::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxSSSxxxx_BCLR(*this, cmd) ;
  }
  XrefType InstrBCLR::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SBI
  InstrSBI::InstrSBI() : Instruction(0b1001101000000000, 0b1111111100000000, "SBI", "Set Bit in I/O Register", false, false, false, false, false)
  {
  }

  InstrSBI::~InstrSBI()
  {
  }

  uint8_t InstrSBI::Execute(Mcu &mcu, Command cmd) const
  {
    Command a, b ;
    xxxxxxxxAAAAAxxx(cmd, a) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;
    mcu.Io(a, mcu.Io(a) | (1<<b)) ;

    return (mcu.IsXmega() || mcu.IsTinyReduced()) ? 1 : 2 ;
  }
  std::string InstrSBI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxAAAAABBB(*this, mcu, cmd) ;
  }
  XrefType InstrSBI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // CBI
  InstrCBI::InstrCBI() : Instruction(0b1001100000000000, 0b1111111100000000, "CBI", "Clear Bit in I/O Register", false, false, false, false, false)
  {
  }

  InstrCBI::~InstrCBI()
  {
  }

  uint8_t InstrCBI::Execute(Mcu &mcu, Command cmd) const
  {
    Command a, b ;
    xxxxxxxxAAAAAxxx(cmd, a) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;
    mcu.Io(a, mcu.Io(a) & ~(1<<b)) ;

    return (mcu.IsXmega() || mcu.IsTinyReduced()) ? 1 : 2 ;
  }
  std::string InstrCBI::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxAAAAABBB(*this, mcu, cmd) ;
  }
  XrefType InstrCBI::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // BST
  InstrBST::InstrBST() : Instruction(0b1111101000000000, 0b1111111000001000, "BST", "Bit Store from Bit in Register to T Flag in SREG", false, false, false, false, false)
  {
  }

  InstrBST::~InstrBST()
  {
  }

  uint8_t InstrBST::Execute(Mcu &mcu, Command cmd) const
  {
    Command nr, b ;
    xxxxxxxRRRRRxxxx(cmd, nr) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;

    uint8_t sreg = mcu.GetSREG() & 0b10111111 ;
    if (mcu.Reg(nr) & (1<<b))
      sreg |= SREG::T ;
    mcu.SetSREG(sreg) ;

    return 1 ;
  }
  std::string InstrBST::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxBBB(*this, cmd) ;
  }
  XrefType InstrBST::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // BLD
  InstrBLD::InstrBLD() : Instruction(0b1111100000000000, 0b1111111000001000, "BLD", "Bit Load from T Flag in SREG to Bit  in Register", false, false, false, false, false)
  {
  }

  InstrBLD::~InstrBLD()
  {
  }

  uint8_t InstrBLD::Execute(Mcu &mcu, Command cmd) const
  {
    Command nd, b ;
    xxxxxxxRRRRRxxxx(cmd, nd) ;
    xxxxxxxxxxxxxBBB(cmd, b) ;

    uint8_t d = mcu.Reg(nd) & ~(1<<b) ;
    if (mcu.GetSREG() && SREG::T)
      d |= 1<<b ;
    mcu.Reg(nd, d) ;

    return 1 ;
  }
  std::string InstrBLD::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxDDDDDxBBB(*this, cmd) ;
  }
  XrefType InstrBLD::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // BREAK
  InstrBREAK::InstrBREAK() : Instruction(0b1001010110011000, 0b1111111111111111, "BREAK", "Break", false, false, false, false, false)
  {
  }

  InstrBREAK::~InstrBREAK()
  {
  }

  uint8_t InstrBREAK::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.Break() ;

    return 1 ;
  }
  std::string InstrBREAK::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrBREAK::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // NOP
  InstrNOP::InstrNOP() : Instruction(0b0000000000000000, 0b1111111111111111, "NOP", "No operation", false, false, false, false, false)
  {
  }

  InstrNOP::~InstrNOP()
  {
  }

  uint8_t InstrNOP::Execute(Mcu &mcu, Command cmd) const
  {
    return 1 ;
  }
  std::string InstrNOP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrNOP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // SLEEP
  InstrSLEEP::InstrSLEEP() : Instruction(0b1001010110001000, 0b1111111111111111, "SLEEP", "Sleep", false, false, false, false, false)
  {
  }

  InstrSLEEP::~InstrSLEEP()
  {
  }

  uint8_t InstrSLEEP::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.Sleep() ;

    return 1 ;
  }
  std::string InstrSLEEP::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrSLEEP::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // WDR
  InstrWDR::InstrWDR() : Instruction(0b1001010110101000, 0b1111111111111111, "WDR", "Watchdog Reset", false, false, false, false, false)
  {
  }

  InstrWDR::~InstrWDR()
  {
  }

  uint8_t InstrWDR::Execute(Mcu &mcu, Command cmd) const
  {
    mcu.WDR() ;

    return 1 ;
  }
  std::string InstrWDR::Disasm(Mcu &mcu, Command cmd) const
  {
    return Disasm_xxxxxxxxxxxxxxxx(*this) ;
  }
  XrefType InstrWDR::Xref(Mcu &mcu, Command cmd, uint32_t &addr) const
  {
    return XrefType::none ;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Instruction Instances
  ////////////////////////////////////////////////////////////////////////////////

#define INSTRinst(name) Instr##name instr##name

  ////////////////////////////////////////////////////////////////////////////////
  // Arithmetic and Logic Instructions
  ////////////////////////////////////////////////////////////////////////////////

  INSTRinst(ADD) ;
  INSTRinst(ADC) ;
  INSTRinst(ADIW) ;
  INSTRinst(SUB) ;
  INSTRinst(SUBI) ;
  INSTRinst(SBC) ;
  INSTRinst(SBCI) ;
  INSTRinst(SBIW) ;
  INSTRinst(AND) ;
  INSTRinst(ANDI) ;
  INSTRinst(OR) ;
  INSTRinst(ORI) ;
  INSTRinst(EOR) ;
  INSTRinst(COM) ;
  INSTRinst(NEG) ;
  //INSTRinst(SBR) ; => ORI r, i
  //INSTRinst(CBR) ; => ANDI r, i
  INSTRinst(INC) ;
  INSTRinst(DEC) ;
  //INSTRinst(TST) ; => AND r, r
  //INSTRinst(CLR) => EOR r, r
  //INSTRinst(SER) => LDI r, 0xff
  INSTRinst(MUL) ;
  INSTRinst(MULS) ;
  INSTRinst(MULSU) ;
  INSTRinst(FMUL) ;
  INSTRinst(FMULS) ;
  INSTRinst(FMULSU) ;
  INSTRinst(DES) ;

  ////////////////////////////////////////////////////////////////////////////////
  // Branch Instructions
  ////////////////////////////////////////////////////////////////////////////////

  INSTRinst(RJMP) ;
  INSTRinst(IJMP) ;
  INSTRinst(EIJMP) ;
  INSTRinst(JMP) ;
  INSTRinst(RCALL) ;
  INSTRinst(ICALL) ;
  INSTRinst(EICALL) ;
  INSTRinst(CALL) ;
  INSTRinst(RET) ;
  INSTRinst(RETI) ;
  INSTRinst(CPSE) ;
  INSTRinst(CP) ;
  INSTRinst(CPC) ;
  INSTRinst(CPI) ;
  INSTRinst(SBRC) ;
  INSTRinst(SBRS) ;
  INSTRinst(SBIC) ;
  INSTRinst(SBIS) ;
  INSTRinst(BRBS) ;
  INSTRinst(BRBC) ;

  ////////////////////////////////////////////////////////////////////////////////
  // Data Transfer Instructions
  ////////////////////////////////////////////////////////////////////////////////

  INSTRinst(MOV) ;
  INSTRinst(MOVW) ;
  INSTRinst(LDI) ;
  INSTRinst(LDS) ;
  INSTRinst(LDx1) ;
  INSTRinst(LDx2) ;
  INSTRinst(LDx3) ;
  INSTRinst(LDy1) ;
  INSTRinst(LDy2) ;
  INSTRinst(LDy3) ;
  INSTRinst(LDy4) ;
  INSTRinst(LDz1) ;
  INSTRinst(LDz2) ;
  INSTRinst(LDz3) ;
  INSTRinst(LDz4) ;
  INSTRinst(STS) ;
  INSTRinst(STx1) ;
  INSTRinst(STx2) ;
  INSTRinst(STx3) ;
  INSTRinst(STy1) ;
  INSTRinst(STy2) ;
  INSTRinst(STy3) ;
  INSTRinst(STy4) ;
  INSTRinst(STz1) ;
  INSTRinst(STz2) ;
  INSTRinst(STz3) ;
  INSTRinst(STz4) ;
  INSTRinst(LPM1) ;
  INSTRinst(LPM2) ;
  INSTRinst(LPM3) ;
  INSTRinst(ELPM1) ;
  INSTRinst(ELPM2) ;
  INSTRinst(ELPM3) ;
  INSTRinst(SPM1) ;
  INSTRinst(SPM2) ;
  INSTRinst(IN) ;
  INSTRinst(OUT) ;
  INSTRinst(PUSH) ;
  INSTRinst(POP) ;
  INSTRinst(XCH) ;
  INSTRinst(LAS) ;
  INSTRinst(LAC) ;
  INSTRinst(LAT) ;

  ////////////////////////////////////////////////////////////////////////////////
  // Bit and Bit-test Instructions
  ////////////////////////////////////////////////////////////////////////////////

  INSTRinst(LSR) ;
  INSTRinst(ROR) ;
  INSTRinst(ASR) ;
  INSTRinst(SWAP) ;
  INSTRinst(BSET) ;
  INSTRinst(BCLR) ;
  INSTRinst(SBI) ;
  INSTRinst(CBI) ;
  INSTRinst(BST) ;
  INSTRinst(BLD) ;

  ////////////////////////////////////////////////////////////////////////////////
  // MCU Control Instructions
  ////////////////////////////////////////////////////////////////////////////////

  INSTRinst(BREAK) ;
  INSTRinst(NOP) ;
  INSTRinst(SLEEP) ;
  INSTRinst(WDR) ;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
