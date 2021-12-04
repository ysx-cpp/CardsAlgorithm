#include "majonghutype.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <bitset>
#include "majongop.h"

namespace algorithm {

// using XGameMJProto::DoorCardType;

bool MajongHuType::CheckHu(const std::vector<ICardPtr> &hand_cards, std::vector<OutDoorCards> &vec_door_cards)
{
    algorithm_.InputHandCard(hand_cards);

    if (!algorithm_.CheckPingHu() && !algorithm_.CheckQiDuiHu())
        return false;

    algorithm_.OutPutDoorCards(vec_door_cards);

    return true;
}

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
    std::map<int, int> mp;
    for (auto &it : vec_door_cards)
    {
        for (auto &iter : it.cards)
        {
            mp[iter->face()]++;
        }
    }

    if (mp.size() != 9)
        return false;

    auto prev = mp.begin();
    auto back = mp.rbegin();
    if (prev->first != MjCard::E_FACE_ONE || prev->second < 3)
        return false;

    if (back->first != MjCard::E_FACE_NINE || back->second < 3)
        return false;

    int count = 1;
    for (auto it = ++mp.begin(); it != mp.end(); ++it)
    {
        if (prev->first + 1 == it->first)
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
        if (IsGang(it))
            count++;
    }
    return count == 4;
}

// 七连对
bool MajongHuType::QiLianDui(const std::vector<OutDoorCards> &vec_door_cards, std::vector<OutDoorCards> &out) const
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

    if (vec.size() < 14)
        return false;

    std::sort(vec.begin(), vec.end());

    auto prev = vec.begin();
    for (auto it = vec.begin() + 2; it != vec.end(); it += 2)
    {
        if ((*prev + 1) == *it)
            count++;
        
        prev = it;
    }

    if (count != 7)
        return false;

    return QiDui(vec_door_cards, out);
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
    int feng_ke_count = 0;
    int feng_jiang_count = 0;
    for (auto &it :vec_door_cards)
    {
        auto &c = it.cards.front();
        if (IsFengKe(it))
            feng_ke_count++;
        else if (it.card_type == DoorCardType::JIANG && IsWind(c))
            feng_jiang_count++; 
    }
    return (feng_ke_count == 3 && feng_jiang_count == 1);
}

// 小三元
bool MajongHuType::XiaoSanYuan(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    int jiang = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsJianKe(it))
            count++;

        if (it.card_type == DoorCardType::JIANG)
        {
            auto &c = it.cards.front();
            if (IsWord(c))
                jiang++;
        }
    }
    return count == 2 && jiang;
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
bool MajongHuType::SiAnKe(const std::vector<OutDoorCards> &vec_door_cards, const MjCalcContext &ctx) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsAnKe(it, ctx))
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
        //std::sort(vec.begin(), vec.end(), [](const ICardPtr &c1, ICardPtr &c2){
        //    return c1->face() < c2->face();
        //});

        if (it.card_type != DoorCardType::SHUN_ZI && it.card_type != DoorCardType::CHI)
            continue;

        if (lamb123(vec))
            count_123++;

        if (lamb789(vec))
            count_789++;
    }

    return (count_123 == 2 && count_789 == 2 && jiang);
}

/**************48番*****************/
// 一色四筒顺
bool MajongHuType::YiSeSiTongShun(const std::vector<OutDoorCards> &hand_door_cards,
        const std::vector<OutDoorCards> &door_cards, 
        std::vector<OutDoorCards> &out) const
{
    auto lamb_sitongshun = [this](const std::vector<OutDoorCards> &vec)
    {
        int count = 0;
        for (auto &iter : vec)
        {
            count = 0;
            if (iter.card_type != DoorCardType::SHUN_ZI &&
                iter.card_type != DoorCardType::CHI)
                continue;

            for (auto &it : vec)
            {
                if (it.card_type != DoorCardType::SHUN_ZI &&
                    it.card_type != DoorCardType::CHI)
                    continue;

                if (IsTongShun(iter, it))
                    count++;

                if (count == 4)
                    return true;
            }
        }
        return false;
    };

    return IsComposeTypeHu(hand_door_cards, door_cards, out, lamb_sitongshun);
}

