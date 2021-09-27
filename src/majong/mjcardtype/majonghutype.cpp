#include "majonghutype.h"
#include <algorithm>
#include <iostream>

namespace algorithm {


/**************88番*****************/
bool MajongHuType::DaSiXi(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it :vec_door_cards)
    {
        if (IsFengKe(it))
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
        if (IsJianKe(it))
            count++;
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
        if (it.card_type == DoorCardType::GANG ||
            it.card_type == DoorCardType::AN_GANG)
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
        if (IsAnKe(it))
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

    int count = 1;
    auto &first = vec_door_cards.front();
    for (auto it = vec_door_cards.begin() + 1; it != vec_door_cards.end(); ++it)
    {
        if (it->card_type == DoorCardType::SHUN_ZI && first.card_type == DoorCardType::SHUN_ZI)
        {
            if (lamb_compare(first.cards, it->cards))
                count++;
        }
    }

    return count >= 4;
}

// 一色四节高
bool MajongHuType::YiSeSiJieGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::KE_ZI && it.card_type != DoorCardType::PENG)
            continue;

        for (auto &iter : it.cards)
        {
            set.insert(iter->face());
        }
    }

    int count = ShunZiCount(set);
    return count == 4;
}

/**************48番*****************/
// 一色四步高
bool MajongHuType::YiSeSiBuGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::SHUN_ZI)
            continue;

        auto min = std::min_element(it.cards.begin(), it.cards.end(), [](const ICardPtr &c1, const ICardPtr &c2){
            return c1->face() < c2->face();
        });

        set.insert((*min)->face());
    }

    int count = 1;

    {
        auto prev = set.begin();
        for (auto it = ++set.begin(); it != set.end(); ++it, ++prev)
        {
            if (*prev + 1 == *it)
                count++;
        }
    }

    if (count == 4)
        return true;

    {
        count = 1;
        auto prev2 = set.begin();
        for (auto it = ++set.begin(); it != set.end(); ++it, ++prev2)
        {
            if (*prev2 + 2 == *it)
                count++;
        }
    }

    return count == 4;    
}

// 三杠
bool MajongHuType::SanGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::GANG ||
            it.card_type == DoorCardType::AN_GANG)
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
bool MajongHuType::QiDui(const std::vector<OutDoorCards> &vec_door_cards)
{
    std::vector<ICardPtr> cards;
    for (auto &it : vec_door_cards)
    {
        cards.insert(cards.end(), it.cards.begin(), it.cards.end());
    }

    algorithm_.InputHandCard(cards);
    return algorithm_.CheckQiDuiHu();
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

// 一色三同顺
bool MajongHuType::YiSeSanTongShun(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    int count = 1;
    auto &first = vec_door_cards.front();
    for (auto it = vec_door_cards.begin() + 1; it != vec_door_cards.end(); ++it)
    {
        if (IsTongShun(first, *it))
            count++;
    }

    return count == 3;
}

// 一色三节高
bool MajongHuType::YiSeSanJieGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::KE_ZI && it.card_type != DoorCardType::PENG)
            continue;

        for (auto &iter : it.cards)
        {
            set.insert(iter->face());
        }
    }

    int count = ShunZiCount(set);
    return count >= 3;
}

/**************16番*****************/
// 清龙
bool MajongHuType::QingLong(const std::vector<OutDoorCards> &vec_door_cards) const
{
    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::SHUN_ZI)
            continue;

        if (it.cards.front()->type() != MjCard::E_MYRIAD_TYPE)
            continue;

        for (auto &iter : it.cards)
        {
            set.insert(iter->face());
        }
    }

    return ShunZiCount(set) == 9;
}
// 一色三步高
bool MajongHuType::YiSeSanBuGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    if (!QingYiSe(vec_door_cards))
        return false;

    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::SHUN_ZI)
            continue;

        auto min = std::min_element(it.cards.begin(), it.cards.end(), [](const ICardPtr &c1, const ICardPtr &c2){
            return c1->face() < c2->face();
        });

        set.insert((*min)->face());
    }

    int count = 1;

    {
        auto prev = set.begin();
        for (auto it = ++set.begin(); it != set.end(); ++it, ++prev)
        {
            if (*prev + 1 == *it)
                count++;
        }
    }

    if (count == 3)
        return true;

    {
        count = 1;
        auto prev2 = set.begin();
        for (auto it = ++set.begin(); it != set.end(); ++it, ++prev2)
        {
            if (*prev2 + 2 == *it)
                count++;
        }
    }

    return count == 4;    
}

