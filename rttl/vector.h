/**
 * @file rttl/vector.h
 *
 * Vector container with statically allocated storage.
 *
 * Provides similar behaviour as `std::vector` with following exclusions:
 *  - maximum elements count in vector, `max_size()`, is limited by the
 *    `MaxSize` template argument;
 *  - because the memory is allocated within the class, it does not takes
 *    Allocator template argument neither itself nor for any member functions,
 *    i.e. does not meet "AllocatorAwareContainer" requirements; for the same
 *    reason, `allocator_type` member type and `get_allocator` member function
 *    are not provided;
 *  - insertion or removal of elements at the end are constant `O(1)` instead of
 *    amortized constant;
 *  - no specialization for `rttl::vector<bool>` provided; that should not lead
 *    to any compatibility issue, but is space-inefficient;
 *  - no opearations change capacity, so iterator invalidation is not possible
 *    for that reason;
 *  - move constructors and move assignment operator behave like those of
 *    `std::vector` with incompatible allocators provided;
 *  - `swap` member function has `O(n)` complexity, do invoke swap and move
 *    operations on elements, and can throw exceptions;
 *  - no `std::swap` overload provided for this class template, so it is
 *    inefficient and makes large allocations on stack; instead, non-member
 *    `rttl::swap` is provided to enable argument-dependent lookup (ADL), thus
 *    fulfilling "Swappable" requirements;
 *  - `std::erase` and `std::erase_if` overloads are not provided;
 *  - `pop_back` operation does not cause undefined behaviour when called on
 *    empty container; it is defined to throw an exception;
 *
 * Important notes on usage:
 *  1. Be careful with placing vectors with `rttl::vector` instantiations on the
 *     stack, as they might cause stack overflow, depending on `MaxSize`,
 *     `sizeof(T)` and available stack size.
 *  2. Using member function `assign` with iterator that does not fulfill
 *     ForwardIterator requirement causes temporary `rttl::vector` to be on the
 *     stack - see note 1.
 *
 */
#ifndef RTTL_VECTOR_H_
#define RTTL_VECTOR_H_
#include <cstdlib>
#include <algorithm>
#include <array>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace rttl {

template <typename T, std::size_t MaxSize>
class vector {
    static_assert(std::is_destructible<T>::value,
                  "T must meet requirements of Erasable");
public:

    /// @section Member types

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /// @section Member functions

    /**
     * @name (constructor)
     */
    ///{
    vector() noexcept = default;

    vector(size_type count, const T& value) {
        assign(count, value);
    }

    explicit vector(size_type count) {
        resize(count);
    }

    template<typename InputIt>
    vector(InputIt first, InputIt last) {
        insert(cbegin(), first, last);
    }

    vector(const vector& other) {
        *this = other;
    }

    template <std::size_t MaxSize1>
    vector(const vector<T,MaxSize1>& other) {
        *this = other;
    }

    vector(vector&& other) {
        *this = std::move(other);
    }

    template <std::size_t MaxSize1>
    vector(vector<T,MaxSize1>&& other) {
        *this = std::move(other);
    }

    vector(std::initializer_list<T> ilist) {
        assign(ilist);
    }
    ///}

    /**
     * @name (destructor)
     */
    ///{
    ~vector() {
        std::destroy(begin(), end());
    }
    ///}

    /**
     * @name operator=
     */
    ///{
    template <std::size_t MaxSize1>
    vector& operator=(const vector<T,MaxSize1>& other) {
        if (other.size() > max_size()) {
            throw std::length_error("rttl::vector");
        }
        clear();
        insert(cbegin(), other.cbegin(), other.cend());
        return *this;
    }

    vector& operator=(const vector& other) {
        clear();
        insert(cbegin(), other.cbegin(), other.cend());
        return *this;
    }

    template <std::size_t MaxSize1>
    vector& operator=(vector<T,MaxSize1>&& other) {
        if (other.size() > max_size()) {
            throw std::length_error("rttl::vector");
        }
        clear();
        insert(cbegin(), std::make_move_iterator(other.cbegin()),
                         std::make_move_iterator(other.cend()));
        return *this;
    }

