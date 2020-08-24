/*
 * @file aipaialgorithm.cpp
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#include "zipaialgorithm.h"
#include <memory.h>

namespace algorithm {

enum CardType
{
	KE_ZI			= 0, //3张
	KAN_ZI			= 1, //4张
	SHUN_ZI			= 2, //顺子
	JIAO_PAI		= 3, //绞牌
	ONE_TWO_THREE	= 4, //一二三
	TWO_SEVEN_TEN	= 5, //二七十
};

ZipaiAlgorithm::ZipaiAlgorithm()
{
}

ZipaiAlgorithm::~ZipaiAlgorithm()
{
}

bool ZipaiAlgorithm::CheckHu(const std::vector<Card> &hand_cards)
{
	door_group_.clear();
	CardGroup cards = { { 0 } };
	InputHandCard(hand_cards, cards);

	ComposeKeZi(cards, hand_cards.back(), false);

	if (CheckHaveKanZi(cards))
		return CheckJiangCanHu(cards);

	return CheckCompose333(cards);
}

void ZipaiAlgorithm::OutPutDoorCards(std::vector<Card> &hand_cards)
{
	for (auto &it : door_group_)
	{
		hand_cards.insert(hand_cards.end(), it->cards.begin(), it->cards.end());
	}
}

void ZipaiAlgorithm::InputHandCard(const std::vector<Card> &hand_cards, CardGroup cards)
{
	for (auto &iter : hand_cards)
	{
		const int color = type(iter) - 1;
		const int vale = value(iter);
		cards[color][vale] += 1;
		cards[color][0] += 1;
	}
}

void ZipaiAlgorithm::ComposeKeZi(CardGroup cards, const Card target_card, const bool is_other)
{
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] < 3) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (!is_other && target_card == RecoverCard(i, j)) continue;
			if (cards[i][j] == 3)
			{
				cards[i][j] -= 3;
				cards[i][0] -= 3;
				auto door_card = std::make_shared<DoorCards>();
				door_card->card_type = KE_ZI;
				door_card->huzi = 3;
				if (i == 1) door_card->huzi *= 2;
				door_card->cards.push_back(RecoverCard(i, j));
				door_card->cards.push_back(RecoverCard(i, j));
				door_card->cards.push_back(RecoverCard(i, j));
				door_group_.push_back(door_card);
			}
		}
	}
}

bool ZipaiAlgorithm::CheckHaveKanZi(const CardGroup cards) const
{
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (cards[i][j] == 4) return true;
		}
	}
	return false;
}

bool ZipaiAlgorithm::CheckCompose333(CardGroup cards)
{
	if (laizi_num_ % 3 == 0 && cards[0][0] + cards[1][0] == 0)
		return true;

	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (cards[i][j] == 0) continue;

			if (CheckComposeKeZi(cards, i, j))
			{
				cards[i][j] -= 3;
				cards[i][0] -= 3;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 3;
				cards[i][0] += 3;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = KE_ZI;
					door_card->huzi = 3;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, j));
					door_card->cards.push_back(RecoverCard(i, j));
					door_card->cards.push_back(RecoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			/************************************1个癞子************************************/

			if (CheckCompose2710LaiziAnd27(cards, i, j))
			{
				cards[i][2] -= 1;
				cards[i][7] -= 1;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][2] += 1;
				cards[i][7] += 1;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->huzi = 3;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, 2));
					door_card->cards.push_back(RecoverCard(i, 7));
					//door_card.cards.push_back(recoverCard(i, 10));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckCompose2710LaiziAnd210(cards, i, j))
			{
				cards[i][2] -= 1;
				cards[i][10] -= 1;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][2] += 1;
				cards[i][10] += 1;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->huzi = 3;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, 2));
					door_card->cards.push_back(RecoverCard(i, 10));
					//door_card.cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckCompose2710LaiziAnd710(cards, i, j))
			{
				cards[i][7] -= 1;
				cards[i][10] -= 1;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][7] += 1;
				cards[i][10] += 1;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->huzi = 3;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, 7));
					door_card->cards.push_back(RecoverCard(i, 10));
					//door_card.cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeLaiZiShuZi(cards, i, j))
			{
				cards[i][j] -= 1;
				cards[i][j + 1] -= 1;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][j + 1] += 1;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					if (j == 1 || j == 2) door_card->huzi = 3;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, j));
					door_card->cards.push_back(RecoverCard(i, j + 1));
					//door_card.cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeMiddleLaiZiShuZi(cards, i, j))
			{
				cards[i][j] -= 1;
				cards[i][j + 2] -= 1;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][j + 2] += 1;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					if (j == 1) door_card->huzi = 3;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, j));
					door_card->cards.push_back(RecoverCard(i, j + 2));
					//door_card.cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeLaiZiKeZi(cards, i, j))
			{
				cards[i][j] -= 2;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 2;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->huzi = 3;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, j));
					door_card->cards.push_back(RecoverCard(i, j));
					//door_card.cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeShuZi(cards, i, j))
			{
				cards[i][j] -= 1;
				cards[i][j + 1] -= 1;
				cards[i][j + 2] -= 1;
				cards[i][0] -= 3;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][j + 1] += 1;
				cards[i][j + 2] += 1;
				cards[i][0] += 3;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->huzi = j == 1 ? 3 : 0;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, j));
					door_card->cards.push_back(RecoverCard(i, j + 1));
					door_card->cards.push_back(RecoverCard(i, j + 2));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckCompose2710(cards, i, j))
			{
				cards[i][2] -= 1;
				cards[i][7] -= 1;
				cards[i][10] -= 1;
				cards[i][0] -= 3;
				is_hu = CheckCompose333(cards);
				cards[i][2] += 1;
				cards[i][7] += 1;
				cards[i][10] += 1;
				cards[i][0] += 3;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->huzi = 3;
					if (i == 1) door_card->huzi *= 2;
					door_card->cards.push_back(RecoverCard(i, 2));
					door_card->cards.push_back(RecoverCard(i, 7));
					door_card->cards.push_back(RecoverCard(i, 10));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeOneSmallTwoBig(cards, j))
			{
				cards[0][j] -= 1;
				cards[0][0] -= 1;
				cards[1][j] -= 2;
				cards[1][0] -= 2;
				is_hu = CheckCompose333(cards);
				cards[0][j] += 1;
				cards[0][0] += 1;
				cards[1][j] += 2;
				cards[1][0] += 2;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->huzi = 0;
					door_card->cards.push_back(RecoverCard(0, j));
					door_card->cards.push_back(RecoverCard(1, j));
					door_card->cards.push_back(RecoverCard(1, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeTowSmallOneBig(cards, j))
			{
				cards[0][j] -= 2;
				cards[0][0] -= 2;
				cards[1][0] -= 1;
				cards[1][j] -= 1;
				is_hu = CheckCompose333(cards);
				cards[0][j] += 2;
				cards[0][0] += 2;
				cards[1][j] += 1;
				cards[1][0] += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->huzi = 0;
					door_card->cards.push_back(RecoverCard(0, j));
					door_card->cards.push_back(RecoverCard(0, j));
					door_card->cards.push_back(RecoverCard(1, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeLaiZiJiaoPai(cards, j))
			{
				cards[0][j] -= 1;
				cards[1][j] -= 1;
				cards[0][0] -= 1;
				cards[1][0] -= 1;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[0][j] += 1;
				cards[1][j] += 1;
				cards[0][0] += 1;
				cards[1][0] += 1;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->cards.push_back(RecoverCard(0, j));
					door_card->cards.push_back(RecoverCard(1, j));
					//door_card.cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			/************************************2个癞子组合************************************/

			if (CheckCompose2710TowLaiziAnd2(j))
			{
				cards[i][2] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][2] += 1;
				cards[i][0] += 1;
				laizi_num_ += 1;
				if (is_hu)
				{
					return true;
				}
			}

			if (CheckCompose2710TowLaiziAnd7(j))
			{
				cards[i][7] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][7] += 1;
				cards[i][0] += 1;
				laizi_num_ += 1;
				if (is_hu)
				{
					return true;
				}
			}

			if (CheckCompose2710TowLaiziAnd10(j))
			{
				cards[i][10] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][10] += 1;
				cards[i][0] += 1;
				laizi_num_ += 1;
				if (is_hu)
				{
					return true;
				}
			}

			if (CheckComposeTwoLaiZiKeZi(cards, i, j))
			{
				cards[i][j] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 2;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][0] += 1;
				laizi_num_ += 2;
				if (is_hu)
				{
					return true;
				}
			}
		}
	}
	return laizi_num_ % 3 == 0 && cards[0][0] + cards[1][0] == 0;
}

