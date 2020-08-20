/*
 * @file pokeralgorithm.cpp
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#include "pokeralgorithm.h"
#include <assert.h>
#include <stdexcept>
#include <string.h>
#include <algorithm>
#include <functional>
#include <map>

namespace algorithm {

ICardPtr PokerAlgorithm::FindMaxCard(const std::vector<CardInfo>& cards_info)
{
	std::vector<CardInfo>::const_iterator max_it = std::max_element(cards_info.begin(), cards_info.end());
	if (max_it == cards_info.end())
	{
		return ICardPtr(0);
	}
	return max_it->cards.front();
}

bool PokerAlgorithm::FindSameColor(const size_t size, const std::vector<CardInfo>& cards_info, std::vector<ICardPtr> &cards)
{
	for (int type = CPkCard::E_SQUARE_TYPE; type > CPkCard::E_TYPE_COUNT_; ++type)
	{
		for (std::vector<CardInfo>::const_reverse_iterator it = cards_info.rbegin();
			it != cards_info.rend();
			++it)
		{
			for (auto &iter : it->cards)
			{
				if (type != iter->FaceId()) continue;
				cards.push_back(iter);
				if (cards.size() == size)
				{
					return true;
				}
			}
		}
		cards.clear();
	}
	cards.clear();
	return false;
}

bool PokerAlgorithm::FindStraightFlush(const int begin_face, const size_t size, const std::vector<CardInfo>& cards_info, std::vector<ICardPtr> &cards)
{
	for (int type = CPkCard::E_SQUARE_TYPE; type < CPkCard::E_TYPE_COUNT_; ++type)
	{
		cards.clear();
		for (std::vector<CardInfo>::const_reverse_iterator it = cards_info.rbegin() + 1; it != cards_info.rend(); ++it)
		{
			if (it->face < begin_face) continue;

			auto iter = std::find_if((it - 1)->cards.begin(), (it - 1)->cards.end(), [type](const ICardPtr & item){
				return item->TypeId() == type;
			});
			if (iter == it->cards.end())
			{
				cards.clear();
				ICardPtr c = *iter;
				cards.push_back(c);
				continue;
			}

			if ((it - 1)->face + 1 == it->face)
			{
				ICardPtr c = *iter;
				cards.push_back(c);
			}
			else
			{
				cards.clear();
				ICardPtr c = *iter;
				cards.push_back(c);
			}
			if (cards.size() == size)
			{
				return true;
			}
		}
	}
	if (cards.size() == size)
	{
		return true;
	}
	cards.clear();
	return false;
}

bool PokerAlgorithm::FindNonContinuousCards(const int /*begin_face*/, const size_t count, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	auto iter_laizi = std::find(cards_info.begin(), cards_info.end(), laizi_card_);

	size_t laizi_card_num = 0;
	if (iter_laizi != cards_info.end())
		laizi_card_num = iter_laizi->cards.size();

	for (std::vector<CardInfo>::const_reverse_iterator it = cards_info.rbegin(); it != cards_info.rend(); ++it)
	{
		if (*it == laizi_card_) continue;
		if (it->cards.size() >= count)
		{
			cards.assign(it->cards.begin(), it->cards.begin() + count);
			return true;
		}
		if (it->cards.size() + laizi_card_num >= count)
		{
			cards.assign(it->cards.begin(), it->cards.end());

			int need_laizi_num = count - it->cards.size();
			cards.insert(cards.end(), iter_laizi->cards.begin(), iter_laizi->cards.begin() + need_laizi_num);
			return true;
		}
	}
	return false;
}

