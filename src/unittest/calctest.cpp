#include "gtest/gtest.h"
#include "mahjong/mahjonghutype.h"
#include "mahjong/mahjongcalc.h"

using namespace algorithm;

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

static void HandCardsConversion(const TestCards &cards, std::vector<ICardPtr> &hand_cards)
{
	for (auto &it : cards)
	{
        hand_cards.emplace_back(CardConversion(it));
	}
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

struct HuCards
{
    HuCards(const TestCards &cards) : hdcards(cards)
    {
    }
    TestCards hdcards;
    VecDoorCards doorcards;
    uint64_t types;
};

uint64_t RunTest(const HuCards &testc)
{
    MajongCalc calc;
    std::vector<ICardPtr> hand_cards;
    HandCardsConversion(testc.hdcards, hand_cards);
    auto ret = calc.CheckHu(hand_cards);
    EXPECT_TRUE(ret);

    MjCalcContext context{hand_cards.back(), HuAction::Invalid, false, false, false, false};
    calc.SetContext(context);

    auto types = calc.CalculateHuType(testc.doorcards);
    auto fan = calc.CalculateFan(types);
    std::cout << "FAN =" << fan << std::endl;
    return types;
}

TEST(MajongCalc, DaSiXi) 
{
    TestCards test_cards {
        0x0010, 0x0010, 0x0010, 0x0020, 0x0020, 0x0020,
        0x0200, 0x0200
    };

    std::vector<ICardPtr> hand_cards;
    HandCardsConversion(test_cards, hand_cards);

    VecDoorCards door_cards;
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0030, 0x0030, 0x0030, 0x0030}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0040, 0x0040, 0x0040, 0x0040}));

    MjCalcContext context{hand_cards[4], HuAction::Invalid, false, false, false, false};
    MajongCalc calc;
    calc.SetContext(context);
    VecDoorCards vec_hand_door;

    ASSERT_TRUE(calc.CheckHu(hand_cards));
    auto types = calc.CalculateHuType(door_cards);
    auto fan = calc.CalculateFan(types);
    std::cout << "FAN =" << fan << std::endl;
}

TEST(MajongCalc, DaSanYuan) 
{
    TestCards test_cards {
        0x0100, 0x0100, 0x0100, 0x0200, 0x0200, 0x0200, 
        0x0007, 0x0008, 0x0009, 0x0010, 0x0010
    };

    std::vector<ICardPtr> hand_cards;
    HandCardsConversion(test_cards, hand_cards);

    VecDoorCards door_cards;
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0300, 0x0300, 0x0300, 0x0300})); 

    MjCalcContext context{hand_cards[4], HuAction::Invalid, false, false, false, false};
    MajongCalc calc;
    calc.SetContext(context);
    VecDoorCards vec_hand_door;

    ASSERT_TRUE(calc.CheckHu(hand_cards));
    auto types = calc.CalculateHuType(door_cards);
    auto fan = calc.CalculateFan(types);
    std::cout << "FAN =" << fan << std::endl;
}

TEST(MajongCalc, JiuBaoLianDeng) 
{
    TestCards test_cards {
        0x0001, 0x0001, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x0009, 0x0009, 0x0009
    };

    std::vector<ICardPtr> hand_cards;
    HandCardsConversion(test_cards, hand_cards);

    VecDoorCards door_cards;

    MjCalcContext context{hand_cards[4], HuAction::Invalid, false, false, false, false};
    MajongCalc calc;
    calc.SetContext(context);
    VecDoorCards vec_hand_door;

    ASSERT_TRUE(calc.CheckHu(hand_cards));
    auto types = calc.CalculateHuType(door_cards);
    auto fan = calc.CalculateFan(types);
    std::cout << "FAN =" << fan << std::endl;
}

