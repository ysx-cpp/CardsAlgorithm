/*
 * @file mjcard.cpp
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#include "mjcard.h"

namespace algorithm {

int MjCard::indexId() const
{
	return m_nIndex_;
}

int MjCard::typeId() const
{
	return m_eType_;
}
int MjCard::faceId() const
{
	return m_eFace_;
}
int MjCard::clrId() const
{
	return 0;
}
std::string MjCard::Name() const
{
	std::string name(std::to_string(m_eFace_));
	switch (m_eType_)
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
		if (m_eFace_ == E_FACE_ONE)
			name = "东";
		else if (m_eFace_ == E_FACE_TWO)
			name = "南";
		else if (m_eFace_ == E_FACE_THREE)
			name = "西";
		else if (m_eFace_ == E_FACE_FOUR)
			name = "北";
	}
		break;
	case MjCard::E_WORD_TYPE:
	{
		if (m_eFace_ == E_FACE_ONE)
			name = "中";
		else if (m_eFace_ == E_FACE_TWO)
			name = "发";
		else if (m_eFace_ == E_FACE_THREE)
			name = "白";
	}
		break;
	default:
		break;
	} 
	name.append(std::to_string(m_nIndex_));

	return name;
}
} //namespace by