bool PokerAlgorithm::FindContinuousCards(const int begin_face, const size_t count, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	size_t continue_size = 1;
	if (cards_info.front().cards.size() >= count)
	{
		auto it = cards_info.cbegin();
		int push_num = std::min(it->cards.size(), count);
		cards.insert(cards.begin(), it->cards.begin(), it->cards.begin() + push_num);
	}

	for (std::vector<CardInfo>::const_iterator it = cards_info.begin() + 1; it != cards_info.end(); ++it)
	{
		if (it->face < begin_face) continue;

		if ((it - 1)->face + 1 == it->face && it->cards.size() >= count && (it - 1)->cards.size() >= count)
		{
			++continue_size;
			int push_num = std::min(it->cards.size(), count);
			cards.insert(cards.end(), it->cards.begin(), it->cards.begin() + push_num);
		}
		else
		{
			continue_size = 1;
			cards.clear();
			if (it->cards.size() >= count)
			{
				int push_num = std::min(it->cards.size(), count);
				cards.insert(cards.end(), it->cards.begin(), it->cards.begin() + push_num);
			}
		}

		if (continue_size == size) 
			return true;
	}
	if (cards.size() / count == size) 
		return true;

	cards.clear();
	return FindLaiZiContinuousCards(begin_face, count, size, cards_info, cards);
}

bool PokerAlgorithm::FindLaiZiContinuousCards(const int begin_face, const size_t count, const size_t size, const std::vector<CardInfo> & cards_info, std::vector<ICardPtr> &cards)
{
	auto iter_laizi = std::find(cards_info.begin(), cards_info.end(), laizi_card_);
	if (iter_laizi == cards_info.end())
		return false;

	if (static_cast<size_t>(CPkCard::E_FACE_TWO - begin_face) < size)
		return false;

	std::map<int, size_t> card_slot;
	for (int first = begin_face; static_cast<size_t>(first) < CPkCard::E_FACE_TWO - size; ++first)
	{
		card_slot.emplace(first, count * size);
		for (size_t k = first; k < first + size; ++k)
		{
			auto it = find(cards_info.begin(), cards_info.end(), k);
			if (it != cards_info.end())
			{
				size_t face_count = it->cards.size();
				card_slot[first] -= std::min(count, face_count);
			}
		}
	}

	auto iter_eligible = std::find_if(card_slot.rbegin(), card_slot.rend(), [iter_laizi](const std::pair<int, int> &item){
		return static_cast<size_t>(item.second) <= iter_laizi->cards.size();
	});

	if (iter_eligible == card_slot.rend())
		return false;

	int need_laizi_num = iter_eligible->second;
	int first_face = iter_eligible->first;

	for (std::vector<CardInfo>::const_iterator it = cards_info.cbegin(); it != cards_info.cend(); ++it)
	{
		if (*it == laizi_card_) continue;
		if (it->face >= first_face && static_cast<size_t>(it->face) < first_face + size)
		{
			int push_num = std::min(it->cards.size(), count);
			cards.insert(cards.end(), it->cards.begin(), it->cards.begin() + push_num);
		}
	}
	cards.insert(cards.end(), iter_laizi->cards.begin(), iter_laizi->cards.begin() + need_laizi_num);

	return true;
}

bool PokerAlgorithm::IsNonContinuousCards(const std::vector<CardInfo> & cards_info, const size_t count)
{
	auto iter_laizi = std::find(cards_info.begin(), cards_info.end(), laizi_card_);

	size_t laizi_card_num = 0;
	if (iter_laizi != cards_info.end())
		laizi_card_num = iter_laizi->cards.size();

	for (std::vector<CardInfo>::const_reverse_iterator it = cards_info.rbegin(); it != cards_info.rend(); ++it)
	{
		if (*it == laizi_card_) continue;
		if (it->cards.size() + laizi_card_num == count ||
			it->cards.size() == count)
		{
			return true;
		}
	}
	return false;
}

bool PokerAlgorithm::IsContinuousCards(const std::vector<CardInfo> & cards_info, const size_t count, const size_t size)
{
	if (size < 2) return false;

	size_t num = 1;
	for (std::vector<CardInfo>::const_iterator it = cards_info.begin() + 1; it != cards_info.end(); ++it)
	{
		if ((it - 1)->face + 1 == it->face && (it - 1)->cards.size() >= count && it->cards.size() >= count)
			num++;
		else
			num = 1;

		if (num == size) return true;
	}
	if (num == size) 
		return true;

	return IsLaiZiContinuousCards(cards_info, count, size);
}