// 一色四节高
bool MajongHuType::YiSeSiJieGao(const std::vector<OutDoorCards> &hand_door_cards,
        const std::vector<OutDoorCards> &door_cards, 
        std::vector<OutDoorCards> &out) const
{
    auto lamb_sijiegao = [this](const std::vector<OutDoorCards> &vec)
    {
        std::set<int> set;
        std::map<int, int> mp;
        for (auto &it : vec)
        {
            if (it.card_type == DoorCardType::JIANG ||
                it.card_type == DoorCardType::CHI)
                continue;

            for (auto &iter : it.cards)
            {
                if (!IsMyriao(iter))
                    continue;

                mp[iter->face()]++;
                if (mp[iter->face()] >= 3)
                    set.insert(iter->face());
            }
        }
        int count = ShunZiCount(set);
        return count == 4;
    };

    return IsComposeTypeHu(hand_door_cards, door_cards, out, lamb_sijiegao);
}

/**************48番*****************/
// 一色四步高
bool MajongHuType::YiSeSiBuGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::SHUN_ZI &&
            it.card_type != DoorCardType::CHI)
            continue;

        auto min = std::min_element(it.cards.begin(), it.cards.end(), [](const ICardPtr &c1, const ICardPtr &c2){
            return c1->face() < c2->face();
        });

        if (!IsMyriao(*min))
            continue;

        set.insert((*min)->face());
    }

    if (set.empty())
        return false;

    int count = ShunZiCount(set);
    if (count == 4)
        return true;

    count = 1;
    auto prev2 = set.begin();
    for (auto it = ++set.begin(); it != set.end(); ++it, ++prev2)
    {
        if (*prev2 + 2 == *it)
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
        if (IsGang(it))
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
bool MajongHuType::QiDui(const std::vector<OutDoorCards> &vec_door_cards, std::vector<OutDoorCards> &out) const
{
    int count = 0;
    std::vector<ICardPtr> cards;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::JIANG)
            count++;

        cards.insert(cards.end(), it.cards.begin(), it.cards.end());
    }

    if (cards.size() != 14)
        return false;

    out.clear();
    MjAlgorithm algo;
    algo.InputHandCard(cards);
    bool ret = algo.CheckQiDuiHu();
    algo.OutPutDoorCards(out);
    return ret;
}

// 清一色
bool MajongHuType::QingYiSe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    auto &door = vec_door_cards.front();
    auto &c = door.cards.front();
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
    int count = 0;
    for (auto &iter : vec_door_cards)
    {
        count = 0;
        if (iter.card_type != DoorCardType::SHUN_ZI &&
            iter.card_type != DoorCardType::CHI)
            continue;

        for (auto &it : vec_door_cards)
        {
            if (!IsMyriao(it.cards.front()))
                continue;

            if (it.card_type != DoorCardType::SHUN_ZI &&
                it.card_type != DoorCardType::CHI)
                continue;

            if (IsTongShun(iter, it))
                count++;

            if (count == 3)
                return true;
        }
    }
    return false;
}

// 一色三节高和三暗刻
bool MajongHuType::YiSeSanJieGaoAndSanAnKe(const std::vector<OutDoorCards> &hand_door_cards,
        const std::vector<OutDoorCards> &door_cards, 
        const MjCalcContext &ctx,
        std::vector<OutDoorCards> &out) const
{
    return IsComposeTypeHu(hand_door_cards, door_cards, out, [this, &ctx](const std::vector<OutDoorCards> &vec){
        return YiSeSanJieGao(vec) && (SanAnKe(vec, ctx) || SiAnKe(vec, ctx));
    });
}

// 一色三节高
bool MajongHuType::YiSeSanJieGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    std::set<int> set;
    std::map<int, int> mp;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::JIANG ||
            it.card_type == DoorCardType::CHI ||
            it.card_type == DoorCardType::SHUN_ZI)
            continue;

        for (auto &iter : it.cards)
        {
            if (!IsMyriao(iter))
                continue;

            mp[iter->face()]++;
            if (mp[iter->face()] >= 3)
                set.insert(iter->face());
        }
    }

    int count = ShunZiCount(set);
    return count == 3;
}

