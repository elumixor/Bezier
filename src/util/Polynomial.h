//
// Created by Vladyslav Yazykov on 28/12/2018.
//

#ifndef BEZIER_POLYNOMIAL_H
#define BEZIER_POLYNOMIAL_H

#include <cstdlib>
#include <initializer_list>

#include "code_organizers.h"
#include "util/math.h"

/// Polynomial class representation
/// \tparam Vector Generic vector class that supports
/// <br>          - Addition:                vector + vector -> vector
/// <br>          - Scalar multiplication:   vector + scalar -> vector
/// \tparam Scalar Generic scalar class
template<typename Vector = float, typename Scalar = float>
class Polynomial {
    //region Zero values
    static const Vector ZeroV;
    static const Scalar ZeroS;
    static const Polynomial ZeroP;
    //endregion

    /// Check for zero coefficients at the highest power
    /// \param C Coefficients
    /// \param power Power
    static size_t get_power(const Vector *const C, size_t power) {
        // Start with the highest coefficient
        for (size_t i{power}; i != 0; i--) {
            if (power == 0) return 0;
            if (C[i] == ZeroV) power--; // Reduce the power if leading coefficient is zero
            else return power; // Seek the first non-zero coefficient
        }

        return power;
    }

    /// Structure represening result of polynomial division
    struct division_result {
        /// Result of division
        Polynomial result{};

        /// Remainder
        Polynomial remainder{};
    };

public:
    //region Fields

    /// Polynomial power
    size_t n{0};

    /// Coefficients
    Vector *C{new Vector[n + 1]{}};
    //endregion

    //region Constructors/assignments/destructors

    /// Empty polynomial
    Polynomial() = default;

    /// From array of coefficients
    /// \param coefficients Coefficients from the lowest to the highest
    explicit Polynomial(const Vector *const coefficients) :
            n{get_power(arr_len(coefficients) + 1)} {
        std::copy(coefficients, coefficients + n + 1, C);
    };

    /// From array of coefficients, limiting the power
    /// \param coefficients Coefficients from the lowest to the highest
    /// \param power Power
    explicit Polynomial(const Vector *const coefficients, size_t power) : n{get_power(coefficients, power)} {
        std::copy(coefficients, coefficients + n + 1, C);
    };

    /// From initializer list of coefficients
    /// \param coefficients Coefficients from the lowest to the highest
    Polynomial(std::initializer_list<Vector> coefficients) : n{
            get_power(coefficients.begin(), coefficients.size() - 1)} {
        std::copy(coefficients.begin(), coefficients.end(), C);
    };

    /// Copy constructor
    Polynomial(const Polynomial &other) : n{other.n} {
        std::copy(other.C, other.C + n + 1, C);
    }

    /// Move constructor
    Polynomial(Polynomial &&other) noexcept : n{other.n}, C{other.C} {
        other.n = 0;
        other.C = nullptr;
    }

    /// Copy assignment
    Polynomial &operator=(const Polynomial &other) {
        n = other.n;
        delete[] C;
        C = new Vector[n + 1];
        std::copy(other.C, other.C + n + 1, C);
        return *this;
    }

    /// Move assignment
    Polynomial &operator=(Polynomial &&other) noexcept {
        n = other.n;
        delete[] C;
        C = other.C;

        other.n = 0;
        other.C = nullptr;

        return *this;
    }

    /// Destructor
    virtual ~Polynomial() { delete[] C; }
    //endregion

    //region Comparisons

#define COMPARATOR(op) inline bool operator op (const Polynomial& other) const { return n op other.n; }
    COMPARATOR(>)
    COMPARATOR(<)
    COMPARATOR(<=)
    COMPARATOR(>=)
    COMPARATOR(==)
    COMPARATOR(!=)
#undef COMPARATOR

    /// Compares polynomials' powers and coefficients
    /// Note possible floating point number comparison
    bool equals(const Polynomial &other) const {
        if (other != *this) return false;

        for (size_t i{0}; i <= n; ++i)
            if (C[i] != other.C[i]) return false;

        return true;
    }

    /// Check if polynomial is zero polynomial
    inline bool is_zero() const {
        return n == 0 and C[0] == ZeroV;
    }

    //endregion

    //region Arithmetic

