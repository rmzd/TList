#include <gtest.h>
#include "TList.h"
#include <sstream>

TEST(TListTest, ConstructorThrowsWhenSizeExceedsMax) {
    EXPECT_THROW({
        TList<int> list(TList<int>::MAX_SIZE + 1);
        }, std::out_of_range);
}

TEST(TListTest, PushFrontThrowsWhenListIsFull) {
    TList<int> list;
    for (size_t i = 0; i < TList<int>::MAX_SIZE; ++i) {
        list.AddEnd(i);
    }

    EXPECT_TRUE(list.IsFull());
    EXPECT_THROW({
        list.AddStart(42);
        }, std::overflow_error);
}

TEST(TListTest, PopFrontThrowsWhenListIsEmpty) {
    TList<int> list;
    EXPECT_TRUE(list.IsEmpty());
    EXPECT_THROW({
        list.RemoveStart();
        }, std::underflow_error);
}

TEST(TListTest, IndexOperatorThrowsForInvalidIndex) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    EXPECT_THROW({
        list[5];
        }, std::out_of_range);

    EXPECT_THROW({
        list[-1];
        }, std::out_of_range);
}

TEST(TListTest, MoveConstructorCreatesValidList) {
    TList<int> list1;
    list1.AddEnd(1);
    list1.AddEnd(2);
    list1.AddEnd(3);
    list1.AddEnd(4);
    list1.AddEnd(5);

    TList<int> list2(std::move(list1));

    EXPECT_EQ(list2.GetSize(), 5);
    EXPECT_TRUE(list1.IsEmpty());
    EXPECT_EQ(list2[0], 1);
    EXPECT_EQ(list2[4], 5);
}

TEST(TListTest, MovedFromListIsEmpty) {
    TList<int> list1;
    list1.AddEnd(10);
    list1.AddEnd(20);
    list1.AddEnd(30);

    TList<int> list2 = std::move(list1);

    EXPECT_EQ(list1.GetSize(), 0);
    EXPECT_TRUE(list1.IsEmpty());
    EXPECT_FALSE(list1.IsFull());
}

TEST(TListTest, AddStartAndRemoveStartWorkCorrectly) {
    TList<int> list;

    list.AddStart(3);
    list.AddStart(2);
    list.AddStart(1);

    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);

    EXPECT_EQ(list.RemoveStart(), 1);
    EXPECT_EQ(list.RemoveStart(), 2);
    EXPECT_EQ(list.RemoveStart(), 3);
    EXPECT_TRUE(list.IsEmpty());
}

TEST(TListTest, AddEndAndRemoveEndWorkCorrectly) {
    TList<int> list;

    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);

    EXPECT_EQ(list.RemoveEnd(), 3);
    EXPECT_EQ(list.RemoveEnd(), 2);
    EXPECT_EQ(list.RemoveEnd(), 1);
    EXPECT_TRUE(list.IsEmpty());
}

TEST(TListTest, InitializerListConstructor) {
    TList<int> list;
    list.AddEnd(10);
    list.AddEnd(20);
    list.AddEnd(30);
    list.AddEnd(40);
    list.AddEnd(50);

    EXPECT_EQ(list.GetSize(), 5);
    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[2], 30);
    EXPECT_EQ(list[4], 50);
}

TEST(TListTest, CopyConstructorCreatesIndependentCopy) {
    TList<int> list1;
    list1.AddEnd(1);
    list1.AddEnd(2);
    list1.AddEnd(3);

    TList<int> list2(list1);

    EXPECT_EQ(list1.GetSize(), list2.GetSize());
    EXPECT_TRUE(list1 == list2);

    list2.AddEnd(4);
    EXPECT_EQ(list1.GetSize(), 3);
    EXPECT_EQ(list2.GetSize(), 4);
    EXPECT_FALSE(list1 == list2);
}

TEST(TListTest, AssignmentOperatorWorksCorrectly) {
    TList<int> list1;
    list1.AddEnd(1);
    list1.AddEnd(2);
    list1.AddEnd(3);
    list1.AddEnd(4);
    list1.AddEnd(5);

    TList<int> list2;

    list2 = list1;

    EXPECT_EQ(list2.GetSize(), 5);
    EXPECT_TRUE(list1 == list2);

    list1 = list1;
    EXPECT_EQ(list1.GetSize(), 5);
}

TEST(TListTest, MoveAssignmentOperator) {
    TList<int> list1;
    list1.AddEnd(100);
    list1.AddEnd(200);
    list1.AddEnd(300);

    TList<int> list2;

    list2 = std::move(list1);

    EXPECT_EQ(list2.GetSize(), 3);
    EXPECT_TRUE(list1.IsEmpty());
    EXPECT_EQ(list2[0], 100);
    EXPECT_EQ(list2[2], 300);
}

