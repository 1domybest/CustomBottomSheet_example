/*
 * Baund Studio Engine
 *
 * List.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_List_hpp
#define BaundStudioEngine_Sources_BaseLibrary_List_hpp

#include "Definitions.hpp"
#include "Algorithm.hpp"
#include <list>
#include <vector>
#include <optional>
#include <functional>

namespace Baund::StudioEngine {
    template< typename _T >
    class List final: public virtual BaseObject {
    public:
        class Element;
        class Accessor;
        class Iterator;
        using Value         = _T;
        using Data          = std::list<Element>;
        using AccessorArray = std::vector<Accessor>;

        ~List() override  = default;
        List()            = default;
        List(List const&) = default;
        List(List     &&);

        auto operator=(List const&) -> List& = default;
        auto operator=(List     &&) -> List&;

        void reset  ()       override;
        bool isValid() const override;
        
        auto operator[](Index const&) const -> Accessor;

        auto add(Value    const&) -> Accessor;
        auto add(Value        &&) -> Accessor;
        auto sub(Value    const&) -> void;
        auto sub(Accessor const&) -> void;
        
        auto sort(std::function<bool(Element const&, Element const&)>) -> void;
        auto sort() -> void;

        auto search(std::function<bool(Value const&)>) const -> AccessorArray;
        auto search(Value const&) const -> AccessorArray;

        auto count() const -> Count;
        auto begin() const -> Iterator;
        auto end  () const -> Iterator;

    private:
        struct MemberVariables {
            Data data;
        } m;
    };
    template< typename _T >
    class List<_T>::Element final: public virtual BaseObject {
        friend List<_T>;
    public:
        using Storage = std::optional<Value>;
        
        ~Element() override     = default;
        Element()               = default;
        Element(Element const&) = default;
        Element(Element     &&);
        Element(Value   const&);
        Element(Value       &&);
        
        auto operator=(Element const&) -> Element& = default;
        auto operator=(Element     &&) -> Element&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto getValue()       -> Value&;
        auto getValue() const -> Value const&;
        
    private:
        struct MemberVariables {
            Storage storage;
        } m;
    };
    template< typename _T >
    class List<_T>::Accessor final: public virtual BaseObject {
        friend List<_T>;
    public:
        ~Accessor() override      = default;
        Accessor()                = default;
        Accessor(Accessor const&) = default;
        Accessor(Accessor     &&);
        Accessor(Element  const&);
        
        auto operator=(Accessor const&) -> Accessor& = default;
        auto operator=(Accessor     &&) -> Accessor&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto operator* () const -> Value&;
        auto operator->() const -> Value*;
        
        auto getValue  () const -> Value&;
        auto getElement() const -> Element&;
        
        auto operator==(Accessor const&) const -> bool;
        auto operator!=(Accessor const&) const -> bool;
        auto operator>=(Accessor const&) const -> bool;
        auto operator<=(Accessor const&) const -> bool;
        auto operator> (Accessor const&) const -> bool;
        auto operator< (Accessor const&) const -> bool;
        
    private:
        struct MemberVariables {
            Element* elementPtr = nullptr;
        } m;
    };
    template< typename _T >
    class List<_T>::Iterator final: public virtual BaseObject {
        friend List<_T>;
    public:
        ~Iterator() override      = default;
        Iterator(Iterator const&) = default;
        Iterator(Iterator     &&);
        Iterator(List const&, Index const&);

        auto operator=(Iterator const&) -> Iterator&;
        auto operator=(Iterator     &&) -> Iterator&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto operator* () const -> Accessor;
        auto operator->() const -> Accessor;
        
        auto operator+=(Count const&)       -> Iterator&;
        auto operator-=(Count const&)       -> Iterator&;
        auto operator+ (Count const&) const -> Iterator;
        auto operator- (Count const&) const -> Iterator;
        
        auto operator++()    -> Iterator&;
        auto operator--()    -> Iterator&;
        auto operator++(int) -> Iterator;
        auto operator--(int) -> Iterator;
        
        auto operator==(Iterator const&) const -> bool;
        auto operator!=(Iterator const&) const -> bool;
        auto operator>=(Iterator const&) const -> bool;
        auto operator<=(Iterator const&) const -> bool;
        auto operator> (Iterator const&) const -> bool;
        auto operator< (Iterator const&) const -> bool;
        
    private:
        struct Constants {
            List const& list;
        } k;
        struct MemberVariables {
            Index index;
        } m;
    };
    
    template< typename _T >
    inline List<_T>::Element::Element(Element&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    template< typename _T >
    inline List<_T>::Element::Element(Value const& value)
    : m({.storage = value}) {
    }
    template< typename _T >
    inline List<_T>::Element::Element(Value&& value)
    : m({.storage = std::move(value)}) {
    }
    template< typename _T >
    inline auto List<_T>::Element::operator=(Element&& in) -> Element& {
        if (this != in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    template< typename _T >
    inline void List<_T>::Element::reset() {
        m.storage.reset();
    }
    template< typename _T >
    inline bool List<_T>::Element::isValid() const {
        return m.storage.has_value();
    }
    template< typename _T >
    inline auto List<_T>::Element::getValue() -> Value& {
        return m.storage.value();
    }
    template< typename _T >
    inline auto List<_T>::Element::getValue() const -> Value const& {
        return m.storage.value();
    }

    template< typename _T >
    inline List<_T>::Accessor::Accessor(Accessor&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    template< typename _T >
    inline List<_T>::Accessor::Accessor(Element const& input)
    : m({.elementPtr = &const_cast<Element&>(input)}) {
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator=(Accessor&& in) -> Accessor& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    template< typename _T >
    inline void List<_T>::Accessor::reset() {
        m = MemberVariables();
    }
    template< typename _T >
    inline bool List<_T>::Accessor::isValid() const {
        return m.elementPtr and m.elementPtr->isValid();
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator* () const -> Value& {
        if (not m.elementPtr) throw Error::NullPointAccess();
        return m.elementPtr->getValue();
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator->() const -> Value* {
        if (not m.elementPtr) throw Error::NullPointAccess();
        return &(m.elementPtr->getValue());
    }
    template< typename _T >
    inline auto List<_T>::Accessor::getValue() const -> Value& {
        if (not m.elementPtr) throw Error::NullPointAccess();
        return m.elementPtr->getValue();
    }
    template< typename _T >
    inline auto List<_T>::Accessor::getElement() const -> Element& {
        if (not m.elementPtr) throw Error::NullPointAccess();
        return *(m.elementPtr);
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator==(Accessor const& in) const -> bool {
        return this->m.elementPtr == in.m.elementPtr;
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator!=(Accessor const& in) const -> bool {
        return this->m.elementPtr != in.m.elementPtr;
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator>=(Accessor const& in) const -> bool {
        return this->m.elementPtr >= in.m.elementPtr;
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator<=(Accessor const& in) const -> bool {
        return this->m.elementPtr <= in.m.elementPtr;
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator> (Accessor const& in) const -> bool {
        return this->m.elementPtr >  in.m.elementPtr;
    }
    template< typename _T >
    inline auto List<_T>::Accessor::operator< (Accessor const& in) const -> bool {
        return this->m.elementPtr <  in.m.elementPtr;
    }

    template< typename _T >
    inline List<_T>::Iterator::Iterator(Iterator&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    template< typename _T >
    inline List<_T>::Iterator::Iterator(List const& list, Index const& index)
    : k({list}), m({index}) {
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator=(Iterator const& in) -> Iterator& {
        if (&(this->k.list) == &(in.k.list)) this->m = in.m;
        return *this;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator=(Iterator&& in) -> Iterator& {
        if(this != &in) {
            if(&(this->mListRef) == &(in.mListRef)) this->member = std::move(in.member);
            in.member = MemberVariables();
        }
        return *this;
    }
    template< typename _T >
    inline void List<_T>::Iterator::reset() {
        m = MemberVariables();
    }
    template< typename _T >
    inline bool List<_T>::Iterator::isValid() const {
        return k.list[m.index].isValid();
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator*() const -> Accessor {
        return k.list[m.index];
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator->() const -> Accessor {
        return k.list[m.index];
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator+=(Count const& input) -> Iterator& {
        m.index += input;
        return *this;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator-=(Count const& input) -> Iterator& {
        m.index -= input;
        return *this;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator+(Count const& input) const -> Iterator {
        return {k.list, m.index+input};
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator-(Count const& input) const -> Iterator {
        return {k.list, m.index-input};
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator++() -> Iterator& {
        ++m.index;
        return *this;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator--() -> Iterator& {
        --m.index;
        return *this;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator++(int) -> Iterator {
        return {k.list, m.index++};
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator--(int) -> Iterator {
        return {k.list, m.index--};
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator==(Iterator const& input) const -> bool {
        return this->m.index == input.m.index;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator!=(Iterator const& input) const -> bool {
        return this->m.index != input.m.index;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator>=(Iterator const& input) const -> bool {
        return this->m.index >= input.m.index;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator<=(Iterator const& input) const -> bool {
        return this->m.index <= input.m.index;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator>(Iterator const& input) const -> bool {
        return this->m.index > input.m.index;
    }
    template< typename _T >
    inline auto List<_T>::Iterator::operator<(Iterator const& input) const -> bool {
        return this->m.index < input.m.index;
    }
    
    template< typename _T >
    inline List<_T>::List(List&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    template< typename _T >
    inline auto List<_T>::operator=(List&& in) -> List& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    template< typename _T >
    inline void List<_T>::reset() {
        m = MemberVariables();
    }
    template< typename _T >
    inline bool List<_T>::isValid() const {
        return not m.data.empty();
    }
    template< typename _T >
    inline auto List<_T>::operator[](Index const& input) const -> Accessor {
        auto size  = this->count();
        auto count = 0;
        for (auto const& element : m.data) {
            if (not element.isValid()) continue;
            else if (count == input) return element;
            else count += 1;
            if (count >= size) break;
        }
        return { };
    }
    template< typename _T >
    inline auto List<_T>::add(Value const& input) -> Accessor {
        m.data.push_back(input);
        return m.data.back();
    }
    template< typename _T >
    inline auto List<_T>::add(Value&& input) -> Accessor {
        m.data.push_back(std::move(input));
        return m.data.back();
    }
    template< typename _T >
    inline auto List<_T>::sub(Value const& input) -> void {
        for (auto& element : m.data) {
            if (not element.isValid()) continue;
            else if (element.getValue() == input) {
                element.reset();
            }
        }
    }
    template< typename _T >
    inline auto List<_T>::sub(Accessor const& input) -> void {
        for (auto& element : m.data) {
            if (not element.isValid()) continue;
            else if (&element == input.m.elementPtr) {
                element.reset();
                return;
            }
        }
    }
    template< typename _T >
    inline auto List<_T>::sort(std::function<bool(Element const&, Element const&)> func) -> void {
        return m.data.sort(func);
    }
    template< typename _T >
    inline auto List<_T>::sort() -> void {
        return m.data.sort([](Element const& lhs, Element const& rhs){
            if (not (lhs.isValid() and rhs.isValid()))
                return lhs.isValid();
            else return lhs.getValue() < rhs.getValue();
        });
    }
    template< typename _T >
    inline auto List<_T>::search(std::function<bool(_T const&)> func) const -> AccessorArray {
        AccessorArray result;
        for (auto const& element : m.data) {
            if (not element.isValid()) continue;
            else if (func(element.getValue()))
                result.push_back(element);
        }
        return result;
    }
    template< typename _T >
    inline auto List<_T>::search(Value const& input) const -> AccessorArray {
        AccessorArray result;
        for (auto const& element : m.data) {
            if (not element.isValid()) continue;
            else if (element.getValue() == input)
                result.push_back(element);
        }
        return result;
    }
    template< typename _T >
    inline auto List<_T>::count() const -> Count {
        auto count = 0;
        for (auto const& element : m.data) {
            if (element.isValid()) count += 1;
        }
        return count;
    }
    template< typename _T >
    inline auto List<_T>::begin() const -> Iterator {
        return {*this, 0};
    }
    template< typename _T >
    inline auto List<_T>::end() const -> Iterator {
        return {*this, count()};
    }
}

#endif//BaundStudioEngine_Sources_BaseLibrary_List_hpp
