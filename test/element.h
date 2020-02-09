#ifndef TEST_ELEMENT_H_
#define TEST_ELEMENT_H_
#include <unordered_set>
#include <UnitTest++/UnitTest++.h>

class Element;

static std::unordered_set<Element*> s_elems_ctored;

class Element {
public:
    Element(int x = 0) {
        CHECK_EQUAL(s_elems_ctored.count(this), 0);
        s_elems_ctored.insert(this);
        m_x = x;
    }

    Element(const Element& other) {
        CHECK_EQUAL(s_elems_ctored.count(this), 0);
        s_elems_ctored.insert(this);
        m_x = other.m_x;
    }

    Element(Element&& other) {
        CHECK_EQUAL(s_elems_ctored.count(this), 0);
        s_elems_ctored.insert(this);
        m_x = other.m_x;
        other.m_x = -1;
    }

    Element& operator=(const Element& other) {
        CHECK_EQUAL(s_elems_ctored.count(this), 1);
        s_elems_ctored.insert(this);
        m_x = other.m_x;
        return *this;
    }

    Element& operator=(Element&& other) {
        CHECK_EQUAL(s_elems_ctored.count(this), 1);
        s_elems_ctored.insert(this);
        m_x = other.m_x;
        other.m_x = -1;
        return *this;
    }


    ~Element() {
        m_x = -1;
        try {
            CHECK_EQUAL(s_elems_ctored.count(this), 1);
        }
        catch (...) {

        }
        s_elems_ctored.erase(this);
    }

    operator int() const {
        return m_x;
    }

private:
    int m_x;

};

#endif // TEST_ELEMENT_H_
