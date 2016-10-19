#include <iostream>

using namespace std;

class Point {
    double x_, y_;
    public:
        Point(double x = 0.0, double y = 0.0) 
            : x_(x), y_(y)
        {}

        double get_x() const {
            return x_;
        }

        double get_y() const {
            return y_;
        }

        void set_x(double x) {
            x_ = x;
        }

        void set_y(double y) {
            y_ = y;
        }

        Point& operator+=(const Point& p) {
            x_ += p.x_;
            y_ += p.y_;
            return *this;
        }

        Point& operator-=(const Point& p) {
            x_ -= p.x_;
            y_ -= p.y_;
            return *this;
        }
};

Point operator+(const Point& p1, const Point& p2) {
    Point result(p1.get_x(), p1.get_y());
    result += p2;
    return result;
}

Point operator-(const Point& p1, const Point& p2) {
    Point result(p1.get_x(), p1.get_y());
    result -= p2;
    return result;
}

Point operator-(const Point& p) {
    Point res(-p.get_x(), -p.get_y());
    return res;
}

ostream& operator<<(ostream &out, const Point& p) {
  return out << '(' << p.get_x() << ',' << p.get_y() << ')';
}

int main() {
    Point p1(3, 4);
    Point p2(7, 8);

    cout << p1 << endl;
    cout << p2 << endl;

    p1 += p2;
    p1 -= p2;

    Point p3 = p1 + p2;
    cout << -p3 << endl;
    return 0;
}
