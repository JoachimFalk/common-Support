#ifndef PAR_MANAGER_HPP
#define PAR_MANAGER_HPP

#include <string>
#include <map>
#include <memory>

class par_manager
{
private:
  std::map<std::string, int> config;
  static std::auto_ptr<par_manager> inst;
  
public:
  static const par_manager& instance();
  int count(const std::string& name) const;  

private:
  par_manager();
  par_manager(const par_manager&);
  par_manager& operator=(const par_manager&);
};

#endif // PAR_MANAGER_HPP