bool ZipaiAlgorithm::CheckJiangCanHu(CardGroup cards)
{
	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (laizi_num() >= 2)
		{
			laizi_num_ -= 2;
			is_hu = CheckCompose433(cards);
			laizi_num_ += 2;
			if (is_hu)
			{
				return true;
			}
		}

		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (cards[i][j] > 2) continue;
			if (cards[i][j] == 2)
			{
				cards[i][j] -= 2;
				cards[i][0] -= 2;
				is_hu = CheckCompose433(cards);
				cards[i][j] += 2;
				cards[i][0] += 2;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->cards.push_back(RecoverCard(i, j));
					door_card->cards.push_back(RecoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (laizi_num() > 0 && cards[i][j] >= 1)
			{
				cards[i][j] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 1;
				is_hu = CheckCompose433(cards);
				cards[i][j] += 1;
				cards[i][0] += 1;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->cards.push_back(RecoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}
		}
	}
	return is_hu;
}

bool ZipaiAlgorithm::CheckCompose433(CardGroup cards)
{
	CardGroup clone_cards = { { 0 } };
	memcpy(clone_cards, cards, sizeof(CardGroup));

	for (int i = 0; i < ROW; ++i)
	{
		if (clone_cards[i][0] < 4) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (clone_cards[i][j] >= 4)
			{
				clone_cards[i][j] -= 4;
				clone_cards[i][0] -= 4;
				auto door_card = std::make_shared<DoorCards>();
				door_card->card_type = KAN_ZI;
				door_card->huzi = 6;
				if (i == 1) door_card->huzi *= 2;
				Card c = RecoverCard(i, j);
				door_card->cards.push_back(c);
				door_card->cards.push_back(c);
				door_card->cards.push_back(c);
				door_card->cards.push_back(c);
				door_group_.push_back(door_card);
			}
		}
	}

	if (CheckCompose333(clone_cards)) 
		return true;

	for (auto &iter : door_group_)
	{
		if (iter->card_type == KAN_ZI)
			iter->cards.clear();
	}
	return false;
}

} //namespace algorithm
