#include "EngineStructs.h"

std::string Engine::TextureTypeToString(TextureType type)
{
	switch (type)
	{
	case TextureType::ALBEDO:
		return "ALBEDO";
	case TextureType::NORMAL:
		return "NORMAL";
	case TextureType::ROUGHNESS:
		return "ROUGHNESS";
	case TextureType::METALLIC:
		return "METALLIC";
	case TextureType::AO:
		return "AO";
	case TextureType::EMISSIVE:
		return "EMISSIVE";
	case TextureType::HEIGHT:
		return "HEIGHT";
	case TextureType::OPACITY:
		return "OPACITY";
	default:
		return "UNKNOWN";
	}
}