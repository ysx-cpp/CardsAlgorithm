/*
 * @file pkcard.h
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#ifndef POKER_PK_CARD_H
#define POKER_PK_CARD_H

#include "icard.h"

namespace algorithm {

class CPkCard : public ICard
{
public:
	enum EFace
	{
		E_INVALID_FACE,
		E_FACE_THREE = 3,
		E_FACE_FOUR,
		E_FACE_FIVE,
		E_FACE_SIX,
		E_FACE_SEVEN,
		E_FACE_EIGHT,
		E_FACE_NINE,
		E_FACE_TEN,
		E_FACE_J,
		E_FACE_Q,
		E_FACE_K,
		E_FACE_A,
		E_FACE_TWO,
		E_FACE_BIG,
		E_FACE_SMALL,
		E_FACE_COUNT_
	};
	enum EType
	{
		E_INVALID_TYPE = -1,
		E_SQUARE_TYPE,		//方块
		E_PLUM_BLOSSOM_TYPE,//梅花
		E_RED_PEACH_TYPE,	//红桃
		E_SPADES_TYPE,		//黑桃
		E_KING_TYPE,		//王
		E_TYPE_COUNT_
	};

	using Card = uint8_t;

public:
	CPkCard(EType type, EFace face, int index)
		: type_(type),
		face_(face),
		index_(index)
	{
	}
	virtual ~CPkCard() {}
	virtual int IndexId() const { return index_; }
	virtual int TypeId() const { return type_; }
	virtual int FaceId() const { return face_; }
	virtual int ClrId() const { return 0; }
	virtual bool IsEmpty() const { return false; }
	virtual std::string Name() const;
	
	//bool operator==(const ICardPtr c) const { return index_ == c->indexId(); }
	bool operator==(const ICardPtr c) const { return face_ == c->FaceId(); }

private:
	EType   type_;
	EFace   face_;
	int     index_;
};

using CPkCardPtr = std::shared_ptr<CPkCard>;

} // namespace algorithm
#endif // POKER_PK_CARD_H
