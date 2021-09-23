#include "majonghutype.h"
#include <algorithm>
#include <set>

namespace algorithm {



/**************88番*****************/
bool MajongHuType::DaSiXi(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it :vec_door_cards)
    {
        auto &c = it.cards.front();
        if (it.card_type == DoorCardType::KE_ZI && IsFeng(c))
            count++;
    }
    return count == 4;
}

// 大三元
bool MajongHuType::DaSanYuan(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::KE_ZI || it.cards.size() == 4)
        {
            auto &c = it.cards.front();
            if (IsZi(c))
                count++;
        }
    }
    return count == 3;
}

// 九宝连灯
bool MajongHuType::BaoLianDeng9(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    //去重排序
    std::set<uint16_t> set;
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            set.insert(iter);
        }
    }

    int count = 1;
    auto prev = set.begin();
    for (auto it = ++set.begin(); it != set.end(); ++it)
    {
        if ((*prev + 1) == *it)
            count++;
    }
    return count == 9;
}

// 四杠
bool MajongHuType::SiGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.cards.size() >= 4)
            count++;
    }
    return count == 4;
}

// 七连对
bool MajongHuType::QiLianDui(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 1;
    std::vector<uint16_t> vec;

    if (!QingYiSe(vec_door_cards))
        return false;

    for (auto &it : vec_door_cards)
    {
        vec.insert(vec.end(), it.cards.begin(), it.cards.end());
    }

    std::sort(vec.begin(), vec.end());

    auto prev = vec.begin();
    for (auto it = vec.begin() + 2; it != vec.end(); it += 2)
    {
        if ((*prev + 1) == *it)
            count++;
        
        prev += 2;
    }

    return count == 7;
}

// 百万石
bool MajongHuType::BaiWanShi(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int sum = 0;

    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            if (!IsWan(iter))
                return false;

            sum += Wan(iter);
        }
    }

    return sum >= 100;
}

/**************64番*****************/
// 小四喜
bool MajongHuType::XiaoSixi(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count2 = 0;
    int count3 = 0;
    for (auto &it :vec_door_cards)
    {
        auto &c = it.cards.front();
        if (it.card_type == DoorCardType::KE_ZI && IsFeng(c))
            count3++;
        else if (it.cards.size() == 2 && IsFeng(c))
            count2++; 
    }
    return (count3 == 3 && count2 == 1);
}

// 字一色
bool MajongHuType::ZiYiSe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            if (!IsFeng(iter) && !IsZi(iter))
                return false;
        }
    }
    return true;
}

// 四暗刻
bool MajongHuType::SiAnKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    return SiGang(vec_door_cards);
}

// 一色双龙会
bool MajongHuType::YiSeShuangLongHui(const std::vector<OutDoorCards> &vec_door_cards) const
{
    auto lamb123 = [](const std::vector<uint16_t> &vec)->bool {
        return (vec[0] == 1 && vec[1] == 2 && vec[2] == 3);
    };

    auto lamb789 = [](const std::vector<uint16_t> &vec) -> bool {
        return (vec[0] == 7 && vec[1] == 8 && vec[2] == 9);
    };

    if (!QingYiSe(vec_door_cards))
        return false;

    int count_123 = 0;
    int count_789 = 0;
    bool jiang = false;
    for (auto &it : vec_door_cards)
    {
        auto &c = it.cards.front();
        if (it.card_type == DoorCardType::JIANG && Value(c) == 5)
            jiang = true;

        auto vec = it.cards;
        std::sort(vec.begin(), vec.end());

        if (it.card_type == DoorCardType::SHUN_ZI && lamb123(vec))
            count_123++;

        if (it.card_type == DoorCardType::SHUN_ZI && lamb789(vec))
            count_789++;
    }

    return (count_123 == 2 && count_789 == 2 && jiang);
}

/**************48番*****************/
// 一色四筒顺
bool MajongHuType::YiSeSiTongShun(const std::vector<OutDoorCards> &vec_door_cards) const
{
    auto lamb_compare = [](const std::vector<uint16_t> &vec1, const std::vector<uint16_t> &vec2)->bool {
        return (vec1[0] == vec2[0] &&
                vec1[1] == vec2[1] &&
                vec1[2] == vec2[2]);
    };

    if (!QingYiSe(vec_door_cards))
        return false;

    auto &first = vec_door_cards.front();
    for (auto it = vec_door_cards.begin() + 1; it != vec_door_cards.end(); ++it)
    {
        if (it->card_type != DoorCardType::SHUN_ZI)
            return false;

        if (!lamb_compare(first.cards, it->cards))
            return false;
    }

    return true;
}

// 一色四节高
bool MajongHuType::YiSeSiJieGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    // 碰碰胡

    std::set<uint16_t> set;
    for (auto &it : vec_door_cards)
    {
        set.insert(it.cards.front());
    }

    int count = 1;
    auto prev = set.begin();
    for (auto it = ++set.begin(); it != set.end(); ++it)
    {
        if ((*prev + 1) == *it)
            count++;
    }
    return count == 4;
}

/**************48番*****************/
// 一色四步高
bool MajongHuType::YiSeSiBuGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    return false;    
}

// 三杠
bool MajongHuType::SanGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::GANG && it.cards.size() == 4)
            count++;
    }
    return count == 3;
}

// 混幺九
bool MajongHuType::HunYao9(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            if (Wan(iter) != 1 && Wan(iter) != 9)
                return false;
        }
    }
    return true;
}

/**************24番*****************/
// 七对
bool MajongHuType::QiDui(const std::vector<OutDoorCards> &) const
{
    return false;
}

// 清一色
bool MajongHuType::QingYiSe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    auto &c = vec_door_cards.front().cards.front();
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            if (Type(c) != Type(iter))
                return false;
        }
    }
    return true;
}

// 一色三同花
bool MajongHuType::YiSeSanTongShun(const std::vector<OutDoorCards> &) const
{
    return false;
}

// 一色三节高
bool MajongHuType::YiSeSanJieGao(const std::vector<OutDoorCards> &) const
{
    return false;
}

MjCard::EType MajongHuType::Type(Card c) const
{
    if (IsWan(c))
        return MjCard::E_MYRIAD_TYPE;
    
    if (IsFeng(c))
        return MjCard::E_WIND_TYPE;

    if (IsZi(c))
        return MjCard::E_WORD_TYPE;

    return MjCard::E_FLOWER_TYPE;
}

uint16_t MajongHuType::Value(Card c) const
{
    if (IsWan(c))
        return c;
    
    if (IsFeng(c))
        return c >> 1;

    if (IsZi(c))
        return c >> 2;
    
    return c >> 3;
}

} //namespace algorithm