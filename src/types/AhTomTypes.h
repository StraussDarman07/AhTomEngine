#pragma once

#include <string>
#include <format>
#include <optional>

namespace Types
{
	struct Version
	{
		const int major;
		const int minor;
		const int patch;

		Version(int major, int minor, int patch) : major(major), minor(minor), patch(patch)
		{}

		const std::string toString() const
		{
			return std::format("{}.{}.{}", major, minor, patch);
		}
	};

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
}