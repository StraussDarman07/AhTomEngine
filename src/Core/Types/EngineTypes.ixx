module;

#include <string>;

export module EngineTypes;

export struct Version
{
	int major = -1;
	int minor = -1;
	int patch = -1;

	std::string toString();
};

std::string Version::toString()
{
	return "v" + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
}
