#ifndef CORE_MATHS_FIXED_H
#define CORE_MATHS_FIXED_H


/*
Copyright 2026 SAE Institute Switzerland SA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Contributors: Elias Farhan
*/


#include <cstdint>
#include <type_traits>


namespace core
{

/**
 * @brief Fixed-point number
 */
template<typename T=int32_t, T Exp=16, typename MulT=int64_t>
class Fixed
{
public:
    constexpr Fixed() = default;

	template<class OtherT>
	requires std::is_floating_point_v<OtherT>
    constexpr explicit Fixed(OtherT f)
    {
        underlyingValue_ = static_cast<T>(f * (1 << Exp));
    }

	template<class OtherT>
	requires std::is_integral_v<OtherT>
    constexpr explicit Fixed(OtherT n)
    {
        underlyingValue_ = static_cast<T>(n) << Exp;
    }

    constexpr explicit Fixed(T up, T down)
    {
        underlyingValue_ = (Fixed{ up } / Fixed{down}).underlyingValue_;
    }

	template<typename OtherT, OtherT OtherExp=16, typename OtherMulT>
	constexpr explicit Fixed(Fixed<OtherT, OtherExp, OtherMulT> other)
	{
		underlyingValue_ = static_cast<T>(other.underlying_value()) << (Exp-OtherExp);
	}

    explicit constexpr operator float() const
    {
        return (static_cast<float>(underlyingValue_) / (1 << Exp));
    }

    explicit constexpr operator T() const
    {
        return underlyingValue_ >> Exp;
    }

    explicit constexpr operator size_t() const
    {
        return static_cast<size_t>(underlyingValue_ >> Exp);
    }

    constexpr Fixed operator+(Fixed other) const
    {
        Fixed result;
        result.underlyingValue_ = underlyingValue_ + other.underlyingValue_;
        return result;
    }

    constexpr Fixed& operator+=(Fixed other)
    {
        underlyingValue_ += other.underlyingValue_;
        return *this;
    }

    constexpr Fixed operator-(Fixed other) const
    {
        Fixed result;
        result.underlyingValue_ = underlyingValue_ - other.underlyingValue_;
        return result;
    }

    constexpr Fixed& operator-=(Fixed other)
    {
        underlyingValue_ -= other.underlyingValue_;
        return *this;
    }

    constexpr Fixed operator-() const
    {
        Fixed result;
        result.underlyingValue_ = -underlyingValue_;
        return result;
    }

    constexpr Fixed operator*(Fixed other) const
    {
        Fixed result;
        MulT leftOp = static_cast<MulT>(underlyingValue_);
        MulT rightOp = static_cast<MulT>(other.underlyingValue_);
        result.underlyingValue_ = static_cast<T>((leftOp * rightOp) >> static_cast<MulT>(Exp));
        return result;
    }

    constexpr Fixed& operator*=(Fixed other)
    {
        MulT leftOp = static_cast<MulT>(underlyingValue_);
        MulT rightOp = static_cast<MulT>(other.underlyingValue_);
        underlyingValue_ = static_cast<T>((leftOp * rightOp) >> static_cast<MulT>(Exp));
        return *this;
    }

    constexpr Fixed operator/(Fixed other) const
    {
        Fixed result{};
        MulT leftOp = static_cast<MulT>(underlyingValue_) * (static_cast<MulT>(1) << static_cast<MulT>(Exp));
        MulT rightOp = static_cast<MulT>(other.underlyingValue_);
        result.underlyingValue_ = static_cast<T>((leftOp / rightOp));
        return result;
    }

    constexpr Fixed& operator/=(Fixed other)
    {
        MulT leftOp = static_cast<MulT>(underlyingValue_) << static_cast<MulT>(Exp);
        MulT rightOp = static_cast<MulT>(other.underlyingValue_);
        underlyingValue_ = static_cast<T>((leftOp / rightOp));
        return *this;
    }

    constexpr bool operator<=>(Fixed f2) const
    {
        return underlyingValue_ <=> f2.underlyingValue_;
    }

    static constexpr Fixed from_underlying_value(T value)
    {
        Fixed f;
        f.underlyingValue_ = value;
        return f;
    }
    [[nodiscard]] constexpr T underlying_value() const { return underlyingValue_; }
    [[nodiscard]] T& underlying_value() { return underlyingValue_; }
private:
    T underlyingValue_ = 0;
};

using Fixed16 = Fixed<>;
using Fixed8 = Fixed<int8_t, 6, int16_t>;

inline constexpr auto kPi16 = Fixed16::from_underlying_value(205887);
inline constexpr auto kEuler16 = Fixed16::from_underlying_value(178145);


} // namespace neko

#endif //CORE_MATHS_FIXED_H