TEST(MajongCalc, SiGang) 
{
    TestCards test_cards {
        0x0001, 0x0001, 0x0001, 0x0002, 0x0003
    };

    std::vector<ICardPtr> hand_cards;
    HandCardsConversion(test_cards, hand_cards);

    VecDoorCards door_cards;
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, {0x0300, 0x0300, 0x0300, 0x0300}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0009, 0x0009, 0x0009, 0x0009}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0004, 0x0004, 0x0004, 0x0004}));
    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0200, 0x0200, 0x0200, 0x0200}));

    MjCalcContext context{hand_cards[4], HuAction::Invalid, false, false, false, false};
    MajongCalc calc;
    calc.SetContext(context);
    VecDoorCards vec_hand_door;

    ASSERT_TRUE(calc.CheckHu(hand_cards));
    auto types = calc.CalculateHuType(door_cards);
    auto fan = calc.CalculateFan(types);
    std::cout << "FAN =" << fan << std::endl;
}

TEST(MajongCalc, HuYiChang)
{
    TestCards test_cards {
        0x0001, 0x0002, 0x0003, 0x0004, 0x0004, 0x0005, 0x0005, 0x0006, 0x0006, 0x0007, 0x0007
    };

    std::vector<ICardPtr> hand_cards;
    HandCardsConversion(test_cards, hand_cards);

    VecDoorCards door_cards;
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, { 0x0009, 0x0009, 0x0009}));

    MjCalcContext context{hand_cards.back(), HuAction::Invalid, false, false, false, false};
    MajongCalc calc;
    calc.SetContext(context);

    ASSERT_TRUE(calc.CheckHu(hand_cards));
    auto types = calc.CalculateHuType(door_cards);
    auto fan = calc.CalculateFan(types);
    std::cout << "FAN =" << fan << std::endl;
}

TEST(MajongCalc, BuNengHu)
{
    TestCards test_cards {
        0x0003, 0x0003, 0x0003, 0x0004, 0x0004, 0x0008, 0x0008, 0x0004
    };

    std::vector<ICardPtr> hand_cards;
    HandCardsConversion(test_cards, hand_cards);

    VecDoorCards door_cards;
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, { 0x0030, 0x0030, 0x0030}));
    door_cards.push_back(GenDoorCards(DoorCardType::PENG, { 0x0006, 0x0006, 0x0006}));

    MajongCalc calc;
    ASSERT_TRUE(calc.CheckHu(hand_cards));
    MjCalcContext context{hand_cards.back(), HuAction::Invalid, false, false, false, false};
    calc.SetContext(context);

    auto types = calc.CalculateHuType(door_cards);
    auto fan = calc.CalculateFan(types);
    std::cout << "FAN =" << fan << std::endl;
}

