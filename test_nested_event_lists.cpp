/*
 * Copyright (c) 2004-2006 Hardware-Software-CoDesign, University of
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

#include <iostream>

#include <CoSupport/SystemC/systemc_support.hpp>
#include <CoSupport/Streams/DebugOStream.hpp>

using namespace CoSupport::SystemC;
using namespace CoSupport;

using CoSupport::Streams::Color;

int sc_main(int argc, char *argv[]) {
  CoSupport::Streams::DebugOStream out(cout);

  {
    Event e0, e1;
    EventOrList<EventWaiter>  orList;
    EventAndList<EventWaiter> andList;
    
    assert(andList);            // empty andList is active per default
    assert(!orList);            // empty orList is NOT active per default
    
    orList          |= andList;
    assert(orList);             // andList is active: orList has to be active
    
    andList       &= e0;
    assert(!andList);           // event NOT active: andList NOT active
    
    andList       &= e1;
    assert(!andList);           // event NOT active: andList NOT active
    
    notify(e1);
    assert(e1);
    assert(!andList);           // event NOT active: andList NOT active
    
    notify(e0);
    assert(e0);
    assert(andList);            // all events active: andList has to be active

    reset(e1);
    assert(!e1);
    assert(!andList);           // event reseted: andList NOT active
  }

  {
    Event e0, e1;
    EventOrList<EventWaiter>  orList;
    EventAndList<EventWaiter> andList;
    
    assert(andList);            // empty andList is active per default
    assert(!orList);            // empty orList is NOT active per default
    
    orList          |= andList;
    assert(orList);             // andList is active: orList has to be active
    
    andList       &= e0;
    assert(!andList);           // event NOT active: andList NOT active

    notify(e1);
    andList       &= e1;
    assert(!andList);           // event NOT active: andList NOT active
    
    notify(e0);
    assert(e1);
    assert(andList);           // event NOT active: andList NOT active
    
    notify(e0);
    assert(e0);
    assert(andList);            // all events active: andList has to be active

    reset(e1);
    assert(!e1);
    assert(!andList);           // event reseted: andList NOT active
  }

  {
    Event e0, e1;
    EventOrList<EventWaiter>  orList0, orList1;
    EventAndList<EventWaiter> andList;
    
    assert(andList);            // empty andList is active per default

    orList0       |= e0;
    assert(!orList0);           // event NOT active: orList NOT active
    orList1       |= e1;
    assert(!orList1);           // event NOT active: orList NOT active
    
    notify(e0);
    assert(e0);
    notify(e1);
    assert(e1);

    assert(orList0);           // event active: orList active
    assert(orList1);           // event active: orList active

    andList       &= orList0;
    andList       &= orList1;
    assert(andList);           // both events active: andList active

    orList0.remove(e0);
    assert(!andList);          // the or list is NOT active: andList NOT active

    orList0       |= e0;
    assert(andList);           // both events active: andList active
  }

  {
    Event e0, e1, e2, e3;

    EventOrList<EventWaiter> orList = e0 | e1 | e2 | e3;
    EventAndList<EventWaiter> andList;

    andList        &= orList;

    notify(e0);
    notify(e1);
    assert(orList);
    assert(andList);

    orList.remove(e0);
    assert(orList);
    assert(andList);

    orList.remove(e1);
    assert(!orList);
    assert(!andList);
  }

  {
    EventOrList<EventWaiter>  topMoC;
    EventAndList<EventWaiter> graphScheduler0;
    EventOrList<EventWaiter>  graphMoC;
    EventAndList<EventWaiter> graphTrans0, graphTrans1;
    
    Event ap00, ap01, ap10, ap11;
    assert(!ap00); // events are NOT active by default
    assert(!ap01); // events are NOT active by default
    assert(!ap10); // events are NOT active by default
    assert(!ap11); // events are NOT active by default
    
    assert(graphTrans0); // it is empty

    graphTrans0       &= ap00;
    assert(!graphTrans0); // has an inactive event

    graphTrans0       &= ap01;
    assert(!graphTrans0); // has two inactive events
    
    graphTrans1       &= ap10;
    graphTrans1       &= ap11;
    assert(!graphTrans1); // has two inactive events

    assert(!graphMoC);    // it is empty
    graphMoC          |= graphTrans0;
    assert(!graphMoC);    // has an inactive and_list

    graphMoC          |= graphTrans1;
    assert(!graphMoC);    // has two inactive and_lists

   
    assert(graphScheduler0);      // it is empty
    graphScheduler0   &= graphMoC;
    assert(!graphScheduler0);     // at least one inactive or_list
    
    assert(!topMoC);              // it is empty
    topMoC            |= graphScheduler0;
    assert(!topMoC);              // has an inactive and_list
    
    notify(ap00);
    assert(ap00);
    assert(!graphTrans0); // still one event missing

    notify(ap01);
    assert(ap01);
    assert(graphTrans0);  // all and´ed events are active

    assert(graphMoC);     //  or´ed one active and on inactive and_list

    notify(ap10);
    assert(ap10);
    assert(!graphTrans1); // still one event missing

    notify(ap11);
    assert(ap11);
    assert(graphTrans1);  // all and´ed events are active

    assert(graphMoC);     //  or´ed two active and_lists

    assert(graphScheduler0); // and´ed one active or_list
    
    assert(topMoC);          // or´ed one active and_list

    topMoC.remove(graphScheduler0);
    assert(!topMoC);                 // it is empty
    assert(graphScheduler0);         // still active
    
    graphScheduler0.remove(graphMoC);
    assert(graphScheduler0);        // it is empty
    assert(graphMoC);              //  still active

    graphScheduler0   &= graphMoC;
    assert(graphScheduler0);

    topMoC            |= graphScheduler0;
    assert(topMoC);


    /*
      out << topMoC << endl;
      if(topMoC) out << "Yes! topMoC is active!\n" << endl;
      else       out << "Not active! :(\n"         << endl;
    */


  }

  { // build from bottom up
    EventOrList<EventWaiter>  topMoC;
    EventAndList<EventWaiter> graphScheduler0;
    EventOrList<EventWaiter>  graphMoC;
    EventAndList<EventWaiter> graphTrans0, graphTrans1;
    
    Event ap00, ap01, ap10, ap11;
    assert(!ap00); // events are NOT active by default
    assert(!ap01); // events are NOT active by default
    assert(!ap10); // events are NOT active by default
    assert(!ap11); // events are NOT active by default
    
    assert(graphTrans0);     // it is empty
    assert(graphTrans1);     // it is empty
    assert(!graphMoC);       // it is empty
    assert(graphScheduler0); // it is empty
    assert(!topMoC);         // it is empty

    graphTrans0       &= ap00;
    graphTrans0       &= ap01;

    graphTrans1       &= ap10;
    graphTrans1       &= ap11;

    graphMoC          |= graphTrans0;
    graphMoC          |= graphTrans1;

    graphScheduler0   &= graphMoC;

    topMoC            |= graphScheduler0;

    assert(!graphTrans0); // has an inactive event
    assert(!graphTrans1); // has two inactive events

    assert(!graphMoC);    // has two inactive and_lists
    assert(!graphScheduler0);     // at least one inactive or_list
    assert(!topMoC);              // has an inactive and_list




    
    notify(ap00);
    assert(ap00);
    assert(!graphTrans0); // still one event missing

    notify(ap01);
    assert(ap01);
    assert(graphTrans0);  // all and´ed events are active

    assert(graphMoC);     //  or´ed one active and on inactive and_list

    notify(ap10);
    assert(ap10);
    assert(!graphTrans1); // still one event missing

    notify(ap11);
    assert(ap11);
    assert(graphTrans1);  // all and´ed events are active

    assert(graphMoC);     //  or´ed two active and_lists

    assert(graphScheduler0); // and´ed one active or_list
    
    assert(topMoC);          // or´ed one active and_list

    topMoC.remove(graphScheduler0);
    assert(!topMoC);                 // it is empty
    assert(graphScheduler0);         // still active
    
    graphScheduler0.remove(graphMoC);
    assert(graphScheduler0);        // it is empty
    assert(graphMoC);              //  still active

    graphScheduler0   &= graphMoC;
    assert(graphScheduler0);

    topMoC            |= graphScheduler0;
    assert(topMoC);


    /*
      out << topMoC << endl;
      if(topMoC) out << "Yes! topMoC is active!\n" << endl;
      else       out << "Not active! :(\n"         << endl;
    */


  }


  { // mixed order
    EventOrList<EventWaiter>  topMoC;
    EventAndList<EventWaiter> graphScheduler0;
    EventOrList<EventWaiter>  graphMoC;
    EventAndList<EventWaiter> graphTrans0, graphTrans1;
    
    Event ap00, ap01, ap10, ap11;
    assert(!ap00); // events are NOT active by default
    assert(!ap01); // events are NOT active by default
    assert(!ap10); // events are NOT active by default
    assert(!ap11); // events are NOT active by default
    
    assert(graphTrans0);     // it is empty
    assert(graphTrans1);     // it is empty
    assert(!graphMoC);       // it is empty
    assert(graphScheduler0); // it is empty
    assert(!topMoC);         // it is empty


    topMoC            |= graphScheduler0;

    graphMoC          |= graphTrans0;
    graphMoC          |= graphTrans1;

    graphScheduler0   &= graphMoC;

    graphTrans0       &= ap00;
    graphTrans0       &= ap01;

    graphTrans1       &= ap10;
    graphTrans1       &= ap11;

    assert(!graphTrans0); // has an inactive event
    assert(!graphTrans1); // has two inactive events

    assert(!graphMoC);    // has two inactive and_lists
    assert(!graphScheduler0);     // at least one inactive or_list
    assert(!topMoC);              // has an inactive and_list


    ////
    notify(ap00);
    assert(ap00);
    assert(!graphTrans0); // still one event missing

    notify(ap01);
    assert(ap01);
    assert(graphTrans0);  // all and´ed events are active

    assert(graphMoC);     //  or´ed one active and on inactive and_list

    notify(ap10);
    assert(ap10);
    assert(!graphTrans1); // still one event missing

    notify(ap11);
    assert(ap11);
    assert(graphTrans1);  // all and´ed events are active

    assert(graphMoC);     //  or´ed two active and_lists

    assert(graphScheduler0); // and´ed one active or_list
    
    assert(topMoC);          // or´ed one active and_list

    topMoC.remove(graphScheduler0);
    assert(!topMoC);                 // it is empty
    assert(graphScheduler0);         // still active
    
    graphScheduler0.remove(graphMoC);
    assert(graphScheduler0);        // it is empty
    assert(graphMoC);              //  still active

    graphScheduler0   &= graphMoC;
    assert(graphScheduler0);

    topMoC            |= graphScheduler0;
    assert(topMoC);


    /*
    out << topMoC << endl;
    if(topMoC) out << "Yes! It is active!\n" << endl;
    else       out << "Not active! :(\n"         << endl;
    */


  }


  { // build from top down (inverse order)
    EventOrList<EventWaiter>  topMoC;
    EventAndList<EventWaiter> graphScheduler0;
    EventOrList<EventWaiter>  graphMoC;
    EventAndList<EventWaiter> graphTrans0, graphTrans1;
    
    Event ap00, ap01, ap10, ap11;
    assert(!ap00); // events are NOT active by default
    assert(!ap01); // events are NOT active by default
    assert(!ap10); // events are NOT active by default
    assert(!ap11); // events are NOT active by default
    
    assert(graphTrans0);     // it is empty
    assert(graphTrans1);     // it is empty
    assert(!graphMoC);       // it is empty
    assert(graphScheduler0); // it is empty
    assert(!topMoC);         // it is empty


    topMoC            |= graphScheduler0;

    graphScheduler0   &= graphMoC;

    graphMoC          |= graphTrans0;
    graphMoC          |= graphTrans1;

    graphTrans0       &= ap00;
    graphTrans0       &= ap01;

    graphTrans1       &= ap10;
    graphTrans1       &= ap11;

    assert(!graphTrans0); // has an inactive event
    assert(!graphTrans1); // has two inactive events

    assert(!graphMoC);    // has two inactive and_lists
    assert(!graphScheduler0);     // at least one inactive or_list
    assert(!topMoC);              // has an inactive and_list


    ////
    notify(ap00);
    assert(ap00);
    assert(!graphTrans0); // still one event missing

    notify(ap01);
    assert(ap01);
    assert(graphTrans0);  // all and´ed events are active

    assert(graphMoC);     //  or´ed one active and on inactive and_list

    notify(ap10);
    assert(ap10);
    assert(!graphTrans1); // still one event missing

    notify(ap11);
    assert(ap11);
    assert(graphTrans1);  // all and´ed events are active

    assert(graphMoC);     //  or´ed two active and_lists

    assert(graphScheduler0); // and´ed one active or_list
    
    assert(topMoC);          // or´ed one active and_list

    topMoC.remove(graphScheduler0);
    assert(!topMoC);                 // it is empty
    assert(graphScheduler0);         // still active
    
    graphScheduler0.remove(graphMoC);
    assert(graphScheduler0);        // it is empty
    assert(graphMoC);              //  still active

    graphScheduler0   &= graphMoC;
    assert(graphScheduler0);

    topMoC            |= graphScheduler0;
    assert(topMoC);


    /*
    out << topMoC << endl;
    if(topMoC) out << "Yes! It is active!\n" << endl;
    else       out << "Not active! :(\n"         << endl;
    */


  }


  out << Color::Green() << "OK: Passed all tests!" << Color::Auto() << std::endl;

  return 0;
}