    vector& operator=(vector&& other) {
        clear();
        insert(cbegin(), std::make_move_iterator(other.cbegin()),
                         std::make_move_iterator(other.cend()));
        return *this;
    }

    vector& operator=(std::initializer_list<T> ilist) {
        assign(ilist);
        return *this;
    }
    ///}

    /**
     * @name
     */
    ///{
    void assign(size_type count, const T& value) {
        if (count > max_size()) {
            throw std::length_error("rttl::vector");
        }
        clear();
        resize(count, value);
    }

    template<typename InputIt>
    typename std::enable_if<std::is_base_of<std::input_iterator_tag,
    typename std::iterator_traits<InputIt>::iterator_category>::value>::type
    assign(InputIt first, InputIt last) {
        if constexpr((std::is_base_of<std::forward_iterator_tag, typename
                    std::iterator_traits<InputIt>::iterator_category>::value)) {
            size_type count = std::distance(first, last);
            if (count > max_size()) {
                throw std::length_error("rttl::vector");
            }
            clear();
            std::uninitialized_copy(first, last, begin());
            m_length = count;
        } else {
            vector tmp(first, last);
            *this = tmp;
        }
    }
    ///}

    void assign(std::initializer_list<T> ilist) {
        assign(ilist.begin(), ilist.end());
    }

    /// @subsection Element access

    /**
     * @name at
     */
    ///{
    reference at(size_type pos) {
        if (pos >= size()) {
            throw std::out_of_range("rttl::vector");
        }
        return this->operator[](pos);
    }

    const_reference at(size_type pos) const {
        if (pos >= size()) {
            throw std::out_of_range("rttl::vector");
        }
        return this->operator[](pos);
    }
    ///}

    /**
     * @name operator[]
     */
    ///{
    reference operator[](size_type pos) {
        return *(begin() + pos);
    }

    const_reference operator[](size_type pos) const {
        return *(begin() + pos);
    }
    ///}

    /**
     * @name front
     */
    ///{
    reference front() noexcept {
        return *begin();
    }

    const_reference front() const noexcept {
        return *begin();
    }
    ///}

    /**
     * @name back
     */
    ///{
    reference back() noexcept {
        return *(begin() + (size() - 1));
    }

    const_reference back() const noexcept {
        return *(begin() + (size() - 1));
    }
    ///}

    /**
     * @name data
     */
    ///{
    T* data() noexcept {
        return reinterpret_cast<T*>(&m_data);
    }

    const T* data() const noexcept {
        return reinterpret_cast<const T*>(&m_data);
    }
    ///}


    /// @subsection Iterators

    /**
     * @name begin
     */
    ///{
    iterator begin() noexcept {
        return data();
    }

    const_iterator begin() const noexcept {
        return data();
    }

    const_iterator cbegin() const noexcept {
        return data();
    }
    ///}

    /**
     * @name end
     */
    ///{
    iterator end() noexcept {
        return begin() + size();
    }

    const_iterator end() const noexcept {
        return cbegin() + size();
    }

    const_iterator cend() const noexcept {
        return cbegin() + size();
    }
    ///}