TEST(MajongCalc, FanError)
{
    std::vector<HuCards> test;

    {
        HuCards item({0x0003, 0x0003, 0x0003, 0x0004, 0x0004, 0x0008, 0x0008, 0x0004});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x0030, 0x0030, 0x0030}));
        item.types = 292734078858297344;
        test.emplace_back(item); //0 {碰碰胡,混一色,二五八将,幺九刻,}
    }

    { 
        HuCards item({0x0001, 0x0001, 0x0001, 0x0003, 0x0004, 0x0007, 0x0007, 0x0008, 0x0008, 0x0008, 0x0002});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x0009, 0x0009, 0x0009, 0x0009}));
        item.types = 289426644871938048;
        test.emplace_back(item); //1
    }
    { 
        HuCards item({0x6, 0x7, 0x10, 0x10, 0x5});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x0300, 0x0300, 0x0300}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x0030, 0x0030, 0x0030}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x3, 0x4, 0x5}));
        item.types = 35253091565568;
        test.emplace_back(item); //2
    }
    {
        HuCards item({0x4, 0x4, 0x4, 0x5, 0x6, 0x6, 0x8, 0x200, 0x200, 0x200, 0x7});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::GANG, {0x10, 0x10, 0x10, 0x10}));
        item.types = 2882339014608683008;
        test.emplace_back(item); //3
    }
    {
        HuCards item({0x2, 0x3, 0x6, 0x5, 0x7, 0x300, 0x300, 0x1});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x200, 0x200, 0x200}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x100, 0x100, 0x100, 0x100}));
        item.types = 1154047473233168384;
        test.emplace_back(item); //4
    }
    {
        HuCards item({0x10, 0x10});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::GANG, {0x40, 0x40, 0x40, 0x40}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x100, 0x100, 0x100, 0x100}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x200, 0x200, 0x200, 0x200}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x300, 0x300, 0x300, 0x300}));
        item.types = 67112980;
        test.emplace_back(item); //5 {大三元,四杠,字一色,三暗刻,}
    }
    {
        HuCards item({0x10, 0x10, 0x10, 0x7, 0x7, 0x7, 0x40, 0x40});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::GANG, {0x20, 0x20, 0x20, 0x20}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x30, 0x30, 0x30, 0x30}));
        item.types = 5477503149935690752;
        test.emplace_back(item); //6
    }
    {
        HuCards item({0x3, 0x3, 0x3, 0x5, 0x300, 0x300, 0x300, 0x4});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x7, 0x8, 0x9}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x7, 0x8, 0x9}));
        item.types = 2341907059324223488;
        test.emplace_back(item); //7 {混一色,箭刻,一般高,坎张,}
    }
    {
        HuCards item({0x3, 0x4, 0x4, 0x4, 0x2});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x20, 0x20, 0x20}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x7, 0x8, 0x9}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x1, 0x1, 0x1, 0x1}));
        item.types = 289356344778031104;
        test.emplace_back(item); //8
    }
    {
        HuCards item({0x2, 0x2, 0x3, 0x4, 0x5, 0x5, 0x6, 0x7, 0x8, 0x8, 0x8});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x100, 0x100, 0x100}));
        item.types = 4538852718936064;
        test.emplace_back(item); //9
    }
    {
        HuCards item({0x10, 0x10});
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x100, 0x100, 0x100, 0x100}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::GANG, {0x20, 0x20, 0x20, 0x20}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x30, 0x30, 0x30}));
        item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x40, 0x40, 0x40}));
        item.types = 5189307855009747968;
        test.emplace_back(item); //10 {小四喜,字一色,箭刻,暗杠,一明杠,单吊将,}
    }

    MajongCalc calc;
    int index = 0;
    for (auto &it : test)
    {
        std::vector<ICardPtr> hand_cards;
        HandCardsConversion(it.hdcards, hand_cards);
        ASSERT_TRUE(calc.CheckHu(hand_cards));

        MjCalcContext context{hand_cards.back(), HuAction::Invalid, false, false, false, false};
        calc.SetContext(context);

        auto types = calc.CalculateHuType(it.doorcards);
        auto fan = calc.CalculateFan(types);
        std::cout << "FAN =" << fan << " index =" << index++ << std::endl;
        ASSERT_TRUE(types == it.types);
    }
}

TEST(MajongCalc, BuNengHu1)
{
    std::vector<TestCards> cards_libs {
        { 0x0007, 0x0008, 0x0009, 0x0001, 0x0001, 0x0030, 0x0030, 0x0030, 0x0100, 0x0100, 0x0100, 0x0200, 0x0200, 0x0001 },
        { 0x0001, 0x0001, 0x0003, 0x0004, 0x0004, 0x0005, 0x0005, 0x0006, 0x0020, 0x0020, 0x0200, 0x0200, 0x0200, 0x0001 },
        { 0x0001, 0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003, 0x0003, 0x0003, 0x0004, 0x0004 },
        {0x1, 0x2, 0x3, 0x3, 0x4, 0x5, 0x6, 0x6, 0x8, 0x8, 0x8, 0x40, 0x40, 0x40},
        {0x4, 0x4, 0x4, 0x5, 0x5, 0x5, 0x5, 0x6, 0x6, 0x6, 0x6, 0x7, 0x7, 0x7},
        {0x1, 0x1, 0x3, 0x3, 0x8, 0x8, 0x8, 0x20, 0x20, 0x100, 0x100, 0x200, 0x200, 0x8},
    };

    MajongCalc calc;
    for (auto &it : cards_libs)
    {
        VecDoorCards door_cards;
        std::vector<ICardPtr> hand_cards;
        HandCardsConversion(it, hand_cards);

        MjCalcContext context{hand_cards.back(), HuAction::Invalid, false, false, false, false};
        calc.SetContext(context);

        ASSERT_TRUE(calc.CheckHu(hand_cards));
        auto types = calc.CalculateHuType(door_cards);
        auto fan = calc.CalculateFan(types);
        std::cout << "\n FAN =" << fan << std::endl;
    }
}

