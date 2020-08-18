#ifndef PLAY_CARD_INFO_H
#define PLAY_CARD_INFO_H
#include <vector>

namespace algorithm {

enum PkOutCardType
{
	E_PK_OCT_INVALID, //E_DCT_INVALID
	E_PK_OCT_LEAFLET, //����
	E_PK_OCT_PAIR, //����
	E_PK_OCT_PAIRS, //����
	E_PK_OCT_STRAIGHT, //˳��
	E_PK_OCT_AIRCRAFT, //�ɻ�
	E_PK_OCT_THREE, //����
	E_PK_OCT_THREE_BELT_ONE, //����һ
	E_PK_OCT_THREE_BELT_TOW, //������
	E_PK_OCT_THREE_BELT_PAIR, //����һ��
	E_PK_OCT_FOUR_BELT_ONE, //�Ĵ�һ
	E_PK_OCT_FOUR_BELT_TOW, //�Ĵ���
	E_PK_OCT_FOUR_BELT_THREE, //�Ĵ���
	E_PK_OCT_BOMB, //ը��
	E_PK_OCT_LAIZI_BOMB, //���ը
	E_PK_OCT_KING_BOMB, //��ը
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
	int			card_type = 0;	//����
	int			card_value = 0; //��ֵ��С
	int			continu_leng = 0;//��������
	int			play_seatId = 0; //���Ƶ���λ
	std::vector<int> vec_play_cards;
};
} //namespace algorithm

#endif