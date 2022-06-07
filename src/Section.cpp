#include "pch.h"
#include "Section.h"

SectionName::SectionName(std::string name)
{
	// Make section name lowercase
	for (char& c : name)
	{
		c = tolower(c);
	}

	// Trim begining and ending trailing whitespace
	name.erase(name.begin(), std::find_if(name.begin(), name.end(), [](char c) { return !std::isspace(c); }));
	name.erase(std::find_if(name.rbegin(), name.rend(), [](unsigned char c) { return !std::isspace(c); }).base(),
		name.end());

	this->name_ = name;
	std::string s = "sfd";
	
}

SectionName::SectionName()
	: name_("")
{ }

std::string SectionName::name() const
{
	return name_;
}

bool inline SectionName::empty() const noexcept
{
	return name_.empty();
}

bool SectionName::operator==(const SectionName& section) const
{
	return false;
}
