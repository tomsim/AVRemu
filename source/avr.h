////////////////////////////////////////////////////////////////////////////////
// avrEmu.h
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <map>

namespace AVR

{
  class Mcu ;
  class Instruction ;
  class Io ;

  using Command = unsigned short ; // 16 bit instruction
  using int8    = signed   char  ; //  8 bit
  using uint8   = unsigned char  ; //  8 bit
  using int16   = signed   short ; // 16 bit
  using uint16  = unsigned short ; // 16 bit
  using int32   = signed   int   ; // 32 bit
  using uint32  = unsigned int   ; // 32 bit

  class Instruction
  {
  protected:
    Instruction(Command pattern, Command mask, const std::string &mnemonic, const std::string &description) ;
    Instruction() = delete ;
    Instruction& operator=(const Instruction&) = delete ;
    virtual ~Instruction() ;

  public:
    // returns execution time
    virtual std::size_t Execute(Mcu &mcu, Command cmd) const = 0 ;
    virtual std::string Disasm (Mcu &mcu, Command cmd) const = 0 ;
    virtual bool        Xref   (Mcu &mcu, Command cmd, uint32 &addr) const = 0 ;

    virtual Command     Pattern()     const { return _pattern     ; }
    virtual Command     Mask()        const { return _mask        ; }
    virtual std::string Mnemonic()    const { return _mnemonic    ; }
    virtual std::string Description() const { return _description ; }

  protected:
    Command _pattern ;
    Command _mask    ;
    std::string _mnemonic ;
    std::string _description ;
  } ;

  class Io
  {
  public:
    class Register
    {
    public:
      virtual const std::string& Name() const = 0 ;
      virtual uint8  Value() const = 0 ;
      virtual uint8& Value() = 0 ;
      virtual uint8  Init() const = 0 ; // bootup value
    } ;
    
  } ;

  class IoRegisterNotImplemented : public Io::Register
  {
  public:
    IoRegisterNotImplemented(const std::string &name) : _name(name), _value(0) {}
    
    virtual const std::string& Name() const { return _name ; }
    virtual uint8  Value() const { return _value ; }
    virtual uint8& Value() { return _value ; }
    virtual uint8  Init() const { return 0 ; }
  private:
    std::string _name ;
    uint8 _value ;
  } ;
  
  class Mcu
  {
  protected:
    struct Xref
    {
      uint32              _addr ;
      std::string         _label ;
      std::vector<uint32> _addrs ;
    } ;
    
  protected:
    Mcu(std::size_t programSize, std::size_t ioSize, std::size_t dataSize, std::size_t eepromSize) ;
    Mcu() = delete ;
    Mcu& operator=(const Mcu&) = delete ;
  public:
    virtual ~Mcu() ;

  public:
    std::size_t Execute() ;
    std::string Disasm() ;
    bool DataAddrName(uint32 addr, std::string &name) const ;
    Command ProgramNext() ;

    std::size_t  PC() const { return _pc ; }
    std::size_t& PC()       { return _pc ; }

    const std::vector<const Instruction*>& Instructions() const { return _instructions ; }
    const std::vector<Command>&            Program()      const { return _program      ; }

    void   ClearProgram() ;
    size_t SetProgram(size_t address, const std::vector<Command> &prg) ;
    size_t SetEeprom(size_t address, const std::vector<uint8> &eeprom) ;

  protected:
    void AddInstruction(const Instruction *instr) ;
    void AnalyzeXrefs() ;

  protected:
    std::size_t _pc ;

    std::size_t _programSize ;

    std::size_t _regSize, _regStart, _regEnd ;
    std::size_t _ioSize, _ioStart, _ioEnd ;
    std::size_t _dataSize, _dataStart, _dataEnd ;
    std::size_t _eepromSize ;

    std::vector<Command>       _program ;
    std::vector<Io::Register*> _io ;
    std::vector<uint8>         _data ;
    std::vector<uint8>         _eeprom ;
    std::map<uint32, std::string> _knownProgramAddresses ;
    std::map<uint32, Xref>     _xrefs ;

    std::vector<const Instruction*> _instructions ;       // map cmd to instruction
  } ;