    /**
     * @name rbegin
     */
    ///{
    reverse_iterator rbegin() noexcept {
        return std::reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return std::reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept {
        return std::reverse_iterator(end());
    }
    ///}

    /**
     * @name rend
     */
    ///{
    reverse_iterator rend() noexcept {
        return std::reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return std::reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept {
        return std::reverse_iterator(begin());
    }
    ///}


    /// @subsection Capacity

    [[nodiscard]] bool empty() const noexcept {
        return size() == 0;
    }

    size_type size() const noexcept {
        return m_length;
    }

    static constexpr size_type max_size() noexcept {
        return MaxSize;
    }

    void reserve(size_type new_cap) {
        if (new_cap > max_size()) {
            throw std::length_error("rttl::vector");
        }
    }

    static constexpr size_type capacity() noexcept {
        return MaxSize;
    }

    void shrink_to_fit() {}


    /// @subsection Modifiers

    void clear() noexcept {
        std::destroy(begin(), end());
        m_length = 0;
    }

    /**
     * @name insert
     */
    ///{
    iterator insert(const_iterator pos, const T& value) {
        return insert(pos, 1, value);
    }


    iterator insert(const_iterator pos, T&& value) {
        if (size() >= max_size()) {
            throw std::length_error("rttl::vector");
        }
        iterator it = begin() + (pos - cbegin());
        if (pos == cend()) {
            std::uninitialized_fill_n(end(), 1, value);
        } else {
            std::uninitialized_move_n(cend() - 1, 1, end());
            std::move_backward(pos, cend() - 1, end());
            *it = value;
        }
        ++m_length;
        return it;
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        if (size() + count > max_size()) {
            throw std::length_error("rttl::vector");
        }
        iterator d_first = begin() + (pos - cbegin());
        iterator d_last = d_first + count;
        if (d_last < end()) {
            std::uninitialized_move(cend() - count, cend(), end());
            std::move_backward(pos, cend() - count, end());
            std::fill(d_first, d_last, value);
        } else {
            std::uninitialized_move(pos, cend(), d_last);
            std::fill(d_first, end(), value);
            std::uninitialized_fill(end(), d_last, value);
        }
        m_length += count;
        return d_first;
    }

    template<typename InputIt>
    typename std::enable_if<std::is_base_of<std::input_iterator_tag,
            typename std::iterator_traits<InputIt>::iterator_category>::value,
            iterator>::type
    insert(const_iterator pos, InputIt first, InputIt last) {
        iterator d_first = begin() + (pos - cbegin());
        if constexpr(std::is_base_of<std::forward_iterator_tag, typename
                     std::iterator_traits<InputIt>::iterator_category>::value) {
            /// Get number of elements to be inserted prior to insertion
            size_type count = std::distance(first, last);
            if (size() + count > max_size()) {
                throw std::length_error("rttl::vector");
            }
            iterator d_last = d_first + count;
            if (d_last < end()) {
                std::uninitialized_move(cend() - count, cend(), end());
                std::move_backward(pos, cend() - count, end());
                std::copy(first, last, d_first);
            } else {
                std::uninitialized_move(pos, cend(), d_last);
                iterator it = d_first;
                while (it != end()) {
                    *it = *first;
                    ++first;
                    ++it;
                }
                std::uninitialized_copy(first, last, end());
            }
            m_length += count;
        } else {
            /// It is impossible to get number of elements to be inserted prior
            /// to insertion
            /// Assuming maximum possible number of elements that does not throw
            /// Move trailing part to the end of allocated space
            size_type count = MaxSize - size();
            iterator d_last = begin() + MaxSize;
            iterator d_first = d_last - (cend() - pos);
            iterator d_uninit = std::max(d_first, end());
            const_iterator s_uninit = pos + (d_uninit - d_first);
            std::uninitialized_move(s_uninit, cend(), d_uninit);
            std::move_backward(pos, s_uninit, end());
            /// Try to insert new elements
            iterator it = begin() + (pos - cbegin());
            while (first != last) {
                /// If the element to be inserted does not fit, recover the
                /// initial state and throw an exception
                if (it == pos + count) {
                    /// All elements were initialized, so the task is simple
                    std::move(d_first, d_last, begin() + (pos - cbegin()));
                    std::destroy(end(), d_last);
                    throw std::length_error("rttl::vector");
                }
                /// Assign an inserted value
                if (it < end()) {
                    std::copy_n(first, 1, it);
                } else {
                    std::uninitialized_copy_n(first, 1, it);
                }
                /// Advance iterators
                ++it;
                ++first;
            }

            /// Remember how many elements were added
            count = it - pos;

            /// Move elemets from the end of the allocated space to the proper
            /// position
            iterator s_first = d_first;
            iterator s_last = d_last;

            d_first = it;
            d_last = d_first + (s_last - s_first);

            iterator d_first_uninit = std::min(std::max(d_first, end()),
                                               d_last);
            iterator d_last_uninit = std::min(s_first, d_last);

            /// There is an unitialized hole in the following range
            iterator s_first_uninit = s_first + (d_first_uninit - d_first);
            iterator s_last_uninit = s_first + (d_last_uninit - d_first);

            std::move(s_first, s_first_uninit, d_first);
            std::uninitialized_move(s_first_uninit, s_last_uninit,
                                    d_first_uninit);
            std::move(s_last_uninit, s_last, d_last_uninit);

            iterator destroy_first = std::max(d_last, s_first);
            iterator destroy_last = s_last;
            std::destroy(destroy_first, destroy_last);

            m_length += count;
        }
        return d_first;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }
    ///}

    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        if (size() >= max_size()) {
            throw std::length_error("rttl::vector");
        }
        iterator it = begin() + (pos - cbegin());
        if (pos != cend()) {
            std::uninitialized_move_n(cend() - 1, 1, end());
            std::move_backward(pos, cend() - 1, end());
            it->~Element();
        }
        ::new(it) T(std::forward<Args>(args)...);
        ++m_length;
        return it;
    }

    /**
     * @name erase
     */
    ///{
    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        iterator result = std::move(last, cend(), begin() + (first - cbegin()));
        size_type elems_erased = last - first;
        std::destroy(end() - elems_erased, end());
        m_length -= elems_erased;
        return result;
    }
    ///}

