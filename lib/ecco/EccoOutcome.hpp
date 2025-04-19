#ifndef ECCOOUTCOME_HPP
#define ECCOOUTCOME_HPP

#include "EccoObject.hpp"
#include <string>
#include <optional>
#include <iostream>

namespace ecco {
enum class OutcomeStatus : std::uint8_t { Success, Failure };

struct TraceFrame {
    public:
        TraceFrame(std::string file, int line, std::string func, std::string_view message = {}) :
        m_file(file),
        m_lineNumber(line),
        m_functionName(func),
        m_message(message)
        {
        };

        std::string m_file;
        int m_lineNumber;
        std::string m_functionName;
        std::string m_message;
};

//Defaulting this to void so we can do EccoOutcome<> instead of EccoOutcome<void> explicitly. This gets using'd away anyways
template<typename T = void>
class EccoOutcome {
public:
        static EccoOutcome Success(T v) { return EccoOutcome(std::move(v)); }
        static EccoOutcome Failure(std::string_view msg = {}) {return EccoOutcome(msg); }
        //using stringviews seems moot here but good practice since there is no copying, i could also
        //just std::move(msg) like i do elsewhere

        //Status Getters
        bool IsSuccess() const { return m_status == OutcomeStatus::Success; }
        bool IsFailure() const { return !IsSuccess(); };
        //this is cool
        explicit operator bool() const { return IsSuccess(); };


        //Value getters
        T& Value() { return *m_value; };
        const T& Value() const { return *m_value; };
        //Maybe we want to return the optional itself?
        const std::optional<T>& Maybe() const { return m_value; };

        void AddTrace(std::string file, int line, std::string function, std::string_view msg = {}) {
            m_stackTrace.emplace_back(file,line,function,msg);
        }
        const std::vector<TraceFrame>& Trace() const { return m_stackTrace; };

        void Print(std::ostream& os = std::cout) const {
        if (IsSuccess()) {
            if constexpr (!std::is_same_v<T, void>) {
                os << "[Success] Value: " << Value() << std::endl;;
            } else {
                os << "[Success] " << std::endl;;
            }
            return;
        }

        os << "[Failure]" << std::endl;;
        for (size_t i = 0; i < m_stackTrace.size(); ++i) {
            const auto& frame = m_stackTrace[i];
            std::string indent(i * 2, ' ');
            os << indent << "-> at ";
            os << frame.m_functionName;
            os << " (" << frame.m_file << ":" << frame.m_lineNumber << ")";
            if (!frame.m_message.empty()) os << " -- " << frame.m_message;
            os << std::endl;;
        }
    }


private:
        //These aren't called directrly Success/Failure static funcs will call these
        //Success constructor
        explicit EccoOutcome(T&& v) : m_status{OutcomeStatus::Success}, m_value{std::move(v)} {};
        //Fail constructor
        explicit EccoOutcome(std::string_view msg) : m_status{OutcomeStatus::Failure} {
            m_stackTrace.emplace_back("",0,"",msg);
        };

        OutcomeStatus m_status;
        std::optional<T> m_value;
        std::vector<TraceFrame> m_stackTrace;
};

//annoying but optional cant take void so for the return status only type Outcome<void> we have to make an
//explicit template instantiation
template<>
class EccoOutcome<void>  {
public:
    static EccoOutcome Success()                          { return EccoOutcome(true); }
    static EccoOutcome Failure(std::string_view msg = {}) { return EccoOutcome(false,msg); }

    bool IsSuccess() const { return m_status == OutcomeStatus::Success; }
    bool IsFailure() const { return !IsSuccess(); }
    explicit operator bool() const { return IsSuccess(); }

    void AddTrace(std::string file, int line, std::string function, std::string_view msg = {}) {
        m_stackTrace.emplace_back(file,line,function,msg);
    }
    const std::vector<TraceFrame>& Trace() const { return m_stackTrace; }

    void Print(std::ostream& os = std::cout) const {
        if (IsSuccess()) {
                os << "[Success] " << std::endl;
            return;
        }

        os << "[Failure] " << std::endl;;
        for (size_t i = 0; i < m_stackTrace.size(); ++i) {
            const auto& frame = m_stackTrace[i];
            std::string indent(i * 2, ' ');
            os << indent << "-> at ";
            os << frame.m_functionName;
            os << " (" << frame.m_file << ":" << frame.m_lineNumber << ")";
            if (!frame.m_message.empty()) os << " -- " << frame.m_message;
            os << std::endl;;
        }
    }

private:
    explicit EccoOutcome(bool ok, std::string_view msg = {}) :
        m_status{ ok ? OutcomeStatus::Success : OutcomeStatus::Failure }
    {
        if (!ok)
            m_stackTrace.emplace_back("",0,"",msg);
    }

    OutcomeStatus               m_status;
    std::vector<TraceFrame>     m_stackTrace;

};

using StatusOutcome = EccoOutcome<>;
template<typename T>
using OutcomeData = EccoOutcome<T>;


#define RETURN_ON_FAIL(func)                                          \
    do {                                                              \
        auto outcome = (func);                                        \
        if (!outcome) {                                               \
            outcome.AddTrace(__FILE__, __LINE__, __func__);           \
            return outcome;                                             \
        }                                                               \
    } while (false)                                                     \


} // namespace ecco

#endif
