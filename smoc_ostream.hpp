#ifndef SMOC_OSTREAM_HPP
#define SMOC_OSTREAM_HPP

#include <iostream>
#include <ios>
#include <string>

class smoc_ostream : public std::basic_ostream<char, std::char_traits<char> > {
public:
  smoc_ostream (std::ostream &os); 

public:
  virtual smoc_ostream&  put ( char ch );

  virtual smoc_ostream&  write ( const char* str , std::streamsize n );  

public:
  //Manipulators
  friend smoc_ostream& endl(smoc_ostream& os);

  friend smoc_ostream& inc_level(smoc_ostream& os);
  friend smoc_ostream& dec_level(smoc_ostream& os);

	virtual std::streamsize width ( ) const;
	virtual std::streamsize width ( std::streamsize wide );

public:

  virtual smoc_ostream& operator<< (bool val );

  virtual smoc_ostream& operator<< (short val );
  virtual smoc_ostream& operator<< (unsigned short val );
  virtual smoc_ostream& operator<< (int val );
  virtual smoc_ostream& operator<< (unsigned int val );  
  virtual smoc_ostream& operator<< (long val);
  virtual smoc_ostream& operator<< (unsigned long val );

  virtual smoc_ostream& operator<< (float val );
  virtual smoc_ostream& operator<< (double val );
  virtual smoc_ostream& operator<< (long double val );

  virtual smoc_ostream& operator<< (void* val );

  //virtual smoc_ostream& operator<< (std::streambuf& sb );
  
  virtual smoc_ostream& operator<<(smoc_ostream& (*m)(smoc_ostream&));
  //without this method, the endl manipulator cannot be modified!!!
  

  virtual smoc_ostream& operator<< (std::ios& ( *pf )(std::ios&));
  virtual smoc_ostream& operator<< (std::ios_base& ( *pf )(std::ios_base&));

 
  friend smoc_ostream& operator<< (smoc_ostream& os, char ch );
  friend smoc_ostream& operator<< (smoc_ostream& os, signed char ch );
  friend smoc_ostream& operator<< (smoc_ostream& os, unsigned char ch );
 
  friend smoc_ostream& operator<< (smoc_ostream& os, const char* str );
  friend smoc_ostream& operator<< (smoc_ostream& os, const signed char* str );
  friend smoc_ostream& operator<< (smoc_ostream& os, const unsigned char* str );

  friend smoc_ostream& operator<< (smoc_ostream& os, const std::string& str );
  friend smoc_ostream& operator<< (smoc_ostream& os, std::string& str );

private:
  std::ostream &std_os;

  unsigned int indent_number;
  bool begin_of_line;

private:
  virtual void indent(void);

};


#endif
