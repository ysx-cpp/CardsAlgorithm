#include "gtest/gtest.h"
#include "mahjong/mahjonghutype.h"

using namespace algorithm;

static MajongHuType hutype;
static MjAlgorithm algo;

using TestCards = std::vector<uint16_t>;
using VecDoorCards = std::vector<OutDoorCards>;

static ICardPtr CardConversion(uint16_t c)
{
    MjCard::EType type = MjCard::E_TYPE_COUNT_;
    MjCard::EFace face = MjCard::E_FACE_COUNT_;
    if (c & 0x000f)
    {
        type = MjCard::E_MYRIAD_TYPE;
        uint16_t v = c & 0x000f;
        face = static_cast<MjCard::EFace>(v);
    }
    else if (c & 0x00f0)
    {
        type = MjCard::E_WIND_TYPE;
        uint16_t v = c & 0x00f0;
        face = static_cast<MjCard::EFace>(v >> 4);
    }
    else if (c & 0x0f00)
    {
        type = MjCard::E_WORD_TYPE;
        uint16_t v = c & 0x0f00;
        face = static_cast<MjCard::EFace>(v >> 8);
    }
    else if (c & 0xf000)
    {
        type = MjCard::E_FLOWER_TYPE;
        uint16_t v = c & 0xf000;
        face = static_cast<MjCard::EFace>(v >> 12);
    }
    else
        assert(false);

    return std::make_shared<MjCard>(type, face, 0);
}

static OutDoorCards GenDoorCards(DoorCardType door_type, const TestCards &cards)
{
    OutDoorCards door;
    door.card_type = door_type;

	for (auto &it : cards)
	{
        door.cards.emplace_back(CardConversion(it));
	}

    return door;
}

TEST(MajongHuType, DaSiXi) 
{
    TestCards hand_cards {
        0x0010, 0x0010, 0x0010, 0x0020, 0x0020, 0x0020,
        0x0200, 0x0200
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);    

    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0030, 0x0030, 0x0030, 0x0030}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0040, 0x0040, 0x0040, 0x0040}));
    
    EXPECT_TRUE(hutype.DaSiXi(door_cards));
}

TEST(MajongHuType, DaSanYuan) 
{
    TestCards hand_cards {
        0x0100, 0x0100, 0x0100, 0x0200, 0x0200, 0x0200, 
        0x0002, 0x0003, 0x0004, 0x0010, 0x0010
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0300, 0x0300, 0x0300, 0x0300}));    

    EXPECT_TRUE(hutype.DaSanYuan(door_cards));
}

TEST(MajongHuType, JiuBaoLianDeng) 
{
    TestCards hand_cards {
        0x0001, 0x0001, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.JiuBaoLianDeng(door_cards));
}

TEST(MajongHuType, SiGang) 
{
    TestCards hand_cards {
        0x0001, 0x0001, 0x0001, 0x0002, 0x0003
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0300, 0x0300, 0x0300, 0x0300}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0009, 0x0009, 0x0009, 0x0009}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0003, 0x0003, 0x0003, 0x0003}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0200, 0x0200, 0x0200, 0x0200}));

    EXPECT_TRUE(hutype.SiGang(door_cards));
}

TEST(MajongHuType, QiLianDui) 
{
    TestCards hand_cards {
        0x0001, 0x0001, 0x0002, 0x0002, 0x0003, 0x0003, 0x0004, 0x0004, 0x0005, 0x0005, 0x0006, 0x0006, 0x0007, 0x0007
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    VecDoorCards vec;
    EXPECT_TRUE(hutype.QiLianDui(door_cards, vec));
}

TEST(MajongHuType, BaiWanShi) 
{
    TestCards hand_cards {
        0x0005, 0x0006, 0x0006, 0x0007, 0x0007, 0x0007, 0x0007, 0x0008, 0x0008, 0x0008, 0x0008, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.BaiWanShi(door_cards));
}

TEST(MajongHuType, XiaoSixi) 
{
    TestCards hand_cards {
        0x0010, 0x0010, 0x0010, 0x0030, 0x0030, 0x0030, 0x0040, 0x0040, 0x0040,
        0x0020, 0x0020, 
        0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.XiaoSixi(door_cards));
}

TEST(MajongHuType, ZiYiSe) 
{
    TestCards hand_cards {
        0x0010, 0x0010, 0x0010, 0x0030, 0x0030, 0x0030, 0x0040, 0x0040, 0x0040, 0x0020, 0x0020, 0x0020, 
        0x0100, 0x0100, 0x0100, 0x200, 0x200
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.ZiYiSe(door_cards));
}

TEST(MajongHuType, SiAnKe) 
{
    TestCards hand_cards {
        0x0001, 0x0001, 0x0001, 0x0002, 0x0003
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0300, 0x0300, 0x0300, 0x0300}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0009, 0x0009, 0x0009, 0x0009}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0003, 0x0003, 0x0003, 0x0003}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0200, 0x0200, 0x0200, 0x0200}));

    MjCalcContext ctx;
    ctx.hu_card = CardConversion(hand_cards.back());
    EXPECT_TRUE(hutype.SiAnKe(door_cards, ctx));
}