    /**
     * @name push_back
     */
    ///{
    void push_back(const T& value) {
        insert(cend(), value);
    }

    void push_back(T&& value) {
        insert(cend(), value);
    }
    ///}

    template<typename... Args>
    reference emplace_back(Args&&... args) {
        emplace(cend(), std::forward<Args>(args)...);
        return back();
    }

    void pop_back() {
        if (empty()) {
            throw std::invalid_argument("rttl::vector");
        }
        resize(size() - 1);
    }

    /**
     * @name resize
     */
    ///{
    void resize(size_type count) {
        if (count > max_size()) {
            throw std::length_error("rttl::vector");
        }
        if (count > size()) {
            std::uninitialized_fill(end(), begin() + count, T());
        } else {
            std::destroy(begin() + count, end());
        }
        m_length = count;
    }

    void resize(size_type count, const value_type& value) {
        if (count > max_size()) {
            throw std::length_error("rttl::vector");
        }
        if (count > size()) {
            std::uninitialized_fill(end(), begin() + count, value);
        } else {
            std::destroy(begin() + count, end());
        }
        m_length = count;
    }
    ///}

    template<size_type MaxSize2>
    void swap(vector<T,MaxSize2>& other) noexcept (MaxSize2 == MaxSize) {
        if constexpr(MaxSize2 != MaxSize) {
            if (other.size() > max_size() || size() > other.max_size()) {
                throw std::length_error("rttl::vector");
            }
        }
        size_type swap_len = std::min(size(), other.size());
        std::swap_ranges(begin(), begin() + swap_len, other.begin());
        if (other.size() > size()) {
            std::uninitialized_move(other.cbegin() + swap_len, other.cend(),
                                    end());
            m_length = other.size();
            other.resize(swap_len);
        } else {
            std::uninitialized_move(cbegin() + swap_len, cend(), other.end());
            other.m_length = size();
            resize(swap_len);
        }
    }

    template<typename Alloc>
    void swap(std::vector<T,Alloc>& other) {
        if (other.size() > max_size() || size() > other.max_size()) {
            throw std::length_error("rttl::vector");
        }
        size_type swap_len = std::min(size(), other.size());
        std::swap_ranges(begin(), begin() + swap_len, other.begin());
        if (other.size() > size()) {
            std::uninitialized_move(other.cbegin() + swap_len, other.cend(),
                                    end());
            m_length = other.size();
            other.resize(swap_len);
        } else {
            other.reserve(size());
            other.insert(other.cend(),
                         std::make_move_iterator(begin() + swap_len),
                         std::make_move_iterator(end()));
            resize(swap_len);
        }
    }

private:
    std::array<typename std::aligned_storage<sizeof(T),alignof(T)>::type,
               MaxSize> m_data;

    size_type m_length = 0;

    /// Friend declaration to allow `swap` with `rttl::vector` of different
    /// `MaxSize`
    template<typename, std::size_t> friend class vector;

};


/// @section Non-member functions

/**
 * @name operator==
 */
