#include <gtest/gtest.h>
#include "../src/StatsHolderTmp.h"
#include "../src/StatsHolderResult.h"
#include "Packet.h"
#include "HttpLayer.h"
#include "IPv4Layer.h"
//В тестах плохо разобрался, совсем не то что хотел вышло
using namespace UserStructs;
//TEST(HostnameAssotiatedTests, BasicCreate) {
//    StatsHolderResult::HostNameAssociated Example{pcpp::IPv4Address("127.0.0.1").toString(), "vk.com", 0, 0, 0, 0};
//    StatsHolderResult::HostNameAssociated ExampleSecond{pcpp::IPv4Address("127.0.0.1").toString(), "vk.com", 0, 0, 0,0};
//}
//ASSERT_EQ(Example, ExampleSecond);
//
//TEST(HostnameAssotiatedTests, BasicCreateWithoutHost) {
//    StatsHolderResult::HostNameAssociated Example{pcpp::IPv4Address("127.0.0.1").toString(), {}, 0, 0, 0, 0};
//    StatsHolderResult::HostNameAssociated ExampleSecond{pcpp::IPv4Address("127.0.0.1").toString(), {}, 0, 0, 0, 0}
//    ASSERT_EQ(Example, ExampleSecond);
//}
//
//TEST(HostnameAssotiatedTests, BasicOverloadOperatorPlusAssignHimSelf) {
//    StatsHolderResult::HostNameAssociated ExampleFirst{pcpp::IPv4Address("127.0.0.1").toString(), "vk.com", 1, 1, 1, 1};
//    ExampleFirst += ExampleFirst;
//    StatsHolderResult::HostNameAssociated ExampleThird{pcpp::IPv4Address("127.0.0.1").toString(), "vk.com", 2, 2, 2, 2}
//    EXPECT_EQ(ExampleFirst, ExampleThrid);
//}
//
//TEST(HostnameAssotiatedTests, FirstHaveNoHostOverloadOperatorPlusAssign) {
//    StatsHolderResult::HostNameAssociated ExampleFirst{pcpp::IPv4Address("127.0.0.1").toString(), {}, 1, 1, 1, 1};
//    StatsHolderResult::HostNameAssociated ExampleSecond{pcpp::IPv4Address("127.0.0.1").toString(), "vk.com", 1, 1, 1,
//                                                        1};
//    ExampleFirst += ExampleSecond;
//    StatsHolderResult::HostNameAssociated ExampleThird{pcpp::IPv4Address("127.0.0.1").toString(), "vk.com", 2, 2, 2, 2}
//    EXPECT_EQ(ExampleFirst, ExampleThird);
//}
//
//TEST(HostnameAssotiatedTests, SecondHaveNoHostOverloadOperatorPlusAssign) {
//    StatsHolderResult::HostNameAssociated ExampleFirst{pcpp::IPv4Address("127.0.0.1").toString(), "vk.com", 1, 1, 1, 1};
//    StatsHolderResult::HostNameAssociated ExampleSecond{pcpp::IPv4Address("127.0.0.1").toString(), {}, 1, 1, 1, 1};
//    ExampleFirst += ExampleSecond;
//    StatsHolderResult::HostNameAssociated ExampleThird{pcpp::IPv4Address("127.0.0.1").toString(), "vk.com", 2, 2, 2, 2};
//    EXPECT_EQ(ExampleFirst, ExampleThird);
//}

/*Не успел разобраться как без препроцессора адекватно тестировать приватные поля классов
  В данном случае мне нужен только size map'ы*/
//TEST(GlobalHostMapTests, IsCorrectAfterMergeByEmptyTempMap) {
//
//    StatsHolderResult Example{};
//    StatsHolderTmp MergeContent;
//    Example.MergeFromTmp(MergeContent);
//    EXPECT_EQ(Example.);
//}
//
//TEST(GlobalHostMapTests, IsFilledAfterMergeBy1PacketTempMap) {
//
//}
//
//TEST(TemporaryHostMapTests, IsEmptyAfterMergeByGlobalHostMap) {
//
//}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
