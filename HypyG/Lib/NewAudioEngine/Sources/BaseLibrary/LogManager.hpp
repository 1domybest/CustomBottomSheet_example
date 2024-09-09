/*
 * Baund Studio Engine
 *
 * LogManager.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_LogManager_hpp
#define BaundStudioEngine_Sources_BaseLibrary_LogManager_hpp

#include "Definitions.hpp"
#include "Algorithm.hpp"
#include "Sources/Support/iOSLogSupport.hpp"
#include "Sources/Support/AndroidLogSupport.hpp"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>

namespace Baund::StudioEngine {
    class Log final: public virtual BaseObject {
    public:
        enum  Level { none, debug, error };
        using Tag     = String;
        using Comment = String;
        using Value   = String;
        
        ~Log() override = default;
        Log()           = default;
        Log(Log const&) = default;
        Log(Log     &&);
        Log(Level const&, Tag const&, Comment const&, Value const& = {});
        Log(Level const&, Tag const&, Comment const&, CString const&);
        template< typename T >
        Log(Level const&, Tag const&, Comment const&, T const&);
        template< typename T >
        Log(Level const&, Tag const&, Comment const&, T* const&);

        auto operator=(Log const&) -> Log& = default;
        auto operator=(Log     &&) -> Log&;

        void reset  ()       override;
        bool isValid() const override;

        auto getLevel  () const -> Level   const&;
        auto getTag    () const -> Tag     const&;
        auto getComment() const -> Comment const&;
        auto getValue  () const -> Value   const&;
        auto print     () const -> void;

    private:
        struct MemberVariables {
            Level   level   = none;
            Tag     tag     = "";
            Comment comment = "";
            Value   value   = "";
        } m;
    };
    extern class LogManager final: public virtual BaseObject {
    public:
        using LogData = std::vector<Log>;
        using Path    = String;
        
        ~LogManager() override        = default;
        LogManager()                  = default;
        LogManager(LogManager const&) = delete;
        LogManager(LogManager     &&) = delete;
        
        auto operator=(LogManager const&) -> LogManager& = delete;
        auto operator=(LogManager     &&) -> LogManager& = delete;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto getLogData() const -> LogData const&;
        auto getLogSize() const -> Size;
        auto getLastLog() const -> Log const&;

        auto add(Log const&) -> void;
        auto add(Log     &&) -> void;
        
        auto writeLogToFile(Path const&) const -> Path;

        auto setDebugMode(bool const&) -> void;
        auto getDebugMode() const -> bool;
        
    private:
        struct Constants {
            static constexpr auto dataSize  = 4096;
        } k;
        struct MemberVariables {
            LogData logData   = LogData(Constants::dataSize);
            Index   point     = -1;
            bool    loop      = false;
            bool    debugMode = false;
        } m;
    } logManager;

    inline Log::Log(Log&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline Log::Log(Level const& level, Tag const& tag, Comment const& comment, Value const& value)
    : m({.level = level, .tag = tag, .comment = comment, .value = value}) {
    }
    inline Log::Log(Level const& level, Tag const& tag, Comment const& comment, CString const& value)
    : m({.level = level, .tag = tag, .comment = comment, .value = String(value)}) {
    }
    template< typename T >
    inline Log::Log(Level const& level, Tag const& tag, Comment const& comment, T const& value)
    : m({.level = level, .tag = tag, .comment = comment, .value = std::to_string(value)}) {
    }
    template< typename T >
    inline Log::Log(Level const& level, Tag const& tag, Comment const& comment, T* const& value)
    : m({.level = level, .tag = tag, .comment = comment,
        .value = (std::stringstream()
                  << "0x" << std::hex << std::setw(sizeof(void*) * 2) << std::setfill('0')
                  << reinterpret_cast<unsigned long long>(value)).str()
    }) {
    }
    inline auto Log::operator=(Log&& in) -> Log& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void Log::reset() {
        m = MemberVariables();
    }
    inline bool Log::isValid() const {
        return m.level != Level::none;
    }
    inline auto Log::getLevel  () const -> Level   const& { return m.level; }
    inline auto Log::getTag    () const -> Tag     const& { return m.tag; }
    inline auto Log::getComment() const -> Comment const& { return m.comment; }
    inline auto Log::getValue  () const -> Value   const& { return m.value; }
    inline auto Log::print     () const -> void {
        auto logLevel = (m.level == Log::debug) ? 'D' : (m.level == Log::error) ? 'E' : 'U';
        Support::printLog(logLevel, m.tag, m.comment, m.value);
    }

    inline void LogManager::reset() {
        m = MemberVariables();
    }
    inline bool LogManager::isValid() const {
        return m.point >= 0;
    }
    inline auto LogManager::getLogData() const -> LogData const& {
        return m.logData;
    }
    inline auto LogManager::getLogSize() const -> Size {
        if (m.loop) return k.dataSize;
        else        return m.point + 1;
    }
    inline auto LogManager::getLastLog() const -> Log const& {
        static const auto errorResult = Log();
        if (m.point < 0) return errorResult;
        else return m.logData[m.point];
    }
    inline auto LogManager::add(Log const& in) -> void {
        auto log = in;
        LogManager::add(std::move(log));
    }
    inline auto LogManager::add(Log&& in) -> void {
        if (in.getLevel() == Log::none) return;
        // 로그 추가.
        auto point = [this,&in] {
            if (++m.point >= k.dataSize) {
                m.point = 0;
                m.loop  = true;
            }
            m.logData[m.point] = std::move(in);
            return m.point;
        }();
        // 로그 화면출력.
        [this,point] {
            if (m.logData[point].getLevel() == Log::error) {
                m.logData[point].print();
            } else if (m.logData[point].getLevel() == Log::debug) {
                if (m.debugMode) {
                    m.logData[point].print();
                }
            }
        }();
    }
    inline auto LogManager::writeLogToFile(Path const& in) const -> Path {
        static const auto errorResult = Path();
        if (m.point < 0) return errorResult;
        // 파일경로 설정 및 생성.
        auto[filePath, stream] = [&in] {
            auto now  = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            auto path = in + "log_" + std::to_string(time) + ".txt";
            auto stream = std::ofstream(path);
            return std::pair{path, std::move(stream)};
        }();
        if (not stream.is_open()) return errorResult;
        // 파일에 쓰기
        [this, &stream = stream] {
            auto loop = m.loop;
            for (auto readPoint = (loop ? m.point : 0); loop or readPoint != (m.point + 1); ++readPoint) {
                if (readPoint >= k.dataSize) {
                    if (loop == true) {
                        readPoint = 0;
                        loop      = false;
                    } else break;
                }
                auto const& log = m.logData[readPoint];
                if (log.getLevel() == Log::none) continue;;
                auto str = (log.getLevel() == Log::debug ? "D/" : "E/") + log.getTag() + " " + log.getComment();
                if (log.getValue().size() != 0) str = str + ": " + log.getValue();
                stream << str << std::endl;
            }
        }();
        stream.close();
        return filePath;
    }
    inline auto LogManager::setDebugMode(bool const& in) -> void {
        m.debugMode = in;
    }
    inline auto LogManager::getDebugMode() const -> bool {
        return m.debugMode;
    }
}

#endif//BaundStudioEngine_Sources_BaseLibrary_LogManager_hpp