/**************16番*****************/
// 清龙
bool MajongHuType::QingLong(const std::vector<OutDoorCards> &vec_door_cards) const
{
    std::bitset<32> bs;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::CHI &&
            it.card_type != DoorCardType::SHUN_ZI)
            continue;

        auto min = std::min_element(it.cards.begin(), it.cards.end(), [](const ICardPtr &c1, const ICardPtr &c2)
                                    { return c1->face() < c2->face(); });

        if (!IsMyriao(*min))
            continue;

        auto face = (*min)->face();
        if (bs.test(face))
            continue;

        bs.set(face);
    }
    return bs.test(1) && bs.test(4) && bs.test(7);
}
// 一色三步高
bool MajongHuType::YiSeSanBuGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    std::set<int> set;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::SHUN_ZI &&
            it.card_type != DoorCardType::CHI)
            continue;

        auto min = std::min_element(it.cards.begin(), it.cards.end(), [](const ICardPtr &c1, const ICardPtr &c2){
            return c1->face() < c2->face();
        });

        if (!IsMyriao(*min))
            continue;

        set.insert((*min)->face());
    }

    if (set.empty())
        return false;

    int count = ShunZiCount(set);
    if (count == 3)
        return true;

    count = 1;
    auto prev2 = set.begin();
    for (auto it = ++set.begin(); it != set.end(); ++it, ++prev2)
    {
        if (*prev2 + 2 == *it)
            count++;
    }

    return count == 3;    
}

// 三暗刻
bool MajongHuType::SanAnKe(const std::vector<OutDoorCards> &vec_door_cards, const MjCalcContext &ctx) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsAnKe(it, ctx))
            count++;
    }
    return (count == 3); //新规则

    // return AnKeCount(vec_door_cards, ctx) == 3;
}

/**************12番*****************/
// 大于5
bool MajongHuType::DaYu5(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        if (it.cards.front()->type() != MjCard::E_MYRIAD_TYPE)
            return false;

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
            return false;

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
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        switch (it.card_type)
        {
        case DoorCardType::JIANG:
            count++;
            break;
	    case DoorCardType::SHUN_ZI:
	    case DoorCardType::CHI:
            return false;
        default:
            break;
        }
    }
    return count == 1;
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
bool MajongHuType::QuanQiuRen(const std::vector<OutDoorCards> &vec_door_cards, bool zimo) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        switch (it.card_type)
        {
        case DoorCardType::SHUN_ZI:
        case DoorCardType::KE_ZI:
        case DoorCardType::AN_GANG:
            return false;
        case DoorCardType::JIANG:
            count++;
            break;
        default:
            break;
        }
    }
    if (count > 1)
        return false;
        
    return !zimo;
}

// 双暗杠
bool MajongHuType::ShuangAnGang(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::AN_GANG)
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
    auto lamb_daiyao = [](const std::vector<ICardPtr> &cards)
    {
        for (auto &iter : cards)
        {
            if (iter->face() == 1 || iter->face() == 9)
                return true;
        }
        return false;
    };

    for (auto &it : vec_door_cards)
    {
        auto &c = it.cards.front();
        if (!IsMyriao(c))
            continue;

        if (!lamb_daiyao(it.cards))
            return false;
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
        case DoorCardType::PENG_GANG:
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
        if (it.card_type == DoorCardType::GANG ||
            it.card_type == DoorCardType::PENG_GANG)
            count++;
    }
    return (count == 2);
}

// 直立
bool MajongHuType::ZhiLi(const std::vector<OutDoorCards> &vec_door_cards, bool zimo, bool baoting) const
{
    return MenQianQing(vec_door_cards, zimo) && baoting;
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
bool MajongHuType::MenQianQing(const std::vector<OutDoorCards> &vec_door_cards, bool zimo) const
{
    return BuQiuRen(vec_door_cards, true) && zimo == false;
}

// 平胡
bool MajongHuType::PingHu(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::SHUN_ZI &&
            it.card_type != DoorCardType::JIANG &&
            it.card_type != DoorCardType::CHI)
        {
            return false;
        }

        if (it.card_type == DoorCardType::JIANG)
        {
            count++;
            auto &c = it.cards.front();
            if (!IsMyriao(c))
                return false;
        }
    }
    return count == 1;
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
bool MajongHuType::ShuangAnKe(const std::vector<OutDoorCards> &vec_door_cards, const MjCalcContext &ctx) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsAnKe(it, ctx))
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
            if (c->face() == 2 || c->face() == 5 || c->face() == 8)
                return true;
        }
    }
    return false;
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
            if (c->face() == 1 || c->face() == 9)
                return true;
        }
    }
    return false;
}