    /// Unary minus
    Polynomial operator-() const {
        Vector coefficients[n + 1];

        // Compute coefficients
        for (size_t i{0}; i <= n; i++)
            coefficients[i] = -C[i];

        return Polynomial(coefficients, n);
    }

    /// Add two polynomials together
    Polynomial operator+(const Polynomial &other) const {
        const Polynomial &min{*this > other ? *this : other};
        Polynomial max{other > *this ? other : *this};

        // Allocate space for coefficients
        Vector coefficients[max.n + 1];

        // Compute coefficients
        for (size_t i{0}; i <= min.n; i++)
            coefficients[i] = max.C[i] + min.C[i];

        for (size_t i{min.n + 1}; i <= max.n; i++)
            coefficients[i] = max.C[i];

        return Polynomial(coefficients, max.n);
    }

    /// Subtract polynomials
    inline Polynomial operator-(const Polynomial &other) const {
        return *this + -other;
    }

    /// Multiply polynomial by scalar
    Polynomial operator*(const Scalar &other) const {
        // Multiplication by zero
        if (other == ZeroS) return ZeroP;

        // Allocate space
        Vector coefficients[n + 1];

        // Compute coefficients
        for (size_t i{0}; i <= n; i++)
            coefficients[i] = C[i] * other;

        return Polynomial(coefficients, n);
    }

    /// Right hand scalar multiplication
    inline friend Polynomial &operator*(const Scalar &a, const Polynomial &b) {
        return b * a;
    }

    /// Divide polynomial by scalar
    Polynomial operator/(const Scalar &other) const {
        // Multiplication by zero
        if (other == ZeroS) return ZeroP;

        // Allocate space
        Vector coefficients[n + 1];

        // Compute coefficients
        for (size_t i{0}; i <= n; i++)
            coefficients[i] = C[i] / other;

        return Polynomial(coefficients, n);
    }

    /// Divide polynomial by another polynomial
    /// \return Result and reminder
    division_result operator/(const Polynomial &divisor) const {
        Polynomial dividend{*this};

        // Division by a greater polynomial
        if (divisor > dividend) return {ZeroP, *this};

        // Division by zero
        if (divisor.is_zero()) throw std::overflow_error("Division by zero polynomial");

        // Power of a resulting polynomial
        size_t power = dividend.n - divisor.n;

        // Allocate coefficients for the result
        size_t ppower{power};

        Vector coefficients[power + 1];

        // Divide while the power of dividend is not less than the power of divisor
        for (; dividend.n >= divisor.n; power--) {
            // Calculate leading coefficient
            coefficients[power] = dividend.C[dividend.n] / divisor.C[divisor.n];
            const Vector &c{coefficients[power]};

            // Compute intermediate - polynomial to be subtracted from current dividend
            // Here, as the subtraction leading coefficients of dividend and intermediate will
            // produce zero, we set power of intermediate (and remainder) to be the power of
            // dividend minus one
            size_t reminder_power{dividend.n - 1};
            Vector intermediate[reminder_power + 1];

            // Set zero coefficients to intermediate's lower powers
            for (size_t i{0}; i < power; i++)
                intermediate[i] = ZeroV;

            // Compute actual intermediate
            for (size_t i{0}; i < divisor.n; i++)
                intermediate[i + power] = c * divisor.C[i];

            // Compute reminder
            Vector reminder[reminder_power + 1];
            for (size_t i{0}; i <= reminder_power; i++)
                reminder[i] = dividend.C[i] - intermediate[i];

            // Check if higher coefficients are zero
            size_t high_power = get_power(reminder, reminder_power);
            power -= (reminder_power - high_power);

            // Proceed dividing the remainder
            dividend = Polynomial(reminder, high_power);
        }

        return {Polynomial(coefficients, ppower), dividend};
    }
    //endregion

    //region Advanced math

    /// Differentiate the polynomial
    /// \param order Order of a derivative
    /// \return Differentiated polynomial
    Polynomial derivative(unsigned order = 1) const {
        Polynomial der{*this};

        for (size_t i{order + 1}; i-- > 1;) {
            if (der.n == 0) return ZeroP; // Derivative of constant
            size_t power{der.n - 1};
            Vector coefficients[power + 1];

            // Compute coefficients
            for (size_t j{0}; j <= power; j++)
                coefficients[j] = der.C[j + 1] * (j + 1);

            der = Polynomial{coefficients, power};
        }

        return der;
    }

