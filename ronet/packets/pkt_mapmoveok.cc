#include "stdafx.h"

#include "ronet/packets/pkt_maploaded.h"

namespace ronet {

pktMapMoveOk::pktMapMoveOk() : Packet(pktMapMoveOkID) {
	ticks = 0;
	start_x = start_y = 0;
	dest_x = dest_y = 0;
}


bool pktMapMoveOk::Decode(ucBuffer& buf) {
	unsigned short buf_id;
	unsigned int v;
	unsigned int i;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! Expected %04x, received %04x.\n", id, buf_id);
		return(false);
	}

	// Packet size is 12...
	if (buf.dataSize() < 12)
		return(false);

	buf.ignore(2);

	unsigned char coord[5];


	buf >> ticks;
	
	buf.read(coord, 5);
	unsigned char *ptr = (unsigned char*)&v;
	v = 0;

	// Read source
	ptr[0] = coord[2];
	ptr[1] = coord[1];
	ptr[2] = coord[0];

	start_x = ((v >> 14) & 0x03FF);
	start_y = ((v >> 4) & 0x03FF);

	// Read destination
	ptr[0] = coord[4];
	ptr[1] = coord[3];
	ptr[2] = coord[2];

	dest_x = ((v >> 10) & 0x03FF);
	dest_y = ((v >> 0) & 0x03FF);

	printf("Moving from %d,%d -> %d,%d @ %d\n", start_x, start_y, dest_x, dest_y, ticks);

	buf.ignore(1);

	return(true);
}

void pktMapMoveOk::getStart(int* x, int* y) const {
	*x = start_x;
	*y = start_y;
}

void pktMapMoveOk::getDest(int* x, int* y) const {
	*x = dest_x;
	*y = dest_y;
}

}