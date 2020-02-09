/**
 * @file rttl/string.h
 *
 * A class template for storing a string of characters without the use of dynamic memory allocation.
 *
 * Behaves like `std::basic_string`, but stores string statically in a class member.
 * Interface made to be as compatible with `std::string` as posible. The differences are:
 *  - Added template argument `MaxLength` that defines maximum string length (not including trailing
 *    null character). `MaxLength` should be greater than 0, else program is ill-formed
 *  - No allocator argument in constructors
 *  - `std::basic_string` functions accepting argument of `std::basic_string` type in this class
 *    have `std::basic_string_view` argument instead, that allows conversion from `std::basic_string`
 *    or `rttl::static_string` of arbitrary `MaxLength`; this should not be a compatibility issue
 *     due to the implicit conversions of both `std::basic_string` and `rttl::basic_string` to
 *    `std::basic_string_view`
 *  - Move construction, move assignment and swapping operations have `O(n)` time complexity,
 *    invalidates iterators
 *
 * Important note: Be careful with allocating lengthy strings on the stack.
 *
 */
#ifndef RTTL_STRING_H_
#define RTTL_STRING_H_
#include <array>
#include <string>
#include <string_view>
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include <iterator>
#include <istream>
#include <iostream>

#if __cplusplus < 201703L
#error "ISO C++ 2017 or later required"
#endif

#if __cplusplus > 201703L
#include <ranges>
#endif


namespace rttl {

template <std::size_t MaxLength, typename CharT, typename Traits = std::char_traits<CharT>>
class basic_string {
public:

	/// @section Member types
	using traits_type = Traits;
	using value_type = CharT;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = CharT&;
	using const_reference = const CharT&;
	using pointer = CharT*;
	using const_pointer = const CharT*;
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_assert(MaxLength > 0, "Empty strings are not allowed");
	static_assert((!std::is_array<value_type>::value), "Character type of basic_string must not be an array");
	static_assert((std::is_standard_layout<value_type>::value), "Character type of basic_string must be standard-layout");
	static_assert((std::is_trivial<value_type>::value), "Character type of basic_string must be trivial");
	static_assert((std::is_same<value_type, typename traits_type::char_type>::value),
		"traits_type::char_type must be the same type as CharT");

	/// @section Constants
	/**
	 * Special value
	 * When used as length, means "until the end of the string"
	 * As a return value, used to indicate no matches
	 */
	static constexpr size_type npos = static_cast<size_type>(-1);

	/// @section Member functions

	/**
	 * @name (constructor)
	 */
	 ///{
    basic_string() noexcept = default;

	basic_string(size_type count, CharT ch) {
		assign(count, ch);
	}

	basic_string(const std::basic_string_view<CharT, Traits>& str) {
		assign(str);
	}

	basic_string(const std::basic_string_view<CharT, Traits>& str, size_type pos, size_type count = npos) {
		assign(str, pos, count);
	}

	basic_string(const CharT* s, size_type count) {
		assign(s, count);
	}

	basic_string(const CharT* s) {
		assign(s);
	}

	template <typename InputIt>
	basic_string(InputIt first, InputIt last) {
        while (first != last) {
            push_back(*first);
            ++first;
        }
	}

	basic_string(const basic_string& other) noexcept {
		m_length = other.m_length;
		Traits::copy(data(), other.data(), m_length);
		m_data[m_length] = CharT();
	}

	basic_string(basic_string&& other) noexcept {
		m_length = other.m_length;
		Traits::copy(data(), other.data(), m_length);
		m_data[m_length] = CharT();
	}

	basic_string(std::initializer_list<CharT> ilist) {
		assign(ilist);
	}
	///}


	/**
	 * @name (destructor)
	 */
	 ///{
    ~basic_string() = default;
	///}


	/**
	 * @name operator=
	 */
	 ///{
	basic_string& operator=(const basic_string& other) noexcept {
		m_length = other.m_length;
		Traits::copy(data(), other.data(), m_length + 1);
		return *this;
	}

	basic_string& operator=(const std::basic_string_view<CharT, Traits>& str) {
		return assign(str);
	}

	basic_string& operator=(const CharT* s) {
		return assign(s);
	}

	basic_string& operator=(CharT ch) noexcept {
		m_length = 1;
		m_data = { ch, CharT() };
		return *this;
	}

	basic_string& operator=(std::initializer_list<CharT> ilist) {
		return assign(ilist);
	}

	basic_string& operator=(basic_string&& str) noexcept {
		return assign(str);
	}
	///}


	/**
	 * @name assign
	 */
	 ///{
	basic_string& assign(size_type count, CharT ch) {
		if (count > max_size()) {
			throw std::length_error("rttl::basic_string");
		}
		m_length = count;
		Traits::assign(data(), count, ch);
		m_data[m_length] = CharT();
		return *this;
	}

	basic_string& assign(const basic_string<MaxLength, CharT, Traits>& str) noexcept {
		m_length = str.m_length;
		Traits::copy(data(), str.data(), str.length() + 1);
		return *this;
	}

	basic_string& assign(const std::basic_string_view<CharT, Traits>& str) {
		if (str.length() > max_size()) {
			throw std::length_error("rttl::basic_string");
		}
		m_length = str.length();
		Traits::copy(data(), str.data(), str.length());
		m_data[m_length] = CharT();
		return *this;
	}