    /// Evaluate polynomial at specific value
    Vector evaluate(const Scalar &at) {
        Vector value{};
        if (at == ZeroS) return C[0];

        for (size_t i{0}; i <= n; i++)
            value = value + C[i] * math::pow(at, i);

        return value;
    }

    /// Evaluate polynomial at specific value
    inline Vector operator()(const Scalar &at) { return evaluate(at); }

    /// Find the number of real roots using the Sturm's theorem
    /// (https://en.wikipedia.org/wiki/Sturm%27s_theorem)
    /// \return
    size_t roots() {
        Polynomial p[n + 1];
        p[0] = *this;
        p[1] = derivative();

        // Calculate Sturm sequence
        size_t sequence_length{2};
        do p[sequence_length] = -((p[sequence_length - 2] / p[sequence_length - 1]).remainder);
        while (p[sequence_length++].n != 0);

        // Calculate sign changes for -inf
        int switches{0};
        int psign{0};
        for (size_t i{0}; i < sequence_length; i++) {
            const Vector &c{p[i].C[n]};

            // Determining the sign is tied to < operator of Vector class
            int sign = math::sign(p[i].n) * (c < ZeroV ? -1 : 1);

            // Count sign change
            if (sign + psign == 0) switches++;
            psign = sign;
        }

        // Subtract sign changes for inf
        psign = 0;
        for (size_t i{0}; i < sequence_length; i++) {
            const Vector &c{p[i].C[n]};

            // Determining the sign is tied to < operator of Vector class
            int sign = (c < ZeroV ? -1 : 1);

            // Count sign change
            if (sign + psign == 0) switches--;
            psign = sign;
        }

        return switches > 0 ? (size_t) switches : 0;
    }

    /// Find the indefinite integral of a polynomial
    /// \param order Integration order
    /// \return Indefinite integral
    Polynomial integrate(unsigned order = 1) const {
        Polynomial der{*this};

        for (size_t i{order + 1}; i-- > 1;) {
            size_t power{der.n + 1};
            Vector coefficients[power + 1];

            coefficients[0] = ZeroV;

            // Compute coefficients
            for (size_t j{1}; j <= power; j++)
                coefficients[j] = der.C[j - 1] / (j);

            der = Polynomial{coefficients, power};
        }

        return der;
    }
    //endregion

    //region Output/representation

    /// Print polynomial to a stream
    /// \param out Output stream
    /// \param from_higher Print higher order coefficients first
    /// \param variable Variable symbol to be used
    /// \param name Polynomial symbol to be used
    void print(std::ostream &out = std::cout, bool from_higher = true, const char *variable = "x",
               const char *name = "P") const {
        out << name << "(" << variable << ") = ";

        if (is_zero()) {
            out << C[0] << ENDL;
            return;
        }

        if (from_higher) {
            for (size_t i{n + 1}; i-- != 0;) {
                if (C[i] != ZeroV) {
                    out << (i == n ? "" : " + ") << C[i];
                    if (i > 0) {
                        out << "*" << variable;
                        if (i > 1) OUT << "^" << i;
                    }
                }
            }
            OUT << ENDL;
            return;
        } else {
            for (size_t i{0}; i <= n; i++) {
                if (C[i] != ZeroV) {
                    out << (i == 0 ? "" : " + ") << C[i];
                    if (i > 0) {
                        out << "*" << variable;
                        if (i > 1) OUT << "^" << i;
                    }
                }
            }
            OUT << ENDL;
        }
    }

    /// Print polynomial to a stream
    /// \param os Output stream
    inline friend std::ostream &operator<<(std::ostream &os, const Polynomial &p) {
        p.print(os);
        return os;
    }
    //endregion
};

template<typename Vector, typename Scalar>
const Vector Polynomial<Vector, Scalar>::ZeroV{};
template<typename Vector, typename Scalar>
const Scalar Polynomial<Vector, Scalar>::ZeroS{};
template<typename Vector, typename Scalar>
const Polynomial<Vector, Scalar> Polynomial<Vector, Scalar>::ZeroP{};

#endif //BEZIER_POLYNOMIAL_H
