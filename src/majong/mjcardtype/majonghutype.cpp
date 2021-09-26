#include "majonghutype.h"
#include <algorithm>
#include <set>
#include <iostream>

namespace algorithm {


/**************88番*****************/
bool MajongHuType::DaSiXi(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it :vec_door_cards)
    {
        auto &c = it.cards.front();
        std::cout << (int)it.card_type << "|" << c->type() << "|" << c->face() << std::endl;
        if (IsWind(c))
        {
            if (it.card_type == DoorCardType::KE_ZI || it.card_type == DoorCardType::GANG)
                count++;
        }
    }
    return count == 4;
}

// 大三元
bool MajongHuType::DaSanYuan(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::KE_ZI || 
        it.card_type == DoorCardType::GANG ||
        it.card_type == DoorCardType::AN_GANG)
        {
            auto &c = it.cards.front();
            if (IsWord(c))
                count++;
        }
    }
    return count == 3;
}

// 九宝连灯
bool MajongHuType::JiuBaoLianDeng(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    //去重排序
    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            set.insert(iter->face());
        }
    }

    int count = 1;
    auto prev = set.begin();
    for (auto it = ++set.begin(); it != set.end(); ++it)
    {
        if ((*prev + 1) == *it)
            count++;

        ++prev;
    }
    return count == 9;
}

// 四杠
bool MajongHuType::SiGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::GANG)
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
        for (auto &iter : it.cards)
        {
            vec.push_back(iter->face());
        }
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
            if (!IsMyriao(iter))
                return false;

            sum += iter->face();
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
        if (it.card_type == DoorCardType::KE_ZI && IsWind(c))
            count3++;
        else if (it.card_type == DoorCardType::JIANG && IsWind(c))
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
            if (!IsWind(iter) && !IsWord(iter))
                return false;
        }
    }
    return true;
}

// 四暗刻
bool MajongHuType::SiAnKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::AN_GANG)
            count++;
    }
    return count == 4;
}

// 一色双龙会
bool MajongHuType::YiSeShuangLongHui(const std::vector<OutDoorCards> &vec_door_cards) const
{
    auto lamb123 = [](const std::vector<ICardPtr> &vec)->bool {
        return (vec[0]->face() == 1 && vec[1]->face() == 2 && vec[2]->face() == 3);
    };

    auto lamb789 = [](const std::vector<ICardPtr> &vec) -> bool {
        return (vec[0]->face() == 7 && vec[1]->face() == 8 && vec[2]->face() == 9);
    };

    if (!QingYiSe(vec_door_cards))
        return false;

    int count_123 = 0;
    int count_789 = 0;
    bool jiang = false;
    for (auto &it : vec_door_cards)
    {
        auto &c = it.cards.front();
        if (it.card_type == DoorCardType::JIANG && c->face() == 5)
            jiang = true;

        auto vec = it.cards;
        std::sort(vec.begin(), vec.end(), [](const ICardPtr &c1, ICardPtr &c2){
            return c1->face() < c2->face();
        });

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
    auto lamb_compare = [](const std::vector<ICardPtr> &vec1, const std::vector<ICardPtr> &vec2)->bool {
        return (vec1[0]->face() == vec2[0]->face() &&
                vec1[1]->face() == vec2[1]->face() &&
                vec1[2]->face() == vec2[2]->face());
    };

    if (!QingYiSe(vec_door_cards))
        return false;

    auto &first = vec_door_cards.front();
    for (auto it = vec_door_cards.begin() + 1; it != vec_door_cards.end(); ++it)
    {
        if (it->card_type == DoorCardType::JIANG)
            continue;        

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

    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::JIANG)
            continue;
            
        set.insert(it.cards.front()->face());
    }

    int count = 1;
    auto prev = set.begin();
    for (auto it = ++set.begin(); it != set.end(); ++it)
    {
        if ((*prev + 1) == *it)
            count++;

        ++prev;
    }
    return count == 4;
}

/**************48番*****************/
// 一色四步高
bool MajongHuType::YiSeSiBuGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    auto vec_copy = vec_door_cards;
    for (auto &it : vec_copy)
    {
        std::sort(it.cards.begin(), it.cards.end(), [](const ICardPtr &item1, const ICardPtr &item2){
            return item1->face() < item2->face();
        });
    }

    std::sort(vec_copy.begin(), vec_copy.end(), [](const OutDoorCards &item1, const OutDoorCards &item2){
        return item1.cards.front()->face() < item2.cards.front()->face();
    });

    int count = 1;
    for (auto it = vec_copy.begin() + 1; it != vec_copy.end(); ++it)
    {
        auto prev = it - 1;
        auto c1 = prev->cards.front();
        auto c2 = it->cards.front();
        if (c1->face() + 1 != c2->face())
            count++;
    }

    if (count == 4)
        return true;

    count = 1;
    for (auto it = vec_copy.begin() + 1; it != vec_copy.end(); ++it)
    {
        auto prev = it - 1;
        auto c1 = prev->cards.front();
        auto c2 = it->cards.front();
        if (c1->face() + 2 != c2->face())
            count++;
    }    

    return count == 4;    
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
            if (IsMyriao(iter) && iter->face() != 1 && iter->face() != 9)
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
            if (c->type() != iter->type())
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

} //namespace algorithm