///{
template <typename T, std::size_t MaxSize1, std::size_t MaxSize2>
bool operator==(const vector<T,MaxSize1>& lhs, const vector<T,MaxSize2>& rhs) {
    return (lhs.size() == rhs.size()) && std::equal(lhs.cbegin(), lhs.cend(),
                                                    rhs.cbegin());
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator==(const vector<T,MaxSize>& lhs, const std::vector<T,Alloc>& rhs) {
    return (lhs.size() == rhs.size()) && std::equal(lhs.cbegin(), lhs.cend(),
                                                    rhs.cbegin());
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator==(const std::vector<T,Alloc>& lhs, const vector<T,MaxSize>& rhs) {
    return (lhs.size() == rhs.size()) && std::equal(lhs.cbegin(), lhs.cend(),
                                                    rhs.cbegin());
}
///}

/**
 * @name operator!=
 */
///{
template <typename T, std::size_t MaxSize1, std::size_t MaxSize2>
bool operator!=(const vector<T,MaxSize1>& lhs, const vector<T,MaxSize2>& rhs) {
    return !(lhs == rhs);
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator!=(const vector<T,MaxSize>& lhs, const std::vector<T,Alloc>& rhs) {
    return !(lhs == rhs);
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator!=(const std::vector<T,Alloc>& lhs, const vector<T,MaxSize>& rhs) {
    return !(lhs == rhs);
}
///}

/**
 * @name operator<
 */
///{
template <typename T, std::size_t MaxSize1, std::size_t MaxSize2>
bool operator<(const vector<T,MaxSize1>& lhs, const vector<T,MaxSize2>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
                                        rhs.cbegin(), rhs.cend());
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator<(const vector<T,MaxSize>& lhs, const std::vector<T,Alloc>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
                                        rhs.cbegin(), rhs.cend());
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator<(const std::vector<T,Alloc>& lhs, const vector<T,MaxSize>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
                                        rhs.cbegin(), rhs.cend());
}
///}

/**
 * @name operator>
 */
///{
template <typename T, std::size_t MaxSize1, std::size_t MaxSize2>
bool operator>(const vector<T,MaxSize1>& lhs, const vector<T,MaxSize2>& rhs) {
    return (rhs < lhs);
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator>(const vector<T,MaxSize>& lhs, const std::vector<T,Alloc>& rhs) {
    return (rhs < lhs);
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator>(const std::vector<T,Alloc>& lhs, const vector<T,MaxSize>& rhs) {
    return (rhs < lhs);
}
///}

/**
 * @name operator<=
 */
///{
template <typename T, std::size_t MaxSize1, std::size_t MaxSize2>
bool operator<=(const vector<T,MaxSize1>& lhs, const vector<T,MaxSize2>& rhs) {
    return !(rhs < lhs);
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator<=(const vector<T,MaxSize>& lhs, const std::vector<T,Alloc>& rhs) {
    return !(rhs < lhs);
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator<=(const std::vector<T,Alloc>& lhs, const vector<T,MaxSize>& rhs) {
    return !(rhs < lhs);
}
///}

/**
 * @name operator>=
 */
///{
template <typename T, std::size_t MaxSize1, std::size_t MaxSize2>
bool operator>=(const vector<T,MaxSize1>& lhs, const vector<T,MaxSize2>& rhs) {
    return !(lhs < rhs);
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator>=(const vector<T,MaxSize>& lhs, const std::vector<T,Alloc>& rhs) {
    return !(lhs < rhs);
}

template <typename T, std::size_t MaxSize, typename Alloc>
bool operator>=(const std::vector<T,Alloc>& lhs, const vector<T,MaxSize>& rhs) {
    return !(lhs < rhs);
}
///}

/**
 * @name swap
 */
///{
template<typename T, std::size_t MaxSize1, std::size_t MaxSize2>
void swap(rttl::vector<T,MaxSize1>& lhs,
          rttl::vector<T,MaxSize2>& rhs) noexcept (MaxSize1 == MaxSize2) {
    lhs.swap(rhs);
}

template<typename T, std::size_t MaxSize, typename Alloc>
void swap(rttl::vector<T,MaxSize>& lhs, std::vector<T,Alloc>& rhs) {
    lhs.swap(rhs);
}

template<typename T, std::size_t MaxSize, typename Alloc>
void swap(std::vector<T,Alloc>& lhs, rttl::vector<T,MaxSize>& rhs) {
    rhs.swap(lhs);
}
///}

}

#endif // RTTL_VECTOR_H_
