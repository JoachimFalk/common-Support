/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <CoSupport/Math/flog2.hpp>

#include <cmath>
#include <cassert>

namespace CoSupport {

// if someone can come up with something better,
// a) benchmark it and b) if it's faster, put it below!!!

size_t flog2f(uint32_t n) {
  assert(n >= 1);

  // does five "&"-Operations per number = log2(32)

  if(n & 0xFFFF0000) {
    if(n & 0xFF000000) {
      if(n & 0xF0000000) {
        if(n & 0xC0000000) {
          if(n & 0x80000000) {
            return 31;
          }
          else {
            return 30;
          }
        } else {
          if(n & 0x20000000) {
            return 29;
          }
          else {
            return 28;
          }
        }
      } else {
        if(n & 0x0C000000) {
          if(n & 0x08000000) {
            return 27;
          }
          else {
            return 26;
          }
        } else {
          if(n & 0x02000000) {
            return 25;
          }
          else {
            return 24;
          }
        }
      }
    } else {
      if(n & 0x00F00000) {
        if(n & 0x00C00000) {
          if(n & 0x00800000) {
            return 23;
          }
          else {
            return 22;
          }
        } else {
          if(n & 0x00200000) {
            return 21;
          }
          else {
            return 20;
          }
        }
      } else {
        if(n & 0x000C0000) {
          if(n & 0x00080000) {
            return 19;
          }
          else {
            return 18;
          }
        } else {
          if(n & 0x00020000) {
            return 17;
          }
          else {
            return 16;
          }
        }
      }
    }
  } else {
    if(n & 0x0000FF00) {
      if(n & 0x0000F000) {
        if(n & 0x0000C000) {
          if(n & 0x00008000) {
            return 15;
          }
          else {
            return 14;
          }
        } else {
          if(n & 0x00002000) {
            return 13;
          }
          else {
            return 12;
          }
        }
      } else {
        if(n & 0x00000C00) {
          if(n & 0x00000800) {
            return 11;
          }
          else {
            return 10;
          }
        } else {
          if(n & 0x00000200) {
            return 9;
          }
          else {
            return 8;
          }
        }
      }
    } else {
      if(n & 0x000000F0) {
        if(n & 0x000000C0) {
          if(n & 0x00000080) {
            return 7;
          }
          else {
            return 6;
          }
        } else {
          if(n & 0x00000020) {
            return 5;
          }
          else {
            return 4;
          }
        }
      } else {
        if(n & 0x0000000C) {
          if(n & 0x00000008) {
            return 3;
          }
          else {
            return 2;
          }
        } else {
          if(n & 0x00000002) {
            return 1;
          }
          else {
            return 0;
          }
        }
      }
    }
  }
}

size_t flog2f(uint64_t n) {
  assert(n >= 1);

  // does six "&"-Operations per number = log2(64)

  if(n & 0xFFFFFFFF00000000ull) {
    if(n & 0xFFFF000000000000ull) {
      if(n & 0xFF00000000000000ull) {
        if(n & 0xF000000000000000ull) {
          if(n & 0xC000000000000000ull) {
            if(n & 0x8000000000000000ull) {
              return 63;
            }
            else {
              return 62;
            }
          } else {
            if(n & 0x2000000000000000ull) {
              return 61;
            }
            else {
              return 60;
            }
          }
        } else {
          if(n & 0x0C00000000000000ull) {
            if(n & 0x0800000000000000ull) {
              return 59;
            }
            else {
              return 58;
            }
          } else {
            if(n & 0x0200000000000000ull) {
              return 57;
            }
            else {
              return 56;
            }
          }
        }
      } else {
        if(n & 0x00F0000000000000ull) {
          if(n & 0x00C0000000000000ull) {
            if(n & 0x0080000000000000ull) {
              return 55;
            }
            else {
              return 54;
            }
          } else {
            if(n & 0x0020000000000000ull) {
              return 53;
            }
            else {
              return 52;
            }
          }
        } else {
          if(n & 0x000C000000000000ull) {
            if(n & 0x0008000000000000ull) {
              return 51;
            }
            else {
              return 50;
            }
          } else {
            if(n & 0x0002000000000000ull) {
              return 49;
            }
            else {
              return 48;
            }
          }
        }
      }
    } else {
      if(n & 0x0000FF0000000000ull) {
        if(n & 0x0000F00000000000ull) {
          if(n & 0x0000C00000000000ull) {
            if(n & 0x0000800000000000ull) {
              return 47;
            }
            else {
              return 46;
            }
          } else {
            if(n & 0x0000200000000000ull) {
              return 45;
            }
            else {
              return 44;
            }
          }
        } else {
          if(n & 0x00000C0000000000ull) {
            if(n & 0x0000080000000000ull) {
              return 43;
            }
            else {
              return 42;
            }
          } else {
            if(n & 0x0000020000000000ull) {
              return 41;
            }
            else {
              return 40;
            }
          }
        }
      } else {
        if(n & 0x000000F000000000ull) {
          if(n & 0x000000C000000000ull) {
            if(n & 0x0000008000000000ull) {
              return 39;
            }
            else {
              return 38;
            }
          } else {
            if(n & 0x0000002000000000ull) {
              return 37;
            }
            else {
              return 36;
            }
          }
        } else {
          if(n & 0x0000000C00000000ull) {
            if(n & 0x0000000800000000ull) {
              return 35;
            }
            else {
              return 34;
            }
          } else {
            if(n & 0x0000000200000000ull) {
              return 33;
            }
            else {
              return 32;
            }
          }
        }
      }
    }
  } else {
    if(n & 0x00000000FFFF0000ull) {
      if(n & 0x00000000FF000000ull) {
        if(n & 0x00000000F0000000ull) {
          if(n & 0x00000000C0000000ull) {
            if(n & 0x0000000080000000ull) {
              return 31;
            }
            else {
              return 30;
            }
          } else {
            if(n & 0x0000000020000000ull) {
              return 29;
            }
            else {
              return 28;
            }
          }
        } else {
          if(n & 0x000000000C000000ull) {
            if(n & 0x0000000008000000ull) {
              return 27;
            }
            else {
              return 26;
            }
          } else {
            if(n & 0x0000000002000000ull) {
              return 25;
            }
            else {
              return 24;
            }
          }
        }
      } else {
        if(n & 0x0000000000F00000ull) {
          if(n & 0x0000000000C00000ull) {
            if(n & 0x0000000000800000ull) {
              return 23;
            }
            else {
              return 22;
            }
          } else {
            if(n & 0x0000000000200000ull) {
              return 21;
            }
            else {
              return 20;
            }
          }
        } else {
          if(n & 0x00000000000C0000ull) {
            if(n & 0x0000000000080000ull) {
              return 19;
            }
            else {
              return 18;
            }
          } else {
            if(n & 0x0000000000020000ull) {
              return 17;
            }
            else {
              return 16;
            }
          }
        }
      }
    } else {
      if(n & 0x000000000000FF00ull) {
        if(n & 0x000000000000F000ull) {
          if(n & 0x000000000000C000ull) {
            if(n & 0x0000000000008000ull) {
              return 15;
            }
            else {
              return 14;
            }
          } else {
            if(n & 0x0000000000002000ull) {
              return 13;
            }
            else {
              return 12;
            }
          }
        } else {
          if(n & 0x0000000000000C00ull) {
            if(n & 0x0000000000000800ull) {
              return 11;
            }
            else {
              return 10;
            }
          } else {
            if(n & 0x0000000000000200ull) {
              return 9;
            }
            else {
              return 8;
            }
          }
        }
      } else {
        if(n & 0x00000000000000F0ull) {
          if(n & 0x00000000000000C0ull) {
            if(n & 0x0000000000000080ull) {
              return 7;
            }
            else {
              return 6;
            }
          } else {
            if(n & 0x0000000000000020ull) {
              return 5;
            }
            else {
              return 4;
            }
          }
        } else {
          if(n & 0x000000000000000Cull) {
            if(n & 0x0000000000000008ull) {
              return 3;
            }
            else {
              return 2;
            }
          } else {
            if(n & 0x0000000000000002ull) {
              return 1;
            }
            else {
              return 0;
            }
          }
        }
      }
    }
  }    
}

size_t flog2c(uint32_t n) {
  if(n == 1)
    return 1;
  return flog2f(n-1)+1;
}

size_t flog2c(uint64_t n) {
  if(n == 1)
    return 1;
  return flog2f(n-1)+1;
}

} // namespace CoSupport

// #include <iostream>
/*int main(int argc, char** argv) {
  assert(argc == 2);
  uint64_t count = atoll(argv[1]);

  size_t x = 0;

  for(; count > 0; --count) {
    x += flog2(count);
  }

  std::cout << x << std::endl;

  return 0;
}*/

