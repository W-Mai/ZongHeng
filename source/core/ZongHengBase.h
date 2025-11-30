//
// ZongHeng Base Classes and Type Conversion Utilities
//

#ifndef ZONGHENG_CORE_BASE_H
#define ZONGHENG_CORE_BASE_H

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <typeinfo>
#include <type_traits>
#include <vector>

// Forward declarations
template<class INPUT_TYPE, class OUTPUT_TYPE>
class Yi;

// ============================================================================
// Type Constraint Macro
// ============================================================================

#define FORWARD_CONSTRAINT(TYPE, TYPE_INNER) <class TYPE, \
    class = std::enable_if<                                                \
        std::is_same<std::remove_cv<TYPE>, TYPE_INNER>::value              \
        || std::is_constructible<TYPE, TYPE_INNER>::value                  \
        || std::is_convertible<TYPE, TYPE_INNER>::value>>

// ============================================================================
// Type Conversion Function
// ============================================================================

template<class INPUT, class OUTPUT>
OUTPUT convert(const INPUT& input) {
    if constexpr (std::is_convertible<INPUT, OUTPUT>::value) {
        return static_cast<OUTPUT>(input);
    } else {
        throw std::runtime_error(
            std::string("Type conversion not possible: ") +
            typeid(INPUT).name() + " -> " + typeid(OUTPUT).name()
        );
    }
}

// ============================================================================
// QinBase - Base class for all ZongHeng nodes
// ============================================================================

class QinBase {
public:
    using SharedQinBase_T = std::shared_ptr<QinBase>;

    virtual ~QinBase() = default;

protected:
    using Zong_t = std::vector<SharedQinBase_T>;
    using Heng_t = std::vector<SharedQinBase_T>;

    Zong_t Zong;  // Vertical dependencies (upstream)
    Heng_t Heng;  // Horizontal relationships (derived)

    SharedQinBase_T self;

public:
    friend void operator<<(std::shared_ptr<QinBase> l, std::shared_ptr<QinBase> r);

    void bind(const std::shared_ptr<QinBase>& src);

    // Public accessors for dependency graph
    const Zong_t& getZong() const { return Zong; }
    const Heng_t& getHeng() const { return Heng; }

    size_t getZongCount() const { return Zong.size(); }
    size_t getHengCount() const { return Heng.size(); }

    /**
     * Type-safe conversion to Yi<IN, OUT>
     * @throws std::runtime_error if type mismatch
     */
    template<class IN, class OUT>
    typename Yi<IN, OUT>::SharedYi_T into() {
        auto result = std::dynamic_pointer_cast<Yi<IN, OUT>>(self);
        if (!result) {
            throw std::runtime_error(
                std::string("Type mismatch in into(): cannot convert to Yi<") +
                typeid(IN).name() + ", " + typeid(OUT).name() + ">"
            );
        }
        return result;
    }

    /**
     * Create dependency relationship between nodes
     */
    void lian(const SharedQinBase_T& q1, const SharedQinBase_T& q2) {
        q1->Heng.push_back(self);
        q2->Heng.push_back(self);
    }
};

#endif // ZONGHENG_CORE_BASE_H