// 一般高
bool MajongHuType::YiBanGao(const std::vector<OutDoorCards> &vec_door_cards) const
{
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::SHUN_ZI &&
            it.card_type != DoorCardType::CHI)
            continue;

        count = 0;
        for (auto &iter : vec_door_cards)
        {
            if (IsTongShun(it, iter))
            {
                if (++count == 2)
                    return true;
            }
        }
    }
    return false;
}

// 连六
bool MajongHuType::LianLiu(const std::vector<OutDoorCards> &vec_door_cards) const
{
    std::bitset<16> bs;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::CHI &&
            it.card_type != DoorCardType::SHUN_ZI)
            continue;

        auto min = std::min_element(it.cards.begin(), it.cards.end(), [](const ICardPtr &c1, const ICardPtr &c2)
                                    { return c1->face() < c2->face(); });

        if (!IsMyriao(*min))
            continue;

        bs.set((*min)->face());
    }

    uint16_t val = bs.to_ulong();
    while (val >= 0x09)
    {
        val >>= 1;
        if ((val & 0x09) == 0x09)
            return true;
    }
    return false;
}

// 老少副
bool MajongHuType::LaoShaoFu(const std::vector<OutDoorCards> &vec_door_cards) const
{
    std::bitset<32> bs;
    for (auto &it : vec_door_cards)
    {
        if (it.card_type != DoorCardType::CHI &&
            it.card_type != DoorCardType::SHUN_ZI)
            continue;

        auto min = std::min_element(it.cards.begin(), it.cards.end(), [](const ICardPtr &c1, const ICardPtr &c2)
                                    { return c1->face() < c2->face(); });

        if (!IsMyriao(*min))
            continue;

        auto face = (*min)->face();
        if (bs.test(face))
            continue;

        bs.set(face);
    }
    return bs.test(1) && bs.test(7);
}

// 幺九刻
bool MajongHuType::YaoJiuKe(const std::vector<OutDoorCards> &vec_door_cards) const
{
    for (auto &it : vec_door_cards)
    {
        if (it.card_type == DoorCardType::KE_ZI ||
            it.card_type == DoorCardType::PENG ||
            it.card_type == DoorCardType::GANG ||
            it.card_type == DoorCardType::PENG_GANG ||
            it.card_type == DoorCardType::AN_GANG)
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
        if (it.card_type == DoorCardType::GANG ||
            it.card_type == DoorCardType::PENG_GANG)
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
                if (c->type() == it.cards[0]->type() && c->face() == it.cards[0]->face())
                    return true;

                if (c->type() == it.cards[2]->type() && c->face() == it.cards[2]->face())
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
            if (c->type() == it.cards[1]->type() && c->face() == it.cards[1]->face())
                return true;
        }
    }
    return false;
}

// 单吊将
bool MajongHuType::DanDiaoJing(const std::vector<OutDoorCards> &vec_door_cards, const ICardPtr &c) const
{
    bool jiang = false;
    int count = 0;
    for (auto &it : vec_door_cards)
    {
        if (IsNotHandDoorCards(it))
            continue;

        auto &iter = it.cards.front();
        if (it.card_type == DoorCardType::JIANG && 
            (iter->type() == c->type() && iter->face() == c->face()))
        {
            jiang = true;
        }
        for (auto &iter : it.cards)
        {
            if (iter->type() == c->type() && iter->face() == c->face())
                count++;
        }
    }
    return count == 2 && jiang;
}

int MajongHuType::ShunZiCount(const std::set<int> &set) const
{
    if (set.empty()) 
        return 0;

    int max = 0;
    int count = 1;
    auto prev = set.begin();
    for (auto it = ++set.begin(); it != set.end(); ++it, ++prev)
    {
        if ((*prev + 1) == *it)
        {
            count++;
            max = std::max(max, count);
        }
        else
            count = 1;
    }
    return max;
}