TEST(MajongHuType, YiSeShuangLongHui) 
{
    TestCards hand_cards {
        0x0001, 0x0002, 0x0003, 0x0001, 0x0002, 0x0003, 0x0007, 0x0007, 0x0008, 0x0008, 0x0009, 0x0009, 0x0005, 0x0005
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.YiSeShuangLongHui(door_cards));
}

TEST(MajongHuType, YiSeSiTongShun) 
{
    TestCards hand_cards {
        0x0001, 0x0001, 0x0007, 0x0007, 0x0007, 0x0007, 0x0008, 0x0008, 0x0008, 0x0008, 0x0009, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    VecDoorCards vec;
    EXPECT_TRUE(hutype.YiSeSiTongShun(door_cards, {}, vec));
}

TEST(MajongHuType, YiSeSiJieGao) 
{
    TestCards hand_cards {
        0x0001, 0x0001,  0x0008, 0x0008, 0x0008, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards hd_door_cards;
    algo.OutPutDoorCards(hd_door_cards);

    VecDoorCards door_cards;
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, { 0x0006, 0x0006, 0x0006}));
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, { 0x0007, 0x0007, 0x0007}));

    VecDoorCards vec;
    EXPECT_TRUE(hutype.YiSeSiJieGao(hd_door_cards, door_cards, vec));
}

TEST(MajongHuType, YiSeSiBuGao) 
{
    TestCards hand_cards {
        0x0001, 0x0002, 0x0003,
        0x0002, 0x0003, 0x0004,
        0x0003, 0x0004, 0x0005,
        0x0004, 0x0005, 0x0006, 
        0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.YiSeSiBuGao(door_cards));


    TestCards hand_cards2 {
        0x0001, 0x0002, 0x0003,
        0x0003, 0x0004, 0x0005,
        0x0005, 0x0006, 0x0007,
        0x0007, 0x0008, 0x0009, 
        0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards2);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards2;
    algo.OutPutDoorCards(door_cards2);

    EXPECT_TRUE(hutype.YiSeSiBuGao(door_cards2));
}

TEST(MajongHuType, SanGang) 
{
    TestCards hand_cards {
        0x0001, 0x0001, 0x0001, 0x0002, 0x0003
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0300, 0x0300, 0x0300, 0x0300}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0009, 0x0009, 0x0009, 0x0009}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0003, 0x0003, 0x0003, 0x0003}));

    EXPECT_TRUE(hutype.SanGang(door_cards));
}

TEST(MajongHuType, HunYao9) 
{
    TestCards hand_cards {
        0x0001, 0x0001, 0x0001, 0x0100, 0x0100, 0x0100, 0x0020, 0x0020, 0x0020, 0x0009, 0x0009, 0x0009, 0x0300, 0x0300
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.HunYao9(door_cards));
}

TEST(MajongHuType, YiSeSanTongShun) 
{
    TestCards hand_cards {
        0x0001, 0x0001,  0x0003, 0x0003, 0x0003, 0x0007, 0x0007, 0x0007, 0x0008, 0x0008, 0x0008, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.YiSeSanTongShun(door_cards));
}

TEST(MajongHuType, YiSeSanJieGao) 
{
    TestCards hand_cards {
        0x0001, 0x0001,  0x0003, 0x0003, 0x0003, 0x0008, 0x0008, 0x0008, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, { 0x0007, 0x0007, 0x0007}));

    EXPECT_TRUE(hutype.YiSeSanJieGao(door_cards));
}

TEST(MajongHuType, QingLong) 
{
    TestCards hand_cards {
        0x0001, 0x0002,  0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, {0x0020, 0x0020, 0x0020}));

    EXPECT_TRUE(hutype.QingLong(door_cards));
}

TEST(MajongHuType, YiSeSanBuGao) 
{
    TestCards hand_cards {
        0x0001, 0x0002,  0x0003, 0x0003, 0x0004, 0x0005, 0x0005, 0x0006, 0x0007, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, { 0x0020, 0x0020, 0x0020}));

    EXPECT_TRUE(hutype.YiSeSanBuGao(door_cards));
}

TEST(MajongHuType, SanAnKe) 
{
    TestCards hand_cards {
        0x0100, 0x0100,  0x0002, 0x0003, 0x0004, 0x0006, 0x0006, 0x0006, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0020, 0x0020, 0x0020, 0x0020}));

    MjCalcContext ctx;
    ctx.is_zimo = true;
    ctx.hu_card = CardConversion(hand_cards.back());
    EXPECT_TRUE(hutype.SanAnKe(door_cards, ctx));
}

