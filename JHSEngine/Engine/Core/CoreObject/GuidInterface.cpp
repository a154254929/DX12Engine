#include "GuidInterface.h"

IGuidInterface::IGuidInterface()
{
	create_guid(&guid);
}