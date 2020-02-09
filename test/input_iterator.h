#ifndef INPUT_ITERATOR_H_
#define INPUT_ITERATOR_H_
#include <iterator>

template <typename Iter>
class InputIterator{
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = typename std::iterator_traits<Iter>::value_type;
    using difference_type = typename std::iterator_traits<Iter>::difference_type;
    using pointer = typename std::iterator_traits<Iter>::pointer;
    using reference = typename std::iterator_traits<Iter>::reference;

    explicit InputIterator(Iter it) : m_it(it) {}
    InputIterator(const InputIterator&) = default;
    InputIterator(InputIterator&&) = default;
    InputIterator& operator=(const InputIterator&) = default;
    InputIterator& operator=(InputIterator&&) = default;
    ~InputIterator() = default;

    InputIterator& operator++() {
        ++m_it; return *this;
    }

    InputIterator operator++(int) {
        InputIterator result;
        ++m_it;
        return result;
    }

    bool operator==(const InputIterator& other) { return  m_it == other.m_it; }
    bool operator!=(const InputIterator& other) { return  m_it != other.m_it; }

    reference operator*() const { return *m_it; }

private:
    Iter m_it;
};

#endif // INPUT_ITERATOR_H_
