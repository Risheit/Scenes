#pragma once

class SectionName
{
	std::string name_; // The unique name of this section

public:
	SectionName(
		std::string name_
	);

	SectionName();

	std::string name() const;

	bool inline empty() const noexcept;

	bool operator==(
		const SectionName &section
		) const;
};

namespace std
{
	template <> 
	struct hash <SectionName>
	{
		size_t operator()(const SectionName& section) const
		{
			return hash<std::string>()(section.name());
		}
	};
}
