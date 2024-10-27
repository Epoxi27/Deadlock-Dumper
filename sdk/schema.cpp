#include "schema.h"
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include "../interfaces.h"
#include "ischemasystem.h"
#include "fnv1a.h"
#include <ranges>
#include <sstream>

struct SchemaData_t
{
	uint32_t uHashedFieldName = 0x0ULL;
	std::uint32_t uOffset = 0x0U;
};

static std::vector<SchemaData_t> vecSchemaData;

bool schema::Dump(const char* szModuleName)
{
	CSchemaSystemTypeScope* pTypeScope = interfaces::SchemaSystem->FindTypeScopeForModule(szModuleName);
	if (pTypeScope == nullptr)
		return false;

	const int nTableSize = pTypeScope->hashClasses.Count();

	// allocate memory for elements
	UtlTSHashHandle_t* pElements = new UtlTSHashHandle_t[nTableSize + 1U];
	const auto nElements = pTypeScope->hashClasses.GetElements(0, nTableSize, pElements);

	for (int i = 0; i < nElements; i++)
	{
		const UtlTSHashHandle_t hElement = pElements[i];

		if (hElement == 0)
			continue;

		CSchemaClassBinding* pClassBinding = pTypeScope->hashClasses[hElement];
		if (pClassBinding == nullptr)
			continue;

		SchemaClassInfoData_t* pDeclaredClassInfo;
		pTypeScope->FindDeclaredClass(&pDeclaredClassInfo, pClassBinding->szBinaryName);

		if (pDeclaredClassInfo == nullptr)
			continue;

		if (pDeclaredClassInfo->nFieldSize == 0)
			continue;
		std::cout << "namespace " << pDeclaredClassInfo->szName << " {\n";

		for (auto j = 0; j < pDeclaredClassInfo->nFieldSize; j++)
		{
			SchemaClassFieldData_t* pFields = pDeclaredClassInfo->pFields;
			std::string szFieldClassBuffer = std::string(pClassBinding->szBinaryName) + "->" + std::string(pFields[j].szName);

			//std::cout << szFieldClassBuffer;

			// store field info
			vecSchemaData.emplace_back(fnv1a_ct(szFieldClassBuffer.c_str()), pFields[j].nSingleInheritanceOffset);

			int offset = pFields[j].nSingleInheritanceOffset;
			std::stringstream stream;
			stream << std::hex << offset;
			std::string result(stream.str());

			std::string szFieldBuffer = "    constexpr uintptr_t "
				+ std::string(pFields[j].szName) + " = 0x" + result + "; // " + std::string(pFields[j].pSchemaType->szName) + "\n";

			// write field info
			std::cout << szFieldBuffer.c_str();
		}

		std::cout << "}\n";
	}

	// free allocated memory
	delete[] pElements;

	return true;
}

