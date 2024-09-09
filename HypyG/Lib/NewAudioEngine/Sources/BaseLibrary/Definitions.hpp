/*
 * Baund Studio Engine
 *
 * Definitions.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_Definitions_hpp
#define BaundStudioEngine_Sources_BaseLibrary_Definitions_hpp

#include <cstddef>
#include <string>
#include <vector>
#include <mutex>
#include <filesystem>

namespace Baund::StudioEngine::Error {
    struct NullPointAccess { };
}
namespace Baund::StudioEngine {
    using Size    = long;
    using Index   = long;
    using Count   = long;
    using String  = std::string;
    using CString = char const*;
    using Path    = std::filesystem::path;
    
    class BaseObject { // 반드시 가상상속을 해야 합니다.
    public:
        using Mutex = std::mutex;
        using Lock  = std::lock_guard<Mutex>;

        virtual ~BaseObject() = default;
        virtual auto reset () -> void = 0; // 객체의 완전 초기화.
        virtual auto getMutex() const -> Mutex& final;
        virtual auto getLock () const -> Lock   final;
        virtual bool isValid() const = 0; // 유효성을 검사. // deprecated.

        BaseObject() = default;
        BaseObject(BaseObject const&);
        BaseObject(BaseObject     &&);
        auto operator=(BaseObject const&) -> BaseObject&;
        auto operator=(BaseObject     &&) -> BaseObject&;
                
    private:
        struct Constants;       // k.
        struct NonCopyables;    // n.
        struct MemberVariables; // m.
        struct NonCopyables {
            mutable Mutex mutex;//std::mutex mutex;
        } n;
    };
    inline BaseObject::BaseObject(BaseObject const&) {
    }
    inline BaseObject::BaseObject(BaseObject&&) {
    }
    inline auto BaseObject::operator=(BaseObject const&) -> BaseObject& {
        return *this;
    }
    inline auto BaseObject::operator=(BaseObject&&) -> BaseObject& {
        return *this;
    }
    inline auto BaseObject::getMutex() const -> Mutex& {
        return n.mutex;
    }
    inline auto BaseObject::getLock () const -> Lock {
        return Lock(n.mutex);
    }
}

#endif//BaundStudioEngine_Sources_BaseLibrary_Definitions_hpp
