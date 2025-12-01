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

template<class T>
class Qin;

namespace ZongHeng {
    template<class T, class Fn>
    std::shared_ptr<Qin<T>> fold(const std::vector<std::shared_ptr<Qin<T>>>&, T, Fn);
}

// Forward declarations for operator friends
template<class T>
std::shared_ptr<Qin<bool>> operator==(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
template<class T>
std::shared_ptr<Qin<bool>> operator!=(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
template<class T>
std::shared_ptr<Qin<bool>> operator<(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
template<class T>
std::shared_ptr<Qin<bool>> operator>(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
template<class T>
std::shared_ptr<Qin<bool>> operator<=(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
template<class T>
std::shared_ptr<Qin<bool>> operator>=(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
template<class T>
std::shared_ptr<Qin<T>> operator-(std::shared_ptr<Qin<T>>);
template<class T>
std::shared_ptr<Qin<T>> operator~(std::shared_ptr<Qin<T>>);
template<class T>
std::shared_ptr<Qin<bool>> operator!(std::shared_ptr<Qin<T>>);

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

    // Friend declarations for node classes
    template<class T>
    friend class Qin;
    template<class IN, class OUT>
    friend class Yi;

    // Friend declarations for combinators and operators
    template<class T, class Fn>
    friend std::shared_ptr<Qin<T>> ZongHeng::fold(const std::vector<std::shared_ptr<Qin<T>>>&, T, Fn);

    // Comparison operators
    template<class T>
    friend std::shared_ptr<Qin<bool>> operator==(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
    template<class T>
    friend std::shared_ptr<Qin<bool>> operator!=(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
    template<class T>
    friend std::shared_ptr<Qin<bool>> operator<(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
    template<class T>
    friend std::shared_ptr<Qin<bool>> operator>(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
    template<class T>
    friend std::shared_ptr<Qin<bool>> operator<=(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);
    template<class T>
    friend std::shared_ptr<Qin<bool>> operator>=(std::shared_ptr<Qin<T>>, std::shared_ptr<Qin<T>>);

    // Unary operators
    template<class T>
    friend std::shared_ptr<Qin<T>> operator-(std::shared_ptr<Qin<T>>);
    template<class T>
    friend std::shared_ptr<Qin<T>> operator~(std::shared_ptr<Qin<T>>);
    template<class T>
    friend std::shared_ptr<Qin<bool>> operator!(std::shared_ptr<Qin<T>>);

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

protected:
    /**
     * @brief Create dependency relationship between nodes (internal use only)
     *
     * Registers this node as derived from q1 and q2 by adding this node
     * to their Heng (downstream) lists. This is a core building block for
     * reactive dependencies.
     *
     * Protected to prevent misuse - only accessible to framework internals
     * (Yi/Qin classes and friend operators/combinators).
     */
    void lian(const SharedQinBase_T& q1, const SharedQinBase_T& q2) {
        q1->Heng.push_back(self);
        q2->Heng.push_back(self);
    }

    // Internal: Add a derived node (for combinators that need direct access)
    void addDerivedNode(const SharedQinBase_T& node) { Heng.push_back(node); }
};

#endif // ZONGHENG_CORE_BASE_H
