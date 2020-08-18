#ifndef PLAY_CARD_INFO_H
#define PLAY_CARD_INFO_H
#include <vector>

namespace algorithm {

enum PkOutCardType
{
	E_PK_OCT_INVALID, //E_DCT_INVALID
	E_PK_OCT_LEAFLET, //单张
	E_PK_OCT_PAIR, //对子
	E_PK_OCT_PAIRS, //连对
	E_PK_OCT_STRAIGHT, //顺子
	E_PK_OCT_AIRCRAFT, //飞机
	E_PK_OCT_THREE, //三张
	E_PK_OCT_THREE_BELT_ONE, //三带一
	E_PK_OCT_THREE_BELT_TOW, //三带二
	E_PK_OCT_THREE_BELT_PAIR, //三带一对
	E_PK_OCT_FOUR_BELT_ONE, //四带一
	E_PK_OCT_FOUR_BELT_TOW, //四带二
	E_PK_OCT_FOUR_BELT_THREE, //四带三
	E_PK_OCT_BOMB, //炸弹
	E_PK_OCT_LAIZI_BOMB, //癞子炸
	E_PK_OCT_KING_BOMB, //王炸
};

struct PlayCardInfo
{
	PlayCardInfo & operator =(const PlayCardInfo & other)
	{
		card_type = other.card_type;
		card_value = other.card_value;
		continu_leng = other.continu_leng;
		play_seatId = other.play_seatId;
		vec_play_cards.assign(other.vec_play_cards.begin(), other.vec_play_cards.end());
		return *this;
	}
	void clear()
	{
		card_type = 0;
		card_value = 0;
		continu_leng = 0;
		play_seatId = 0;
		vec_play_cards.clear();
	}
	int			card_type = 0;	//牌型
	int			card_value = 0; //牌值大小
	int			continu_leng = 0;//连续长度
	int			play_seatId = 0; //出牌的座位
	std::vector<int> vec_play_cards;
};
} //namespace algorithm

#endif