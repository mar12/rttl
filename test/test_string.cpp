#include <cstring>
#include <string>
#include <vector>
#include <UnitTest++/UnitTest++.h>
#include "rttl/string.h"


TEST(constructor_1) { /// Default constructor
	rttl::string<32> s;
    CHECK_EQUAL(strcmp("", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 0u);
}

TEST(constructor_2) {
	rttl::string<32> s(5, 'a');
	CHECK_EQUAL(std::strcmp("aaaaa", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 5u);
}

TEST(constructor_3) {
	std::string_view sv("Hello, World!");
	rttl::string<32> s1(sv, 7);
	CHECK_EQUAL(std::strcmp("World!", s1.c_str()), 0);
    CHECK_EQUAL(s1.length(), 6u);
	rttl::string<32> s2(sv, 3, 7);
	CHECK_EQUAL(std::strcmp("lo, Wor", s2.c_str()), 0);
    CHECK_EQUAL(s2.length(), 7u);
}

TEST(constructor_4) {
	rttl::string<32> s("Hello, World!", 9);
	CHECK_EQUAL(std::strcmp("Hello, Wo", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 9u);
}

TEST(constructor_5) {
	rttl::string<32> s("Hello, World!");
	CHECK_EQUAL(std::strcmp("Hello, World!", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(constructor_6) {
	std::array<char, 5> a = { 'H', 'e', 'l', 'l', 'o' };
	rttl::string<32> s(a.cbegin(), a.cend());
	CHECK_EQUAL(std::strcmp("Hello", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 5u);
}

TEST(constructor_7) { /// Copy constructor
	std::string s("Hello, World!");
	rttl::string<32> s1(s);
	CHECK_EQUAL(std::strcmp("Hello, World!", s1.c_str()), 0);
    CHECK_EQUAL(s1.length(), 13u);
	rttl::string<32> s2(s1);
	CHECK_EQUAL(std::strcmp("Hello, World!", s2.c_str()), 0);
    CHECK_EQUAL(s2.length(), 13u);
	rttl::string<16> s3(s2);
	CHECK_EQUAL(std::strcmp("Hello, World!", s3.c_str()), 0);
    CHECK_EQUAL(s3.length(), 13u);
}

TEST(constructor_8) { /// Move ctor
	std::string s("Hello, World!");
	rttl::string<32> s1(std::move(s));
	CHECK_EQUAL(std::strcmp("Hello, World!", s1.c_str()), 0);
    CHECK_EQUAL(s1.length(), 13u);
	rttl::string<32> s2(std::move(s1));
	CHECK_EQUAL(std::strcmp("Hello, World!", s2.c_str()), 0);
    CHECK_EQUAL(s2.length(), 13u);
	rttl::string<16> s3(std::move(s2));
	CHECK_EQUAL(std::strcmp("Hello, World!", s3.c_str()), 0);
    CHECK_EQUAL(s3.length(), 13u);
}

TEST(constructor_9) {
	rttl::string<32> s({ 'H','e','l','l','o' });
	CHECK_EQUAL(std::strcmp("Hello", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 5u);
}

TEST(operator_assign_1) {
	std::string s("Hello, World!");
	rttl::string<32> s1;
	auto& r1 = s1 = s;
	CHECK_EQUAL(&r1, &s1);
	CHECK_EQUAL(std::strcmp("Hello, World!", s1.c_str()), 0);
    CHECK_EQUAL(s1.length(), 13u);
	rttl::string<32> s2;
	auto& r2 = s2 = s1;
	CHECK_EQUAL(&r2, &s2);
	CHECK_EQUAL(std::strcmp("Hello, World!", s2.c_str()), 0);
    CHECK_EQUAL(s2.length(), 13u);
	rttl::string<16> s3;
	auto& r3 = s3 = s2;
	CHECK_EQUAL(&r3, &s3);
	CHECK_EQUAL(std::strcmp("Hello, World!", s3.c_str()), 0);
    CHECK_EQUAL(s3.length(), 13u);
}

TEST(operator_assign_2) {
	std::string s("Hello, World!");
	rttl::string<32> s1;
	auto& r1 = s1 = std::move(s);
	CHECK_EQUAL(&r1, &s1);
	CHECK_EQUAL(std::strcmp("Hello, World!", s1.c_str()), 0);
    CHECK_EQUAL(s1.length(), 13u);
	rttl::string<32> s2;
	auto& r2 = s2 = std::move(s1);
	CHECK_EQUAL(&r2, &s2);
	CHECK_EQUAL(std::strcmp("Hello, World!", s2.c_str()), 0);
    CHECK_EQUAL(s2.length(), 13u);
	rttl::string<16> s3;
	auto& r3 = s3 = std::move(s2);
	CHECK_EQUAL(&r3, &s3);
	CHECK_EQUAL(std::strcmp("Hello, World!", s3.c_str()), 0);
    CHECK_EQUAL(s3.length(), 13u);
}

TEST(operator_assign_3) {
	rttl::string<32> s;
	auto& r = s = "Hello, World!";
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp("Hello, World!", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(operator_assign_4) {
	rttl::string<32> s;
	auto& r = s = 'Z';
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp("Z", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 1u);
}

TEST(operator_assign_5) {
	rttl::string<32> s;
	auto& r = s = { 'H','e','l','l','o' };
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp("Hello", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 5u);
}

TEST(assign_1) {
	rttl::string<32> s;
	auto& r = s.assign(7, 'Z');
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp("ZZZZZZZ", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 7u);
	CHECK_THROW(s.assign(33, 'A'), std::length_error);
}

TEST(assign_2) {
	std::string s("Hello, World!");
	rttl::string<32> s1;
	auto& r1 = s1.assign(s);
	CHECK_EQUAL(&r1, &s1);
	CHECK_EQUAL(std::strcmp("Hello, World!", s1.c_str()), 0);
    CHECK_EQUAL(s1.length(), 13u);
	rttl::string<32> s2;
	auto& r2 = s2.assign(s1);
	CHECK_EQUAL(&r2, &s2);
	CHECK_EQUAL(std::strcmp("Hello, World!", s2.c_str()), 0);
    CHECK_EQUAL(s2.length(), 13u);
	rttl::string<16> s3;
	auto& r3 = s3.assign(s2);
	CHECK_EQUAL(&r3, &s3);
	CHECK_EQUAL(std::strcmp("Hello, World!", s3.c_str()), 0);
    CHECK_EQUAL(s3.length(), 13u);
	rttl::string<12> s4;
	CHECK_THROW(s4.assign(s), std::length_error);
}

TEST(assign_3) {
	std::string s("Hello, World!");
	rttl::string<32> s1;
    auto& r1 = s1.assign(s, 1, 11u);
	CHECK_EQUAL(&r1, &s1);
	CHECK_EQUAL(std::strcmp("ello, World", s1.c_str()), 0);
    CHECK_EQUAL(s1.length(), 11u);
	rttl::string<32> s2;
	auto& r2 = s2.assign(s1, 3, 6);
	CHECK_EQUAL(&r2, &s2);
	CHECK_EQUAL(std::strcmp("o, Wor", s2.c_str()), 0);
    CHECK_EQUAL(s2.length(), 6u);
	rttl::string<16> s3;
	auto& r3 = s3.assign(s2, 3, 2);
	CHECK_EQUAL(&r3, &s3);
	CHECK_EQUAL(std::strcmp("Wo", s3.c_str()), 0);
    CHECK_EQUAL(s3.length(), 2u);

}

TEST(assign_4) {
	std::string s("Hello, World!");
	rttl::string<32> s1;
	auto& r1 = s1.assign(std::move(s));
	CHECK_EQUAL(&r1, &s1);
	CHECK_EQUAL(std::strcmp("Hello, World!", s1.c_str()), 0);
    CHECK_EQUAL(s1.length(), 13u);
	rttl::string<32> s2;
	auto& r2 = s2.assign(std::move(s1));
	CHECK_EQUAL(&r2, &s2);
	CHECK_EQUAL(std::strcmp("Hello, World!", s2.c_str()), 0);
    CHECK_EQUAL(s2.length(), 13u);
	rttl::string<16> s3;
	auto& r3 = s3.assign(std::move(s2));
	CHECK_EQUAL(&r3, &s3);
	CHECK_EQUAL(std::strcmp("Hello, World!", s3.c_str()), 0);
    CHECK_EQUAL(s3.length(), 13u);
}

TEST(assign_5) {
	rttl::string<32> s;
	auto& r = s.assign("Hello, World!", 5);
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp("Hello", s.c_str()), 0);
}

TEST(assign_6) {
	rttl::string<32> s;
	auto& r = s.assign("Hello, World!");
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp("Hello, World!", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(assign_7) {
	std::array<char, 5> a = { 'H', 'e', 'l', 'l', 'o' };
	rttl::string<32> s;
	auto& r = s.assign(a.cbegin(), a.cend());
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp("Hello", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 5u);
	rttl::string<4> s1;
	CHECK_THROW(s1.assign(a.cbegin(), a.cend()), std::length_error);
}

TEST(assign_8) {
	rttl::string<32> s;
	auto& r = s.assign({ 'H','e','l','l','o' });
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp("Hello", s.c_str()), 0);
    CHECK_EQUAL(s.length(), 5u);
}

TEST(at) {
	rttl::string<32> s("Hello, World!");
	CHECK_EQUAL(s.at(0), 'H');
	CHECK_EQUAL(s.at(12), '!');
	CHECK_EQUAL(s.at(13), '\0');
	const auto& s1 = s;
	CHECK_EQUAL(s1.at(0), 'H');
	CHECK_EQUAL(s1.at(12), '!');
	CHECK_EQUAL(s1.at(13), '\0');
}

TEST(operator_at) {
	rttl::string<32> s("Hello, World!");
	const auto& sc = s;
	CHECK_EQUAL(sc[0], 'H');
	CHECK_EQUAL(sc[12], '!');
	CHECK_EQUAL(sc[13], '\0');
	s[0] = 'A';
	s[12] = '?';
	CHECK_EQUAL(std::strcmp(s.c_str(), "Aello, World?"), 0);
}

TEST(front) {
	rttl::string<32> s("Hello, World!");
	const auto& sc = s;
	CHECK_EQUAL(sc.front(), 'H');
	s.front() = 'Z';
	CHECK_EQUAL(std::strcmp(s.c_str(), "Zello, World!"), 0);
}

TEST(back) {
	rttl::string<32> s("Hello, World!");
	const auto& sc = s;
	CHECK_EQUAL(sc.back(), '!');
	s.back() = 'y';
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Worldy"), 0);
}

TEST(data) {
	rttl::string<32> s("Hello, World!");
	CHECK_EQUAL(std::strcmp(s.data(), "Hello, World!"), 0);
	const auto& sc = s;
	CHECK_EQUAL(std::strcmp(sc.data(), "Hello, World!"), 0);
}

TEST(c_str) {
	rttl::string<32> s("Hello, World!");
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!"), 0);
}

TEST(operator_string_view) {
	rttl::string<32> s("Hello, World!");
	std::string_view sv = s;
	CHECK_EQUAL(sv, "Hello, World!");
    CHECK_EQUAL(sv.length(), 13u);
}

TEST(iterator) {
	rttl::string<32> s("Hello, World!");
	std::fill(s.begin() + 2, s.end() - 1, 'z');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hezzzzzzzzzz!"), 0);
}

TEST(const_iterator) {
	rttl::string<32> s("Hello, World!");
	std::vector<char> v(s.cbegin(), s.cend());
	CHECK_EQUAL(std::strncmp(v.data(), "Hello, World!", 13), 0);
}

TEST(reverse_iterator) {
	rttl::string<32> s("Hello, World!");
	std::fill(s.rbegin() + 8, s.rend() - 3, 'z');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Helzz, World!"), 0);
}

TEST(reverse_const_iterator) {
	rttl::string<32> s("Hello, World!");
	std::vector<char> v(s.crbegin(), s.crend());
	CHECK_EQUAL(std::strncmp(v.data(), "!dlroW ,olleH", 13), 0);
}

TEST(empty) {
	rttl::string<32> s;
	CHECK_EQUAL(s.empty(), true);
	s = "H";
	CHECK_EQUAL(s.empty(), false);
	s = "\0";
	CHECK_EQUAL(s.empty(), true);
}

TEST(size_length) {
	rttl::string<32> s;
    CHECK_EQUAL(s.size(), 0u);
    CHECK_EQUAL(s.length(), 0u);
	s = "A";
    CHECK_EQUAL(s.size(), 1u);
    CHECK_EQUAL(s.length(), 1u);
	s = "Hello";
    CHECK_EQUAL(s.length(), 5u);
    CHECK_EQUAL(s.size(), 5u);
}

TEST(max_size) {
	rttl::string<32> s;
    CHECK_EQUAL(s.max_size(), 32u);
}

TEST(reserve) {
	rttl::string<32> s;
	s.reserve(0);
	s.reserve(32);
	CHECK_THROW(s.reserve(33), std::length_error);
}

TEST(capacity) {
	rttl::string<32> s;
    CHECK_EQUAL(s.capacity(), 32u);
}

TEST(shink_to_fit) {
	rttl::string<32> s;
	s.shrink_to_fit();
}

TEST(clear) {
	rttl::string<32> s("Hello, World!");
	s.clear();
	CHECK_EQUAL(std::strcmp(s.c_str(), ""), 0);
    CHECK_EQUAL(s.length(), 0u);
}

TEST(insert_1) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.insert(5, 3, 'w');
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hellowww, World!"), 0);
    CHECK_EQUAL(s.length(), 16u);
}

TEST(insert_2) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.insert(7, "My Little ");
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, My Little World!"), 0);
    CHECK_EQUAL(s.length(), 23u);
}

TEST(insert_3) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.insert(7, "My Little ", 3);
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, My World!"), 0);
    CHECK_EQUAL(s.length(), 16u);
}

TEST(insert_4) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1("My ");
	std::string s2("Little ");
	auto& r1 = s.insert(7, s1);
	auto& r2 = s.insert(10, s2);
	CHECK_EQUAL(&r1, &s);
	CHECK_EQUAL(&r2, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, My Little World!"), 0);
    CHECK_EQUAL(s.length(), 23u);
}

TEST(insert_5) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1("My ", 2);
	std::string s2("Little ", 3);
	auto& r1 = s.insert(7, s1);
	auto& r2 = s.insert(9, s2);
	CHECK_EQUAL(&r1, &s);
	CHECK_EQUAL(&r2, &s);
    CHECK_THROW(s.insert(0, s), std::length_error);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, MyLitWorld!"), 0);
    CHECK_EQUAL(s.length(), 18u);
}

TEST(insert_6) {
	rttl::string<32> s("Hello, World!");
	auto it = s.insert(s.cbegin() + 5, 'w');
	CHECK_EQUAL(*it, 'w');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hellow, World!"), 0);
    CHECK_EQUAL(s.length(), 14u);
}

TEST(insert_7) {
	rttl::string<32> s("Hello, World!");
	auto it = s.insert(s.cbegin() + 5, 3, 'w');
	CHECK_EQUAL(*it, 'w');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hellowww, World!"), 0);
    CHECK_EQUAL(s.length(), 16u);
}

TEST(insert_8) {
	std::vector<char> c = { '.', 'c', 'o', 'm' };
	rttl::string<32> s("Hello, World!");
	auto it = s.insert(s.cend() - 1, c.cbegin(), c.cend());
	CHECK_EQUAL(*it, '.');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World.com!"), 0);
    CHECK_EQUAL(s.length(), 17u);
	c.resize(16, '!');
	CHECK_THROW(s.insert(s.cend(), c.cbegin(), c.cend()), std::length_error);	
}

TEST(insert_9) {
	rttl::string<32> s("Hello, World!");
	auto it = s.insert(s.cend() - 1, { '.', 'c', 'o', 'm' });
	CHECK_EQUAL(*it, '.');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World.com!"), 0);
    CHECK_EQUAL(s.length(), 17u);
}

TEST(erase_1) {
	rttl::string<32> s("Hello, World!");
	CHECK_THROW(s.erase(14), std::out_of_range);
	auto& r1 = s.erase(4, 2);
	CHECK_EQUAL(&r1, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hell World!"), 0);
    CHECK_EQUAL(s.length(), 11u);
	auto& r2 = s.erase(4);
	CHECK_EQUAL(&r2, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hell"), 0);
    CHECK_EQUAL(s.length(), 4u);
	auto& r3 = s.erase();
	CHECK_EQUAL(&r3, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), ""), 0);
    CHECK_EQUAL(s.length(), 0u);
}

TEST(erase_2) {
	rttl::string<32> s("Hello, World!");
	auto it = s.erase(s.cbegin() + 4);
	CHECK_EQUAL(*it, ',');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hell, World!"), 0);
    CHECK_EQUAL(s.length(), 12u);
}

TEST(erase_3) {
	rttl::string<32> s("Hello, World!");
	auto it = s.erase(s.cbegin() + 4, s.cend() - 1);
	CHECK_EQUAL(*it, '!');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hell!"), 0);
    CHECK_EQUAL(s.length(), 5u);
}

TEST(push_back) {
	rttl::string<32> s("Hello, World!");
	s.push_back('!');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!!"), 0);
    CHECK_EQUAL(s.length(), 14u);
}

TEST(pop_back) {
	rttl::string<32> s("Hello, World!");
	s.pop_back();
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World"), 0);
    CHECK_EQUAL(s.length(), 12u);
	s.erase();
	s.pop_back();
	CHECK_EQUAL(std::strcmp(s.c_str(), ""), 0);
    CHECK_EQUAL(s.length(), 0u);
}

TEST(append_1) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.append(4, '!');
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!!!!!"), 0);
    CHECK_EQUAL(s.length(), 17u);
	CHECK_THROW(s.append(16, '?'), std::length_error);
}

TEST(append_2) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1(" Bye");
	std::string s2("-bye!");
	auto& r1 = s.append(s1);
	CHECK_EQUAL(&r1, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye"), 0);
    CHECK_EQUAL(s.length(), 17u);
	auto& r2 = s.append(s2);
	CHECK_EQUAL(&r2, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye-bye!"), 0);
    CHECK_EQUAL(s.length(), 22u);
	CHECK_THROW(s1.append(s), std::length_error);
}

TEST(append_3) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1(" Bye");
	std::string s2("-bye!");
	auto& r1 = s.append(s1, 0, 3);
	CHECK_EQUAL(&r1, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! By"), 0);
    CHECK_EQUAL(s.length(), 16u);
	auto& r2 = s.append(s2, 1, 3);
	CHECK_EQUAL(&r2, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bybye"), 0);
    CHECK_EQUAL(s.length(), 19u);
}

TEST(append_4) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.append(" Bye-bye!", 4);
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye"), 0);
    CHECK_EQUAL(s.length(), 17u);
}

TEST(append_5) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.append(" Bye-bye!");
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye-bye!"), 0);
    CHECK_EQUAL(s.length(), 22u);
}

TEST(append_6) {
	rttl::string<32> s("Hello, World!");
	std::vector<char> v = { ' ', 'B', 'y', 'e', '!' };
	auto& r = s.append(v.cbegin(), v.cend());
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye!"), 0);
    CHECK_EQUAL(s.length(), 18u);
}

TEST(append_7) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.append({ ' ', 'B', 'y', 'e', '!' });
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye!"), 0);
    CHECK_EQUAL(s.length(), 18u);
}

TEST(operator_append_1) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1(" Bye");
	std::string s2("-bye!");
	auto& r1 = s += s1;
	CHECK_EQUAL(&r1, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye"), 0);
    CHECK_EQUAL(s.length(), 17u);
	auto& r2 = s += s2;
	CHECK_EQUAL(&r2, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye-bye!"), 0);
    CHECK_EQUAL(s.length(), 22u);
}

TEST(operator_append_2) {
	rttl::string<32> s("Hello, World!");
	auto& r = s += '!';
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!!"), 0);
    CHECK_EQUAL(s.length(), 14u);
}

TEST(operator_append_3) {
	rttl::string<32> s("Hello, World!");
	auto& r = s += " Bye-bye!";
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye-bye!"), 0);
    CHECK_EQUAL(s.length(), 22u);
}

TEST(operator_append_4) {
	rttl::string<32> s("Hello, World!");
	auto& r = s += { ' ', 'B', 'y', 'e', '-', 'b', 'y', 'e', '!' };
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World! Bye-bye!"), 0);
    CHECK_EQUAL(s.length(), 22u);
}

TEST(compare_1) {
	rttl::string<32> s("Hello");
	std::string s1("Hello");
	rttl::string<32> s2("World");
	rttl::string<16> s3("Bye");
	int r1 = s.compare(s1);
	CHECK(r1 == 0);
	int r2 = s.compare(s2);
	CHECK(r2 < 0);
	int r3 = s.compare(s3);
	CHECK(r3 > 0);
}

TEST(compare_2) {
	rttl::string<32> s("Hello, World!");
	std::string s1("Hello");
	rttl::string<32> s2("World");
	rttl::string<16> s3("Bye");
	int r1 = s.compare(6, 6, s1);
	CHECK(r1 < 0);
	int r2 = s.compare(7, 5, s2);
	CHECK(r2 == 0);
	int r3 = s.compare(0, 1, s3);
	CHECK(r3 > 0);
}

TEST(compare_3) {
	rttl::string<32> s("Hello, World!");
	std::string s1("Hello");
	rttl::string<32> s2("World");
	rttl::string<16> s3("Bye");
	int r1 = s.compare(1, 4, s1, 1, 4);
	CHECK(r1 == 0);
	int r2 = s.compare(7, 4, s2, 0, 5);
	CHECK(r2 < 0);
	int r3 = s.compare(3, 8, s3, 2, 1);
	CHECK(r3 > 0);
}

TEST(compare_4) {
	rttl::string<32> s("Hello");
	int r1 = s.compare("Hello");
	CHECK(r1 == 0);
	int r2 = s.compare("Hellp");
	CHECK(r2 < 0);
	int r3 = s.compare("Helln");
	CHECK(r3 > 0);
}

TEST(compare_5) {
	rttl::string<32> s("Hello");
	int r1 = s.compare(1, 3, "ell");
	CHECK(r1 == 0);
	int r2 = s.compare(3, 1, "zxc");
	CHECK(r2 < 0);
	int r3 = s.compare(0, 2, "Hd");
	CHECK(r3 > 0);
}

TEST(compare_6) {
	rttl::string<32> s("Hello");
	int r1 = s.compare(1, 3, "ella", 3);
	CHECK(r1 == 0);
	int r2 = s.compare(3, 1, "moo", 3);
	CHECK(r2 < 0);
	int r3 = s.compare(0, 2, "Hel", 1);
	CHECK(r3 > 0);
}

#if __cplusplus > 201703L

TEST(starts_with) {
	rttl::string<32> s("Hello, World!");
	bool r1 = s.starts_with('H');
	CHECK_EQUAL(r1, true);
	bool r2 = s.starts_with('!');
	CHECK_EQUAL(r2, false);
}

TEST(ends_with) {
	rttl::string<32> s("Hello, World!");
	bool r1 = s.ends_with('!');
	CHECK_EQUAL(r1, true);
	bool r2 = s.ends_with('H');
	CHECK_EQUAL(r2, false);
}

#endif

TEST(replace_1a) {
	rttl::string<32> s("Hello, World!");
    rttl::string<16> s1("Comrade?");
    std::string s2(" Marik");
	auto& r1 = s.replace(7, 5, s1);
	CHECK_EQUAL(&r1, &s);
    CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comrade?!"), 0);
    CHECK_EQUAL(s.length(), 16u);
	auto& r2 = s.replace(14, 1, s2);
	CHECK_EQUAL(&r2, &s);
    CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comrade Marik!"), 0);
    CHECK_EQUAL(s.length(), 21u);
	CHECK_THROW(s1.replace(7, s1.npos, s), std::length_error);
    auto& r3 = s.replace(0, 6, s2);
    CHECK_EQUAL(&r3, &s);
    CHECK_EQUAL(std::strcmp(s.c_str(), " Marik Comrade Marik!"), 0);
    CHECK_EQUAL(s.length(), 21u);
}

TEST(replace_1b) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1("Comrade?");
	std::string s2(" Marik");
	auto& r1 = s.replace(s.cbegin() + 7, s.cend() - 1, s1);
	CHECK_EQUAL(&r1, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comrade?!"), 0);
    CHECK_EQUAL(s.length(), 16u);
	auto& r2 = s.replace(s.cend() - 2, s.cend() - 1, s2);
	CHECK_EQUAL(&r2, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comrade Marik!"), 0);
    CHECK_EQUAL(s.length(), 21u);
}

TEST(replace_2a) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1("Comrade?");
	std::string s2(" Marik");
	auto& r1 = s.replace(7, 5, s1, 0, 4);
	CHECK_EQUAL(&r1, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comr!"), 0);
    CHECK_EQUAL(s.length(), 12u);
	auto& r2 = s.replace(7, 4, s2, 1, 3);
	CHECK_EQUAL(&r2, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Mar!"), 0);
    CHECK_EQUAL(s.length(), 11u);
}

TEST(replace_2b) {
	rttl::string<32> s("Hello, World!");
	std::vector v = { 'C', 'o', 'm', 'r', 'a', 'd', 'e' };
	auto& r = s.replace(s.cbegin() + 7, s.cend() - 1, v.cbegin(), v.cend());
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comrade!"), 0);
    CHECK_EQUAL(s.length(), 15u);
	v.resize(33, '@');
	CHECK_THROW(s.replace(s.cbegin(), s.cend(), v.cbegin(), v.cend()), std::length_error);
    auto& r1 = s.replace(s.cbegin(), s.cbegin() + 7, v.cbegin(), v.cbegin() + 7);
    CHECK_EQUAL(&r1, &s);
    CHECK_EQUAL(std::strcmp(s.c_str(), "ComradeComrade!"), 0);
    CHECK_EQUAL(s.length(), 15u);
}

TEST(replace_3a) {
	rttl::string<32> s("Hello, World!");
	CHECK_THROW(s.replace(14, 1, "?"), std::out_of_range);
	CHECK_THROW(s.replace(13, s.npos, "ZzzzZzzzZzzzZzzzZzzzZ"), std::length_error);
	auto& r = s.replace(7, 5, "Comrade", 4);
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comr!"), 0);
    CHECK_EQUAL(s.length(), 12u);
}

TEST(replace_3b) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.replace(s.cbegin() + 7, s.cend() - 1, "Comrade", 4);
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comr!"), 0);
    CHECK_EQUAL(s.length(), 12u);
}

TEST(replace_4a) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.replace(7, 5, "Comrade");
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comrade!"), 0);
    CHECK_EQUAL(s.length(), 15u);
}

TEST(replace_4b) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.replace(s.cbegin() + 7, s.cend() - 1, "Comrade");
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comrade!"), 0);
    CHECK_EQUAL(s.length(), 15u);
}

TEST(replace_5a) {
	rttl::string<32> s("Hello, World!");
	CHECK_THROW(s.replace(14, 5, 3, 'A'), std::out_of_range);
	CHECK_THROW(s.replace(13, s.npos, 21, 'Z'), std::length_error);
	auto& r = s.replace(7, 5, 3, 'A');
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, AAA!"), 0);
    CHECK_EQUAL(s.length(), 11u);
    auto& r1 = s.replace(0, 5, 5, 'Z');
    CHECK_EQUAL(&r1, &s);
    CHECK_EQUAL(std::strcmp(s.c_str(), "ZZZZZ, AAA!"), 0);
    CHECK_EQUAL(s.length(), 11u);
}

TEST(replace_5b) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.replace(s.cbegin() + 7, s.cend() - 1, 3, 'A');
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, AAA!"), 0);
    CHECK_EQUAL(s.length(), 11u);
}

TEST(replace_6) {
	rttl::string<32> s("Hello, World!");
	auto& r = s.replace(s.cbegin() + 7, s.cend() - 1, { 'C', 'o', 'm', 'r', 'a', 'd', 'e' });
	CHECK_EQUAL(&r, &s);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, Comrade!"), 0);
    CHECK_EQUAL(s.length(), 15u);
}

TEST(substr) {
	rttl::string<32> s("Hello, World!");
	auto s1 = s.substr(7, 5);
	CHECK_EQUAL(std::strcmp(s1.c_str(), "World"), 0);
    CHECK_EQUAL(s1.length(), 5u);
	auto s2 = s.substr(1);
	CHECK_EQUAL(std::strcmp(s2.c_str(), "ello, World!"), 0);
    CHECK_EQUAL(s2.length(), 12u);
}

TEST(copy) {
	rttl::string<32> s("Hello, World!");
	char buf[33];
	buf[5] = '?';
	s.copy(buf, 5, 7);
	CHECK_EQUAL(std::strncmp(buf, "World?", 6), 0);
	s.copy(buf, s.npos, 0);
	CHECK_EQUAL(std::strcmp(buf, "Hello, World!"), 0);
	CHECK_THROW(s.copy(buf, 5, 14), std::out_of_range);
}

TEST(resize_1) {
	rttl::string<32> s("Hello, World!");
	s.resize(6);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello,"), 0);
    CHECK_EQUAL(s.length(), 6u);
	s.resize(10);
	CHECK_EQUAL(std::strncmp(s.c_str(), "Hello,\0\0\0\0\0", 11), 0);
    CHECK_EQUAL(s.length(), 10u);
	CHECK_THROW(s.resize(33), std::length_error);
}

TEST(resize_2) {
	rttl::string<32> s("Hello, World!");
	s.resize(6, 'z');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello,"), 0);
    CHECK_EQUAL(s.length(), 6u);
	s.resize(10, 'z');
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello,zzzz"), 0);
    CHECK_EQUAL(s.length(), 10u);
	CHECK_THROW(s.resize(33, 'z'), std::length_error);
}

TEST(swap) {
	rttl::string<32> s("Hello, World!");
	rttl::string<32> s1("Bye-bye!");
	rttl::string<16> s2("Is Marik!");
	std::string s3("Cheeki-Breeki");
	s.swap(s1);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Bye-bye!"), 0);
    CHECK_EQUAL(s.length(), 8u);
	CHECK_EQUAL(std::strcmp(s1.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s1.length(), 13u);
    s.swap(s1);
    CHECK_EQUAL(std::strcmp(s1.c_str(), "Bye-bye!"), 0);
    CHECK_EQUAL(s1.length(), 8u);
    CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s.length(), 13u);
    s.swap(s2);
    CHECK_EQUAL(std::strcmp(s2.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s2.length(), 13u);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Is Marik!"), 0);
    CHECK_EQUAL(s.length(), 9u);
	s.swap(s3);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Cheeki-Breeki"), 0);
    CHECK_EQUAL(s.length(), 13u);
	CHECK_EQUAL(std::strcmp(s3.c_str(), "Is Marik!"), 0);
    CHECK_EQUAL(s3.length(), 9u);
	s.swap(s3);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Is Marik!"), 0);
    CHECK_EQUAL(s.length(), 9u);
	CHECK_EQUAL(std::strcmp(s3.c_str(), "Cheeki-Breeki"), 0);
    CHECK_EQUAL(s3.length(), 13u);
}

TEST(find_1) {
	rttl::string<32> s("Hello Hello");
	rttl::string<16> s1("Hello");
	std::string s2("Hello");
	auto r1 = s.find(s1);
    CHECK_EQUAL(r1, 0u);
	auto r2 = s.find(s2, 1);
    CHECK_EQUAL(r2, 6u);
	auto r3 = s.find(s2, 7);
	CHECK_EQUAL(r3, s.npos);
}

TEST(find_2) {
	rttl::string<32> s("Hello Hello");
	auto r1 = s.find("Hellow", 0, 3);
    CHECK_EQUAL(r1, 0u);
	auto r2 = s.find("Helloween", 1, 5);
    CHECK_EQUAL(r2, 6u);
	auto r3 = s.find("Hel", 7, 4);
	CHECK_EQUAL(r3, s.npos);
}

TEST(find_3) {
	rttl::string<32> s("Hello Hello");
	auto r1 = s.find("Hello");
    CHECK_EQUAL(r1, 0u);
	auto r2 = s.find("Hello", 1);
    CHECK_EQUAL(r2, 6u);
	auto r3 = s.find("Hel", 7);
	CHECK_EQUAL(r3, s.npos);
}

TEST(find_4) {
	rttl::string<32> s("Hello, World!");
	auto r = s.find('o');
    CHECK_EQUAL(r, 4u);
	auto r2 = s.find('o', 5);
    CHECK_EQUAL(r2, 8u);
	auto r3 = s.find('o', 9);
	CHECK_EQUAL(r3, s.npos);
}

TEST(rfind_1) {
	rttl::string<32> s("Hello Hello");
	rttl::string<16> s1("Hello");
	std::string s2("World");
	auto r1 = s.rfind(s1, 5);
    CHECK_EQUAL(r1, 0u);
	auto r2 = s.rfind(s2);
	CHECK_EQUAL(r2, s.npos);
}


TEST(rfind_2) {
	rttl::string<32> s("Hello Hello");
	auto r1 = s.rfind("Hellow", s.npos, 5);
    CHECK_EQUAL(r1, 6u);
	auto r2 = s.rfind("World", 7, 3);
	CHECK_EQUAL(r2, s.npos);
}

TEST(rfind_3) {
	rttl::string<32> s("Hello Hello");
	auto r1 = s.rfind("Hello", 5);
    CHECK_EQUAL(r1, 0u);
	auto r2 = s.rfind("World", 7);
	CHECK_EQUAL(r2, s.npos);
}

TEST(rfind_4) {
	rttl::string<32> s("Hello, World!");
	auto r = s.rfind('o');
    CHECK_EQUAL(r, 8u);
	auto r2 = s.rfind('o', 3);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_of_1) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1("oil");
	std::string s2("error");
	auto r1 = s.find_first_of(s1);
    CHECK_EQUAL(r1, 2u);
	auto r2 = s.find_first_of(s2, 10);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_of_2) {
	rttl::string<32> s("Hello, World!");
	auto r1 = s.find_first_of("oil", 0, 2);
    CHECK_EQUAL(r1, 4u);
	auto r2 = s.find_first_of("error", 2, 1);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_of_3) {
	rttl::string<32> s("Hello, World!");
	auto r1 = s.find_first_of("oil", 0);
    CHECK_EQUAL(r1, 2u);
	auto r2 = s.find_first_of("error", 10);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_of_4) {
	rttl::string<32> s("Hello, World!");
	auto r1 = s.find_first_of('o');
    CHECK_EQUAL(r1, 4u);
	auto r2 = s.find_first_of('o', 9);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_of_5) {
	rttl::string<32> s("Hello, World!");
	std::string_view s1("oil");
	std::string_view s2("error");
	auto r1 = s.find_first_of(s1);
    CHECK_EQUAL(r1, 2u);
	auto r2 = s.find_first_of(s2, 10);
	CHECK_EQUAL(r2, s.npos);
}


TEST(find_first_not_of_1) {
	rttl::string<32> s("Hello, World!");
	rttl::string<16> s1("oil");
	std::string s2("!older");
	auto r1 = s.find_first_not_of(s1);
    CHECK_EQUAL(r1, 0u);
	auto r2 = s.find_first_not_of(s2, 8);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_not_of_2) {
	rttl::string<32> s("Hello, World!");
	auto r1 = s.find_first_not_of("oil", 2, 2);
    CHECK_EQUAL(r1, 2u);
	auto r2 = s.find_first_not_of("!older", 8, 6);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_not_of_3) {
	rttl::string<32> s("Hello, World!");
	auto r1 = s.find_first_not_of("oil");
    CHECK_EQUAL(r1, 0u);
	auto r2 = s.find_first_not_of("!older", 8);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_not_of_4) {
	rttl::string<32> s("Hello, World!");
	auto r1 = s.find_first_not_of('H');
    CHECK_EQUAL(r1, 1u);
	auto r2 = s.find_first_not_of('!', 12);
	CHECK_EQUAL(r2, s.npos);
}

TEST(find_first_not_of_5) {
	rttl::string<32> s("Hello, World!");
	std::string_view s1("oil");
	std::string_view s2("!older");
	auto r1 = s.find_first_not_of(s1);
    CHECK_EQUAL(r1, 0u);
	auto r2 = s.find_first_not_of(s2, 8);
	CHECK_EQUAL(r2, s.npos);
}

/// @todo test_find_last_of
/// @todo test_find_last_not_of

TEST(operator_add_1a) {
	rttl::string<32> s1("Hello, ");
	rttl::string<16> s2("World");
	std::string s3("!");
	auto s = s1 + s2 + s3;
	static_assert(std::is_same<decltype(s), decltype(s1)>::value);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(operator_add_1b) {
	std::string s1("Hello, ");
	rttl::string<32> s2("World");
	rttl::string<16> s3("!");
	auto s = s1 + s2 + s3;
	//static_assert(std::is_same<decltype(s), decltype(s1)>::value);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(operator_add_2) {
	rttl::string<32> s1(" World!");
	auto s = "Hello," + s1;
	static_assert(std::is_same<decltype(s), decltype(s1)>::value);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(operator_add_3) {
	rttl::string<32> s1("ello, World!");
	auto s = 'H' + s1;
	static_assert(std::is_same<decltype(s), decltype(s1)>::value);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(operator_add_4) {
	rttl::string<32> s1("Hello,");
	auto s = s1 + " World!";
	static_assert(std::is_same<decltype(s), decltype(s1)>::value);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(operator_add_5) {
	rttl::string<32> s1("Hello, World");
	auto s = s1 + '!';
	static_assert(std::is_same<decltype(s), decltype(s1)>::value);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s.length(), 13u);
}

TEST(operator_eq) {
	using namespace std;
	using namespace rttl;
	rttl::string<32> s1("Hello");
	rttl::string<16> s2("Hello");
	std::string s3("World!");
	auto r1 = (s1 == s2);
	static_assert(std::is_same<decltype(r1), bool>::value);
	CHECK_EQUAL(r1, true);
	auto r2 = (s1 == s3);
	static_assert(std::is_same<decltype(r2), bool>::value);
	CHECK_EQUAL(r2, false);
}

/// @todo test_operator!=
/// @todo test_operator<
/// @todo test_operator>
/// @todo test_operator<=
/// @todo test_operator>=

TEST(std_swap) {
	rttl::string<32> s("Hello, World!");
	rttl::string<32> s1("Bye-bye!");
	rttl::string<16> s2("Is Marik!");
	std::string s3("Cheeki-Breeki");
	std::swap(s, s1);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Bye-bye!"), 0);
    CHECK_EQUAL(s.length(), 8u);
	CHECK_EQUAL(std::strcmp(s1.c_str(), "Hello, World!"), 0);
    CHECK_EQUAL(s1.length(), 13u);
	std::swap(s, s2);
	CHECK_EQUAL(std::strcmp(s2.c_str(), "Bye-bye!"), 0);
    CHECK_EQUAL(s2.length(), 8u);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Is Marik!"), 0);
    CHECK_EQUAL(s.length(), 9u);
	std::swap(s, s3);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Cheeki-Breeki"), 0);
    CHECK_EQUAL(s.length(), 13u);
	CHECK_EQUAL(std::strcmp(s3.c_str(), "Is Marik!"), 0);
    CHECK_EQUAL(s3.length(), 9u);
	std::swap(s3, s);
	CHECK_EQUAL(std::strcmp(s3.c_str(), "Cheeki-Breeki"), 0);
    CHECK_EQUAL(s3.length(), 13u);
	CHECK_EQUAL(std::strcmp(s.c_str(), "Is Marik!"), 0);
    CHECK_EQUAL(s.length(), 9u);
}

/// @todo test_operator<<
/// @todo test_operator>>
/// @todo test_getline

TEST(stoi) {
	rttl::string<32> s1(" -123 kg");
	std::size_t pos;
	auto r1 = stoi(s1, &pos);
	static_assert(std::is_same<decltype(r1), int>::value);
	CHECK_EQUAL(r1, -123);
    CHECK_EQUAL(pos, 5u);
    rttl::string<16> s2("7654");
    auto r2 = stoi(s2);
    static_assert(std::is_same<decltype(r2), int>::value);
    CHECK_EQUAL(r2, 7654);
    rttl::string<32> s3("kg");
    CHECK_THROW(stoi(s3), std::invalid_argument);
    rttl::string<32> s4("10000000000");
    CHECK_THROW(stoi(s4), std::out_of_range);
}

/// @todo test_stol
/// @todo test_stoll
/// @todo test_stoul
/// @todo test_stoull
/// @todo test_stof
/// @todo test_stod
/// @todo test_stold

TEST(to_string_1) {
	auto s = rttl::to_string<32>(123);
	static_assert(std::is_same<decltype(s), rttl::string<32>>::value);
    CHECK_EQUAL(s.length(), 3u);
    CHECK_EQUAL(std::strcmp(s.c_str(), "123"), 0);
	CHECK_THROW(rttl::to_string<4>(12345), std::length_error);
}

/// @todo test_to_string_1 ... test_to_string_9

/// @todo test_to_wstring

TEST(std_hash) {
	rttl::string<32> s1("Hello, World!");
	rttl::string<32> s2("Hello, World!");
	rttl::string<32> s3("Bye-bye!");
	auto r1 = std::hash<rttl::string<32>>{}(s1);
	auto r2 = std::hash<rttl::string<32>>{}(s2);
	auto r3 = std::hash<rttl::string<32>>{}(s3);
	static_assert(std::is_same<decltype(r1), std::size_t>::value);
	static_assert(std::is_same<decltype(r2), std::size_t>::value);
	static_assert(std::is_same<decltype(r3), std::size_t>::value);
	CHECK(r1 == r2);
	CHECK(r1 != r3);
}

int main(int, const char* []) {
    return UnitTest::RunAllTests();
}
