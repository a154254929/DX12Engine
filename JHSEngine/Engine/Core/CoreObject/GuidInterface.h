#pragma once
#include "../../EngineMinimal.h"

//提供一个对象的身份ID
class IGuidInterface
{
public:
	IGuidInterface();

	bool operator==(const IGuidInterface& inOther)
	{
		return guid_equal(&guid, &inOther.guid);
	}

	simple_c_guid GetGuid() { return guid; }

private:
	simple_c_guid guid;
};