#pragma once
#include "sdk/ischemasystem.h"


namespace interfaces {
	namespace core {
		bool Setup();
	}

	inline ISchemaSystem* SchemaSystem = nullptr;
}