TEST(TListTest, ConcatenationOperator) {
    TList<int> list1;
    list1.AddEnd(1);
    list1.AddEnd(2);
    list1.AddEnd(3);

    TList<int> list2;
    list2.AddEnd(4);
    list2.AddEnd(5);
    list2.AddEnd(6);

    TList<int> result = list1 + list2;

    EXPECT_EQ(result.GetSize(), 6);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[3], 4);
    EXPECT_EQ(result[5], 6);

    EXPECT_EQ(list1.GetSize(), 3);
    EXPECT_EQ(list2.GetSize(), 3);
}

TEST(TListTest, EqualityAndInequalityOperators) {
    TList<int> list1;
    list1.AddEnd(1);
    list1.AddEnd(2);
    list1.AddEnd(3);

    TList<int> list2;
    list2.AddEnd(1);
    list2.AddEnd(2);
    list2.AddEnd(3);

    TList<int> list3;
    list3.AddEnd(1);
    list3.AddEnd(2);
    list3.AddEnd(4);

    TList<int> list4;
    list4.AddEnd(1);
    list4.AddEnd(2);

    EXPECT_TRUE(list1 == list2);
    EXPECT_FALSE(list1 == list3);
    EXPECT_FALSE(list1 == list4);

    EXPECT_FALSE(list1 != list2);
    EXPECT_TRUE(list1 != list3);
    EXPECT_TRUE(list1 != list4);
}

TEST(TListTest, CircularStructureThroughIterator) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    auto it = list.begin();

    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 1);

    --it;
    EXPECT_EQ(*it, 3);
    --it;
    EXPECT_EQ(*it, 2);
    --it;
    EXPECT_EQ(*it, 1);
    --it;
    EXPECT_EQ(*it, 3);
}

TEST(TListTest, IsEmptyAndIsFull) {
    TList<int> list;

    EXPECT_TRUE(list.IsEmpty());
    EXPECT_FALSE(list.IsFull());

    for (size_t i = 0; i < TList<int>::MAX_SIZE; ++i) {
        list.AddEnd(i);
    }

    EXPECT_FALSE(list.IsEmpty());
    EXPECT_TRUE(list.IsFull());
}

TEST(TListTest, SingleElementList) {
    TList<int> list;

    list.AddStart(42);
    EXPECT_EQ(list.GetSize(), 1);
    EXPECT_EQ(list[0], 42);
    EXPECT_FALSE(list.IsEmpty());

    auto it = list.begin();
    ++it;
    EXPECT_EQ(*it, 42);

    list.RemoveEnd();
    EXPECT_TRUE(list.IsEmpty());
}

TEST(TListTest, InsertAfterIterator) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(3);
    list.AddEnd(4);

    auto it = list.begin();
    list.insertAfter(it, 2);

    EXPECT_EQ(list.GetSize(), 4);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);
    EXPECT_EQ(list[3], 4);
}

TEST(TListTest, RemoveElementByIterator) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);
    list.AddEnd(4);
    list.AddEnd(5);

    auto it = list.begin();
    ++it;

    list.remove(it);

    EXPECT_EQ(list.GetSize(), 4);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 3);
    EXPECT_EQ(list[2], 4);
    EXPECT_EQ(list[3], 5);
}

TEST(TListTest, IteratorOperations) {
    TList<int> list;
    list.AddEnd(10);
    list.AddEnd(20);
    list.AddEnd(30);
    list.AddEnd(40);
    list.AddEnd(50);

    auto it1 = list.begin();
    EXPECT_EQ(*it1, 10);
    EXPECT_EQ(*(++it1), 20);

    auto it2 = list.begin();
    EXPECT_EQ(*(it2++), 10);
    EXPECT_EQ(*it2, 20);

    auto it3 = list.begin();
    ++it3; ++it3;
    EXPECT_EQ(*(--it3), 20);

    auto it4 = list.begin();
    ++it4;
    EXPECT_EQ(*(it4--), 20);
    EXPECT_EQ(*it4, 10);
}

TEST(TListTest, StreamOutputOperator) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    std::stringstream ss;

    ss << list;

    EXPECT_EQ(ss.str(), "[1 <-> 2 <-> 3]");
}

TEST(TListTest, StreamOutputEmptyList) {
    TList<int> list;
    std::stringstream ss;

    ss << list;

    EXPECT_EQ(ss.str(), "[]");
}

TEST(TListTest, StreamInputOperator) {
    TList<int> list;
    std::stringstream ss("10 20 30 40");

    ss >> list;

    EXPECT_EQ(list.GetSize(), 4);
    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 20);
    EXPECT_EQ(list[2], 30);
    EXPECT_EQ(list[3], 40);
}

TEST(TListTest, IndexOperatorForModification) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);
    list.AddEnd(4);
    list.AddEnd(5);

    list[2] = 100;

    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 100);
    EXPECT_EQ(list[3], 4);
    EXPECT_EQ(list[4], 5);
}

