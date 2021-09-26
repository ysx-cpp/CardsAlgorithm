#ifndef MAJONG_MJ_CARD_TYPE_H
#define MAJONG_MJ_CARD_TYPE_H

#include "majong/mjalgorithm.h"

namespace algorithm {

static const std::vector<uint16_t> CARDS_WALL {
    0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
    0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
    0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
    0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
    0x10, 0x20, 0x30, 0x40,
    0x10, 0x20, 0x30, 0x40,
    0x10, 0x20, 0x30, 0x40,
    0x10, 0x20, 0x30, 0x40,
    0x100, 0x200, 0x300,
    0x100, 0x200, 0x300,
    0x100, 0x200, 0x300,
    0x100, 0x200, 0x300,
    0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000 
};

enum class HuType
{
    DA_SI_XI,           //大四喜
    DA_SAN_YUAN,        //大三元
};

class MajongHuType
{
    using Card = uint16_t;

public:
    MajongHuType() = default;
    ~MajongHuType() = default;

    /**************88番*****************/
    // 大四喜
    bool DaSiXi(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 大三元 
    bool DaSanYuan(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 九宝连灯
    bool BaoLianDeng9(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 四杠
    bool SiGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 七连对
    bool QiLianDui(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 百万石
    bool BaiWanShi(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************64番*****************/
    // 小四喜
    bool XiaoSixi(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 字一色
    bool ZiYiSe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 四暗刻
    bool SiAnKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色双龙会
    bool YiSeShuangLongHui(const std::vector<OutDoorCards> &vec_door_cards) const;

    /**************48番*****************/
    // 一色四筒顺
    bool YiSeSiTongShun(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色四节高
    bool YiSeSiJieGao(const std::vector<OutDoorCards> &vec_door_cards) const;

    /**************32番*****************/
    // 一色四步高
    bool YiSeSiBuGao(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 三杠
    bool SanGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 混幺九
    bool HunYao9(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************24番*****************/
    // 七对
    bool QiDui(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 清一色
    bool QingYiSe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色三同花
    bool YiSeSanTongShun(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色三节高
    bool YiSeSanJieGao(const std::vector<OutDoorCards> &vec_door_cards) const;

private:
    uint16_t Wan(Card c) const {return c & 0x000F;}
    uint16_t Feng(Card c) const {return c & 0x00F0;}
    uint16_t Zi(Card c) const {return c & 0x0F00;}
    uint16_t Hua(Card c) const {return c & 0xF000;}

    bool IsMyriao(const ICardPtr &c) const {return c->type() == MjCard::E_MYRIAD_TYPE;} //万
    bool IsWind(const ICardPtr &c) const {return c->type() == MjCard::E_WIND_TYPE;} //风
    bool IsWord(const ICardPtr &c) const {return c->type() == MjCard::E_WORD_TYPE;} //字
    bool IsFlower(const ICardPtr &c) const {return c->type() == MjCard::E_FLOWER_TYPE;} //花

private:
    MjAlgorithm algorithm_;
};

} //namespace algorithm

# endif //MAJONG_MJ_CARD_TYPE_H