  ////////////////////////////////////////////////////////////////////////////////
  // AVR any
  ////////////////////////////////////////////////////////////////////////////////
  
  class ATany : public Mcu
  {
  public:
    ATany() ;
    virtual ~ATany() ;
  } ;

  ////////////////////////////////////////////////////////////////////////////////
  // ATmega48A/PA/88A/PA/168A/PA/328/P
  ////////////////////////////////////////////////////////////////////////////////

  class ATmegaXX8 : public Mcu
  {
  protected:
    ATmegaXX8(std::size_t programSize, std::size_t ioSize, std::size_t dataSize, std::size_t eepromSize) ;
    virtual ~ATmegaXX8() ;
  } ;

  class ATmega328P : public ATmegaXX8
  {
  public:
    ATmega328P() ;
    virtual ~ATmega328P() ;
  } ;
  
  class ATmega168PA : public ATmegaXX8
  {
  public:
    ATmega168PA() ;
    virtual ~ATmega168PA() ;
  } ;
  
  class ATmega88PA : public ATmegaXX8
  {
  public:
    ATmega88PA() ;
    virtual ~ATmega88PA() ;
  } ;
  
  class ATmega48PA : public ATmegaXX8
  {
  public:
    ATmega48PA() ;
    virtual ~ATmega48PA() ;
  } ;

  ////////////////////////////////////////////////////////////////////////////////
  // ATtiny24A/44A/84A
  ////////////////////////////////////////////////////////////////////////////////

  class ATtinyX4 : public Mcu
  {
  protected:
    ATtinyX4(std::size_t programSize, std::size_t ioSize, std::size_t dataSize, std::size_t eepromSize) ;
    virtual ~ATtinyX4() ;
  } ;

  class ATtiny84A : public ATtinyX4
  {
  public:
    ATtiny84A() ;
    virtual ~ATtiny84A() ;
  } ;
  
  class ATtiny44A : public ATtinyX4
  {
  public:
    ATtiny44A() ;
    virtual ~ATtiny44A() ;
  } ;
  
  class ATtiny24A : public ATtinyX4
  {
  public:
    ATtiny24A() ;
    virtual ~ATtiny24A() ;
  } ;
  
  ////////////////////////////////////////////////////////////////////////////////
  // ATtiny25/V/45/V/85/V
  ////////////////////////////////////////////////////////////////////////////////

  class ATtinyX5 : public Mcu
  {
  protected:
    ATtinyX5(std::size_t programSize, std::size_t ioSize, std::size_t dataSize, std::size_t eepromSize) ;
    virtual ~ATtinyX5() ;
  } ;

  class ATtiny85 : public ATtinyX5
  {
  public:
    ATtiny85() ;
    virtual ~ATtiny85() ;
  } ;
  
  class ATtiny45 : public ATtinyX5
  {
  public:
    ATtiny45() ;
    virtual ~ATtiny45() ;
  } ;
  
  class ATtiny25 : public ATtinyX5
  {
  public:
    ATtiny25() ;
    virtual ~ATtiny25() ;
  } ;
  
  ////////////////////////////////////////////////////////////////////////////////
  // ATxmega AU
  ////////////////////////////////////////////////////////////////////////////////

  class ATxmegaAU : public Mcu
  {
  protected:
    ATxmegaAU(std::size_t programSize, std::size_t ioSize, std::size_t dataSize, std::size_t eepromSize) ;
    virtual ~ATxmegaAU() ;
  } ;

  class ATxmega128A4U : public ATxmegaAU
  {
  public:
    ATxmega128A4U() ;
    virtual ~ATxmega128A4U() ;
  } ;
  
  class ATxmega64A4U : public ATxmegaAU
  {
  public:
    ATxmega64A4U() ;
    virtual ~ATxmega64A4U() ;
  } ;
  
  class ATxmega32A4U : public ATxmegaAU
  {
  public:
    ATxmega32A4U() ;
    virtual ~ATxmega32A4U() ;
  } ;
  
  class ATxmega16A4U : public ATxmegaAU
  {
  public:
    ATxmega16A4U() ;
    virtual ~ATxmega16A4U() ;
  } ;
  
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
