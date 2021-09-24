#include "gtest/gtest.h"
#include "majong/mjcardtype/majonghutype.h"

using namespace algorithm;

static MjAlgorithm algo;
static MajongHuType hutype;

using TestCards = std::vector<uint16_t>;
using VecDoorCards = std::vector<OutDoorCards>;

static OutDoorCards GenDoorCards(DoorCardType door_type, const TestCards &cards)
{
    OutDoorCards door;
    door.card_type = door_type;
    door.cards.assign(cards.begin(), cards.end());
    return door;
}

TEST(MajongHuType, test) 
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); 
}