// 三暗刻
bool MajongHuType::SanAnKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsAnKe(it))
            count++;
    }
    return count == 3;
}

/**************12番*****************/
// 大于5
bool MajongHuType::DaYu5(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        if (it.cards.front()->type() != MjCard::E_MYRIAD_TYPE)
            continue;

        for (auto &iter : it.cards)
        {
            if (iter->face() <= MjCard::E_FACE_FIVE)
                return false;
        }
    }
    return true;
}

// 小于5
bool MajongHuType::XiaoYu5(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        if (it.cards.front()->type() != MjCard::E_MYRIAD_TYPE)
            continue;

        for (auto &iter : it.cards)
        {
            if (iter->face() >= MjCard::E_FACE_FIVE)
                return false;
        }
    }
    return true;
}

// 三风刻
bool MajongHuType::SanFengKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsFengKe(it))
            count++;
    }
    return count == 3;
}

/**************6番*****************/
 // 碰碰胡
bool MajongHuType::PengPengHu(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        switch (it.card_type)
        {
        case DoorCardType::JIANG:	//将牌
	    case DoorCardType::KE_ZI:   //刻子
	    case DoorCardType::PENG:    //碰
	    case DoorCardType::GANG:    //杠
	    case DoorCardType::AN_GANG: //暗杠
            break;
        default:
            return false;
        }
    }
    return true;
}

// 混一色
bool MajongHuType::HunYiSe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            switch (iter->type())
            {
            case MjCard::E_MYRIAD_TYPE:
            case MjCard::E_WIND_TYPE:
            case MjCard::E_WORD_TYPE:
                break;
            default:
                return false;
            }
        }
    }
    return true;
}

// 全球人
bool MajongHuType::QuanQiuRen(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        switch (it.card_type)
        {
        case DoorCardType::JIANG:	//将牌
	    case DoorCardType::PENG:    //碰
	    case DoorCardType::GANG:    //杠
	    //case DoorCardType::AN_GANG: //暗杠
            break;
        default:
            return false;
        }
    }
    return true;
}

// 双暗刻
bool MajongHuType::ShuangAnGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsAnKe(it))
            count++;
    }
    return count == 2;
}

// 双箭刻
bool MajongHuType::ShuangJianKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsJianKe(it))
            count++;
    }
    return count == 2;
}

/**************4番*****************/
// 全带幺
bool MajongHuType::QuanDaiYao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            if (iter->type() != MjCard::E_WIND_TYPE &&
                iter->type() != MjCard::E_WORD_TYPE)
            {
                if (iter->face() != MjCard::E_FACE_ONE)
                    return false;
            }
        }
    }
    return true;
}

// 不求人
bool MajongHuType::BuQiuRen(const std::vector<OutDoorCards> &vec_door_cards, bool zimo) const
{
    for (auto &it : vec_door_cards)
    {
        switch (it.card_type)
        {
        case DoorCardType::CHI:
        case DoorCardType::PENG:
        case DoorCardType::GANG:
            return false;
        default:
            break;
        }
    }
    return zimo;
}

// 双明杠
bool MajongHuType::ShuangMingGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::GANG)
            count++;
    }
    return (count == 2);
}

// 直立
bool MajongHuType::ZhiLi(const std::vector<OutDoorCards> &vec_door_cards, HuAction hu_action, bool baoting) const
{
    return MenQianQing(vec_door_cards, hu_action) && baoting;
}

/**************2番*****************/
// 箭刻
bool MajongHuType::JianKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsJianKe(it))
            count++;
    }
    return count == 1;
}

// 门前清
bool MajongHuType::MenQianQing(const std::vector<OutDoorCards> &vec_door_cards, HuAction hu_action) const
{
    return BuQiuRen(vec_door_cards, true) && hu_action == HuAction::HaiDiLaoYue;
}