TEST(MajongHuType, DaYu5) 
{
    TestCards hand_cards {
        0x0007, 0x0007, 0x0007, 0x0007, 0x0008, 0x0008, 0x0008, 0x0008, 0x0009, 0x0009, 0x0009, 0x0009, 0x0006, 0x0006,
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.DaYu5(door_cards));
}

TEST(MajongHuType, XiaoYu5) 
{
    TestCards hand_cards {
        0x0002, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003, 0x0003, 0x0003, 0x0004, 0x0004, 0x0004, 0x0004, 0x0001, 0x0001,
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.XiaoYu5(door_cards));
}

TEST(MajongHuType, SanFengKe) 
{
    TestCards hand_cards {
        0x0002, 0x0003, 0x0004, 0x0030, 0x0030, 0x0030, 0x0040, 0x0040, 0x0040, 0x0001, 0x0001,
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0020, 0x0020, 0x0020, 0x0020}));

    EXPECT_TRUE(hutype.SanFengKe(door_cards));
}

TEST(MajongHuType, PengPengHu) 
{
    TestCards hand_cards {
        0x0030, 0x0030, 0x0030, 0x0040, 0x0040, 0x0040, 0x0001, 0x0001,
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0020, 0x0020, 0x0020, 0x0020}));
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, {0x0002, 0x0002, 0x0002}));

    EXPECT_TRUE(hutype.PengPengHu(door_cards));
}

TEST(MajongHuType, HunYiSe) 
{
    TestCards hand_cards {
        0x0030, 0x0030, 0x0030, 0x0040, 0x0040, 0x0040, 0x0300, 0x0300,
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0020, 0x0020, 0x0020, 0x0020}));
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, {0x0002, 0x0002, 0x0002}));

    EXPECT_TRUE(hutype.HunYiSe(door_cards));
}

TEST(MajongHuType, QuanQiuRen) 
{
    TestCards hand_cards {
        0x0300, 0x0300
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.QuanQiuRen(door_cards, false));
}

TEST(MajongHuType, ShuangAnGang) 
{
    TestCards hand_cards {
        0x0030, 0x0030, 0x0030, 0x0040, 0x0040, 0x0040, 0x0300, 0x0300,
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0020, 0x0020, 0x0020, 0x0020}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0002, 0x0002, 0x0002, 0x0002}));

    EXPECT_TRUE(hutype.ShuangAnGang(door_cards));
}

TEST(MajongHuType, ShuangJianKe) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0300, 0x0300
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0020, 0x0020, 0x0020, 0x0020}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0002, 0x0002, 0x0002, 0x0002}));
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, {0x0200, 0x0200, 0x0200}));

    EXPECT_TRUE(hutype.ShuangJianKe(door_cards));
}

TEST(MajongHuType, QuanDaiYao) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0300, 0x0300, 0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, {0x0200, 0x0200, 0x0200}));

    EXPECT_TRUE(hutype.QuanDaiYao(door_cards));
}

TEST(MajongHuType, BuQiuRen) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0300, 0x0300, 0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009, 0x0200, 0x0200, 0x0200
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.BuQiuRen(door_cards, true));
}

TEST(MajongHuType, ShuangMingGang) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0010, 0x0010, 0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0200, 0x0200, 0x0200, 0x0200}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0300, 0x0300, 0x0300, 0x0300}));

    EXPECT_TRUE(hutype.ShuangMingGang(door_cards));
}

TEST(MajongHuType, ZhiLi) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0300, 0x0300, 0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009, 0x0200, 0x0200, 0x0200
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.ZhiLi(door_cards, false, true));
}

TEST(MajongHuType, JianKe) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0300, 0x0300
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0020, 0x0020, 0x0020, 0x0020}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0002, 0x0002, 0x0002, 0x0002}));
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, {0x0010, 0x0010, 0x0010}));

    EXPECT_TRUE(hutype.JianKe(door_cards));
}

TEST(MajongHuType, MenQianQing) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0300, 0x0300, 0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009, 0x0200, 0x0200, 0x0200
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.MenQianQing(door_cards, false));
}

TEST(MajongHuType, PingHu) 
{
    TestCards hand_cards {
         0x0001, 0x0002, 0x0003, 0x0002, 0x0003, 0x0004, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.PingHu(door_cards));
}

TEST(MajongHuType, SiGuiYi) 
{
    TestCards hand_cards {
        0x0001, 0x0002, 0x0003, 0x0002, 0x0003, 0x0004, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0004, 0x0004
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.SiGuiYi(door_cards));
}

