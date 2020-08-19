/*
 * @file mjcard.cpp
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#include "mjcard.h"

namespace algorithm {

int MjCard::IndexId() const
{
	return index_;
}

int MjCard::TypeId() const
{
	return type_;
}
int MjCard::FaceId() const
{
	return face_;
}
int MjCard::ClrId() const
{
	return 0;
}
std::string MjCard::Name() const
{
	std::string name(std::to_string(face_));
	switch (type_)
	{
	case MjCard::E_MYRIAD_TYPE:
		name.append("万");
		break;
	case MjCard::E_CYLINDER_TYPE:
		name.append("筒");
		break;
	case MjCard::E_SLIVER_TYPE:
		name.append("条");
		break;
	case MjCard::E_WIND_TYPE:
	{
		if (face_ == E_FACE_ONE)
			name = "东";
		else if (face_ == E_FACE_TWO)
			name = "南";
		else if (face_ == E_FACE_THREE)
			name = "西";
		else if (face_ == E_FACE_FOUR)
			name = "北";
	}
		break;
	case MjCard::E_WORD_TYPE:
	{
		if (face_ == E_FACE_ONE)
			name = "中";
		else if (face_ == E_FACE_TWO)
			name = "发";
		else if (face_ == E_FACE_THREE)
			name = "白";
	}
		break;
	default:
		break;
	} 
	name.append(std::to_string(index_));

	return name;
}
} //namespace by