#define RUN_ALL
#ifdef RUN_ALL
TEST(MajongCalc, JiuBaoLianDengError)
{
    HuCards item({0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x8, 0x9, 0x9, 0x9});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x1, 0x1, 0x1}));
    item.types = 364791569819107328; //{清一色,二五八将,连六,幺九刻,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiJieGao2SiAnKeError)
{
    HuCards item({0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x5, 0x5});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x1, 0x1, 0x1, 0x1}));
    item.types = 4905545894115483648; //{四暗刻,一色四节高,清一色,暗杠,二五八将,幺九刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types); 
}
TEST(MajongCalc, QiDuiError)
{
    HuCards item({0x1, 0x1, 0x3, 0x3, 0x8, 0x8, 0x8, 0x20, 0x20, 0x100, 0x100, 0x200, 0x200, 0x8});
    item.types = 13792342579347456; //{七对,混一色,四归一,二五八将,幺九头,}
    uint64_t types = RunTest(item);
    ASSERT_EQ(types , item.types);
}
TEST(MajongCalc, QiDui2SiTongShun)
{
    HuCards item({0x7, 0x7, 0x7, 0x7, 0x8, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0x9, 0x2, 0x2});
    item.types = 4616400724287324224; //{百万石,一色四筒顺,门前清,二五八将,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, QiDui2SiTongShun1)
{
    HuCards item({0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x4, 0x4});
    item.types = 4611897125198921728; //{一色四筒顺,清一色,小于5,门前清,平胡,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, YiSeShuangLongHuiError)
{
    HuCards item({ 0x1, 0x2, 0x3, 0x5, 0x7, 0x8, 0x9, 0x7, 0x8, 0x9, 0x5});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x1, 0x2, 0x3}));
    item.types = 4616189618054774784;
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types); //{一色双龙会,二五八将,单吊将,}
}
TEST(MajongCalc, YiSeSanJieGaoError)
{
    HuCards item({ 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x7, 0x8, 0x9, 0x9, 0x9});
    item.types = 1161999072683360256; //{清一色,一色三节高,三暗刻,门前清,幺九头,边张,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, QingLongError)
{
    HuCards item({ 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x3, 0x3, 0x3, 0x4, 0x4});
    item.types = 351843739762688; //{清一色,清龙,门前清,四归一,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, QingLongError1)
{
    HuCards item({ 0x3, 0x4, 0x4, 0x5, 0x5, 0x6, 0x7, 0x8, 0x9, 0x9, 0x9});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x1, 0x2, 0x3}));
    item.types = 1162069441368817664; //{清一色,清龙,平胡,幺九头,边张,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiJieGaoError)
{
    HuCards item({ 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x5, 0x5});
    item.types = 4904419994208641024; //{四暗刻,一色四节高,清一色,二五八将,幺九刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiJieGaoError1)
{
    HuCards item({ 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x5, 0x5});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x1, 0x1, 0x1}));
    item.types = 4904419994275741696; //{一色四节高,清一色,三暗刻,二五八将,幺九刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiJieGaoError2)
{
    HuCards item({ 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x6, 0x6});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x1, 0x1, 0x1}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x2, 0x2, 0x2}));
    item.types = 4900479344534683648; //{一色四节高,清一色,双暗刻,幺九刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiBuGaoError)
{
    HuCards item({ 0x1, 0x2, 0x3, 0x3, 0x4, 0x5, 0x5, 0x6, 0x7, 0x10, 0x10});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x7, 0x8, 0x9}));
    item.types = 4611686087146995712; //{一色四步高,混一色,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SanBuGaoError)
{
    HuCards item({0x3, 0x4, 0x5, 0x10, 0x10, 0x10, 0x100, 0x100});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x1, 0x2, 0x3}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x2, 0x3, 0x4}));
    item.types = 4899916463332130816; //{一色三步高,混一色,幺九刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SanBuGaoError1)
{
    HuCards item({0x2, 0x3, 0x3, 0x4, 0x4, 0x5, 0x5, 0x6, 0x7, 0x9, 0x9});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x1, 0x2, 0x3}));
    item.types = 4692891549244063744; //{清一色,一色三步高,平胡,幺九头,连六,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiGangError)
{
    HuCards item({0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x4, 0x4});
    item.types = 4611897125198921728; //{一色四筒顺,清一色,小于5,门前清,平胡,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiTongShunError1)
{
    HuCards item({0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x9, 0x9});
    item.types = 4611898224173678592; //{一色四筒顺,清一色,门前清,平胡,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiTongShunError)
{
    HuCards item({0x7, 0x7, 0x7, 0x7, 0x8, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0x9, 0x6, 0x6});
    item.types = 4611897124928389184; //{百万石,一色四筒顺,大于5,门前清,平胡,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiGangError1)
{
    HuCards item({0x4, 0x4});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x1, 0x1, 0x1, 0x1}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x2, 0x2, 0x2, 0x2}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x3, 0x3, 0x3, 0x3}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::GANG, {0x5, 0x5, 0x5, 0x5}));
    item.types = 288230376229306384; //{清一色,一色三步高,平胡,幺九头,连六,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SiGangError2)
{
    HuCards item({0x7, 0x7, 0x7, 0x7, 0x8, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0x9, 0x6, 0x6});
    item.types = 4611897124928389184; //{百万石,一色四筒顺,大于5,门前清,平胡,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, LianLiuError)
{
    HuCards item({0x3, 0x4, 0x5, 0x8, 0x8});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x1, 0x2, 0x3}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x6, 0x6, 0x6, 0x6}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::GANG, {0x30, 0x30, 0x30, 0x30}));
    item.types = 5482006715136212992; //{混一色,暗杠,二五八将,幺九刻,一明杠,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, LianLiuError1)
{
    HuCards item({0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x8, 0x9, 0x9, 0x9});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x1, 0x1, 0x1}));
    item.types = 364791569819107328; //{清一色,二五八将,连六,幺九刻,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, LianLiuError2)
{
    HuCards item({0x1, 0x2, 0x3, 0x3, 0x4, 0x5, 0x6, 0x6, 0x8, 0x8, 0x8, 0x40, 0x40, 0x40});
    item.types = 288300813615366144; //{混一色,门前清,幺九刻,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SanBuGaoError3)
{
    HuCards item({0x1, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x6, 0x7, 0x8, 0x9, 0x9, 0x9, 0x4});
    item.types = 9077568034570240; //{清一色,一色三步高,门前清,幺九头,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, LianLiu_SanBuGao)
{
    HuCards item({0x2, 0x3, 0x3, 0x4, 0x4, 0x5, 0x5, 0x6, 0x7, 0x9, 0x9});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x1, 0x2, 0x3}));
    item.types = 4692891549244063744; //{清一色,一色三步高,平胡,幺九头,连六,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SanTongShun2SiJieGao)
{
    HuCards item({0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x7, 0x8, 0x9, 0x9, 0x9});
    item.types = 1161999072683360256; //{清一色,一色三节高,三暗刻,门前清,幺九头,边张,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, YiSeSiJieGao)
{
    HuCards item({0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x9, 0x9});
    item.types = 4620693217684299776; //{四暗刻,一色四节高,清一色,幺九头,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, QiuDuiZiYiSe)
{
    HuCards item({0x10, 0x10, 0x20, 0x20, 0x30, 0x30, 0x40, 0x40, 0x100, 0x100, 0x200, 0x200, 0x300, 0x300});
    item.types = 1052672; //{字一色,七对,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SanTongShunError)
{
    HuCards item({0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x5, 0x5, 0x2});
    item.types = 45387839996690432; //{清一色,门前清,四归一,幺九头,一般高,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, QiuduiLianLiu)
{
    HuCards item({0x1, 0x1, 0x2, 0x2, 0x3, 0x3, 0x4, 0x4, 0x5, 0x5, 0x6, 0x6, 0x9, 0x9});
    item.types = 13510798885257216; //{七对,清一色,二五八将,幺九头,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, JianKeError)
{
    HuCards item({0x100, 0x100});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::GANG, {0x30, 0x30, 0x30, 0x30}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x20, 0x20, 0x20, 0x20}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x300, 0x300, 0x300}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x40, 0x40, 0x40}));
    item.types = 5189307856083488768; //{七对,清一色,二五八将,幺九头,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, TongShunAnKeError)
{
    HuCards item({0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x6, 0x7, 0x8, 0x9, 0x9});
    item.types = 4620763586503901184; //{清一色,一色三节高,三暗刻,门前清,幺九头,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, YiSeSiJieGaoError2)
{
    HuCards item({0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x10, 0x10});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x4, 0x4, 0x4}));
    item.types = 4899916463365750784; //{一色四节高,三暗刻,混一色,幺九刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, YiSeSiJieGaoError3)
{
    HuCards item({0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x10, 0x10, 0x10, 0x20, 0x20});
    item.types = 4899916463306973184; //{一色四节高,三暗刻,混一色,幺九刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, SanTongShunError3)
{
    HuCards item({0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x10, 0x10});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x5, 0x5, 0x5}));
    item.types = 4899916497733812224; //{一色三节高,三暗刻,碰碰胡,混一色,幺九刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, YiSeSiBuGaoError)
{
    HuCards item({0x3, 0x4, 0x4, 0x5, 0x5, 0x6, 0x6, 0x6, 0x7, 0x7, 0x8, 0x8, 0x9, 0x6});
    item.types = 72550175249268736; //{清一色,门前清,平胡,四归一,连六,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, LaoShaoFuError)
{
    HuCards item({0x1, 0x2, 0x3, 0x40, 0x40, 0x200, 0x200, 0x200});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x7, 0x8, 0x9}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x6, 0x6, 0x6}));
    item.types = 144150441167421440; //{混一色,箭刻,老少副,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, YiBanGaoError)
{
    HuCards item({0x1, 0x2, 0x3, 0x7, 0x8, 0x9, 0x10, 0x10, 0x10, 0x20, 0x20});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::CHI, {0x1, 0x2, 0x3}));
    item.types = 4791831171753312256; //{混一色,全带幺,一般高,老少副,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, DaSanYuanError) //大三元幺九刻
{
    HuCards item({0x100, 0x100, 0x100, 0x200, 0x200, 0x200, 0x300, 0x300, 0x300, 0x10, 0x10, 0x10, 0x20, 0x20});
    item.types = 4611686018427400196; //{大三元,字一色,四暗刻,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
TEST(MajongCalc, BuNengHuError)
{
    HuCards item({0x3, 0x10, 0x10, 0x10, 0x3});
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::AN_GANG, {0x40, 0x40, 0x40, 0x40}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x1, 0x1, 0x1}));
    item.doorcards.emplace_back(GenDoorCards(DoorCardType::PENG, {0x20, 0x20, 0x20}));
    item.types = 4613374972440608768; //{三风刻,碰碰胡,混一色,双暗刻,暗杠,单吊将,}
    uint64_t types = RunTest(item);
    ASSERT_TRUE(types == item.types);
}
#endif
