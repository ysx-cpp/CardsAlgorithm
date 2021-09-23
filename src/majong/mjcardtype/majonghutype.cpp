#include "majonghutype.h"

namespace algorithm {

bool MajongHuType::DaSiXi(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it :vec_door_cards)
    {
        auto &c = it.hand_cards.front();
        if (it.card_type == DoorCardType::KE_ZI && IsFeng(c))
            count++;
    }
    return count == 4;
}

/**************88番*****************/
// 大四喜
bool MajongHuType::DaSiXi(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
}

// 大三元
bool MajongHuType::DaSanYuan(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 九宝连灯
bool MajongHuType::BaoLianDeng9(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 四杠
bool MajongHuType::SiGang(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 七连对
bool MajongHuType::QingLianDui(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 百万石
bool MajongHuType::BaiWanShi(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

/**************64番*****************/
// 小四喜
bool MajongHuType::XiaoSixi(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 字一色
bool MajongHuType::ZiYiSe(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 四暗刻
bool MajongHuType::SiAnKe(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 一色双龙会
bool MajongHuType::YiSeShuangLongHui(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

/**************48番*****************/
// 一色四筒顺
bool MajongHuType::YiSeSiTongShun(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 一色四节高
bool MajongHuType::YiSeSiJieGao(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

/**************48番*****************/
// 一色四步高
bool MajongHuType::YiSeSiBuGao(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 三杠
bool MajongHuType::SanGang(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

// 混幺九
bool MajongHuType::HunYao9(const std::vector<OutDoorCards> &vec_door_cards) const
{

}

} //namespace algorithm