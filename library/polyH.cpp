#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::map;

template <typename T>
class Polynomial {
 private:
    map<size_t, T> data;

 public:
    Polynomial(const vector<T>& d) {
        for (size_t iter = 0; iter < d.size(); ++iter) {
            if (d[iter] != 0) {
                data[iter] = d[iter];
            }
        }
    }

    Polynomial(const T& val = T()) {
        if (val != 0) {
            data[0] = val;
        }
    }

    template <typename It>
    Polynomial(const It& begin, const It& end) {
        vector <T> res;
        for (It i = begin; i != end; ++i) {
            res.push_back(*i);
        }
        *this = Polynomial(res);
    }

    long Degree() const {
        if (data.empty()) {
            return -1;
        }
        return (data.rbegin())->first;
    }

    const T& operator[] (size_t i) const {
        auto find = data.find(i);
        if (find == data.end()) {
            return 0;
        }
        return find->second;
    }

    const T& operator() (T x) const {
        T res = (*this)[Degree()];
        for (long iter = Degree() - 1; iter >= 0; --iter) {
            res = (*this)[iter] + x * res;
        }
        return res;
    }

    typename std::map<size_t, T>::iterator begin() {
        return data.begin();
    }

    typename std::map<size_t, T>::const_iterator begin() const {
        return data.begin();
    }

    typename std::map<size_t, T>::iterator end() {
        return data.end();
    }

    typename std::map<size_t, T>::const_iterator end() const {
        return data.end();
    }

    bool operator == (const Polynomial& other) {
        return(data == other.data);
    }

    template <typename Skal>
    bool operator == (const Skal& other) {
        auto poly = Polynomial(other);
        return(data == poly.data);
    }

    bool operator != (const Polynomial& other) {
        return(data != other.data);
    }

    template <typename Skal>
    bool operator != (const Skal& other) {
        auto poly = Polynomial(other);
        return(data != poly.data);
    }

    friend Polynomial<T> operator + (const Polynomial<T>& first, const Polynomial<T>& other) {
        long iter = 0;
        std::vector<T> res;
        res.resize(std::max(first.Degree(), other.Degree()) + 1, 0);

        for (iter = 0; (iter <= first.Degree()) && (iter <= other.Degree()); ++iter) {
            res[iter] = first[iter] + other[iter];
        }

        while (iter <= first.Degree()) {
            res[iter] = first[iter];
            ++iter;
        }

        while (iter <= other.Degree()) {
            res[iter] = other[iter];
            ++iter;
        }
        return Polynomial(res);
    }

    Polynomial<T>& operator += (const Polynomial& other) {
        *this = *this + other;
        return *this;
    }

    friend Polynomial<T> operator - (const Polynomial<T>& first, const Polynomial<T>& other) {
        long iter = 0;
        std::vector<T> res;
        res.resize(std::max(first.Degree(), other.Degree()) + 1, 0);

        for (iter = 0; (iter <= first.Degree()) && (iter <= other.Degree()); ++iter) {
            res[iter] = first[iter] - other[iter];
        }

        while (iter <= first.Degree()) {
            res[iter] = first[iter];
            ++iter;
        }

        while (iter <= other.Degree()) {
            res[iter] = -other[iter];
            ++iter;
        }
        return Polynomial(res);
    }

    Polynomial<T>& operator -= (const Polynomial& other) {
        *this = *this - other;
        return *this;
    }

    friend Polynomial<T> operator * (const Polynomial<T>& first, const Polynomial<T>& other) {
        std::vector<T> res;
        res.resize(first.Degree() + other.Degree() + 2, 0);

        for (long iterFirst = first.Degree(); iterFirst >= 0; --iterFirst) {
            for (long iterOther = other.Degree(); iterOther >= 0; --iterOther) {
                res[iterFirst + iterOther] += first[iterFirst] * other[iterOther];
            }
        }

        return Polynomial(res);
    }