	basic_string& assign(const std::basic_string_view<CharT, Traits>& str, size_type pos, size_type count = npos) {
		return assign(str.substr(pos, count));
	}

	basic_string& assign(const CharT* s, size_type count) {
		return assign(std::basic_string_view<CharT, Traits>(s, count));
	}

	basic_string& assign(const CharT* s) {
		return assign(std::basic_string_view<CharT, Traits>(s));
	}

	template <class InputIt>
	typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value, basic_string&>::type
	assign(InputIt first, InputIt last) {
        return replace(cbegin(), cend(), first, last);
	}

	basic_string& assign(std::initializer_list<CharT> ilist) {
        return assign(ilist.begin(), ilist.size());
	}
	///}


	/// @subsection Elements access


	/**
	 * @name at
	 */
	 ///{
	reference at(size_type pos) {
        if (pos >= length()) {
            throw std::out_of_range("rttl::basic_string");
        }
        return m_data[pos];
	}

	const_reference at(size_type pos) const {
        if (pos >= length()) {
            throw std::out_of_range("rttl::basic_string");
        }
        return m_data[pos];
	}
	///}


	/**
	 * @name operator[]
	 */
	 ///{
	reference operator[](size_type pos) noexcept {
		return m_data[pos];
	}

	const_reference operator[](size_type pos) const noexcept {
		return m_data[pos];
	}
	///}

	/**
	 * @name front
	 */
	 ///{
	CharT& front() noexcept {
		return m_data.front();
	}

	const CharT& front() const noexcept {
		return m_data.front();
	}
	///}

	/**
	 * @name back
	 */
	 ///{
	CharT& back() noexcept {
		return m_data[m_length - 1];
	}

	const CharT& back() const noexcept {
		return m_data[m_length - 1];
	}
	///}

	/**
	 * @name data
	 */
	 ///{
	const CharT* data() const noexcept {
		return m_data.data();
	}

	CharT* data() noexcept {
		return m_data.data();
	}
	///}

	/**
	 * @name c_str
	 */
	const CharT* c_str() const noexcept {
		return data();
	}

	/**
	 * operator basic_string_view
	 */
	operator std::basic_string_view<CharT, Traits>() const noexcept {
		return std::basic_string_view<CharT, Traits>(c_str(), length());
	}


	/// @subsection Iterators

	/**
	 * @name begin, cbegin
	 */
	 ///{
	iterator begin() noexcept {
		return &data()[0];
	}

	const_iterator begin() const noexcept {
		return &data()[0];
	}

	const_iterator cbegin() const noexcept {
		return &data()[0];
	}
	///}

	/**
	 * @name end, cend
	 */
	 ///{
	iterator end() noexcept {
		return begin() + m_length;
	}

	const_iterator end() const noexcept {
		return begin() + m_length;
	}

	const_iterator cend() const noexcept {
		return cbegin() + m_length;
	}
	///}


