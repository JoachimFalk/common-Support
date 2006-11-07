
#include "smoc_ostream.hpp"

smoc_ostream::smoc_ostream (std::ostream &os)
  : std::basic_ostream<char, std::char_traits<char> >(os.rdbuf()),
  std_os(os), indent_number(0), begin_of_line(true)
{
}

void smoc_ostream::indent(void){
  if (begin_of_line){
    for (unsigned int i = 0; i < indent_number; i++)
      std_os.put(' ');
    begin_of_line = false;
  }
}

smoc_ostream&  smoc_ostream::put ( char ch ) {
  if (ch == '\n'){
    begin_of_line = true;
  }else{
    indent();
  }
  std_os.put(ch);
  return *this;
}

smoc_ostream&  smoc_ostream::write ( const char* str , std::streamsize n ) {
  for (std::streamsize i = 0; i < n; i++)
    std_os.put(str[i]);
  return *this;
}

smoc_ostream& smoc_ostream::operator<<(smoc_ostream& (*m)(smoc_ostream&)){
  //without this method, the endl manipulator cannot be modified!!!
  return (*m)(*this);
}

std::streamsize smoc_ostream::width ( ) const{
	return std_os.width();
}
std::streamsize smoc_ostream::width ( std::streamsize wide ){
	return std_os.width(wide);
}

smoc_ostream& endl(smoc_ostream& os){
  os << '\n';
  os.flush();
  return os;
}

smoc_ostream& inc_level(smoc_ostream& os){ 
  os.indent_number++;
  return os;
}

smoc_ostream& dec_level(smoc_ostream& os){ 
  if (os.indent_number > 0)
    os.indent_number--;
  return os;
}


smoc_ostream& smoc_ostream::operator<< (bool val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (short val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (unsigned short val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (int val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (unsigned int val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (long val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (unsigned long val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (float val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (double val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (long double val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (void* val ){
  indent();
  std_os << val;
  return *this;
}

/*
smoc_ostream& smoc_ostream::operator<< (std::streambuf& sb ){
  //do not indent
  std_os << sb;
  return *this;
}
*/

smoc_ostream& smoc_ostream::operator<< (std::ios& ( *pf )(std::ios&)){
  //do not indent
  std_os << (pf);
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (std::ios_base& ( *pf )(std::ios_base&)){
  //do not indent
  std_os << (pf);
  return *this;
}


smoc_ostream& operator<< (smoc_ostream& os, char ch ){
  return os.put(ch);
}

smoc_ostream& operator<< (smoc_ostream& os, signed char ch ){
  return os.put(ch);
}

smoc_ostream& operator<< (smoc_ostream& os, unsigned char ch ){
  return os.put(ch);
}
 
smoc_ostream& operator<< (smoc_ostream& os, const char* str ){
  int i = 0;
  while (str[i] != 0){
    os<<str[i];
    i++;
  };
  return os;
}

smoc_ostream& operator<< (smoc_ostream& os, const signed char* str ){
  int i = 0;
  while (str[i] != 0){
    os<<str[i];
    i++;
  };
  return os;
}

smoc_ostream& operator<< (smoc_ostream& os, const unsigned char* str ){
  int i = 0;
  while (str[i] != 0){
    os<<str[i];
    i++;
  };
  return os;
}

smoc_ostream& operator<< (smoc_ostream& os, const std::string& str ){
  for(unsigned int i = 0; i < str.length(); i++){
    os << str[i];
  }
  return os;
}

smoc_ostream& operator<< (smoc_ostream& os, std::string& str ){
  for(unsigned int i = 0; i < str.length(); i++){
    os << str[i];
  }
  return os;
}

