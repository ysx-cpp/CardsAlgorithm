#include "gtest/gtest.h"
#include "majong/mjcardtype/majonghutype.h"

using namespace algorithm;

static MjAlgorithm algo;
static MajongHuType hutype;

TEST(MajongHuType, test) 
{

    std::vector<OutDoorCards> vec_door_cards {
        {0x0002, 0x0002 },
        {0x0003, 0x0003}
    };
    hutype.DaSiXi(vec_door_cards);
    
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); 
}