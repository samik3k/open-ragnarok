/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
	Author: kR105
*/
#include "stdafx.h"

#include "ronet/packets/pkt_loginerror.h"

ronet::pktLoginError::pktLoginError() : Packet(pktLoginErrorID) {
	errorId = 0;
}

ronet::pktLoginError::~pktLoginError() {
	// D:
}

void ronet::pktLoginError::Dump() {
	ronet::Packet::Dump();
}

bool ronet::pktLoginError::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);

	unsigned char result;
	result = buf.getBuffer()[2];

	if (result != 6) { //eAthena, login.c @ 1260
		if (buf.dataSize() == 23)
			buf.clear(); // We don't need any more data after this packet
		else if (buf.dataSize() < 23)
			return(false); // We need more data
		// It should never be > 23
	} else {
		// 6 = Your are Prohibited to log in until %s
		buf.clear(); // TODO: Handle this!
	}

	errorId = result;

	return(true);
}

short ronet::pktLoginError::getErrorId() const {
	return(errorId);
}