	/**
	 * @name rbegin, crbegin
	 */
	 ///{
	reverse_iterator rbegin() noexcept {
		return std::reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const noexcept {
		return std::reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const noexcept {
		return std::reverse_iterator(cend());
	}
	///}

	/**
	 * @name rend, crend
	 */
	 ///{
	reverse_iterator rend() noexcept {
		return std::reverse_iterator(begin());
	}

	const_reverse_iterator rend() const noexcept {
		return std::reverse_iterator(begin());
	}

	const_reverse_iterator crend() const noexcept {
		return std::reverse_iterator(cbegin());
	}
	///}


	/// @subsection Capacitance

	size_type size() const noexcept {
		return m_length;
	}

	size_type length() const noexcept {
		return size();
	}

	[[nodiscard]] bool empty() const noexcept {
		return (m_length == 0);
	}

	static constexpr size_type max_size() noexcept {
		return MaxLength;
	}

	void reserve(size_type size = 0) {
		if (size > max_size()) {
			throw std::length_error("rttl::basic_string");
		}
	}

	static constexpr size_type capacity() noexcept {
		return max_size();
	}

	void shrink_to_fit() noexcept {}


	/// @subsection Operations

	void clear() noexcept {
		m_length = 0;
		m_data[0] = CharT();
	}

	/**
	 * @name insert
	 */
	 ///{
	basic_string& insert(size_type pos, const std::basic_string_view<CharT, Traits>& str) {
		return replace(pos, 0, str);
	}

	basic_string& insert(size_type pos, const std::basic_string_view<CharT, Traits>& str, size_type subpos, size_type sublen = npos) {
		return insert(pos, str.substr(subpos, sublen));
	}

	basic_string& insert(size_type pos, const CharT* s) {
		return insert(pos, std::basic_string_view<CharT, Traits>(s));
	}

	basic_string& insert(size_type pos, const CharT* s, size_type n) {
		return insert(pos, std::basic_string_view<CharT, Traits>(s, n));
	}

	basic_string& insert(size_type pos, size_type n, CharT c) {
		insert(cbegin() + pos, n, c);
		return *this;
	}

	iterator insert(const_iterator p, size_type n, CharT c) {
		replace(p, p, n, c);
		return begin() + (p - cbegin());
	}

	iterator insert(const_iterator p, CharT c) {
		return insert(p, 1, c);
	}

	template <class InputIt>
	typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value, iterator>::type
	insert(const_iterator p, InputIt first, InputIt last) {
		replace(p, p, first, last);
		return begin() + (p - cbegin());
	}

	iterator insert(const_iterator p, std::initializer_list<CharT> ilist) {
        insert(p - cbegin(), ilist.begin(), ilist.size());
        return begin() + (p - cbegin());
    }
	///}

	/**
	 * @name erase
	 */
	 ///{
	basic_string& erase(size_type pos = 0, size_type len = npos) {
		if (pos > m_length) {
			throw std::out_of_range("rttl::basic_string");
		}
		erase(cbegin() + pos, cbegin() + pos + std::min(len, size() - pos));
		return *this;
	}

	iterator erase(const_iterator p) noexcept {
		return erase(p, p + 1);
	}

	iterator erase(const_iterator first, const_iterator last) noexcept {
		replace(first, last, 0, CharT());
		return begin() + (first - cbegin());
	}
	///}

	void push_back(CharT ch) {
		append(1, ch);
	}

	void pop_back() noexcept {
		if (m_length == 0) {
			return;
		}
		--m_length;
		m_data[m_length] = CharT();
	}

	/**
	 * @name append
	 */
	 ///{
	basic_string& append(const std::basic_string_view<CharT, Traits>& str) {
		return insert(length(), str);
	}

	basic_string& append(const std::basic_string_view<CharT, Traits>& str, size_type subpos, size_type sublen = npos) {
		return append(str.substr(subpos, sublen));
	}

	basic_string& append(const CharT* s) {
		return append(std::basic_string_view<CharT, Traits>(s));
	}

	basic_string& append(const CharT* s, size_type n) {
		return append(std::basic_string_view<CharT, Traits>(s, n));
	}

	basic_string& append(size_type n, CharT c) {
		return insert(length(), n, c);
	}

	template <class InputIt>
	typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value, basic_string&>::type
	append(InputIt first, InputIt last) {
		insert(cend(), first, last);
		return *this;
	}

	basic_string& append(std::initializer_list<CharT> ilist) {
        return append(ilist.begin(), ilist.size());
	}
	///}

	/**
	 * @name operator+=
	 */
	 ///{
	basic_string& operator+=(const std::basic_string_view<CharT, Traits>& str) {
		return append(str);
	}

	basic_string& operator+=(const CharT* s) {
		return append(s);
	}

	basic_string& operator+=(CharT ch) {
		push_back(ch);
		return *this;
	}

	basic_string& operator+=(std::initializer_list<CharT> ilist) {
		return append(ilist);
	}
	///}

	/**
	 * @name compare
	 */
	 ///{
	int compare(const std::basic_string_view<CharT, Traits>& str) const noexcept {
		return std::basic_string_view<CharT, Traits>(c_str(), length()).compare(str);
	}

	int compare(size_type pos, size_type len, const std::basic_string_view<CharT, Traits>& str) const {
		return std::basic_string_view<CharT, Traits>(c_str(), length()).compare(pos, len, str);
	}

	int compare(size_type pos, size_type len, const std::basic_string_view<CharT, Traits>& str, size_type subpos, size_type sublen = npos) const {
		return std::basic_string_view<CharT, Traits>(*this).compare(pos, len, str, subpos, sublen);
	}

	int compare(const CharT* s) const {
		return std::basic_string_view<CharT, Traits>(*this).compare(s);
	}

	int compare(size_type pos, size_type len, const CharT* s) const {
		return std::basic_string_view<CharT, Traits>(*this).compare(pos, len, s);
	}

	int compare(size_type pos, size_type len, const CharT* s, size_type n) const {
		return std::basic_string_view<CharT,Traits>(*this).compare(pos, len, s, n);
	}
	///}


	/**
	 * @name starts_with
	 */
	 ///{
	bool starts_with(std::basic_string_view<CharT, Traits> sv) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), size()).starts_with(sv);
	}

