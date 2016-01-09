#include <iostream>

using namespace std;

class RationalException {
    string message_;

    public:
        RationalException(string message)
            : message_(message)
        {}

        string get_message() const {
            return message_;
        }
};

class Rational {
    long numerator_, denominator_;

    public:
        Rational(long num = 0, long den = 1)
            : numerator_(num)
        {
            set_denominator(den);
            normalize();
        }

        void set_denominator(long value) {
            if (value == 0) {
                throw RationalException("The denominator of a rational number cannot be zero.");
            }

            denominator_ = value;
        }

        long get_numerator() const {
            return numerator_;
        }

        long get_denominator() const {
            return denominator_;
        }

    private:
        long calculate_gcd(long a, long b) {
            int r = a % b;
            while (r != 0) {
                a = b;
                b = r;
                r = a % b;
            }

            return b;
        }

        void normalize() {
            if (denominator_ < 0) {
                numerator_ = -numerator_;
                denominator_ = -denominator_;
            }

            reduce();
        }

        void reduce() {
            if (numerator_ == 0) {
                denominator_ = 1;
            } else {
                long gcd = calculate_gcd(numerator_, denominator_);

                if (gcd > 1) {
                    numerator_ /= gcd;
                    denominator_ /= gcd;
                }
            }
        }
};

ostream& operator<<(ostream& out, const Rational& r) {
    return out << "(" << r.get_numerator() << "/" << r.get_denominator() << ")";
}

Rational operator+(const Rational& first, const Rational& second) {
    long new_numerator = (first.get_numerator() * second.get_denominator()) +
                          (second.get_numerator() * first.get_denominator());
    long new_denominator = first.get_denominator() * second.get_denominator();

    Rational result(new_numerator, new_denominator);
    return result;
}

Rational operator-(const Rational& first, const Rational& second) {
    long new_numerator = (first.get_numerator() * second.get_denominator()) -
                          (second.get_numerator() * first.get_denominator());
    long new_denominator = first.get_denominator() * second.get_denominator();

    Rational result(new_numerator, new_denominator);
    return result;
}

Rational operator/(const Rational& first, const Rational& second) {
    long new_numerator = first.get_numerator() * second.get_denominator();
    long new_denominator = first.get_denominator() * second.get_numerator();
    Rational result(new_numerator, new_denominator);
    return result;
}

Rational operator*(const Rational& first, const Rational& second) {
    long new_numerator = first.get_numerator() * second.get_numerator();
    long new_denominator = first.get_denominator() * second.get_denominator();
    Rational result(new_numerator, new_denominator);
    return result;
}

int main() {
    try {
        Rational r1(18, 24);
        Rational r2(2, 3);
        Rational r3 = r1 * r2; 
        cout << r1 << "*" << r2 << " = " << r3 << endl;
        Rational r4 = r1 / r2;
        cout << r1 << "/" << r2 << " = " << r4 << endl;
        Rational r5 = r2 + r4;
        cout << r2 << "+" << r4 << " = " << r5 << endl;
        Rational r6 = r1 - r2;
        cout << r1 << "-" << r2 << " = " << r6 << endl;
    } catch (const RationalException& e) {
        cout << e.get_message() << endl;
    }

    return 0;
}
