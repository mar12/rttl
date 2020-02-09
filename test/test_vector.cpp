#include <cassert>
#include <deque>
#include <UnitTest++/UnitTest++.h>
#include "rttl/vector.h"
#include "element.h"
#include "input_iterator.h"

using TestVector = rttl::vector<Element, 32>;

TEST(constructor_1) {
    TestVector v;
    CHECK_EQUAL(true, v.empty());
    CHECK_EQUAL(0u, v.size());
}

TEST(constructor_3) {
    TestVector v(10, 123);
    CHECK_EQUAL(10u, v.size());
    for (std::size_t i = 0; i < 10; ++i) {
        CHECK_EQUAL(123, v[i]);
    }
    CHECK_THROW(TestVector(33, 0), std::length_error);
}

TEST(constructor_4) {
    TestVector v(10);
    CHECK_EQUAL(10u, v.size());
    for (std::size_t i = 0; i < 10; ++i) {
        CHECK_EQUAL(0, v[i]);
    }
    CHECK_THROW(TestVector(33), std::length_error);
}

TEST(constructor_5) {
    std::deque<int> t = { 123, 456, 789, 0 };
    TestVector v(t.cbegin(), t.cend());
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    t.resize(33);
    CHECK_THROW(TestVector(t.cbegin(), t.cend()), std::length_error);
}

