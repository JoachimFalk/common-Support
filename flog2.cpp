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

size_t flog2(uint32_t n) {
  assert(n);

  // does five "&"-Operations per number = log2(32)

  if(n & 0xFFFF0000) {
    if(n & 0xFF000000) {
      if(n & 0xF0000000) {
        if(n & 0xC0000000) {
          if(n & 0x80000000) {
            return 32;
          }
          else {
            return 31;
          }
        } else {
          if(n & 0x20000000) {
            return 30;
          }
          else {
            return 29;
          }
        }
      } else {
        if(n & 0x0C000000) {
          if(n & 0x08000000) {
            return 28;
          }
          else {
            return 27;
          }
        } else {
          if(n & 0x02000000) {
            return 26;
          }
          else {
            return 25;
          }
        }
      }
    } else {
      if(n & 0x00F00000) {
        if(n & 0x00C00000) {
          if(n & 0x00800000) {
            return 24;
          }
          else {
            return 23;
          }
        } else {
          if(n & 0x00200000) {
            return 22;
          }
          else {
            return 21;
          }
        }
      } else {
        if(n & 0x000C0000) {
          if(n & 0x00080000) {
            return 20;
          }
          else {
            return 19;
          }
        } else {
          if(n & 0x00020000) {
            return 18;
          }
          else {
            return 17;
          }
        }
      }
    }
  } else {
    if(n & 0x0000FF00) {
      if(n & 0x0000F000) {
        if(n & 0x0000C000) {
          if(n & 0x00008000) {
            return 16;
          }
          else {
            return 15;
          }
        } else {
          if(n & 0x00002000) {
            return 14;
          }
          else {
            return 13;
          }
        }
      } else {
        if(n & 0x00000C00) {
          if(n & 0x00000800) {
            return 12;
          }
          else {
            return 11;
          }
        } else {
          if(n & 0x00000200) {
            return 10;
          }
          else {
            return 9;
          }
        }
      }
    } else {
      if(n & 0x000000F0) {
        if(n & 0x000000C0) {
          if(n & 0x00000080) {
            return 8;
          }
          else {
            return 7;
          }
        } else {
          if(n & 0x00000020) {
            return 6;
          }
          else {
            return 5;
          }
        }
      } else {
        if(n & 0x0000000C) {
          if(n & 0x00000008) {
            return 4;
          }
          else {
            return 3;
          }
        } else {
          if(n & 0x00000002) {
            return 2;
          }
          else {
            return 1;
          }
        }
      }
    }
  }
}

