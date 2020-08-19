/*
 * @file mjalgorithm.cpp
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#include "mjalgorithm.h"

namespace algorithm {

MjAlgorithm::MjAlgorithm()
{
}

MjAlgorithm::~MjAlgorithm()
{
}

void MjAlgorithm::InputHandCard(const std::vector<ICardPtr> &hand_cards)
{
	hand_cards_info_.clear();
	memset(card_group_, 0, sizeof(CardGroup));

	for (auto &iter : hand_cards)
	{
		if (iter->TypeId() == type(laizi_card_) && iter->FaceId() == value(laizi_card_))
			continue;

		card_group_[iter->TypeId()][iter->FaceId()] += 1;
		card_group_[iter->TypeId()][0] += 1;

		auto c = recoverCard(iter->TypeId(), iter->FaceId());
		hand_cards_info_[c].push_back(iter->IndexId());
	}
}

//void MjAlgorithm::OutPutDoorCards(std::vector<DoorGroupPtr> &door_card_group)
//{
//	for (auto &it : door_group_)
//	{
//		DoorGroupPtr door_ptr = std::make_shared<DoorGroup>();
//		for (auto &iter : it->cards)
//		{
//			if (hand_cards_info_.find(iter) != hand_cards_info_.end() && !hand_cards_info_[iter].empty())
//			{
//				int index = hand_cards_info_[iter].back();
//				door_ptr->m_vSelfCard.push_back(index);
//				hand_cards_info_[iter].pop_back();
//			}		
//		}
//		door_card_group.push_back(door_ptr);
//	}
//}

bool MjAlgorithm::CheckPingHu()
{
	return CheckJiangCanHu(card_group_);
}

bool MjAlgorithm::CheckQiDuiHu()
{
	return QiDuiHu(card_group_);
}

bool MjAlgorithm::CheckShiSanYaoHu()
{
	return ShiSanYaoHu(card_group_);
}

bool MjAlgorithm::CheckJiangCanHu(CardGroup cards)
{
	if (laizi_num() >= 2 && CheckLaiZiJiangHu(cards))
	{
		return true;
	}

	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (cards[i][j] > 2) continue;
			if (cards[i][j] == 2)
			{
				cards[i][j] -= 2;
				cards[i][0] -= 2;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 2;
				cards[i][0] += 2;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (cards[i][j] == 1 && laizi_num() >= 1)
			{
				cards[i][j] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][0] += 1;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}
		}
	}
	return is_hu;
}

bool MjAlgorithm::CheckLaiZiJiangHu(CardGroup cards)
{
	bool is_hu = false;

	laizi_num_ -= 2;
	is_hu = CheckCompose333(cards);
	laizi_num_ += 2;

	if (is_hu)
	{
		auto door_card = std::make_shared<DoorCards>();
		door_card->cards.push_back(laizi_card_);
		door_card->cards.push_back(laizi_card_);
		door_group_.push_back(door_card);
		return true;
	}
	return is_hu;
}

bool MjAlgorithm::CheckCompose333(CardGroup cards)
{
	if (laizi_num_ % 3 == 0 && cards[0][0] + cards[1][0] == 0)
	{
		auto door_card = std::make_shared<DoorCards>();
		for (int i = 0; i < laizi_num_; ++i)
			door_card->cards.push_back(laizi_card_);

		door_group_.push_back(door_card);
		return true;
	}

	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (cards[i][j] == 0) continue;

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
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j + 1));
					door_card->cards.push_back(recoverCard(i, j + 2));
					door_group_.push_back(door_card);
					return true;
				}
			}

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
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
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
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j + 1));
					door_card->cards.push_back(laizi_card_);
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
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(laizi_card_);
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
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j + 2));
					door_card->cards.push_back(laizi_card_);
					door_group_.push_back(door_card);
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
					auto door_card = std::make_shared<DoorCards>();
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(laizi_card_);
					door_card->cards.push_back(laizi_card_);
					door_group_.push_back(door_card);
					return true;
				}
			}
		}
	}
	return laizi_num_ % 3 == 0 && cards[0][0] + cards[1][0] == 0;
}

bool MjAlgorithm::QiDuiHu(CardGroup cards)
{
	int single_cnt = 0;
	int all_card = 0;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		all_card += cards[i][0];
		if (cards[i][0] == 1)
		{
			++single_cnt;
			continue;
		}
		for (unsigned j = 1; j < CLOU; j++)
		{
			if (cards[i][j] == 0) continue;
			if (cards[i][j] & 1) //奇数不可能有七对
			{
				single_cnt++;
			}
		}
	}
	//1.红中+手牌不等于14张牌即不能胡七对
	//2.单张牌数据不等于红中数量则不能胡七对
	if ((all_card + laizi_num_ != 14) || single_cnt != laizi_num_)
	{
		return false;
	}
	return true;
}

bool MjAlgorithm::ShiSanYaoHu(CardGroup cards)
{
	int jiang_num = 0;
	int count = 0;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (i < 3 && j != 1 && j != 9) //万 筒 条
			{
				return false;
			}

			if (jiang_num == 0 && cards[i][j] == 2)
			{
				jiang_num = 1;
			}
			if (jiang_num == 1 && cards[i][j] != 1)
			{
				return false;
			}
			else
			{
				++count;
			}
		}
	}
	return count + laizi_num_ + jiang_num * 2 == 14;
}

} //namespace algorithm