    Polynomial<T>& operator *= (const Polynomial& other) {
        *this = *this * other;
        return *this;
    }

    friend Polynomial<T> operator & (const Polynomial<T>& first, const Polynomial<T>& other) {
        if (first.data.empty()) {
            if (other.data.empty()) {
                return T();
            }
            return other[0];
        }
        Polynomial <T> res(first.data.rbegin()->second);
        for (long iter = first.Degree() - 1; iter >= 0; --iter) {
            res = first[iter] + other * res;
        }
        return res;
    }

    friend Polynomial<T> operator / (const Polynomial<T>& first, const T value) {
        Polynomial<T> copy = first;
        for (auto iter = 0; iter <= copy.Degree(); ++iter) {
            copy.data[iter] /= value;
        }
        return copy;
    }

    friend Polynomial<T> operator / (const Polynomial<T>& first, const Polynomial<T>& other) {
        Polynomial<T> res, copy = first;
        while (copy.Degree() >= other.Degree()) {
            vector<T> thisStep;

            thisStep.resize(copy.Degree() - other.Degree() + 1, 0);
            thisStep[copy.Degree() - other.Degree()] = copy[copy.Degree()] / other[other.Degree()];
            Polynomial polyStep(thisStep);

            copy -= polyStep * other;
            res += polyStep;
        }
        return res;
    }

    friend Polynomial<T> operator % (const Polynomial<T>& first, const Polynomial<T>& other) {
        Polynomial<T> res, copy = first;
        while (copy.Degree() >= other.Degree()) {
            vector<T> thisStep;

            thisStep.resize(copy.Degree() - other.Degree() + 1, 0);
            thisStep[copy.Degree() - other.Degree()] = copy[copy.Degree()] / other[other.Degree()];
            Polynomial polyStep(thisStep);

            copy -= polyStep * other;
            res += polyStep;
        }
        return copy;
    }

    Polynomial<T>& operator %= (const Polynomial& other) {
        *this = *this % other;
        return *this;
    }

    friend Polynomial<T> operator , (Polynomial<T> first, Polynomial<T> other) {
        while (other != 0) {
            first %= other;
            std::swap(a.data, b.data);
        }
        return (first / first[first.Degree()]);
    }
};

template <typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T>& poly) {
    if (poly.Degree() == -1) {
        out << 0;
        return out;
    }
    bool printed = 0;
    for (long deg = poly.Degree(); deg != -1; --deg) {
        if (poly[deg] != 0) {
            if (deg == 0) {
                if (poly[deg] < 0) {
                    out << poly[deg];
                    printed = 1;
                } else {
                    if (printed) {
                        out << "+" << poly[deg];
                    } else {
                        out << poly[deg];
                        printed = 1;
                    }
                }
            } else if (deg == 1) {
                if (poly[deg] == -1) {
                    out << "-x";
                    printed = 1;
                } else if (poly[deg] < 0) {
                    out << poly[deg] <<"*x";
                    printed = 1;
                } else if (poly[deg] == 1) {
                    if (printed) {
                        out << "+x";
                    } else {
                        out << "x";
                        printed = 1;
                    }
                } else {
                    if (printed) {
                        out << "+" << poly[deg] << "*x";
                    } else {
                        out << poly[deg] << "*x";
                        printed = 1;
                    }
                }
            } else {
                if (poly[deg] == -1) {
                    out << "-x^" << deg;
                    printed = 1;
                } else if (poly[deg] < 0) {
                    out << poly[deg] <<"*x^" << deg;
                    printed = 1;
                } else if (poly[deg] == 1) {
                    if (printed) {
                        out << "+x^" << deg;
                    } else {
                        out << "x^" << deg;
                        printed = 1;
                    }
                } else {
                    if (printed) {
                        out << "+" << poly[deg] << "*x^" << deg;
                    } else {
                        out << poly[deg] << "*x^" << deg;
                        printed = 1;
                    }
                }
            }
        }
    }
    return out;
}
