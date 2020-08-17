#ifndef ROBOT_ZP_HU_PAI_ALGO_H
#define ROBOT_ZP_HU_PAI_ALGO_H

#include <stdint.h>
#include <vector>
#include <memory>

namespace by {
namespace zipaialgorithm {

static const int32_t ROW = 2;
static const int32_t CLOU = 11;
using Card = unsigned char;
using CardGroup = Card[ROW][CLOU];

static CardGroup ZIPAI_CARD_GROUP = { {0} };

class ZpHuPaiAlgorithm
{
public:
	ZpHuPaiAlgorithm();
	~ZpHuPaiAlgorithm();

	bool CheckHu(const std::vector<Card> &hand_cards);
	void OutPutDoorCards(std::vector<Card> &hand_cards);

	void SetLaiZi(const int num) { laizi_num_ = num; }
	const int laizi_num() const { return laizi_num_; }

private:
	void InputHandCard(const std::vector<Card> &hand_cards, CardGroup cards);
	void ComposeKeZi(CardGroup cards, const Card target_card, const bool is_other);
	bool CheckHaveKanZi(const CardGroup cards) const;
	bool CheckCompose333(CardGroup cards);
	bool CheckJiangCanHu(CardGroup cards);
	bool CheckCompose433(CardGroup cards);

	bool CheckComposeShuZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (value > 8) return false;
		return cards[type][value + 1] > 0 && cards[type][value + 2];
	}

	bool CheckCompose2710(const CardGroup cards, const int &type, const int &value) const
	{
		if (value != 2 && value != 7 && value != 10) return false;
		return cards[type][2] > 0 && cards[type][7] > 0 && cards[type][10] > 0;
	}

	bool CheckComposeKeZi(const CardGroup cards, const int &type, const int &value) const
	{
		return cards[type][value] == 3;
	}

	bool CheckComposeKanZi(const CardGroup cards, const int &type, const int &value) const
	{
		return cards[type][value] >= 4;
	}

	//1个小字2个大字
	bool CheckComposeOneSmallTwoBig(const CardGroup cards, const int &type, const int &value) const
	{
		return cards[0][value] >= 1 && cards[1][value] >= 2;
	}

	//2个小字1个大字
	bool CheckComposeTowSmallOneBig(const CardGroup cards, const int &type, const int &value) const
	{
		return cards[0][value] >= 2 && cards[1][value] >= 1;
	}

	/**********************************1癞子**************************************/
	//27%
	bool CheckCompose2710LaiziAnd27(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() <= 0) return false;
		return value == 2 && cards[type][7] > 0;
	}

	//2%10
	bool CheckCompose2710LaiziAnd210(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() <= 0) return false;
		return value == 2 && cards[type][10] > 0;
	}

	//%710
	bool CheckCompose2710LaiziAnd710(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() <= 0) return false;
		return value == 7 && cards[type][10] > 0;
	}

	//12%
	bool CheckComposeLaiZiShuZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() == 0 || value >= 10) return false;
		return cards[type][value + 1] > 0;
	}

	//1%3
	bool CheckComposeMiddleLaiZiShuZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() == 0 || value >= 9) return false;
		return cards[type][value + 2];
	}

	//11%
	bool CheckComposeLaiZiKeZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() <= 0) return false;
		return cards[type][value] >= 2;
	}

	//一壹%, 二贰%， 三叁%, ... 绞牌
	bool CheckComposeLaiZiJiaoPai(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() == 0) return false;
		return cards[0][value] > 0 && cards[1][value] > 0;
	}

	/************************************2癞子************************************/
	//2%% 2710
	bool CheckCompose2710TowLaiziAnd2(const CardGroup cards, const int &type, const int &value) const
	{
		return laizi_num() >= 2 && value == 2;
	}

	//%7% 2710
	bool CheckCompose2710TowLaiziAnd7(const CardGroup cards, const int &type, const int &value) const
	{
		return laizi_num() >= 2 && value == 7;
	}

	//%%10 2710
	bool CheckCompose2710TowLaiziAnd10(const CardGroup cards, const int &type, const int &value) const
	{
		return laizi_num() >= 2 && value == 10;
	}

	//1%% 123
	bool CheckCompose123TowLaiziAnd1(const CardGroup cards, const int &type, const int &value) const
	{
		return laizi_num() >= 2 && value == 1;
	}

	//%2% 123 
	bool CheckCompose123TowLaiziAnd2(const CardGroup cards, const int &type, const int &value) const
	{
		return laizi_num() >= 2 && value == 2;
	}

	//%%3 123
	bool CheckCompose123TowLaiziAnd3(const CardGroup cards, const int &type, const int &value) const
	{
		return laizi_num() >= 2 && value == 3;
	}

	//1%%
	bool CheckComposeTwoLaiZiKeZi(const CardGroup cards, const int &type, const int &value) const
	{
		return laizi_num() >= 2 && cards[type][value] > 0;
	}

	Card recoverCard(const int &type, const int &value) const
	{
		return ((type + 1) << 4) | (value & 0x0F);
	}

	int type(const Card c) const 
	{
		return (c & 0xF0) >> 4;
	}

	int value(const Card c) const
	{
		return c & 0x0F;
	}

private:
	int laizi_num_;

	struct DoorCards
	{
		int huzi = 0;
		int card_type = -1;
		std::vector<Card> cards;
	};
	std::vector<std::shared_ptr<DoorCards>> door_group_;
};

} //namespace zipaialgorithm
} //namespace by

#endif //HU_PAI_ALGO_H