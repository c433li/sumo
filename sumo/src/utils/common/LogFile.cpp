//---------------------------------------------------------------------------//
//                        LogFile.cpp -
//  A MsgRetriever - file implementation for message logging
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Mon, 23.06.2003
//  copyright            : (C) 2003 by Daniel Krajzewicz
//  organisation         : IVF/DLR http://ivf.dlr.de
//  email                : Daniel.Krajzewicz@dlr.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//
// $Log$
// Revision 1.2  2003/06/24 14:40:22  dkrajzew
// Error/Warning-prefix added to all messages; endlines are now prompted correctly within log-files
//
// Revision 1.1  2003/06/24 08:16:21  dkrajzew
// LogFile - a MsgRetriever that writes into a files (logs an applications output) added
//
//
/* =========================================================================
 * included modules
 * ======================================================================= */
#include <fstream>
#include <iostream>
#include <string>
#include "LogFile.h"


/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;


/* =========================================================================
 * method definitions
 * ======================================================================= */
LogFile::LogFile(const std::string &name)
{
    myFile.open(name.c_str());
}


LogFile::~LogFile()
{
    myFile.close();
}


void
LogFile::inform(const std::string &msg)
{
    myFile << msg << endl;
}


bool
LogFile::good()
{
    return myFile.good();
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/
//#ifdef DISABLE_INLINE
//#include "LogFile.icc"
//#endif

// Local Variables:
// mode:C++
// End:
