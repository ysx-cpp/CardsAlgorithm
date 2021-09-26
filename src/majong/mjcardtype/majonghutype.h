#ifndef MAJONG_MJ_CARD_TYPE_H
#define MAJONG_MJ_CARD_TYPE_H

#include <set>
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
    bool JiuBaoLianDeng(const std::vector<OutDoorCards> &vec_door_cards) const;
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
    bool QiDui(const std::vector<OutDoorCards> &vec_door_cards);
    // 清一色
    bool QingYiSe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色三同顺
    bool YiSeSanTongShun(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色三节高
    bool YiSeSanJieGao(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************12番*****************/
    // 清龙
    bool QingLong(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色三步高
    bool YiSeSanBuGao(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 三暗刻
    bool SanAnKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************12番*****************/
    // 大于5
    bool DaYu5(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 小于5
    bool XiaoYu5(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 三风刻
    bool SanFengKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************8番*****************/
    
    /**************6番*****************/
    bool PengPengHu(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 碰碰胡
    bool HunYiSe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 双暗刻
    bool ShuangAnGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 双箭刻
    bool ShuangJianKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************4番*****************/
    // 全带幺
    bool QuanDaiYao(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 不求人
    bool BuQiuRen(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 双明杠
    bool ShuangMingGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 和绝张
    bool HeJueZhang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 直立
    bool ZhiLi(const std::vector<OutDoorCards> &vec_door_cards) const;
    /**************2番*****************/
    // 箭刻
    bool JianKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 门前清
    bool MenQianQing(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 四归一
    bool SiGuiYi(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 双暗刻
    bool ShuangAnKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 暗杠
    bool YiAnGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 断幺
    bool DuanYao(const std::vector<OutDoorCards> &vec_door_cards) const;

    /**************1番*****************/
    // 二五八将
    bool ErWuBaJiang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 幺九头
    bool YaoJiuTou(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一般高
    bool YiBanGao(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 连六
    bool LianLiu(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 老少副
    bool LaoShaoFu(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 幺九刻
    bool YaoJiuKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一明杠
    bool YiMingGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 边张
    bool BianZhang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 坎张
    bool KanZhang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 点吊将
    bool DanDiaoJing(const std::vector<OutDoorCards> &vec_door_cards) const;

private:
    int ShunZiCount(const std::set<int> &set) const;

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