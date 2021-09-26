#include "gtest/gtest.h"
#include "majong/mjcardtype/majonghutype.h"

using namespace algorithm;

static MajongHuType hutype;

using TestCards = std::vector<uint16_t>;
using VecDoorCards = std::vector<OutDoorCards>;

static OutDoorCards GenDoorCards(DoorCardType door_type, const TestCards &cards)
{
    OutDoorCards door;
    door.card_type = door_type;

	for (auto &it : cards)
	{
		MjCard::EType type = MjCard::E_TYPE_COUNT_;
		MjCard::EFace face = MjCard::E_FACE_COUNT_;
		if (it & 0x000f)
		{
			type = MjCard::E_MYRIAD_TYPE;
			uint16_t v = it & 0x000f;
			face = static_cast<MjCard::EFace>(v);
		}
		else if (it & 0x00f0)
		{
			type = MjCard::E_WIND_TYPE;
			uint16_t v = it & 0x00f0;
			face = static_cast<MjCard::EFace>(v >> 4);
		}
		else if (it & 0x0f00)
		{
			type = MjCard::E_WORD_TYPE;
			uint16_t v = it & 0x0f00;
			face = static_cast<MjCard::EFace>(v >> 8);
		}	
		else if (it & 0xf000)
		{
			type = MjCard::E_FLOWER_TYPE;
			uint16_t v = it & 0xf000;
			face = static_cast<MjCard::EFace>(v >> 12);
		}
		else
			assert(false);

		ICardPtr mjc = std::make_shared<MjCard>(type, face, 0);
		door.cards.push_back(mjc);
	}

    return door;
}

TEST(MajongHuType, DaSiXi) 
{
    TestCards hand_cards {
        0x0010, 0x0010, 0x0010, 0x0020, 0x0020, 0x0020,
        0x0200, 0x0200
    };

    MjAlgorithm algo;
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

    MjAlgorithm algo;
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

    MjAlgorithm algo;
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

    MjAlgorithm algo;
    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    door_cards.push_back(GenDoorCards(DoorCardType::GANG, { 0x0300, 0x0300, 0x0300, 0x0300}));
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

    MjAlgorithm algo;
    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.QiLianDui(door_cards));
}

TEST(MajongHuType, BaiWanShi) 
{
    TestCards hand_cards {
        0x0005, 0x0006, 0x0006, 0x0007, 0x0007, 0x0007, 0x0007, 0x0008, 0x0008, 0x0008, 0x0008, 0x0009, 0x0009, 0x0009
    };

    MjAlgorithm algo;
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

    MjAlgorithm algo;
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

    MjAlgorithm algo;
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

    MjAlgorithm algo;
    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0300, 0x0300, 0x0300, 0x0300}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0009, 0x0009, 0x0009, 0x0009}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0003, 0x0003, 0x0003, 0x0003}));
    door_cards.push_back(GenDoorCards(DoorCardType::AN_GANG, { 0x0200, 0x0200, 0x0200, 0x0200}));

    EXPECT_TRUE(hutype.SiAnKe(door_cards));
}

TEST(MajongHuType, YiSeShuangLongHui) 
{
    TestCards hand_cards {
        0x0001, 0x0002, 0x0003, 0x0001, 0x0002, 0x0003, 0x0007, 0x0007, 0x0008, 0x0008, 0x0009, 0x0009, 0x0005, 0x0005
    };

    MjAlgorithm algo;
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

    MjAlgorithm algo;
    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.YiSeSiTongShun(door_cards));
}

TEST(MajongHuType, YiSeSiJieGao) 
{
    TestCards hand_cards {
        0x0001, 0x0001, 0x0006, 0x0007, 0x0007, 0x0007, 0x0006, 0x0008, 0x0008, 0x0008, 0x0006, 0x0009, 0x0009, 0x0009
    };

    MjAlgorithm algo;
    algo.InputHandCard(hand_cards);
    ASSERT_TRUE(algo.CheckPingHu());

    VecDoorCards door_cards;
    algo.OutPutDoorCards(door_cards);

    EXPECT_TRUE(hutype.YiSeSiJieGao(door_cards));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); 
}