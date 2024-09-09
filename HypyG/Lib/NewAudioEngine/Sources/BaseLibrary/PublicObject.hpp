/*
 * Baund Studio Engine
 *
 * PublicObject.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_PublicObject_hpp
#define BaundStudioEngine_Sources_BaseLibrary_PublicObject_hpp

#include "Definitions.hpp"
#include "Algorithm.hpp"
#include "LogManager.hpp"

namespace Baund::StudioEngine {
    class PublicObject: public virtual BaseObject {
    public:
        struct ID { long value; };
        using Name = String;
        static constexpr auto none = ID({.value = 0});

        virtual ~PublicObject() override = default;
        PublicObject()                   = default;
        PublicObject(PublicObject const&);
        PublicObject(PublicObject     &&);
        
        auto operator=(PublicObject const&) -> PublicObject&;
        auto operator=(PublicObject     &&) -> PublicObject&;
        
        virtual void reset  ()       override = 0;
        virtual bool isValid() const override = 0;

        virtual void        setObjectName(Name const&) final;
        virtual Name const& getObjectName() const final;
        virtual ID   const& getObjectID  () const final;
        
    private:
        static inline auto count = ID({.value = 1});
        struct Constants {
            ID const id = { .value = count.value++ };
        } k;
        struct MemberVariables {
            Name name = "";
        } m;
    };
    
    inline PublicObject::PublicObject(PublicObject const& in)
    : m(in.m) {
    }
    inline PublicObject::PublicObject(PublicObject&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto PublicObject::operator=(PublicObject const& in) -> PublicObject& {
        this->m = in.m;
        return *this;
    }
    inline auto PublicObject::operator=(PublicObject&& in) -> PublicObject& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline auto PublicObject::reset() -> void {
        m = MemberVariables();
    }
    inline auto PublicObject::isValid() const -> bool {
        return k.id.value != none.value;
    }
    inline auto PublicObject::setObjectName(Name const& in) -> void {
        m.name = in;
    }
    inline auto PublicObject::getObjectName() const -> Name const& {
        return m.name;
    }
    inline auto PublicObject::getObjectID() const -> ID const& {
        return k.id;
    }
}

#endif//BaundStudioEngine_Sources_BaseLibrary_PublicObject_hpp