TEST(TListTest, ComplexDataType) {
    TList<std::string> list;

    list.AddEnd("Hello");
    list.AddEnd("World");
    list.AddEnd("!");

    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list[0], "Hello");
    EXPECT_EQ(list[1], "World");
    EXPECT_EQ(list[2], "!");

    TList<std::string> list2;
    list2.AddEnd("Test");
    list2.AddEnd("123");

    auto result = list + list2;

    EXPECT_EQ(result.GetSize(), 5);
    EXPECT_EQ(result[3], "Test");
    EXPECT_EQ(result[4], "123");
}

TEST(TListTest, MultiplePassesThroughCircularList) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    auto it = list.begin();

    for (int i = 0; i < 10; ++i) {
        ++it;
    }

    EXPECT_EQ(*it, 2);
}

TEST(TListTest, WorkingWithMaxSize) {
    TList<int> list;

    for (size_t i = 0; i < TList<int>::MAX_SIZE; ++i) {
        list.AddEnd(static_cast<int>(i));
    }

    EXPECT_EQ(list.GetSize(), TList<int>::MAX_SIZE);
    EXPECT_TRUE(list.IsFull());

    for (size_t i = 0; i < TList<int>::MAX_SIZE / 2; ++i) {
        list.RemoveStart();
    }

    EXPECT_EQ(list.GetSize(), TList<int>::MAX_SIZE / 2);
    EXPECT_FALSE(list.IsFull());
}

TEST(TListTest, CopyAndModifyIndependence) {
    TList<int> original;
    original.AddEnd(1);
    original.AddEnd(2);
    original.AddEnd(3);
    original.AddEnd(4);
    original.AddEnd(5);

    TList<int> copy = original;

    copy.RemoveEnd();
    copy.AddEnd(100);

    EXPECT_EQ(original.GetSize(), 5);
    EXPECT_EQ(original[4], 5);

    EXPECT_EQ(copy.GetSize(), 5);
    EXPECT_EQ(copy[4], 100);
}

TEST(TListTest, IteratorWithEmptyList) {
    TList<int> list;

    auto begin = list.begin();
    auto end = list.end();

    EXPECT_TRUE(begin == end);
}

TEST(TListTest, LargeNumberOfElements) {
    TList<int> list;
    const int N = 1000;

    for (int i = 0; i < N; ++i) {
        list.AddEnd(i * 2);
    }

    EXPECT_EQ(list.GetSize(), N);

    EXPECT_EQ(list[0], 0);
    EXPECT_EQ(list[N / 2], (N / 2) * 2);
    EXPECT_EQ(list[N - 1], (N - 1) * 2);

    int expected = 0;
    auto it = list.begin();
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(*it, expected);
        ++it;
        expected += 2;
    }
}

TEST(TListTest, ConstIndexOperator) {
    TList<int> temp;
    temp.AddEnd(10);
    temp.AddEnd(20);
    temp.AddEnd(30);
    const TList<int> list = temp;

    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 20);
    EXPECT_EQ(list[2], 30);
}

TEST(TListTest, InsertAfterEndIterator) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    auto it = list.begin();
    ++it; ++it;

    list.insertAfter(it, 4);

    EXPECT_EQ(list.GetSize(), 4);
    EXPECT_EQ(list[3], 4);
}

TEST(TListTest, ConcatenationWithEmptyLists) {
    TList<int> empty1;
    TList<int> empty2;
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    auto result1 = empty1 + empty2;
    EXPECT_TRUE(result1.IsEmpty());

    auto result2 = empty1 + list;
    EXPECT_EQ(result2.GetSize(), 3);
    EXPECT_TRUE(result2 == list);

    auto result3 = list + empty1;
    EXPECT_EQ(result3.GetSize(), 3);
    EXPECT_TRUE(result3 == list);
}

TEST(TListTest, RemoveBoundaryCases) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    auto it1 = list.begin();
    list.remove(it1);
    EXPECT_EQ(list.GetSize(), 2);
    EXPECT_EQ(list[0], 2);
    EXPECT_EQ(list[1], 3);

    list = TList<int>();
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);

    auto it2 = list.begin();
    ++it2; ++it2;
    list.remove(it2);
    EXPECT_EQ(list.GetSize(), 2);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
}

TEST(TListTest, AddWithIndex) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(3);
    list.AddEnd(4);

    list.Add(2, 1);

    EXPECT_EQ(list.GetSize(), 4);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);
    EXPECT_EQ(list[3], 4);

    list.Add(0, 0);
    EXPECT_EQ(list[0], 0);

    list.Add(5, 5);
    EXPECT_EQ(list[5], 5);
}

TEST(TListTest, RemoveWithIndex) {
    TList<int> list;
    list.AddEnd(1);
    list.AddEnd(2);
    list.AddEnd(3);
    list.AddEnd(4);
    list.AddEnd(5);

    EXPECT_EQ(list.Remove(2), 3);

    EXPECT_EQ(list.GetSize(), 4);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 4);
    EXPECT_EQ(list[3], 5);

    EXPECT_EQ(list.Remove(0), 1);
    EXPECT_EQ(list[0], 2);

    EXPECT_EQ(list.Remove(2), 5);
    EXPECT_EQ(list.GetSize(), 2);
}
