/*
 * @file commonalgorithm.cpp
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#include "commonalgorithm.h"

namespace algorithm {

int CommonAlgorithm::FindCardType(const std::vector<ICardPtr>& hand_cards, const PlayCardInfo & play_info, std::vector<ICardPtr> &cards)
{
	std::vector<CardInfo> cards_info;
	poker_algo_.CardsToCardInfo(hand_cards, cards_info);

	switch (play_info.card_type)
	{
	case E_PK_OCT_LEAFLET:
	{
		auto max_card = poker_algo_.FindMaxCard(cards_info);
		if (max_card->faceId() > play_info.card_value)
		{
			cards.push_back(max_card);
			return E_PK_OCT_LEAFLET;
		}
	}break;
	case E_PK_OCT_PAIR:
	{
		if (FindPair(play_info.card_value + 1, cards_info, cards))
			return E_PK_OCT_PAIR;
	}break;
	case E_PK_OCT_PAIRS:
	{
		if (FindContinuousPairs(play_info.card_value + 1, play_info.continu_leng, cards_info, cards))
			return E_PK_OCT_PAIR;
	}break;
	case E_PK_OCT_STRAIGHT:
	{
		if (FindStraight(play_info.card_value + 1, play_info.continu_leng, cards_info, cards))
			return E_PK_OCT_STRAIGHT;
	}break;
	case E_PK_OCT_AIRCRAFT:
	{
		if (FindAircraft(play_info.card_value + 1, play_info.continu_leng, 2, cards_info, cards))
			return E_PK_OCT_AIRCRAFT;
	}break;
	case E_PK_OCT_THREE:
	{

	}break;
	case E_PK_OCT_THREE_BELT_ONE:
	{
		if (FindThreeBeltOne(play_info.card_value + 1, cards_info, cards))
			return E_PK_OCT_THREE_BELT_ONE;
	}break;
	case E_PK_OCT_THREE_BELT_TOW:
	{
		if (FindThreeBeltTwo(play_info.card_value + 1, cards_info, cards))
			return E_PK_OCT_THREE_BELT_TOW;
	}break;
	case E_PK_OCT_THREE_BELT_PAIR:
	{
		if (FindThreeBeltPair(play_info.card_value + 1, cards_info, cards))
			return E_PK_OCT_THREE_BELT_PAIR;
	}break;
	case E_PK_OCT_FOUR_BELT_ONE:
	{
		if (FindFourBeltOne(play_info.card_value + 1, cards_info, cards))
			return E_PK_OCT_FOUR_BELT_ONE;
	}break;
	case E_PK_OCT_FOUR_BELT_TOW:
	{
		if (FindFourBeltTwo(play_info.card_value + 1, cards_info, cards))
			return E_PK_OCT_FOUR_BELT_TOW;
	}break;
	case E_PK_OCT_FOUR_BELT_THREE:
	{
		if (FindFourBeltThree(play_info.card_value + 1, cards_info, cards))
			return E_PK_OCT_FOUR_BELT_THREE;
	}break;
	default:
		break;
	}

	if (FindBomb(play_info.card_value + 1, cards_info, cards))
		return E_PK_OCT_BOMB;
	if (FindKingBomb(cards_info, cards))
		return E_PK_OCT_FOUR_BELT_ONE;
	if (FindLaiZiBomb(cards_info, cards))
		return E_PK_OCT_FOUR_BELT_ONE;

	return E_PK_OCT_INVALID;
}

int CommonAlgorithm::DiscernCardType(const std::vector<ICardPtr>& play_cards, PlayCardInfo & play_info)
{
	if (play_cards.empty())
	{
		return E_PK_OCT_INVALID;
	}

	std::vector<CardInfo> cards_info;
	poker_algo_.CardsToCardInfo(play_cards, cards_info);

	for (auto &iter : play_cards)
		play_info.vec_play_cards.push_back(iter->indexId());

	switch (play_cards.size())
	{
	case 1:
	{
		play_info.card_type = E_PK_OCT_LEAFLET;
		play_info.continu_leng = 1;
		play_info.card_value = cards_info.front().face;
		return E_PK_OCT_LEAFLET;
	}break;
	case 2:
	{
		if (IsPair(cards_info))
		{
			play_info.card_type = E_PK_OCT_PAIR;
			play_info.continu_leng = 1;
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_PAIR;
		}
		if (IsKingBomb(cards_info))
		{
			play_info.card_type = E_PK_OCT_KING_BOMB;
			play_info.continu_leng = 1;
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_KING_BOMB;
		}
	}break;
	case 3:
	case 4:
	{
		if (IsBomb(cards_info))
		{
			play_info.card_type = E_PK_OCT_BOMB;
			play_info.continu_leng = 1;
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_BOMB;
		}
		if (IsThreeBeltOne(cards_info))
		{
			play_info.card_type = E_PK_OCT_THREE_BELT_ONE;
			play_info.continu_leng = 1;
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_THREE_BELT_ONE;
		}
	}break;
	default:
	{
		if (IsThreeBeltTwo(cards_info))
		{
			play_info.card_type = E_PK_OCT_THREE_BELT_TOW;
			play_info.continu_leng = 1;
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_THREE_BELT_TOW;
		}
		if (IsFourBeltOne(cards_info))
		{
			play_info.card_type = E_PK_OCT_FOUR_BELT_ONE;
			play_info.continu_leng = 1;
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_FOUR_BELT_ONE;
		}
		if (IsFourBeltTwo(cards_info))
		{
			play_info.card_type = E_PK_OCT_FOUR_BELT_TOW;
			play_info.continu_leng = 1;
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_FOUR_BELT_TOW;
		}
		if (IsFourBeltThree(cards_info))
		{
			play_info.card_type = E_PK_OCT_FOUR_BELT_THREE;
			play_info.continu_leng = 1;
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_FOUR_BELT_THREE;
		}
		if (IsStraight(cards_info))
		{
			play_info.card_type = E_PK_OCT_STRAIGHT;
			play_info.continu_leng = play_cards.size();
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_STRAIGHT;
		}
		if (IsContinuousPairs(cards_info))
		{
			play_info.card_type = E_PK_OCT_PAIRS;
			play_info.continu_leng = play_cards.size();
			play_info.card_value = cards_info.front().face;
			return E_PK_OCT_PAIRS;
		}
		if (IsAircraft(cards_info, 2))
		{
			play_info.card_type = E_PK_OCT_AIRCRAFT;
			play_info.card_value = cards_info.front().face;

			int play_card_num = play_cards.size();
			if ((play_card_num - 4) % 3 == 0)
				play_info.continu_leng = 2;
			if ((play_card_num - 6) % 3 == 0)
				play_info.continu_leng = 3;
			if ((play_card_num - 8) % 3 == 0)
				play_info.continu_leng = 4;

			return E_PK_OCT_AIRCRAFT;
		}
	}break;
	}

	return E_PK_OCT_INVALID;
}

bool CommonAlgorithm::FindPair(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return poker_algo_.FindContinuousCards(begin_face, 2, 1, cards_info, cards);
}

bool CommonAlgorithm::FindContinuousPairs(const int begin_face, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return poker_algo_.FindContinuousCards(begin_face, 2, size, cards_info, cards);
}

bool CommonAlgorithm::FindThreeBeltOne(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return FindBeltCards(begin_face, 3, 1, cards_info, cards);
}

bool CommonAlgorithm::FindThreeBeltTwo(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return FindBeltCards(begin_face, 3, 2, cards_info, cards);
}

bool CommonAlgorithm::FindThreeBeltPair(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	if (!poker_algo_.FindContinuousCards(begin_face, 3, 1, cards_info, cards))
		return false;

	std::vector<ICardPtr> wing_cards;
	for (auto &it : cards_info)
	{
		if (it.cards.size() < 2 && it != poker_algo_.laizi_card_)
			continue;

		for (auto &iter : it.cards)
		{
			auto iter_exist = std::find_if(cards.begin(), cards.end(), [iter](const ICardPtr & item){
				return item->indexId() == iter->indexId();
			});

			if (iter_exist == cards.end())
			{
				wing_cards.push_back(iter);
			}
			if (wing_cards.size() == 2)
				return true;
		}
	}
	if (wing_cards.size() == 2)
		cards.insert(cards.end(), wing_cards.begin(), wing_cards.end());

	return wing_cards.size() == 2;
}

bool CommonAlgorithm::FindFourBeltOne(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return FindBeltCards(begin_face, 4, 1, cards_info, cards);
}

bool CommonAlgorithm::FindFourBeltTwo(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return FindBeltCards(begin_face, 4, 2, cards_info, cards);
}

bool CommonAlgorithm::FindFourBeltThree(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return FindBeltCards(begin_face, 4, 3, cards_info, cards);
}

bool CommonAlgorithm::FindBeltCards(const int begin_face, const size_t conut, const size_t wing_num, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	if (!poker_algo_.FindContinuousCards(begin_face, conut, 1, cards_info, cards))
		return false;
	if (!AppendWing(wing_num, cards_info, cards))
		return false;

	return cards.size() % conut == wing_num;
}

bool CommonAlgorithm::FindAircraft(const int begin_face, const size_t size, const size_t wing_num, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	if (!poker_algo_.FindContinuousCards(begin_face, 3, size, cards_info, cards))
		return false;
	
	if (!AppendWing(wing_num * size, cards_info, cards))
		return false;

	return cards.size() % (3 * size) == wing_num * size;
}

bool CommonAlgorithm::FindStraight(const int begin_face, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return poker_algo_.FindContinuousCards(begin_face, 1, size, cards_info, cards);
}

bool CommonAlgorithm::FindBomb(const int begin_face, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	return poker_algo_.FindNonContinuousCards(begin_face, 4, cards_info, cards);
}

bool CommonAlgorithm::FindLaiZiBomb(const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	for (auto &iter : cards_info)
	{
		if (iter == poker_algo_.laizi_card() && iter.cards.size() == 4)
		{
			cards.assign(iter.cards.begin(), iter.cards.end());
			return true;
		}
	}
	return false;
}

bool CommonAlgorithm::FindKingBomb(const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	int count = 0;
	for (auto &iter : cards_info)
	{
		if (cards_info.front().face == CPkCard::E_FACE_BIG ||
			cards_info.back().face == CPkCard::E_FACE_SMALL)
		{
			cards.push_back(iter.cards.front());
		}
		if (count == 2)
		{
			return true;
		}
	}
	return false;
}

bool CommonAlgorithm::AppendWing(const int wing_num, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	int count = 0;
	std::vector<ICardPtr> laizi_cards;
	for (auto &it : cards_info)
	{
		if (it == poker_algo_.laizi_card_)
		{
			laizi_cards.assign(it.cards.begin(), it.cards.end());
			continue;
		}
		for (auto &iter : it.cards)
		{
			auto iter_exist = std::find_if(cards.begin(), cards.end(), [iter](const ICardPtr & item){
				return item->indexId() == iter->indexId();
			});

			if (iter_exist == cards.end())
			{
				++count;
				cards.push_back(iter);
			}
			if (count == wing_num)
				return true;
		}
	}

	for (auto &iter : laizi_cards)
	{
		auto iter_exist = std::find_if(cards.begin(), cards.end(), [iter](const ICardPtr & item){
			return item->indexId() == iter->indexId();
		});

		if (iter_exist == cards.end())
		{
			++count;
			cards.push_back(iter);
		}
		if (count == wing_num)
			return true;
	}

	return count == wing_num;
}

bool CommonAlgorithm::IsPair(const std::vector<CardInfo>& cards_info)
{
	return poker_algo_.IsNonContinuousCards(cards_info, 2);
}

bool CommonAlgorithm::IsContinuousPairs(const std::vector<CardInfo>& cards_info)
{
	int card_count = GetCardsCount(cards_info);
	int size = card_count / 2;
	return poker_algo_.IsContinuousCards(cards_info, 2, size);
}

bool CommonAlgorithm::IsThree(const std::vector<CardInfo>& cards_info)
{
	return poker_algo_.IsNonContinuousCards(cards_info, 3);
}

bool CommonAlgorithm::IsThreeBeltOne(const std::vector<CardInfo>& cards_info)
{
	return IsBeltCards(cards_info, 3, 1);
}

bool CommonAlgorithm::IsThreeBeltTwo(const std::vector<CardInfo>& cards_info)
{
	return IsBeltCards(cards_info, 3, 2);
}

bool CommonAlgorithm::IsFourBeltOne(const std::vector<CardInfo>& cards_info)
{
	return IsBeltCards(cards_info, 4, 1);
}

bool CommonAlgorithm::IsFourBeltTwo(const std::vector<CardInfo>& cards_info)
{
	return IsBeltCards(cards_info, 4, 2);
}

bool CommonAlgorithm::IsFourBeltThree(const std::vector<CardInfo>& cards_info)
{
	return IsBeltCards(cards_info, 4, 3);
}

bool CommonAlgorithm::IsBeltCards(const std::vector<CardInfo>& cards_info, const size_t count, const size_t wing_num)
{
	if (!poker_algo_.IsNonContinuousCards(cards_info, count))
		return false;

	size_t num = 0;
	for (std::vector<CardInfo>::const_iterator it = cards_info.begin(); it != cards_info.end(); ++it)
	{
		num += it->cards.size();
	}

	return num % count == wing_num;
}

bool CommonAlgorithm::IsAircraft(const std::vector<CardInfo>& cards_info, const size_t wing_num)
{
	int card_count = GetCardsCount(cards_info);
	int size = 2;
	if (card_count == 9 + (3 * wing_num))
		size = 3;
	else if (card_count == 12 + (4 * wing_num))
		size = 4;

	if (!poker_algo_.IsContinuousCards(cards_info, 3, size))
		return false;

	return card_count % (3 * size) == wing_num * size;
}

bool CommonAlgorithm::IsStraight(const std::vector<CardInfo> & cards_info)
{
	size_t size = cards_info.size();
	return poker_algo_.IsContinuousCards(cards_info, 1, size);
}

bool CommonAlgorithm::IsBomb(const std::vector<CardInfo>& cards_info)
{
	if (!poker_algo_.IsNonContinuousCards(cards_info, 4))
		return false;

	int card_count = GetCardsCount(cards_info);

	return card_count == 4;
}

bool CommonAlgorithm::IsKingBomb(const std::vector<CardInfo> & cards_info)
{
	if (cards_info.size() != 2) return false;

	if (cards_info.front().face == CPkCard::E_FACE_BIG &&
		cards_info.back().face == CPkCard::E_FACE_SMALL)
		return true;

	return false;
}

int CommonAlgorithm::GetCardsCount(const std::vector<CardInfo>& cards_info)
{
	int count = 0;
	for (std::vector<CardInfo>::const_iterator it = cards_info.begin(); it != cards_info.end(); ++it)
	{
		count += it->cards.size();
	}
	return count;
}

} //namespace algorithm