	bool starts_with(CharT c) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), size()).starts_with(c);
	}

	bool starts_with(const CharT* s) const {
		return std::basic_string_view<CharT, Traits>(data(), size()).starts_with(s);
	}
	///}


	/**
	 * @name ends_with
	 */
	 ///{
	bool ends_with(std::basic_string_view<CharT, Traits> sv) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), size()).starts_with(sv);
	}

	bool ends_with(CharT c) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), size()).starts_with(c);
	}

	bool ends_with(const CharT* s) const {
		return std::basic_string_view<CharT, Traits>(data(), size()).starts_with(s);
	}
	///}


	/**
	 * @name replace
	 */
	 ///{
	basic_string& replace(size_type pos, size_type count, const std::basic_string_view<CharT, Traits>& str) {
		if (pos > length()) {
			throw std::out_of_range("rttl::basic_string");
		}
        count = std::min(count, length() - pos);
        return replace(cbegin() + pos, cbegin() + pos + count, str);
	}

	basic_string& replace(const_iterator first, const_iterator last, const std::basic_string_view<CharT, Traits>& str) {
        auto first_nc = begin() + (first - cbegin());
        size_type count = last - first;
        if (count != str.length()) {
            if (length() - count + str.size() > max_size()) {
                throw std::length_error("rttl::basic_string");
            }
            /// Move the trailing part of the string (incl. null-terminating character) to the final position
            Traits::move(first_nc + str.length(), last, cend() - last + 1);
            m_length = m_length - count + str.length();
        }
        Traits::copy(first_nc, str.data(), str.length());
        return *this;
	}

	basic_string& replace(size_type pos, size_type count, const std::basic_string_view<CharT, Traits>& str, size_type pos2, size_type count2 = npos) {
		return replace(pos, count, str.substr(pos2, count2));
	}

	template<class InputIt>
	typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value, basic_string&>::type
	replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2) {
        return replace(first, last, basic_string(first2, last2));

        size_type count2;
        iterator first_nc = begin() + (first - cbegin());
		if constexpr(std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value) {
			/// `InputIt` is at least a forward iterator - we can do multiple passes
			count2 = std::distance(first2, last2);
            size_type count = std::distance(first, last);
            if (count != count2) {
				if (length() - count + count2 > max_size()) {
					throw std::length_error("rttl::basic_string");
				}
				/// Move the trailing part of the string (incl. null-terminating character) to the final position
				Traits::move(first_nc + count2, last, cend() - last + 1);
				m_length = m_length - count + count2;
			}
			std::copy(first2, last2, first_nc);
		} else {
            /// `InputIt` is just an input iterator - only one pass is allowed, we need extra memory
            count2 = 0;
            /// @todo Avoid on-stack memory allocation, if possible
            std::array<CharT,MaxLength> tmp;
            while (first2 != last2) {
                if (count2 >= MaxLength) {
                    throw std::length_error("rttl::basic_string");
                }
                tmp[count2] = *first2;
                ++count2;
            }
			size_type count = std::distance(first, last);
			if (count != count2) {
				if (length() - count + count2 > max_size()) {
					throw std::length_error("rttl::basic_string");
				}
				/// Move the trailing part of the string (incl. null-terminating character) to the final position
				Traits::move(first + count2, last, cend() - last + 1);
				m_length = m_length - count + count2;
			}
			Traits::copy(first_nc, tmp.data(), count2);
		}
		return *this;
	}

	basic_string& replace(size_type pos, size_type count, const CharT* cstr, size_type count2) {
		return replace(pos, count, std::basic_string_view<CharT, Traits>(cstr, count2));
	}

	basic_string& replace(const_iterator first, const_iterator last, const CharT* cstr, size_type count2) {
		return replace(first, last, std::basic_string_view<CharT, Traits>(cstr, count2));
	}

	basic_string& replace(size_type pos, size_type count, const CharT* cstr) {
        return replace(pos, count, std::basic_string_view<CharT, Traits>(cstr));
	}

	basic_string& replace(const_iterator first, const_iterator last, const CharT* cstr) {
		return replace(first, last, std::basic_string_view<CharT, Traits>(cstr));
	}

	basic_string& replace(size_type pos, size_type count, size_type count2, CharT ch) {
		if (pos > length()) {
			throw std::out_of_range("rttl::basic_string");
		}
        count = std::min(count, length() - pos);
		if (count != count2) {
			if (length() - count + count2 > max_size()) {
				throw std::length_error("rttl::basic_string");
			}
			/// Move the trailing part of the string into the final position
			Traits::move(begin() + pos + count2, begin() + pos + count, length() - (pos + count) + 1);
			m_length = m_length - count + count2;
		}
		Traits::assign(begin() + pos, count2, ch);
		return *this;
	}

	basic_string& replace(const_iterator first, const_iterator last, size_type count2, CharT ch) {
		return replace(first - cbegin(), last - first, count2, ch);
	}

	basic_string& replace(const_iterator first, const_iterator last, std::initializer_list<CharT> ilist) {
        return replace(first, last, ilist.begin(), ilist.size());
	}
	///}

	basic_string substr(size_type pos = 0, size_type count = std::string::npos) const {
		return basic_string(*this, pos, count);
	}

	size_type copy(CharT* dest, size_type count, size_type pos = 0) const {
		if (pos > length()) {
			throw std::out_of_range("rttl::basic_string");
		}
        count = std::min(count, length() - pos);
		Traits::copy(dest, data() + pos, count);
		return count;
	}

	/**
	 * @name resize
	 */
	 ///{
	void resize(size_type count) {
		resize(count, CharT());
	}

	void resize(size_type count, CharT ch) {
		if (count > m_length) {
			if (count > max_size()) {
				throw std::length_error("rttl::basic_string");
			}
			Traits::assign(begin() + m_length, count - m_length, ch);
		}
		m_length = count;
		m_data[m_length] = CharT();
	}
	///}

	/**
	 * @name swap
	 */
	///{
	void swap(basic_string& other) noexcept {
		size_type swap_len = std::min(length(), other.length());
		std::swap_ranges(begin(), begin() + swap_len, other.begin());
		if (other.length() > length()) {
			resize(other.length());
			Traits::copy(data() + swap_len, other.data() + swap_len, other.length() - swap_len + 1);
			other.resize(swap_len);
		} else {
			other.resize(length());
			Traits::copy(other.data() + swap_len, data() + swap_len, length() - swap_len + 1);
			resize(swap_len);
		}
	}

	template <std::size_t MaxLength2>
	void swap(basic_string<MaxLength2,CharT,Traits>& other) {
		size_type swap_len = std::min(length(), other.length());
		std::swap_ranges(begin(), begin() + swap_len, other.begin());
		if (other.length() > length()) {
			resize(other.length());
			Traits::copy(data() + swap_len, other.data() + swap_len, other.length() - swap_len + 1);
			other.resize(swap_len);
		}
		else {
			other.resize(length());
			Traits::copy(other.data() + swap_len, data() + swap_len, length() - swap_len + 1);
			resize(swap_len);
		}
	}

	template <typename Allocator>
	void swap(std::basic_string<CharT, Traits, Allocator>& other) {
		size_type swap_len = std::min(length(), other.length());
		std::swap_ranges(begin(), begin() + swap_len, other.begin());
		if (other.length() > length()) {
			resize(other.length());
			Traits::copy(data() + swap_len, other.data() + swap_len, other.length() - swap_len + 1);
			other.resize(swap_len);
		}
		else {
			other.resize(length());
			Traits::copy(other.data() + swap_len, data() + swap_len, length() - swap_len + 1);
			resize(swap_len);
		}
	}
	///}


	/// @subsection Search

	/**
	 * @name find
	 */
	 ///{
	size_type find(const std::basic_string_view<CharT, Traits>& str, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find(str, pos);
	}

	size_type find(const CharT* s, size_type pos, size_type count) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find(s, pos, count);
	}
	size_type find(const CharT* s, size_type pos = 0) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find(s, pos);
	}

	size_type find(CharT ch, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find(ch, pos);
	}
	///}


	/**
	 * @name rfind
	 */
	 ///{
	size_type rfind(const std::basic_string_view<CharT, Traits>& str, size_type pos = npos) const noexcept {
		return std::basic_string_view<CharT,Traits>(data(), length()).rfind(str, pos);
	}

	size_type rfind(const CharT* s, size_type pos, size_type count) const {
		return std::basic_string_view<CharT,Traits>(data(), length()).rfind(s, pos, count);
	}
	size_type rfind(const CharT* s, size_type pos = npos) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).rfind(s, pos);
	}

	size_type rfind(CharT ch, size_type pos = npos) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).rfind(ch, pos);
	}
	///}


	/**
	 * @name find_first_of
	 */
	 ///{
	size_type find_first_of(const std::basic_string_view<CharT, Traits>& str, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_first_of(str, pos);
	}

	size_type find_first_of(const CharT* s, size_type pos, size_type count) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_first_of(s, pos, count);
	}
	size_type find_first_of(const CharT* s, size_type pos = 0) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_first_of(s, pos);
	}

	size_type find_first_of(CharT ch, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_first_of(ch, pos);
	}
	///}


	/**
	 * @name find_first_not_of
	 */
	 ///{
	size_type find_first_not_of(const std::basic_string_view<CharT, Traits>& str, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_first_not_of(str, pos);
	}

	size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_first_not_of(s, pos, count);
	}
	size_type find_first_not_of(const CharT* s, size_type pos = 0) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_first_not_of(s, pos);
	}

	size_type find_first_not_of(CharT ch, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_first_not_of(ch, pos);
	}
	///}


	/**
	 * @name find_last_of
	 */
	 ///{
	size_type find_last_of(const std::basic_string_view<CharT, Traits>& str, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_last_of(str, pos);
	}

	size_type find_last_of(const CharT* s, size_type pos, size_type count) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_last_of(s, pos, count);
	}
	size_type find_last_of(const CharT* s, size_type pos = 0) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_last_of(s, pos);
	}

	size_type find_last_of(CharT ch, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_last_of(ch, pos);
	}
	///}


	/**
	 * @name find_last_not_of
	 */
	 ///{
	size_type find_last_not_of(const std::basic_string_view<CharT, Traits>& str, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_last_not_of(str, pos);
	}

	size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_last_not_of(s, pos, count);
	}
	size_type find_last_not_of(const CharT* s, size_type pos = 0) const {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_last_not_of(s, pos);
	}

	size_type find_last_not_of(CharT ch, size_type pos = 0) const noexcept {
		return std::basic_string_view<CharT, Traits>(data(), length()).find_last_not_of(ch, pos);
	}
	///}