bool MajongHuType::IsAnKe(const OutDoorCards &door_cards, const MjCalcContext &ctx) const
{
    switch (door_cards.card_type)
    {
        case DoorCardType::KE_ZI:
        {
            auto front = door_cards.cards.front();
            if (!ctx.is_zimo &&
                front->type() == ctx.hu_card->type() && 
                front->face() == ctx.hu_card->face())
                return false;
        }
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
    case DoorCardType::PENG_GANG:
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
    case DoorCardType::PENG_GANG:
    case DoorCardType::AN_GANG:
        break;
    default:
        return false;
    }

    return true;    
}
bool MajongHuType::IsGang(const OutDoorCards &door_cards) const
{
    switch (door_cards.card_type)
    {
    case DoorCardType::GANG:
    case DoorCardType::PENG_GANG:
    case DoorCardType::AN_GANG:
        break;
    default:
        return false;
    }
    return true;    
}
bool MajongHuType::IsTongShun(const OutDoorCards &door_cards1, const OutDoorCards &door_cards2) const
{
    if (door_cards1.card_type != DoorCardType::SHUN_ZI &&
        door_cards1.card_type != DoorCardType::CHI)
        return false;

    if (door_cards2.card_type != DoorCardType::SHUN_ZI &&
        door_cards2.card_type != DoorCardType::CHI)
        return false;

    std::bitset<32> bs1;
    for (auto &it : door_cards1.cards)
        bs1.set(it->face());

    std::bitset<32> bs2;
    for (auto &it : door_cards2.cards)
        bs2.set(it->face());

    return bs1.to_ulong() == bs2.to_ulong();
}

bool MajongHuType::IsShunZi123(const OutDoorCards &door_cards) const
{
    std::bitset<16> bs;
    for (auto &it : door_cards.cards)
        bs.set(it->face());

    return bs.to_ulong() == 0x0E;

    // auto &vec = door_cards.cards;
    // return (vec[0]->face() == 1 && vec[1]->face() == 2 && vec[2]->face() == 3);
}

bool MajongHuType::IsShunZi789(const OutDoorCards &door_cards) const
{
    std::bitset<16> bs;
    for (auto &it : door_cards.cards)
        bs.set(it->face());

    return bs.to_ulong() == 0x380;

    // auto &vec = door_cards.cards;
    // return (vec[0]->face() == 7 && vec[1]->face() == 8 && vec[2]->face() == 9);
}

bool MajongHuType::IsNotHandDoorCards(const OutDoorCards &door_cards) const
{
    switch (door_cards.card_type)
    {
    case DoorCardType::CHI:
    case DoorCardType::PENG:
    case DoorCardType::GANG:
    case DoorCardType::PENG_GANG:
    case DoorCardType::AN_GANG:
        break;
    default:
        return false;
    }
    return true;
}

bool MajongHuType::IsComposeTypeHu(const std::vector<OutDoorCards> &hand_door_cards,
                     const std::vector<OutDoorCards> &door_cards,
                     std::vector<OutDoorCards> &out,
                     std::function<bool (const std::vector<OutDoorCards>&)> func) const
{
    std::vector<ICardPtr> hdcards;
    std::set<int> set;
    for (auto &it : hand_door_cards)
    {
        for (auto &iter : it.cards)
        {
            hdcards.push_back(iter);
            set.insert(iter->face());
        }
    }

    MaJongOp mjop(hdcards);
    for (auto &it : set)
    {
        out.clear();
        if (mjop.AssignJiangComposeKeZiHu(out, it))
        {
            std::vector<OutDoorCards> vec = out;
            vec.insert(vec.end(), door_cards.begin(), door_cards.end());
            if (func(vec))
                return true;
        }
        
        out.clear();
        if (mjop.AssignJiangComposeShunZiHu(out, it))
        {
            std::vector<OutDoorCards> vec = out;
            vec.insert(vec.end(), door_cards.begin(), door_cards.end());
            if (func(vec))
                return true;
        }
    }
    return false;
}

} //namespace algorithm