TEST(MajongHuType, ShuangAnKe) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0300, 0x0300, 0x0010, 0x0010, 0x0010
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0020, 0x0020, 0x0020, 0x0020}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0002, 0x0002, 0x0002, 0x0002}));

    MjCalcContext ctx;
    ctx.is_zimo = true;
    ctx.hu_card = CardConversion(hand_cards.front());
    EXPECT_TRUE(hutype.ShuangAnKe(door_cards, ctx));
}

TEST(MajongHuType, YiAnGang) 
{
    TestCards hand_cards {
         0x0100, 0x0100, 0x0100, 0x0300, 0x0300, 0x0010, 0x0010, 0x0010
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0020, 0x0020, 0x0020, 0x0020}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0002, 0x0002, 0x0002, 0x0002}));

    EXPECT_TRUE(hutype.YiAnGang(door_cards));
}

TEST(MajongHuType, DuanYao) 
{
    TestCards hand_cards {
         0x0002, 0x0002, 0x0002, 0x0006, 0x0007, 0x0008, 0x0008, 0x0008
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0005, 0x0005, 0x0005, 0x0005}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0004, 0x0004, 0x0004, 0x0004}));

    EXPECT_TRUE(hutype.DuanYao(door_cards));
}

TEST(MajongHuType, ErWuBaJiang) 
{
    TestCards hand_cards {
         0x0003, 0x0003, 0x0003, 0x0006, 0x0007, 0x0008, 0x0005, 0x0005
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0001, 0x0001, 0x0001, 0x0001}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0004, 0x0004, 0x0004, 0x0004}));

    EXPECT_TRUE(hutype.ErWuBaJiang(door_cards));
}

TEST(MajongHuType, YaoJiuTou) 
{
    TestCards hand_cards {
         0x0001, 0x0001, 0x0004, 0x0004, 0x0004, 0x0006, 0x0006, 0x0006, 0x0007, 0x0008, 0x0009, 0x0009, 0x0009, 0x0001
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.YaoJiuTou(door_cards));
}

TEST(MajongHuType, YiBanGao) 
{
    TestCards hand_cards {
         0x0001, 0x0002, 0x0003, 0x0001, 0x0002, 0x0003, 0x0006, 0x0006, 0x0006, 0x0007, 0x0008, 0x0009, 0x0009, 0x0009
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.YiBanGao(door_cards));
}

TEST(MajongHuType, LianLiu) 
{
    TestCards hand_cards {
         0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x0010, 0x0010, 0x0010, 0x0200, 0x0200, 0x0200, 0x0002, 0x0002
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.LianLiu(door_cards));
}

TEST(MajongHuType, LaoShaoFu) 
{
    TestCards hand_cards {
         0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009, 0x0010, 0x0010, 0x0010, 0x0200, 0x0200, 0x0200, 0x0002, 0x0002
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.LaoShaoFu(door_cards));
}

TEST(MajongHuType, YaoJiuKe) 
{
    TestCards hand_cards {
         0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009, 0x0005, 0x0005, 0x0005, 0x0200, 0x0200, 0x0200, 0x0002, 0x0002
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.YaoJiuKe(door_cards));
}

TEST(MajongHuType, YiMingGang) 
{
    TestCards hand_cards {
         0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009, 0x0005, 0x0005, 0x0005, 0x0002, 0x0002
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0004, 0x0004, 0x0004, 0x0004}));
    EXPECT_TRUE(hutype.YiMingGang(door_cards));
}

TEST(MajongHuType, BianZhang) 
{
    TestCards hand_cards {
        0x0001, 0x0002, 0x0003, 0x0003, 0x0007, 0x0008, 0x0009, 0x0005, 0x0005, 0x0005, 0x0020, 0x20, 0x20, 0x0003
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    ICardPtr hucard = CardConversion(hand_cards.back());
    EXPECT_TRUE(hutype.BianZhang(door_cards, hucard));
}

TEST(MajongHuType, KanZhang) 
{
    TestCards hand_cards {
        0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009, 0x0005, 0x0006, 0x0007, 0x0002, 0x0002
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0004, 0x0004, 0x0004, 0x0004}));

    ICardPtr hucard = CardConversion(hand_cards.back());
    EXPECT_TRUE(hutype.KanZhang(door_cards, hucard));
}

TEST(MajongHuType, DanDiaoJing) 
{
    TestCards hand_cards {
        0x0001, 0x0002, 0x0003, 0x0007, 0x0008, 0x0009, 0x0005, 0x0006, 0x0007, 0x0010, 0x0010
    };

    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, {0x0004, 0x0004, 0x0004, 0x0004}));

    ICardPtr hucard = CardConversion(hand_cards.back());
    EXPECT_TRUE(hutype.DanDiaoJing(door_cards, hucard));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); 
}