TEST(constructor_6a) {
    /// Copy ctor
    std::deque<int> t = { 123, 456, 789, 0 };
    TestVector other(t.cbegin(), t.cend());
    TestVector v(other);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(constructor_6b) {
    /// Copy ctor
    std::deque<int> t = { 123, 456, 789, 0 };
    TestVector other(t.cbegin(), t.cend());
    rttl::vector<Element,16> v(other);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(constructor_7a) {
    /// Move ctor
    std::deque<int> t = { 123, 456, 789, 0 };
    TestVector other(t.cbegin(), t.cend());
    TestVector v(std::move(other));
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(constructor_7b) {
    /// Move ctor
    std::deque<int> t = { 123, 456, 789, 0 };
    TestVector other(t.cbegin(), t.cend());
    rttl::vector<Element,16> v(std::move(other));
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(constructor_8) {
    TestVector v = { 123, 456, 789, 0 };
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    CHECK_THROW(TestVector({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }), std::length_error);
}

TEST(operator_assign_1a) {
    TestVector other = { 123, 456, 789, 0 };
    TestVector v;
    auto& r = v = other;
    CHECK_EQUAL(&v, &r);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(operator_assign_1b) {
    TestVector other = { 123, 456, 789, 0 };
    rttl::vector<Element,16> v;
    auto& r = v = other;
    CHECK_EQUAL(&v, &r);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    other.resize(17);
    CHECK_THROW(v = other, std::length_error);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}


TEST(operator_assign_2a) {
    TestVector other = { 123, 456, 789, 0 };
    TestVector v;
    auto& r = v = std::move(other);
    CHECK_EQUAL(&v, &r);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(operator_assign_2b) {
    TestVector other = { 123, 456, 789, 0 };
    rttl::vector<Element,16> v;
    auto& r = v = std::move(other);
    CHECK_EQUAL(&v, &r);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    other.resize(17);
    CHECK_THROW(v = std::move(other), std::length_error);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(operator_assign_3) {
    TestVector v;
    auto& r = v = { 123, 456, 789, 0 };
    CHECK_EQUAL(&v, &r);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    CHECK_THROW(v.operator=({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }), std::length_error);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);

}

TEST(assign_1) {
    TestVector v;
    v.assign(10, 123);
    CHECK_THROW(v.assign(33, 123), std::length_error);
    CHECK_EQUAL(10u, v.size());
    for (std::size_t i = 0; i < 10; ++i) {
        CHECK_EQUAL(123, v[i]);
    }
}

TEST(assign_2) {
    std::deque<int> t = { 123, 456, 789, 0 };
    TestVector v;
    v.assign(t.cbegin(), t.cend());
    t.resize(33);
    CHECK_THROW(v.assign(t.cbegin(), t.cend()), std::length_error);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(assign_3) {
    TestVector v;
    v.assign({ 123, 456, 789, 0 });
    CHECK_THROW(v.assign({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }), std::length_error);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(at) {
    TestVector v = { 123, 456, 789, 0 };
    CHECK_EQUAL(123, v.at(0));
    CHECK_EQUAL(456, v.at(1));
    CHECK_EQUAL(789, v.at(2));
    CHECK_EQUAL(0, v.at(3));
    CHECK_THROW(v.at(4), std::out_of_range);
    const TestVector& cv = v;
    CHECK_EQUAL(123, cv.at(0));
    CHECK_EQUAL(456, cv.at(1));
    CHECK_EQUAL(789, cv.at(2));
    CHECK_EQUAL(0, cv.at(3));
    CHECK_THROW(cv.at(4), std::out_of_range);
}

TEST(operator_at) {
    TestVector v = { 123, 456, 789, 0 };
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    const TestVector& cv = v;
    CHECK_EQUAL(123, cv[0]);
    CHECK_EQUAL(456, cv[1]);
    CHECK_EQUAL(789, cv[2]);
    CHECK_EQUAL(0, cv[3]);
}

TEST(front) {
    TestVector v = { 123, 456, 789 };
    CHECK_EQUAL(123, v.front());
    const TestVector& cv = v;
    CHECK_EQUAL(123, cv.front());
}

TEST(back) {
    TestVector v = { 123, 456, 789 };
    CHECK_EQUAL(789, v.back());
    const TestVector& cv = v;
    CHECK_EQUAL(789, cv.back());
}

TEST(data) {
    TestVector v = { 123, 456, 789 };
    TestVector::value_type* p = v.data();
    CHECK_EQUAL(123, *p);
    const TestVector& cv = v;
    const TestVector::value_type* cp = cv.data();
    CHECK_EQUAL(123, *cp);
}

TEST(begin) {
    TestVector v = { 123, 456, 789 };
    CHECK_EQUAL(123, *(v.begin()));
    CHECK_EQUAL(123, *(v.cbegin()));
    const TestVector& cv = v;
    CHECK_EQUAL(123, *(cv.begin()));
}

TEST(end) {
    TestVector v = { 123, 456, 789 };
    CHECK_EQUAL(789, *(v.end() - 1));
    CHECK_EQUAL(789, *(v.cend() - 1));
    const TestVector& cv = v;
    CHECK_EQUAL(789, *(cv.end() - 1));
}

TEST(rbegin) {
    TestVector v = { 123, 456, 789 };
    CHECK_EQUAL(789, *(v.rbegin()));
    CHECK_EQUAL(789, *(v.crbegin()));
    const TestVector& cv = v;
    CHECK_EQUAL(789, *(cv.rbegin()));
}

TEST(rend) {
    TestVector v = { 123, 456, 789 };
    CHECK_EQUAL(123, *(v.rend() - 1));
    CHECK_EQUAL(123, *(v.crend() - 1));
    const TestVector& cv = v;
    CHECK_EQUAL(123, *(cv.rend() - 1));
}

TEST(empty) {
    TestVector v;
    CHECK_EQUAL(true, v.empty());
    v.assign({ 123, 456, 789 });
    CHECK_EQUAL(false, v.empty());
    v.clear();
    CHECK_EQUAL(true, v.empty());
}

TEST(size) {
    TestVector v;
    CHECK_EQUAL(0u, v.size());
    v.assign({ 123, 456, 789 });
    CHECK_EQUAL(3u, v.size());
    v.clear();
    CHECK_EQUAL(0u, v.size());
}

TEST(max_size) {
    TestVector v;
    CHECK_EQUAL(32u, v.max_size());
}

TEST(reserve) {
    TestVector v = { 123, 456, 789, 0 };
    v.reserve(32);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    v.reserve(0);
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    CHECK_THROW(v.reserve(33), std::length_error);
}

TEST(capacity) {
    TestVector v;
    CHECK_EQUAL(32u, v.capacity());
}

TEST(shrink_to_fit) {
    TestVector v = { 123, 456, 789, 0 };
    v.shrink_to_fit();
    CHECK_EQUAL(4u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
}

TEST(clear) {
    TestVector v = { 123, 456, 789, 0 };
    v.clear();
    CHECK_EQUAL(0u, v.size());
}

TEST(insert_1) {
    TestVector v = { 123, 456, 789, 0 };
    v.insert(v.cbegin() + 2, 555);
    CHECK_EQUAL(5u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(555, v[2]);
    CHECK_EQUAL(789, v[3]);
    CHECK_EQUAL(0, v[4]);
    v.insert(v.cend(), 69);
    CHECK_EQUAL(6u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(555, v[2]);
    CHECK_EQUAL(789, v[3]);
    CHECK_EQUAL(0, v[4]);
    CHECK_EQUAL(69, v[5]);
    v.insert(v.cbegin(), 765);
    CHECK_EQUAL(7u, v.size());
    CHECK_EQUAL(765, v[0]);
    CHECK_EQUAL(123, v[1]);
    CHECK_EQUAL(456, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(789, v[4]);
    CHECK_EQUAL(0, v[5]);
    CHECK_EQUAL(69, v[6]);
    v = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    CHECK_THROW(v.insert(v.cbegin(), 69), std::length_error);
}

TEST(insert_2) {
    TestVector v = { 123, 456, 789, 0 };
    Element e1(555);
    v.insert(v.cbegin() + 2, std::move(e1));
    CHECK_EQUAL(5u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(555, v[2]);
    CHECK_EQUAL(789, v[3]);
    CHECK_EQUAL(0, v[4]);
    Element e2(69);
    v.insert(v.cend(), std::move(e2));
    CHECK_EQUAL(6u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(555, v[2]);
    CHECK_EQUAL(789, v[3]);
    CHECK_EQUAL(0, v[4]);
    CHECK_EQUAL(69, v[5]);
    Element e3(765);
    v.insert(v.cbegin(), std::move(e3));
    CHECK_EQUAL(7u, v.size());
    CHECK_EQUAL(765, v[0]);
    CHECK_EQUAL(123, v[1]);
    CHECK_EQUAL(456, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(789, v[4]);
    CHECK_EQUAL(0, v[5]);
    CHECK_EQUAL(69, v[6]);
    Element e4(0);
    v = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    CHECK_THROW(v.insert(v.cbegin(), std::move(e4)), std::length_error);
}

TEST(insert_3) {
    TestVector v = { 123, 456, 789, 0 };
    v.insert(v.cbegin() + 2, 3, 555);
    CHECK_EQUAL(7u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(555, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(555, v[4]);
    CHECK_EQUAL(789, v[5]);
    CHECK_EQUAL(0, v[6]);
    v.insert(v.cend(), 1, 69);
    CHECK_EQUAL(8u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(555, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(555, v[4]);
    CHECK_EQUAL(789, v[5]);
    CHECK_EQUAL(0, v[6]);
    CHECK_EQUAL(69, v[7]);
    v.insert(v.cbegin(), 2, 765);
    CHECK_THROW(v.insert(v.cbegin(), 23, 0), std::length_error);
    CHECK_EQUAL(10u, v.size());
    CHECK_EQUAL(765, v[0]);
    CHECK_EQUAL(765, v[1]);
    CHECK_EQUAL(123, v[2]);
    CHECK_EQUAL(456, v[3]);
    CHECK_EQUAL(555, v[4]);
    CHECK_EQUAL(555, v[5]);
    CHECK_EQUAL(555, v[6]);
    CHECK_EQUAL(789, v[7]);
    CHECK_EQUAL(0, v[8]);
    CHECK_EQUAL(69, v[9]);
}

TEST(insert_4a) {
    TestVector v = { 123, 456, 789, 0 };
    std::deque d = { 765, 555, 69 };
    v.insert(v.cbegin() + 2, d.cbegin(), d.cend());
    CHECK_EQUAL(7u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(765, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(69, v[4]);
    CHECK_EQUAL(789, v[5]);
    CHECK_EQUAL(0, v[6]);
    v.insert(v.cend(), d.cbegin(), d.cend());
    CHECK_EQUAL(10u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(765, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(69, v[4]);
    CHECK_EQUAL(789, v[5]);
    CHECK_EQUAL(0, v[6]);
    CHECK_EQUAL(765, v[7]);
    CHECK_EQUAL(555, v[8]);
    CHECK_EQUAL(69, v[9]);
    v.insert(v.cbegin(), d.cbegin(), d.cend());
    d.resize(20);
    CHECK_THROW(v.insert(v.cbegin(), d.cbegin(), d.cend()), std::length_error);
    CHECK_EQUAL(13u, v.size());
    CHECK_EQUAL(765, v[0]);
    CHECK_EQUAL(555, v[1]);
    CHECK_EQUAL(69, v[2]);
    CHECK_EQUAL(123, v[3]);
    CHECK_EQUAL(456, v[4]);
    CHECK_EQUAL(765, v[5]);
    CHECK_EQUAL(555, v[6]);
    CHECK_EQUAL(69, v[7]);
    CHECK_EQUAL(789, v[8]);
    CHECK_EQUAL(0, v[9]);
    CHECK_EQUAL(765, v[10]);
    CHECK_EQUAL(555, v[11]);
    CHECK_EQUAL(69, v[12]);
}

TEST(insert_4b) {
    /// Test with an InputIterator that is not a ForwardIterator
    TestVector v = { 123, 456, 789, 0 };
    std::deque d = { 765, 555, 69 };
    v.insert(v.cbegin() + 2, InputIterator(d.cbegin()), InputIterator(d.cend()));
    CHECK_EQUAL(7u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(765, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(69, v[4]);
    CHECK_EQUAL(789, v[5]);
    CHECK_EQUAL(0, v[6]);
    v.insert(v.cend(), InputIterator(d.cbegin()), InputIterator(d.cend()));
    CHECK_EQUAL(10u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(765, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(69, v[4]);
    CHECK_EQUAL(789, v[5]);
    CHECK_EQUAL(0, v[6]);
    CHECK_EQUAL(765, v[7]);
    CHECK_EQUAL(555, v[8]);
    CHECK_EQUAL(69, v[9]);
    v.insert(v.cbegin(), InputIterator(d.cbegin()), InputIterator(d.cend()));
    d.resize(20);
    CHECK_THROW(v.insert(v.cbegin(), InputIterator(d.cbegin()), InputIterator(d.cend())), std::length_error);
    CHECK_EQUAL(13u, v.size());
    CHECK_EQUAL(765, v[0]);
    CHECK_EQUAL(555, v[1]);
    CHECK_EQUAL(69, v[2]);
    CHECK_EQUAL(123, v[3]);
    CHECK_EQUAL(456, v[4]);
    CHECK_EQUAL(765, v[5]);
    CHECK_EQUAL(555, v[6]);
    CHECK_EQUAL(69, v[7]);
    CHECK_EQUAL(789, v[8]);
    CHECK_EQUAL(0, v[9]);
    CHECK_EQUAL(765, v[10]);
    CHECK_EQUAL(555, v[11]);
    CHECK_EQUAL(69, v[12]);
}


TEST(insert_5) {
    TestVector v = { 123, 456, 789, 0 };
    v.insert(v.cbegin() + 2, { 765, 555, 69 });
    CHECK_EQUAL(7u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(765, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(69, v[4]);
    CHECK_EQUAL(789, v[5]);
    CHECK_EQUAL(0, v[6]);
    v.insert(v.cend(), { 765, 555, 69 });
    CHECK_EQUAL(10u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(765, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(69, v[4]);
    CHECK_EQUAL(789, v[5]);
    CHECK_EQUAL(0, v[6]);
    CHECK_EQUAL(765, v[7]);
    CHECK_EQUAL(555, v[8]);
    CHECK_EQUAL(69, v[9]);
    v.insert(v.cbegin(), { 765, 555, 69 });
    CHECK_EQUAL(13u, v.size());
    CHECK_EQUAL(765, v[0]);
    CHECK_EQUAL(555, v[1]);
    CHECK_EQUAL(69, v[2]);
    CHECK_EQUAL(123, v[3]);
    CHECK_EQUAL(456, v[4]);
    CHECK_EQUAL(765, v[5]);
    CHECK_EQUAL(555, v[6]);
    CHECK_EQUAL(69, v[7]);
    CHECK_EQUAL(789, v[8]);
    CHECK_EQUAL(0, v[9]);
    CHECK_EQUAL(765, v[10]);
    CHECK_EQUAL(555, v[11]);
    CHECK_EQUAL(69, v[12]);
}

TEST(emplace) {
    TestVector v = { 123, 456, 789, 0 };
    v.emplace(v.cbegin() + 2, 555);
    CHECK_EQUAL(5u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(555, v[2]);
    CHECK_EQUAL(789, v[3]);
    CHECK_EQUAL(0, v[4]);
    v.emplace(v.cend(), 69);
    CHECK_EQUAL(6u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(555, v[2]);
    CHECK_EQUAL(789, v[3]);
    CHECK_EQUAL(0, v[4]);
    CHECK_EQUAL(69, v[5]);
    v.emplace(v.cbegin(), 765);
    CHECK_EQUAL(7u, v.size());
    CHECK_EQUAL(765, v[0]);
    CHECK_EQUAL(123, v[1]);
    CHECK_EQUAL(456, v[2]);
    CHECK_EQUAL(555, v[3]);
    CHECK_EQUAL(789, v[4]);
    CHECK_EQUAL(0, v[5]);
    CHECK_EQUAL(69, v[6]);
    v = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    CHECK_THROW(v.emplace(v.cbegin(), 765), std::length_error);
}

TEST(erase_1) {
    TestVector v = { 123, 456, 789, 0 };
    v.erase(v.cbegin() + 2);
    CHECK_EQUAL(3u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(0, v[2]);
    v.erase(v.cbegin());
    CHECK_EQUAL(2u, v.size());
    CHECK_EQUAL(456, v[0]);
    CHECK_EQUAL(0, v[1]);
    v.erase(v.cend() - 1);
    CHECK_EQUAL(1u, v.size());
    CHECK_EQUAL(456, v[0]);
}

TEST(erase_2) {
    TestVector v = { 123, 456, 789, 0 };
    v.erase(v.cbegin() + 2, v.cend() - 1);
    CHECK_EQUAL(3u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(0, v[2]);
    v.erase(v.cbegin(), v.cbegin() + 2);
    CHECK_EQUAL(1u, v.size());
    CHECK_EQUAL(0, v[0]);
    v.erase(v.cbegin(), v.cend());
    CHECK_EQUAL(0u, v.size());
}



TEST(push_back) {
    TestVector v = { 123, 456, 789, 0 };
    v.push_back(69);
    CHECK_EQUAL(5u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    CHECK_EQUAL(69, v[4]);
    Element e(765);
    v.push_back(std::move(e));
    CHECK_EQUAL(6u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    CHECK_EQUAL(69, v[4]);
    CHECK_EQUAL(765, v[5]);
}

TEST(emplace_back) {
    TestVector v = { 123, 456, 789, 0 };
    auto& r = v.emplace_back(69);
    CHECK_EQUAL(&v[4], &r);
    CHECK_EQUAL(5u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    CHECK_EQUAL(69, v[4]);
}

TEST(pop_back) {
    TestVector v = { 123, 456, 789, 0 };
    v.pop_back();
    CHECK_EQUAL(3u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    v.clear();
    CHECK_THROW(v.pop_back(), std::exception);
}

TEST(resize_1) {
    TestVector v = { 123, 456, 789, 0 };
    CHECK_THROW(v.resize(33), std::length_error);
    v.resize(5);
    CHECK_EQUAL(5u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    CHECK_EQUAL(0, v[4]);
    v.resize(2);
    CHECK_EQUAL(2u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
}

TEST(resize_2) {
    TestVector v = { 123, 456, 789, 0 };
    CHECK_THROW(v.resize(69, 33), std::length_error);
    v.resize(5, 69);
    CHECK_EQUAL(5u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
    CHECK_EQUAL(789, v[2]);
    CHECK_EQUAL(0, v[3]);
    CHECK_EQUAL(69, v[4]);
    v.resize(2, 69);
    CHECK_EQUAL(2u, v.size());
    CHECK_EQUAL(123, v[0]);
    CHECK_EQUAL(456, v[1]);
}

TEST(swap_1a) {
    TestVector v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    v1.swap(v2);
    CHECK_EQUAL(4u, v2.size());
    CHECK_EQUAL(123, v2[0]);
    CHECK_EQUAL(456, v2[1]);
    CHECK_EQUAL(789, v2[2]);
    CHECK_EQUAL(0, v2[3]);
    CHECK_EQUAL(3u, v1.size());
    CHECK_EQUAL(765, v1[0]);
    CHECK_EQUAL(555, v1[1]);
    CHECK_EQUAL(69, v1[2]);
    v1.swap(v2);
    CHECK_EQUAL(4u, v1.size());
    CHECK_EQUAL(123, v1[0]);
    CHECK_EQUAL(456, v1[1]);
    CHECK_EQUAL(789, v1[2]);
    CHECK_EQUAL(0, v1[3]);
    CHECK_EQUAL(3u, v2.size());
    CHECK_EQUAL(765, v2[0]);
    CHECK_EQUAL(555, v2[1]);
    CHECK_EQUAL(69, v2[2]);
}

TEST(swap_1b) {
    TestVector v1 = { 123, 456, 789, 0 };
    rttl::vector<Element,4> v2 = { 765, 555, 69 };
    v1.swap(v2);
    CHECK_EQUAL(4u, v2.size());
    CHECK_EQUAL(123, v2[0]);
    CHECK_EQUAL(456, v2[1]);
    CHECK_EQUAL(789, v2[2]);
    CHECK_EQUAL(0, v2[3]);
    CHECK_EQUAL(3u, v1.size());
    CHECK_EQUAL(765, v1[0]);
    CHECK_EQUAL(555, v1[1]);
    CHECK_EQUAL(69, v1[2]);
    v1.swap(v2);
    CHECK_EQUAL(4u, v1.size());
    CHECK_EQUAL(123, v1[0]);
    CHECK_EQUAL(456, v1[1]);
    CHECK_EQUAL(789, v1[2]);
    CHECK_EQUAL(0, v1[3]);
    CHECK_EQUAL(3u, v2.size());
    CHECK_EQUAL(765, v2[0]);
    CHECK_EQUAL(555, v2[1]);
    CHECK_EQUAL(69, v2[2]);
    v1.push_back(111);
    CHECK_THROW(v1.swap(v2), std::length_error);
    CHECK_THROW(v2.swap(v1), std::length_error);
}


TEST(swap_2) {
    TestVector v1 = { 123, 456, 789, 0 };
    std::vector<Element> v2 = { 765, 555, 69 };
    v1.swap(v2);
    CHECK_EQUAL(4u, v2.size());
    CHECK_EQUAL(123, v2[0]);
    CHECK_EQUAL(456, v2[1]);
    CHECK_EQUAL(789, v2[2]);
    CHECK_EQUAL(0, v2[3]);
    CHECK_EQUAL(3u, v1.size());
    CHECK_EQUAL(765, v1[0]);
    CHECK_EQUAL(555, v1[1]);
    CHECK_EQUAL(69, v1[2]);
    v1.swap(v2);
    CHECK_EQUAL(4u, v1.size());
    CHECK_EQUAL(123, v1[0]);
    CHECK_EQUAL(456, v1[1]);
    CHECK_EQUAL(789, v1[2]);
    CHECK_EQUAL(0, v1[3]);
    CHECK_EQUAL(3u, v2.size());
    CHECK_EQUAL(765, v2[0]);
    CHECK_EQUAL(555, v2[1]);
    CHECK_EQUAL(69, v2[2]);
    v2.resize(33);
    CHECK_THROW(v1.swap(v2), std::length_error);
}

TEST(operator_equal_1) {
    TestVector v1 = { 123, 456, 789 };
    TestVector v2 = { 765, 555, 69 };
    bool r1 = (v1 == v2);
    CHECK_EQUAL(false, r1);
    TestVector v3 = { 123, 456, 789 };
    bool r2 = (v1 == v3);
    CHECK_EQUAL(true, r2);
    v3.push_back(0);
    bool r3 = (v1 == v3);
    CHECK_EQUAL(false, r3);
}

TEST(operator_equal_2) {
    TestVector v1 = { 123, 456, 789 };
    std::vector<Element> v2 = { 765, 555, 69 };
    bool r1 = (v1 == v2);
    CHECK_EQUAL(false, r1);
    TestVector v3 = { 123, 456, 789 };
    bool r2 = (v1 == v3);
    CHECK_EQUAL(true, r2);
    v3.push_back(0);
    bool r3 = (v1 == v3);
    CHECK_EQUAL(false, r3);
}

TEST(operator_equal_3) {
    std::vector<Element> v1 = { 123, 456, 789 };
    TestVector v2 = { 765, 555, 69 };
    bool r1 = (v1 == v2);
    CHECK_EQUAL(false, r1);
    TestVector v3 = { 123, 456, 789 };
    bool r2 = (v1 == v3);
    CHECK_EQUAL(true, r2);
    v3.push_back(0);
    bool r3 = (v1 == v3);
    CHECK_EQUAL(false, r3);
}

TEST(operator_not_equal_1) {
    TestVector v1 = { 123, 456, 789 };
    TestVector v2 = { 765, 555, 69 };
    bool r1 = (v1 != v2);
    CHECK_EQUAL(true, r1);
    TestVector v3 = { 123, 456, 789 };
    bool r2 = (v1 != v3);
    CHECK_EQUAL(false, r2);
    v3.push_back(0);
    bool r3 = (v1 != v3);
    CHECK_EQUAL(true, r3);
}

TEST(operator_not_equal_2) {
    TestVector v1 = { 123, 456, 789 };
    std::vector<Element> v2 = { 765, 555, 69 };
    bool r1 = (v1 != v2);
    CHECK_EQUAL(true, r1);
    TestVector v3 = { 123, 456, 789 };
    bool r2 = (v1 != v3);
    CHECK_EQUAL(false, r2);
    v3.push_back(0);
    bool r3 = (v1 != v3);
    CHECK_EQUAL(true, r3);
}

TEST(operator_not_equal_3) {
    std::vector<Element> v1 = { 123, 456, 789 };
    TestVector v2 = { 765, 555, 69 };
    bool r1 = (v1 != v2);
    CHECK_EQUAL(true, r1);
    TestVector v3 = { 123, 456, 789 };
    bool r2 = (v1 != v3);
    CHECK_EQUAL(false, r2);
    v3.push_back(0);
    bool r3 = (v1 != v3);
    CHECK_EQUAL(true, r3);
}


TEST(operator_less_1) {
    TestVector v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    TestVector v3 = { 111, 222, 333, 444, 555 };
    TestVector v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 < v2);
    CHECK_EQUAL(true, r1);
    bool r2 = (v1 < v3);
    CHECK_EQUAL(false, r2);
    bool r3 = (v1 < v4);
    CHECK_EQUAL(false, r3);
}

TEST(operator_less_2) {
    TestVector v1 = { 123, 456, 789, 0 };
    std::vector<Element> v2 = { 765, 555, 69 };
    std::vector<Element> v3 = { 111, 222, 333, 444, 555 };
    std::vector<Element> v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 < v2);
    CHECK_EQUAL(true, r1);
    bool r2 = (v1 < v3);
    CHECK_EQUAL(false, r2);
    bool r3 = (v1 < v4);
    CHECK_EQUAL(false, r3);
}

TEST(operator_less_3) {
    std::vector<Element> v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    TestVector v3 = { 111, 222, 333, 444, 555 };
    TestVector v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 < v2);
    CHECK_EQUAL(true, r1);
    bool r2 = (v1 < v3);
    CHECK_EQUAL(false, r2);
    bool r3 = (v1 < v4);
    CHECK_EQUAL(false, r3);
}


TEST(operator_more_1) {
    TestVector v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    TestVector v3 = { 111, 222, 333, 444, 555 };
    TestVector v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 > v2);
    CHECK_EQUAL(false, r1);
    bool r2 = (v1 > v3);
    CHECK_EQUAL(true, r2);
    bool r3 = (v1 > v4);
    CHECK_EQUAL(false, r3);
}


TEST(operator_more_2) {
    TestVector v1 = { 123, 456, 789, 0 };
    std::vector<Element> v2 = { 765, 555, 69 };
    std::vector<Element> v3 = { 111, 222, 333, 444, 555 };
    std::vector<Element> v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 > v2);
    CHECK_EQUAL(false, r1);
    bool r2 = (v1 > v3);
    CHECK_EQUAL(true, r2);
    bool r3 = (v1 > v4);
    CHECK_EQUAL(false, r3);
}


TEST(operator_more_3) {
    std::vector<Element> v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    TestVector v3 = { 111, 222, 333, 444, 555 };
    TestVector v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 > v2);
    CHECK_EQUAL(false, r1);
    bool r2 = (v1 > v3);
    CHECK_EQUAL(true, r2);
    bool r3 = (v1 > v4);
    CHECK_EQUAL(false, r3);
}

TEST(operator_less_equal_1) {
    TestVector v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    TestVector v3 = { 111, 222, 333, 444, 555 };
    TestVector v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 <= v2);
    CHECK_EQUAL(true, r1);
    bool r2 = (v1 <= v3);
    CHECK_EQUAL(false, r2);
    bool r3 = (v1 <= v4);
    CHECK_EQUAL(true, r3);
}

TEST(operator_less_equal_2) {
    TestVector v1 = { 123, 456, 789, 0 };
    std::vector<Element> v2 = { 765, 555, 69 };
    std::vector<Element> v3 = { 111, 222, 333, 444, 555 };
    std::vector<Element> v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 <= v2);
    CHECK_EQUAL(true, r1);
    bool r2 = (v1 <= v3);
    CHECK_EQUAL(false, r2);
    bool r3 = (v1 <= v4);
    CHECK_EQUAL(true, r3);
}

TEST(operator_less_equal_3) {
    std::vector<Element> v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    TestVector v3 = { 111, 222, 333, 444, 555 };
    TestVector v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 <= v2);
    CHECK_EQUAL(true, r1);
    bool r2 = (v1 <= v3);
    CHECK_EQUAL(false, r2);
    bool r3 = (v1 <= v4);
    CHECK_EQUAL(true, r3);
}


TEST(operator_more_equal_1) {
    TestVector v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    TestVector v3 = { 111, 222, 333, 444, 555 };
    TestVector v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 >= v2);
    CHECK_EQUAL(false, r1);
    bool r2 = (v1 >= v3);
    CHECK_EQUAL(true, r2);
    bool r3 = (v1 >= v4);
    CHECK_EQUAL(true, r3);
}

TEST(operator_more_equal_2) {
    TestVector v1 = { 123, 456, 789, 0 };
    std::vector<Element> v2 = { 765, 555, 69 };
    std::vector<Element> v3 = { 111, 222, 333, 444, 555 };
    std::vector<Element> v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 >= v2);
    CHECK_EQUAL(false, r1);
    bool r2 = (v1 >= v3);
    CHECK_EQUAL(true, r2);
    bool r3 = (v1 >= v4);
    CHECK_EQUAL(true, r3);
}

TEST(operator_more_equal_3) {
    std::vector<Element> v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    TestVector v3 = { 111, 222, 333, 444, 555 };
    TestVector v4 = { 123, 456, 789, 0 };
    bool r1 = (v1 >= v2);
    CHECK_EQUAL(false, r1);
    bool r2 = (v1 >= v3);
    CHECK_EQUAL(true, r2);
    bool r3 = (v1 >= v4);
    CHECK_EQUAL(true, r3);
}


TEST(adl_swap_1) {
    TestVector v1 = { 123, 456, 789, 0 };
    TestVector v2 = { 765, 555, 69 };
    swap(v1, v2);
    CHECK_EQUAL(4u, v2.size());
    CHECK_EQUAL(123, v2[0]);
    CHECK_EQUAL(456, v2[1]);
    CHECK_EQUAL(789, v2[2]);
    CHECK_EQUAL(0, v2[3]);
    CHECK_EQUAL(3u, v1.size());
    CHECK_EQUAL(765, v1[0]);
    CHECK_EQUAL(555, v1[1]);
    CHECK_EQUAL(69, v1[2]);
}

TEST(adl_swap_2) {
    TestVector v1 = { 123, 456, 789, 0 };
    std::vector<Element> v2 = { 765, 555, 69 };
    swap(v1, v2);
    CHECK_EQUAL(4u, v2.size());
    CHECK_EQUAL(123, v2[0]);
    CHECK_EQUAL(456, v2[1]);
    CHECK_EQUAL(789, v2[2]);
    CHECK_EQUAL(0, v2[3]);
    CHECK_EQUAL(3u, v1.size());
    CHECK_EQUAL(765, v1[0]);
    CHECK_EQUAL(555, v1[1]);
    CHECK_EQUAL(69, v1[2]);
}

TEST(adl_swap_3) {
    TestVector v1 = { 123, 456, 789, 0 };
    std::vector<Element> v2 = { 765, 555, 69 };
    swap(v2, v1);
    CHECK_EQUAL(4u, v2.size());
    CHECK_EQUAL(123, v2[0]);
    CHECK_EQUAL(456, v2[1]);
    CHECK_EQUAL(789, v2[2]);
    CHECK_EQUAL(0, v2[3]);
    CHECK_EQUAL(3u, v1.size());
    CHECK_EQUAL(765, v1[0]);
    CHECK_EQUAL(555, v1[1]);
    CHECK_EQUAL(69, v1[2]);
}


int main(int, const char* []) {
    int r = UnitTest::RunAllTests();
    assert(s_elems_ctored.size() == 0); /// Check memory leaks
    return r;
}
