/*
 * @file mjcard.h
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#ifndef MAJONG_MJ_CARD_H
#define MAJONG_MJ_CARD_H

#include "icard.h"

namespace algorithm {

class MjCard : public ICard
{
public:
	enum EFace
	{
		E_INVALID_FACE,
		E_FACE_ONE,
		E_FACE_TWO,
		E_FACE_THREE,
		E_FACE_FOUR,
		E_FACE_FIVE,
		E_FACE_SIX,
		E_FACE_SEVEN,
		E_FACE_EIGHT,
		E_FACE_NINE,
		E_FACE_COUNT_
	};
	enum EType
	{
		E_MYRIAD_TYPE,		//万
		E_CYLINDER_TYPE,	//筒
		E_SLIVER_TYPE,		//条
		E_WIND_TYPE,		//风
		E_WORD_TYPE,		//字
		E_TYPE_COUNT_
	};

public:
	MjCard(EType type, EFace face, int index)
		: type_(type),
		face_(face),
		index_(index)
	{
	}
	virtual ~MjCard() {}
	virtual int IndexId() const;
	virtual int TypeId() const;
	virtual int FaceId() const;
	virtual int ClrId() const;
	virtual bool IsEmpty() const { return false; };
	virtual std::string Name() const;
	

private:
	EType   type_;
	EFace   face_;
	int     index_;
};

using MjCardPtr = std::shared_ptr<MjCard>;

} // namespace by
#endif // MAJONG_MJ_CARD_H