private:
    size_type m_length = 0;
    std::array<CharT, MaxLength + 1> m_data = { 0 };    

};


/// @section Specializations

template <std::size_t MaxLength> using string = basic_string<MaxLength, char>;
template <std::size_t MaxLength> using wstring = basic_string<MaxLength, wchar_t>;
#if __cplusplus > 201703L
template <std::size_t MaxLength> using u8string = basic_string<MaxLength, char8_t>;
#endif
template <std::size_t MaxLength> using u16string = basic_string<MaxLength, char16_t>;
template <std::size_t MaxLength> using u32string = basic_string<MaxLength, char32_t>;


/// @section Non-member functions

/**
 * @name operator+
 */
///{
template<typename T, typename U>
typename std::enable_if<
	std::is_same<T, basic_string<T::max_size(), typename T::value_type, typename T::traits_type>>::value &&
	std::is_same<U, basic_string<U::max_size(), typename U::value_type, typename U::traits_type>>::value, T>::type
operator+(T lhs, const U& rhs) {
	return lhs.append(rhs);
}

template<typename T>
typename std::enable_if<std::is_same<T, basic_string<T::max_size(), typename T::value_type, typename T::traits_type>>::value, T>::type
operator+(T lhs, const std::basic_string_view<typename T::value_type, typename T::traits_type>& rhs) {
	return lhs.append(rhs);
}

