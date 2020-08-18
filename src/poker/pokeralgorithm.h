#ifndef PKGAME_POKER_ALGORITHM_H
#define PKGAME_POKER_ALGORITHM_H

#include <algorithm>
#include <vector>
#include "pkcard.h"

namespace algorithm {

struct CardInfo
{
	CardInfo(const int & value) :face(value) {}
	bool operator==(const ICardPtr & card) const { return card->faceId() == face; }
	bool operator!=(const ICardPtr & card) const { return card->faceId() != face; }
	bool operator==(const CardInfo & other) const { return face == other.face; }
	bool operator!=(const CardInfo & other) const { return face != other.face; }
	bool operator<(const CardInfo & other) const { return  face < other.face; }
	bool operator>(const CardInfo & other) const { return  face > other.face; }
	
	int face;
	std::vector<ICardPtr> cards;
};

class PokerAlgorithm
{
public:
	PokerAlgorithm() 
		: laizi_card_(std::make_shared<CPkCard>(CPkCard::E_INVALID_TYPE, CPkCard::E_INVALID_FACE, -1))
	{
	}
	virtual ~PokerAlgorithm() {}

	void SetLaiZiCard(const ICardPtr & card_ptr) { laizi_card_ = card_ptr; }
	const ICardPtr laizi_card() const { return laizi_card_; }

private:
	friend class CommonAlgorithm;

	ICardPtr FindMaxCard(const std::vector<CardInfo> & cards_info);
	bool FindSameColor(const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindStraightFlush(const int begin_face, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindNonContinuousCards(const int begin_face, const size_t count, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindContinuousCards(const int begin_face, const size_t count, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindLaiZiContinuousCards(const int begin_face, const size_t count, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);

	bool IsSameColor(const std::vector<CardInfo>& cards_info);
	bool IsStraightFlush(const std::vector<CardInfo>& cards_info);
	bool IsNonContinuousCards(const std::vector<CardInfo> & cards_info, const size_t count);
	bool IsLaiZiNonContinuousCards(const std::vector<CardInfo> & cards_info, const size_t count);
	bool IsContinuousCards(const std::vector<CardInfo> & cards_info, const size_t count, const size_t size);
	bool IsLaiZiContinuousCards(const std::vector<CardInfo> & cards_info, const size_t count, const size_t size);

	void SrotCard(std::vector<ICardPtr> & cards);
	void PrintCards(const std::vector<ICardPtr>& cards);
	static bool CompareCardType(const int & type1, const int & type2);
	static bool CompareCardValue(const ICardPtr & c1, const ICardPtr & c2);
	static bool CompareCardIndex(const ICardPtr & c1, const ICardPtr & c2);
	void CardInfoToPokerGroup(const std::vector<CardInfo> & cards_info, std::vector<int> & poker_group);
	void CardsToCardInfo(const std::vector<ICardPtr> & cards, std::vector<CardInfo> & hand_cards_info);

private:
	ICardPtr laizi_card_;
};

} // algorithm
#endif // PKGAME_POKER_ALGORITHM_H