bool PokerAlgorithm::IsLaiZiContinuousCards(const std::vector<CardInfo> & cards_info, const size_t count, const size_t size)
{
	auto iter_laizi = std::find(cards_info.begin(), cards_info.end(), laizi_card_);
	if (iter_laizi == cards_info.end())
		return false;

	std::map<int, size_t> card_slot;
	int begin_face = cards_info.front().face;
	if (static_cast<size_t>(begin_face) > CPkCard::E_FACE_TWO - size)
		begin_face = CPkCard::E_FACE_TWO - size;

	for (int first = begin_face; static_cast<size_t>(first) <= CPkCard::E_FACE_TWO - size; ++first)
	{
		card_slot.emplace(first, count * size);
		for (int k = first; static_cast<size_t>(k) < first + size; ++k)
		{
			if (k == laizi_card_->FaceId()) continue;
			auto it = find_if(cards_info.begin(), cards_info.end(), [k](const CardInfo & item){
				return item.face == k;
			});
			if (it != cards_info.end())
			{
				size_t face_count = it->cards.size();
				card_slot[first] -= std::min(count, face_count);
			}
		}
	}

	auto iter_eligible = std::find_if(card_slot.rbegin(), card_slot.rend(), [iter_laizi](const std::pair<int, int> &item){
		return static_cast<size_t>(item.second) <= iter_laizi->cards.size();
	});

	return iter_eligible == card_slot.rend();
}

void PokerAlgorithm::SrotCard(std::vector<ICardPtr>& cards)
{
	std::sort(cards.begin(), cards.end(), CompareCardValue);
}

void PokerAlgorithm::CardInfoToPokerGroup(const std::vector<CardInfo> & cards_info, std::vector<int> & poker_group)
{
	poker_group.resize(CPkCard::E_FACE_COUNT_, 0);
	for (std::vector<CardInfo>::const_iterator it = cards_info.begin(); it != cards_info.end(); ++it)
	{
		for (auto & iter : it->cards)
		{
			//poker_group[it->face] += 1;原本是这样，为了处理编译警告改成如下
			poker_group[iter->FaceId()] += 1;
			poker_group[0] += 1;
		}
	}
}

void PokerAlgorithm::CardsToCardInfo(const std::vector<ICardPtr> & hand_cards, std::vector<CardInfo> & hand_cards_info)
{
	for (auto it = hand_cards.begin(); it != hand_cards.end(); ++it)
	{	
		auto iter = std::find(hand_cards_info.begin(), hand_cards_info.end(), *it);
		int face = (*it)->FaceId();
		if (iter == hand_cards_info.end())
		{
			CardInfo cardInfo(face);
			cardInfo.cards.push_back(*it);
			hand_cards_info.push_back(cardInfo);
		}
		else
		{
			iter->cards.push_back(*it);
		}
	}
	
	std::sort(hand_cards_info.begin(), hand_cards_info.end());

	for (std::vector<CardInfo>::iterator it = hand_cards_info.begin(); it != hand_cards_info.end(); ++it)
	{
		assert(!it->cards.empty());
	//	std::sort(it->cards.begin(), it->cards.end(), &CompareCardType);
	}
}

void PokerAlgorithm::PrintCards(const std::vector<ICardPtr>& /*cards*/)
{

}

bool PokerAlgorithm::CompareCardType(const int & type1, const int & type2)
{
	return type1 < type2;
}

bool PokerAlgorithm::CompareCardValue(const ICardPtr & c1, const ICardPtr & c2)
{
	if (c1->FaceId() == c2->FaceId())
		return c1->TypeId() < c2->TypeId();

	return c1->FaceId() < c2->FaceId();
}

bool PokerAlgorithm::CompareCardIndex(const ICardPtr & c1, const ICardPtr & c2)
{
	return c1->IndexId() < c2->IndexId();
}

} //namespace algorithm