template<typename T>
typename std::enable_if<std::is_same<T, basic_string<T::max_size(), typename T::value_type, typename T::traits_type>>::value, T>::type
operator+(const std::basic_string_view<typename T::value_type, typename T::traits_type>& lhs, const T& rhs) {
	return T(lhs).append(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
basic_string<MaxLength, CharT, Traits> operator+(const basic_string<MaxLength, CharT, Traits>& lhs, const CharT* rhs) {
	return basic_string<MaxLength, CharT, Traits>(lhs).append(rhs);
}

template< std::size_t MaxLength, typename CharT, typename Traits>
basic_string<MaxLength, CharT, Traits> operator+(const basic_string<MaxLength, CharT, Traits>& lhs, CharT rhs) {
	return basic_string<MaxLength, CharT, Traits>(lhs).append(1, rhs);
}

template< std::size_t MaxLength, typename CharT, typename Traits>
basic_string<MaxLength, CharT, Traits> operator+(const std::basic_string_view<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) {
	return basic_string<MaxLength, CharT, Traits>(lhs).append(rhs);
}

template< std::size_t MaxLength, typename CharT, typename Traits>
basic_string<MaxLength, CharT, Traits> operator+(const CharT* lhs, const basic_string<MaxLength, CharT, Traits>& rhs) {
	return basic_string<MaxLength,CharT,Traits>(lhs).append(rhs);
}

template< std::size_t MaxLength, typename CharT, typename Traits>
basic_string<MaxLength, CharT, Traits> operator+(CharT lhs, const basic_string<MaxLength, CharT, Traits>& rhs) {
	return basic_string<MaxLength, CharT, Traits>(1, lhs).append(rhs);
}
///}


/// @subsection Comparision

/**
 * @name operator==
 */
///{
template<std::size_t MaxLength1, std::size_t MaxLength2, typename CharT, typename Traits>
constexpr bool operator==(const basic_string<MaxLength1,CharT,Traits>& lhs, const basic_string<MaxLength2,CharT,Traits>& rhs) noexcept {
	return std::basic_string_view<CharT,Traits>(lhs) == std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator==(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string_view<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) == rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator==(const std::basic_string_view<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs == std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits, typename Alloc>
constexpr bool operator==(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string<CharT, Traits, Alloc>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) == rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits, typename Alloc>
constexpr bool operator==(const std::basic_string<CharT, Traits, Alloc>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs == std::basic_string_view<CharT, Traits>(rhs);
}
///}

/**
 * @name operator!=
 */
///{
template<std::size_t MaxLength1, std::size_t MaxLength2, typename CharT, typename Traits>
constexpr bool operator!=(const basic_string<MaxLength1, CharT, Traits>& lhs, const basic_string<MaxLength2, CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) != std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator!=(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string_view<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) != rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator!=(const std::basic_string_view<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs != std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator!=(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) == rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator!=(const std::basic_string<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs == std::basic_string_view<CharT, Traits>(rhs);
}
///}

/**
 * @name operator<
 */
 ///{
template<std::size_t MaxLength1, std::size_t MaxLength2, typename CharT, typename Traits>
constexpr bool operator<(const basic_string<MaxLength1, CharT, Traits>& lhs, const basic_string<MaxLength2, CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) < std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator<(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string_view<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) < rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator<(const std::basic_string_view<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs < std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator<(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) < rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator<(const std::basic_string<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs < std::basic_string_view<CharT, Traits>(rhs);
}
///}

/**
 * @name operator>
 */
 ///{
template<std::size_t MaxLength1, std::size_t MaxLength2, typename CharT, typename Traits>
constexpr bool operator>(const basic_string<MaxLength1, CharT, Traits>& lhs, const basic_string<MaxLength2, CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) > std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator>(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string_view<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) > rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator>(const std::basic_string_view<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs > std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator>(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) > rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator>(const std::basic_string<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs > std::basic_string_view<CharT, Traits>(rhs);
}
///}

/**
 * @name operator<=
 */
 ///{
template<std::size_t MaxLength1, std::size_t MaxLength2, typename CharT, typename Traits>
constexpr bool operator<=(const basic_string<MaxLength1, CharT, Traits>& lhs, const basic_string<MaxLength2, CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) <= std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator<=(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string_view<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) <= rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator<=(const std::basic_string_view<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs <= std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator<=(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) <= rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator<=(const std::basic_string<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs <= std::basic_string_view<CharT, Traits>(rhs);
}
///}

/**
 * @name operator>=
 */
 ///{
template<std::size_t MaxLength1, std::size_t MaxLength2, typename CharT, typename Traits>
constexpr bool operator>=(const basic_string<MaxLength1, CharT, Traits>& lhs, const basic_string<MaxLength2, CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) >= std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator>=(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string_view<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) >= rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator>=(const std::basic_string_view<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs >= std::basic_string_view<CharT, Traits>(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator>=(const basic_string<MaxLength, CharT, Traits>& lhs, const std::basic_string<CharT, Traits>& rhs) noexcept {
	return std::basic_string_view<CharT, Traits>(lhs) >= rhs;
}

template<std::size_t MaxLength, typename CharT, typename Traits>
constexpr bool operator>=(const std::basic_string<CharT, Traits>& lhs, const basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	return lhs >= std::basic_string_view<CharT, Traits>(rhs);
}
///}


/// @subsection I/O

using std::operator<<;

template <std::size_t MaxLength, typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, basic_string<MaxLength, CharT, Traits>& str) {
	typename std::basic_istream<CharT, Traits>::sentry sentry(is, false);
	if (sentry) {
		str.clear();
		std::size_t n = is.width();
		if (n == 0) { n = str.max_size(); }
		while (n > 0 && !is.eof()) {
			CharT c = is.rdbuf()->sgetc();
			if (std::isspace(c, is.getloc())) {
				break;
			}
			is.rdbuf()->gbump(1);
			str.push_back(c);
			--n;
		}
		if (n > 0 && str.empty()) {
			is.setstate(std::ios_base::failbit);
		}
		is.width(0);
	}
	return is;
}

/**
 * @name getline
 */
///{
template<std::size_t MaxLength, typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& getline(std::basic_istream<CharT, Traits>&& input, basic_string<MaxLength, CharT, Traits>& str, CharT delim) {
	typename std::basic_istream<CharT, Traits>::sentry sentry(input, true);
	if (sentry) {
		bool no_chars_extracted = true;
		str.clear();
		while (!input.eof()) {
			CharT c = input.rdbuf()->sgetc();
			no_chars_extracted = false;
			if (Traits::eq(c, delim)) {
				break;
			}
			if (str.length() >= str.max_size()) {
				input.setstate(std::ios_base::failbit);
				break;
			}
			str.push_back(c);
			input.rdbuf()->gbump(1);
		}
		if (no_chars_extracted) {
			input.setstate(std::ios_base::failbit);
		}
	}
	return input;
}
///}

template<std::size_t MaxLength, typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& getline(std::basic_istream<CharT, Traits>&& input, basic_string<MaxLength, CharT, Traits>& str) {
	return getline(input, str, input.widen('\n'));
}


/// @subsection Numeric conversions

/**
 * @name stoi
 */
///{
template <std::size_t MaxLength> int stoi(const string<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	long result = std::strtol(str.c_str(), &ptr, base);
    if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
    if constexpr(std::numeric_limits<long>::min() < std::numeric_limits<int>::min() ||
                 std::numeric_limits<long>::max() > std::numeric_limits<long>::max()) {
        if ((result < std::numeric_limits<int>::min()) || (result > std::numeric_limits<int>::max())) {
            throw std::out_of_range("rttl::basic_string");
        }
    }
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return static_cast<int>(result);
}

template <std::size_t MaxLength> int stoi(const wstring<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	long result = std::wcstol(str.c_str(), &ptr, base);
    if (errno == ERANGE) {
        throw std::out_of_range("rttl::basic_string");
    }
    if constexpr(std::numeric_limits<long>::min() < std::numeric_limits<int>::min() ||
                 std::numeric_limits<long>::max() > std::numeric_limits<long>::max()) {
        if ((result < std::numeric_limits<int>::min()) || (result > std::numeric_limits<int>::max())) {
            throw std::out_of_range("rttl::basic_string");
        }
    }
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return static_cast<int>(result);
}
///}

/**
 * @name stol
 */
///{
template <std::size_t MaxLength> long stol(const string<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	long result = std::strtol(str.c_str(), &ptr, base);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}

template <std::size_t MaxLength> long stol(const wstring<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	long result = std::wcstol(str.c_str(), &ptr, base);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}
///}

/**
 * @name stoll
 */
///{
template <std::size_t MaxLength> long long stoll(const string<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	long long result = std::strtoll(str.c_str(), &ptr, base);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}

template <std::size_t MaxLength> long long stoll(const wstring<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	long long result = std::wcstoll(str.c_str(), &ptr, base);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}
///}


/**
 * @name stoul
 */
///{
template <std::size_t MaxLength> unsigned long stol(const string<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	unsigned long result = std::strtoul(str.c_str(), &ptr, base);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}

template <std::size_t MaxLength> unsigned long stoul(const wstring<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	unsigned long result = std::wcstoul(str.c_str(), &ptr, base);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}
///}

/**
 * @name stoull
 */
///{
template <std::size_t MaxLength> unsigned long long stoull(const string<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	unsigned long long result = std::strtoull(str.c_str(), &ptr, base);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}

template <std::size_t MaxLength> unsigned long long stoull(const wstring<MaxLength>& str, std::size_t* pos = nullptr, int base = 10) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	unsigned long long result = std::wcstoull(str.c_str(), &ptr, base);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}
///}


/**
 * @name stof
 */
///{
template <std::size_t MaxLength> float stof(const string<MaxLength>& str, std::size_t* pos = nullptr) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	float result = std::strtof(str.c_str(), &ptr);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}

template <std::size_t MaxLength> float stof(const wstring<MaxLength>& str, std::size_t* pos = nullptr) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	float result = std::wcstof(str.c_str(), &ptr);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}
///}

/**
 * @name stod
 */
///{
template <std::size_t MaxLength> double stod(const string<MaxLength>& str, std::size_t* pos = nullptr) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	double result = std::strtod(str.c_str(), &ptr);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}

template <std::size_t MaxLength> double stod(const wstring<MaxLength>& str, std::size_t* pos = nullptr) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	double result = std::wcstod(str.c_str(), &ptr);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}
///}

/**
 * @name stold
 */
///{
template <std::size_t MaxLength> long double stold(const string<MaxLength>& str, std::size_t* pos = nullptr) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	long double result = std::strtold(str.c_str(), &ptr);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}

template <std::size_t MaxLength> long double stold(const wstring<MaxLength>& str, std::size_t* pos = nullptr) {
	typename string<MaxLength>::value_type* ptr;
	errno = 0;
	long double result = std::wcstold(str.c_str(), &ptr);
	if (errno == ERANGE) {
		throw std::out_of_range("rttl::basic_string");
	}
	if (ptr == str.c_str()) {
		throw std::invalid_argument("rttl::basic_string");
	}
	if (pos != nullptr) {
		*pos = ptr - str.c_str();
	}
	return result;
}
///}


/**
 * @name to_string
 */
///{
template <std::size_t MaxLength> string<MaxLength> to_string(int value) {
    string<MaxLength> result(MaxLength, char());
	int length = std::snprintf(result.data(), MaxLength + 1, "%d", value);
	if (static_cast<std::size_t>(length) > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> string<MaxLength> to_string(long value) {
	string<MaxLength> result;
	result.resize(MaxLength);
	int length = std::snprintf(result.data(), MaxLength + 1, "%ld", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> string<MaxLength> to_string(long long value) {
	string<MaxLength> result;
	result.resize(MaxLength);
	int length = std::snprintf(result.data(), MaxLength + 1, "%lld", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> string<MaxLength> to_string(unsigned value) {
	string<MaxLength> result;
	result.resize(MaxLength);
	int length = std::snprintf(result.data(), MaxLength + 1, "%u", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}


template <std::size_t MaxLength> string<MaxLength> to_string(unsigned long value) {
	string<MaxLength> result;
	result.resize(MaxLength);
	int length = std::snprintf(result.data(), MaxLength + 1, "%lu", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> string<MaxLength> to_string(unsigned long long value) {
	string<MaxLength> result;
	result.resize(MaxLength);
	int length = std::snprintf(result.data(), MaxLength + 1, "%llu", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> string<MaxLength> to_string(float value) {
	string<MaxLength> result;
	result.resize(MaxLength);
	int length = std::snprintf(result.data(), MaxLength + 1, "%f", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> string<MaxLength> to_string(double value) {
	string<MaxLength> result;
	result.resize(MaxLength);
	int length = std::snprintf(result.data(), MaxLength + 1, "%f", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> string<MaxLength> to_string(long double value) {
	string<MaxLength> result;
	result.resize(MaxLength);
	int length = std::snprintf(result.data(), MaxLength + 1, "%Lf", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}
///}

/**
 * @name to_wstring
 */
///{
template <std::size_t MaxLength> wstring<MaxLength> to_wstring(int value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%d", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> wstring<MaxLength> to_wstring(long value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%ld", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> wstring<MaxLength> to_wstring(long long value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%lld", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> wstring<MaxLength> to_wstring(unsigned value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%u", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}


template <std::size_t MaxLength> wstring<MaxLength> to_wstring(unsigned long value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%lu", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> wstring<MaxLength> to_wstring(unsigned long long value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%llu", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> wstring<MaxLength> to_wstring(float value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%f", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> wstring<MaxLength> to_wtring(double value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%f", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}

template <std::size_t MaxLength> wstring<MaxLength> to_wstring(long double value) {
	wstring<MaxLength> result;
	result.resize(MaxLength);
	int length = std::swprintf(result.data(), MaxLength + 1, "%Lf", value);
	if (length > MaxLength) {
		throw std::length_error("basic_string");
	}
	result.resize(length);
	return result;
}
///}

#if __cplusplus > 201703L
/// @section Helper template
template<typename CharT, typename Traits>
inline constexpr bool std::ranges::enable_safe_range<basic_string<CharT, Traits>> = true;
#endif

}

namespace std {

template<std::size_t MaxLength, typename CharT, typename Traits>
void swap(rttl::basic_string<MaxLength, CharT, Traits>& lhs, rttl::basic_string<MaxLength, CharT, Traits>& rhs) noexcept {
	lhs.swap(rhs);
}


template<std::size_t MaxLength1, std::size_t MaxLength2, typename CharT, typename Traits>
void swap(rttl::basic_string<MaxLength1, CharT, Traits>& lhs, rttl::basic_string<MaxLength2, CharT, Traits>& rhs) {
	lhs.swap(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits, typename Alloc>
void swap(rttl::basic_string<MaxLength, CharT, Traits>& lhs, std::basic_string<CharT, Traits, Alloc>& rhs) {
	lhs.swap(rhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits, typename Alloc>
void swap(std::basic_string<CharT, Traits, Alloc>& lhs, rttl::basic_string<MaxLength, CharT, Traits>& rhs) {
	rhs.swap(lhs);
}

template<std::size_t MaxLength, typename CharT, typename Traits, class U>
void erase(rttl::basic_string<MaxLength, CharT, Traits>& c, const U& value) {
	c.erase(std::remove(c.begin(), c.end(), value), c.end());
}

template<std::size_t MaxLength, typename CharT, typename Traits, class Pred>
void erase_if(rttl::basic_string<MaxLength, CharT, Traits>& c, Pred pred) {
	c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
}

template <std::size_t MaxLength, typename CharT, typename Traits>
class hash<rttl::basic_string<MaxLength, CharT, Traits>> {
public:
	size_t operator()(const rttl::basic_string<MaxLength, CharT, Traits>& s) const {
		std::basic_string_view<CharT, Traits> sv(s.data(), s.length());
		return std::hash<std::basic_string_view<CharT, Traits>>{}(sv);
	}
};

}


#endif // RTTL_STRING_H_

