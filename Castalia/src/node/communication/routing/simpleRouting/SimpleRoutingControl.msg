cplusplus {{
#include "RoutingPacket_m.h"
}}

class RoutingPacket;

enum SimpleRoutingControl_type {
	SIMPLE_ROUTING_RECEIVERS_REQUEST = 1;
	SIMPLE_ROUTING_RECEIVERS_REPLY = 2;
}

packet SimpleRoutingControl extends RoutingPacket {
	int SimpleRoutingControlKind enum (SimpleRoutingControl_type);
}