// 四归一
bool MajongHuType::SiGuiYi(const std::vector<OutDoorCards> &vec_door_cards) const
{
    static const int32_t ROW = MjCard::E_TYPE_COUNT_;
	static const int32_t CLOU = MjCard::E_FACE_COUNT_;
    int map_stiatics[ROW][CLOU] = {{0}};

    for (auto &it : vec_door_cards)
    {
        if (IsNotHandDoorCards(it))
            continue;

        for (auto &iter : it.cards)
        {
            map_stiatics[iter->type()][iter->face()] += 1;
            if (map_stiatics[iter->type()][iter->face()] >= 4)
                return true;
        }
    }
    return false;
}

// 双暗刻
bool MajongHuType::ShuangAnKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsAnKe(it))
            count++;
    }
    return count == 2;
}

// 暗杠
bool MajongHuType::YiAnGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::AN_GANG)
            count++;
    }
    return count == 1;
}

// 断幺
bool MajongHuType::DuanYao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            if (IsMyriao(iter) && (iter->face() == 1 || iter->face() == 9))
                return false;
            
            if (IsWind(iter) || IsWord(iter))
                return false;
        }
    }
    return true;
}

/**************1番*****************/
// 二五八将
bool MajongHuType::ErWuBaJiang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::JIANG)
            continue;

        auto &c = it.cards.front();

        if (IsMyriao(c))
        {
            if (c->face() != 2 && c->face() != 5 && c->face() != 8)
                return false;
        }
    }
    return true;
}

// 幺九头
bool MajongHuType::YaoJiuTou(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::JIANG)
            continue;

        auto &c = it.cards.front();

        if (IsMyriao(c))
        {
            if (c->face() != 1 && c->face() != 9)
                return false;
        }
    }
    return true;
}

// 一般高
bool MajongHuType::YiBanGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 1;
    auto &first = vec_door_cards.front();
    for (auto it = vec_door_cards.begin() + 1; it != vec_door_cards.end(); ++it)
    {
        if (IsTongShun(first, *it))
            count++;
    }

    return count == 2;
}

// 连六
bool MajongHuType::LianLiu(const std::vector<OutDoorCards> &hand_door_cards) const
{
    std::set<int> set;
    for (auto &it : hand_door_cards)
    {
        for (auto &iter : it.cards)
        {
            if (!IsMyriao(iter))
                continue;

            set.insert(iter->face());
        }
    }
    return ShunZiCount(set) == 6;
}

// 老少副
bool MajongHuType::LaoShaoFu(const std::vector<OutDoorCards> &vec_door_cards) const
{
    auto lamb123 = [](const std::vector<ICardPtr> &vec)->bool {
        return (vec[0]->face() == 1 && vec[1]->face() == 2 && vec[2]->face() == 3);
    };

    auto lamb789 = [](const std::vector<ICardPtr> &vec) -> bool {
        return (vec[0]->face() == 7 && vec[1]->face() == 8 && vec[2]->face() == 9);
    };

    int count_123 = 0;
    int count_789 = 0;
    for (auto &it : vec_door_cards)
    {
        auto vec = it.cards;
        std::sort(vec.begin(), vec.end(), [](const ICardPtr &c1, ICardPtr &c2){
            return c1->face() < c2->face();
        });

        if (it.card_type == DoorCardType::SHUN_ZI && lamb123(vec))
            count_123++;

        if (it.card_type == DoorCardType::SHUN_ZI && lamb789(vec))
            count_789++;
    }

    return (count_123 >= 1 && count_789 >= 1);    
}

// 幺九刻
bool MajongHuType::YaoJiuKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::KE_ZI)
        {
            auto &c = it.cards.front();
            if (IsMyriao(c) && (c->face() == 1 || c->face() == 9))
                return true;

            if (IsWind(c) || IsWord(c))
                return true;
        }
    }
    return false;
}

// 一明杠
bool MajongHuType::YiMingGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::GANG)
            count++;
    }
    return count == 1;
}

