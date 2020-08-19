/*
 * @file commonalgorithm.h
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#ifndef POKER_COMMON_ALGORITHM_H
#define POKER_COMMON_ALGORITHM_H

#include <vector>
#include "pkcard.h"
#include "pokeralgorithm.h"
#include "playcardinfo.h"

namespace algorithm {

class CommonAlgorithm
{
public:
	CommonAlgorithm(){}
	~CommonAlgorithm(){}

	virtual int FindCardType(const std::vector<ICardPtr>& hand_cards, const PlayCardInfo & play_info, std::vector<ICardPtr> &cards);
	virtual int DiscernCardType(const std::vector<ICardPtr>& play_cards, PlayCardInfo & play_info);

private:
	bool FindPair(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindContinuousPairs(const int begin_face, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindThreeBeltOne(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindThreeBeltTwo(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindThreeBeltPair(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindFourBeltOne(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindFourBeltTwo(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindFourBeltThree(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindBeltCards(const int begin_face, const size_t conut, const size_t wing_num, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindAircraft(const int begin_face, const size_t size, const size_t wing_num, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindStraight(const int begin_face, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindBomb(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindLaiZiBomb(const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool FindKingBomb(const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);
	bool AppendWing(const int wing_num, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards);

	bool IsPair(const std::vector<CardInfo>& cards_info);
	bool IsContinuousPairs(const std::vector<CardInfo>& cards_info);
	bool IsThree(const std::vector<CardInfo>& cards_info);
	bool IsThreeBeltOne(const std::vector<CardInfo>& cards_info); 
	bool IsThreeBeltTwo(const std::vector<CardInfo>& cards_info);
	bool IsFourBeltOne(const std::vector<CardInfo>& cards_info);
	bool IsFourBeltTwo(const std::vector<CardInfo>& cards_info);
	bool IsFourBeltThree(const std::vector<CardInfo>& cards_info);
	bool IsBeltCards(const std::vector<CardInfo>& cards_info, const size_t count, const size_t wing_num);
	bool IsAircraft(const std::vector<CardInfo>& cards_info, const size_t wing_num);
	bool IsStraight(const std::vector<CardInfo> & cards_info);
	bool IsBomb(const std::vector<CardInfo>& cards_info);
	bool IsKingBomb(const std::vector<CardInfo> & cards_info);

private:
	int GetCardsCount(const std::vector<CardInfo>& cards_info);

	PokerAlgorithm poker_algo_;
};

} // namespace algorithm
#endif // PDKGAME_POKER_ALGORITHM_H