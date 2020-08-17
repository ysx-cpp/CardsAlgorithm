#include "pkcard.h"

namespace algorithm {

std::string CPkCard::Name() const
{
	std::string name(std::to_string(index_));
	switch (type_)
	{
	case CPkCard::E_SQUARE_TYPE:
		name.append("方");
		break;
	case CPkCard::E_PLUM_BLOSSOM_TYPE:
		name.append("梅");
		break;
	case CPkCard::E_RED_PEACH_TYPE:
		name.append("红");
		break;
	case CPkCard::E_SPADES_TYPE:
		name.append("黑");
		break;
	default:
		break;
	} 
	name.append(std::to_string(index_));

	return name;
}
} //namespace by