size_t flog2(uint64_t n) {
  assert(n);

  // does six "&"-Operations per number = log2(64)

  if(n & 0xFFFFFFFF00000000ull) {
    if(n & 0xFFFF000000000000ull) {
      if(n & 0xFF00000000000000ull) {
        if(n & 0xF000000000000000ull) {
          if(n & 0xC000000000000000ull) {
            if(n & 0x8000000000000000ull) {
              return 64;
            }
            else {
              return 63;
            }
          } else {
            if(n & 0x2000000000000000ull) {
              return 62;
            }
            else {
              return 61;
            }
          }
        } else {
          if(n & 0x0C00000000000000ull) {
            if(n & 0x0800000000000000ull) {
              return 60;
            }
            else {
              return 59;
            }
          } else {
            if(n & 0x0200000000000000ull) {
              return 58;
            }
            else {
              return 57;
            }
          }
        }
      } else {
        if(n & 0x00F0000000000000ull) {
          if(n & 0x00C0000000000000ull) {
            if(n & 0x0080000000000000ull) {
              return 56;
            }
            else {
              return 55;
            }
          } else {
            if(n & 0x0020000000000000ull) {
              return 54;
            }
            else {
              return 53;
            }
          }
        } else {
          if(n & 0x000C000000000000ull) {
            if(n & 0x0008000000000000ull) {
              return 52;
            }
            else {
              return 51;
            }
          } else {
            if(n & 0x0002000000000000ull) {
              return 50;
            }
            else {
              return 49;
            }
          }
        }
      }
    } else {
      if(n & 0x0000FF0000000000ull) {
        if(n & 0x0000F00000000000ull) {
          if(n & 0x0000C00000000000ull) {
            if(n & 0x0000800000000000ull) {
              return 48;
            }
            else {
              return 47;
            }
          } else {
            if(n & 0x0000200000000000ull) {
              return 46;
            }
            else {
              return 45;
            }
          }
        } else {
          if(n & 0x00000C0000000000ull) {
            if(n & 0x0000080000000000ull) {
              return 44;
            }
            else {
              return 43;
            }
          } else {
            if(n & 0x0000020000000000ull) {
              return 42;
            }
            else {
              return 41;
            }
          }
        }
      } else {
        if(n & 0x000000F000000000ull) {
          if(n & 0x000000C000000000ull) {
            if(n & 0x0000008000000000ull) {
              return 40;
            }
            else {
              return 39;
            }
          } else {
            if(n & 0x0000002000000000ull) {
              return 38;
            }
            else {
              return 37;
            }
          }
        } else {
          if(n & 0x0000000C00000000ull) {
            if(n & 0x0000000800000000ull) {
              return 36;
            }
            else {
              return 35;
            }
          } else {
            if(n & 0x0000000200000000ull) {
              return 34;
            }
            else {
              return 33;
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
              return 32;
            }
            else {
              return 31;
            }
          } else {
            if(n & 0x0000000020000000ull) {
              return 30;
            }
            else {
              return 29;
            }
          }
        } else {
          if(n & 0x000000000C000000ull) {
            if(n & 0x0000000008000000ull) {
              return 28;
            }
            else {
              return 27;
            }
          } else {
            if(n & 0x0000000002000000ull) {
              return 26;
            }
            else {
              return 25;
            }
          }
        }
      } else {
        if(n & 0x0000000000F00000ull) {
          if(n & 0x0000000000C00000ull) {
            if(n & 0x0000000000800000ull) {
              return 24;
            }
            else {
              return 23;
            }
          } else {
            if(n & 0x0000000000200000ull) {
              return 22;
            }
            else {
              return 21;
            }
          }
        } else {
          if(n & 0x00000000000C0000ull) {
            if(n & 0x0000000000080000ull) {
              return 20;
            }
            else {
              return 19;
            }
          } else {
            if(n & 0x0000000000020000ull) {
              return 18;
            }
            else {
              return 17;
            }
          }
        }
      }
    } else {
      if(n & 0x000000000000FF00ull) {
        if(n & 0x000000000000F000ull) {
          if(n & 0x000000000000C000ull) {
            if(n & 0x0000000000008000ull) {
              return 16;
            }
            else {
              return 15;
            }
          } else {
            if(n & 0x0000000000002000ull) {
              return 14;
            }
            else {
              return 13;
            }
          }
        } else {
          if(n & 0x0000000000000C00ull) {
            if(n & 0x0000000000000800ull) {
              return 12;
            }
            else {
              return 11;
            }
          } else {
            if(n & 0x0000000000000200ull) {
              return 10;
            }
            else {
              return 9;
            }
          }
        }
      } else {
        if(n & 0x00000000000000F0ull) {
          if(n & 0x00000000000000C0ull) {
            if(n & 0x0000000000000080ull) {
              return 8;
            }
            else {
              return 7;
            }
          } else {
            if(n & 0x0000000000000020ull) {
              return 6;
            }
            else {
              return 5;
            }
          }
        } else {
          if(n & 0x000000000000000Cull) {
            if(n & 0x0000000000000008ull) {
              return 4;
            }
            else {
              return 3;
            }
          } else {
            if(n & 0x0000000000000002ull) {
              return 2;
            }
            else {
              return 1;
            }
          }
        }
      }
    }
  }    
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

