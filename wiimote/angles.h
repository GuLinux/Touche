#ifndef ANGLES_H
#define ANGLES_H
#include <QDebug>
#include <QString>
#include <QMetaType>

#define operation(symbol, a2) Angles<T>(yaw() symbol a2.yaw(), pitch() symbol a2.pitch(), roll() symbol a2.roll() )
#define operation_t(symbol, t) (*this) symbol Angles<T>(t,t,t)
#define operation_inline(symbol, a2) m_yaw symbol a2.yaw(); m_pitch symbol a2.pitch(); m_roll symbol a2.roll(); return (*this);
#define operation_inline_t(symbol, t) return (*this) symbol Angles<T>(t,t,t)
template <typename T> class Angles {

public:
    Angles(T yaw, T pitch, T roll)
        : m_yaw(yaw), m_pitch(pitch), m_roll(roll), m_isNull(false) {}
    Angles(bool isNull=false) : m_isNull(isNull) {}

    template<typename J> Angles<J> transform(Angles<J>) { return Angles<J>(m_yaw, m_pitch, m_roll); }

    inline bool isNull() const { return m_isNull; }

    inline T yaw() const { return m_yaw; }
    inline T pitch() const { return m_pitch; }
    inline T roll() const { return m_roll; }

    inline T x() const { return m_yaw; }
    inline T y() const { return m_pitch; }
    inline T z() const { return m_roll; }


    inline bool operator==(const Angles<T> &a2) {
        return m_yaw == a2.yaw() && m_pitch == a2.pitch() && m_roll == a2.roll();
    }

//    inline Angles<T> operator=(const Angles<T> &a2) {
//        m_yaw=a2.yaw();
//        m_pitch=a2.pitch();
//        m_roll=a2.roll();
//        return *this;
//    }

    inline bool operator !=(const Angles<T> &a2) { return ! (*this==a2); }

    inline Angles<T> operator+(const Angles<T> &a2) { return operation(+, a2); }
    inline Angles<T> operator+(const T &t) { return operation_t(+, t); }
    inline Angles<T> &operator+=(const Angles<T> &a2) { operation_inline(+=, a2); }
    inline Angles<T> &operator+=(const T &t) { operation_inline_t(+=, t); }

    inline Angles<T> operator-(const Angles<T> &a2) { return operation(-, a2); }
    inline Angles<T> operator-(const T &t) { return operation_t(-, t); }
    inline Angles<T> &operator-=(const Angles<T> &a2) { operation_inline(-=, a2); }
    inline Angles<T> &operator-=(const T &t) { operation_inline_t(-=, t); }

    inline Angles<T> operator*(const Angles<T> &a2) { return operation(*, a2); }
    inline Angles<T> operator*(const T &t) { return operation_t(*, t); }
    inline Angles<T> &operator*=(const Angles<T> &a2) { operation_inline(*=, a2); }
    inline Angles<T> &operator*=(const T &t) { operation_inline_t(*=, t); }

    inline Angles<T> operator/(const Angles<T> &a2) { return operation(/, a2); }
    inline Angles<T> operator/(const T &t) { return operation_t(/, t); }
    inline Angles<T> &operator/=(const Angles<T> &a2) { operation_inline(/=, a2); }
    inline Angles<T> &operator/=(const T &t) { operation_inline_t(/=, t); }

    QString toString() const { return QString("[yaw=%1, pitch=%2, roll=%3]").arg(yaw()).arg(pitch()).arg(roll()) ; }

    Angles<T> calculateDelta(T timeInterval, Angles<T> zeroes) {
        Angles delta(m_yaw, m_pitch, m_roll);
        delta -= zeroes;
        delta *= timeInterval;
        return delta;
    }
private:
    T m_yaw;
    T m_pitch;
    T m_roll;
    bool m_isNull;
};
typedef Angles<double> WiimoteVector3;

#endif // ANGLES_H