// 边张
bool MajongHuType::BianZhang(const std::vector<OutDoorCards> &vec_door_cards, const ICardPtr &c) const
{
    // auto lamb_is_same = [](const ICardPtr &c1, const ICardPtr &c2)->bool {
    //     return c1->type() == c2->type() && c1->face() == c2->face();
    // };

    for (auto &it : vec_door_cards)
    {
        if (IsNotHandDoorCards(it))
            continue;

        if (it.card_type == DoorCardType::SHUN_ZI)
        {
            if (IsShunZi123(it) || IsShunZi789(it))
            {
                if (c == it.cards[0])
                    return true;

                if (c == it.cards[2])
                    return true;
            }
        }
    }
    return false;
}

// 坎张
bool MajongHuType::KanZhang(const std::vector<OutDoorCards> &vec_door_cards, const ICardPtr &c) const
{
    for (auto &it : vec_door_cards)
    {
        if (IsNotHandDoorCards(it))
            continue;

        if (it.card_type == DoorCardType::SHUN_ZI)
        {
            if (c == it.cards[1])
                return true;
        }
    }
    return false;
}

// 单吊将
bool MajongHuType::DanDiaoJing(const std::vector<OutDoorCards> &vec_door_cards, const ICardPtr &c) const
{
    for (auto &it : vec_door_cards)
    {
        if (IsNotHandDoorCards(it))
            continue;

        if (it.card_type == DoorCardType::JIANG)
        {
            for (auto &iter : it.cards)
            {
                if (iter == c)
                    return true;
            }
        }
    }
    return false;
}

int MajongHuType::ShunZiCount(const std::set<int> &set) const
{
    int count = 1;
    auto prev = set.begin();
    for (auto it = ++set.begin(); it != set.end(); ++it, ++prev)
    {
        if ((*prev + 1) == *it)
            count++;
        else
            count = 1;
    }
    return count;
}

bool MajongHuType::IsAnKe(const OutDoorCards &door_cards) const
{
    switch (door_cards.card_type)
    {
        case DoorCardType::KE_ZI:
        case DoorCardType::AN_GANG:
            break;
        default:
            return false;
    }
    return true;
}

bool MajongHuType::IsJianKe(const OutDoorCards &door_cards) const
{
    auto &c = door_cards.cards.front();
    if (!IsWord(c))
        return false;

    switch (door_cards.card_type)
    {
    case DoorCardType::KE_ZI:
    case DoorCardType::PENG:
    case DoorCardType::GANG:
    case DoorCardType::AN_GANG:
        break;
    default:
        return false;
    }

    return true;
}

bool MajongHuType::IsFengKe(const OutDoorCards &door_cards) const
{
    auto &c = door_cards.cards.front();
    if (!IsWind(c)) 
        return false;

    switch (door_cards.card_type)
    {
    case DoorCardType::KE_ZI:
    case DoorCardType::PENG:
    case DoorCardType::GANG:
    case DoorCardType::AN_GANG:
        break;
    default:
        return false;
    }

    return true;    
}

bool MajongHuType::IsTongShun(const OutDoorCards &door_cards1, const OutDoorCards &door_cards2) const
{
    if (door_cards1.card_type != DoorCardType::SHUN_ZI ||
        door_cards2.card_type != DoorCardType::SHUN_ZI)
        return false;

    auto &vec1 = door_cards1.cards;
    auto &vec2 = door_cards2.cards;

    if (vec1.front()->type() != vec2.front()->type())
        return false;

    return (vec1[0]->face() == vec2[0]->face() &&
            vec1[1]->face() == vec2[1]->face() &&
            vec1[2]->face() == vec2[2]->face());
}

bool MajongHuType::IsShunZi123(const OutDoorCards &door_cards) const
{
    auto &vec = door_cards.cards;
    return (vec[0]->face() == 1 && vec[1]->face() == 2 && vec[2]->face() == 3);
}

bool MajongHuType::IsShunZi789(const OutDoorCards &door_cards) const
{
    auto &vec = door_cards.cards;
    return (vec[0]->face() == 7 && vec[1]->face() == 8 && vec[2]->face() == 9);
}

bool MajongHuType::IsNotHandDoorCards(const OutDoorCards &door_cards) const
{
    switch (door_cards.card_type)
    {
    case DoorCardType::CHI:
    case DoorCardType::PENG:
    case DoorCardType::GANG:
    case DoorCardType::AN_GANG:
        break;
    default:
        return false;
    }
    return true;
}

} //namespace algorithm