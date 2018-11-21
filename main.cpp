#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

namespace Geometry {
    template <class T>
    class Point {
    private:
        using value = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using size_type = size_t;

        value _x, _y;
    public:
        Point(const_reference x, const_reference y) : _x(x), _y(y) {}

        Point() {}

        value getX() const {
            return _x;
        }

        value getY() const {
            return _y;
        }

        long double operator^(const Point<value> &p) {
            return _x * p._y - _y * p._x;
        }

        friend std::ostream& operator<<(std::ostream &os, const Point<value> &p) {
            os << p.getX() << ' ' << p.getY();
            return os;
        }

        friend std::istream& operator>>(std::istream &is, Point<value> &p) {
            is >> p._x >> p._y;
            return is;
        }
    };

    template <class T>
    class Segment {
    private:
        using value = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using size_type = size_t;

        size_type _id;
        value _left, _right;
    public:
        Segment(const_reference &a, const_reference &b) : _left(a), _right(b) {}

        const_reference first() {
            return _left;
        }

        const_reference second() {
            return _right;
        }

        const_reference minX() {
            return (_left.getX() < _right.getY() ? _left : _right);
        }

        const_reference maxX() {
            return (_left.getX() < _right.getY() ? _right : _left);
        }


        size_type getId() const {
            return _id;
        }
    };

    template <class T>
    class Polygon {
    private:
        enum STATE {
            INSIDE,
            BORDER,
            OUTSIDE
        };

        enum POSITION {
            VERTICAL,
            UP,
            DOWN
        };

        template <class U>
        class Event {
        public:
            enum TYPE {
                QUERY,
                CLOSE,
                OPEN,
            };
        private:
            ssize_t _id;
            TYPE type;
            U p;
        public:
            Event(ssize_t id, TYPE type, U p) : _id(id), type(type), p(p) {}

            bool operator<(const Event& other) const {
                if (p.getX() == other.p.getX()) {
                    return type < other.type;
                }

                return p.getX() < other.p.getX();
            }
        };

        using value = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using size_type = size_t;

        std::vector<value> _points, _query;
        std::vector<STATE> _ans;

        std::vector<Segment<T>> _edges;
        std::vector<POSITION> _is_inside;

        std::vector<Event<T>> _events;
    public:
        Polygon() {}

        void reserve(size_type size) {
            _is_inside.reserve(size);
            _edges.reserve(size);
            _points.reserve(size);
        }

        void setOrder() {
            long double sq = 0;
            for (size_type i = 0; i < _points.size(); ++i) {
                sq += _points[i] ^ _points[(i + 1) % _points.size()];
            }

            if (sq > 0) {
                std::reverse(_points.begin(), _points.end());
            }
        }

        void setEdges() {
            for (size_type i = 0; i < _points.size(); ++i) {
                _edges.push_back(Segment<T>(_points[i], _points[(i + 1) % _points.size()]));
                if (_edges.back().first().getX() < _edges.back().second().getX()) {
                    _is_inside.push_back(POSITION::DOWN);
                } else if (_edges.back().first().getX() == _edges.back().second().getX()){
                    _is_inside.push_back(POSITION::VERTICAL);
                } else {
                    _is_inside.push_back(POSITION::UP);
                }
            }

            for (auto x : _is_inside) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        }

        void run() {
            auto cmp = [](const Segment<T> &a, const Segment<T> &b) {
                if (a.minX() == b.minX()) {

                }
            };
            std::set<Segment<T>, decltype(cmp)> open;

            for (auto e : _events) {
                switch (e.type) {
                    case Event<T>::OPEN:
                        open.insert(_edges[e.id]);
                        break;
                    case Event<T>::CLOSE:
                        open.insert(_edges[e.id]);
                        break;
                    case Event<T>::QUERY:
                        break;
                }
            }
        }

        void setEvents() {
            size_t id;

            id = 0;
            for (auto e : _edges) {
                _events.push_back(Event<T>(id, Event<T>::OPEN, e.minX()));
                _events.push_back(Event<T>(id++, Event<T>::CLOSE, e.maxX()));
            }

            id = 0;
            for (auto e : _query) {
                _events.push_back(Event<T>(id++, Event<T>::QUERY, e));
            }

            sort(_events.begin(), _events.end());
        }

        void reserve_query(size_type size) {
            _query.reserve(size);
            _ans.resize(size);
        }

        template <class U>
        void push_back(U&& p) {
            _points.push_back(std::forward<T>(p));
        }

        template <class U>
        void push_query(U&& p) {
            _query.push_back(std::forward<T>(p));
        }

        void clear() {
            _points.clear();
            _query.clear();
        }

        void print_cerr() const {
            for (auto x : _points) {
                std::cout << x << std::endl;
            }
        }
    };
}

template <class T>
class TestCase {
private:
    enum STATUS {
        OK,
        INVALID_INPUT,
        INVALID_QUERRY,
        NOT_PREPARED
    };

    using value = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using size_type = size_t;

    bool _is_prepared = false;

    size_t _size;
    Geometry::Polygon<value> _polygon;

    bool _set_prepared(bool info) {
        _is_prepared = info;
    }

    bool _get_prepared() const {
        return _is_prepared;
    }
public:
    STATUS Input(std::istream &is) {
        is >> _size;

        _polygon.reserve(_size);
        value point;
        for (size_t i = 0; i < _size; ++i) {
            is >> point;

            _polygon.push_back(point);
        }

        _set_prepared(false);
        return STATUS::OK;
    }

    STATUS Query(std::istream &is) {
        is >> _size;

        _polygon.reserve_query(_size);
        value point;
        for (size_t i = 0; i < _size; ++i) {
            is >> point;

            _polygon.push_query(point);
        }

        _set_prepared(false);
        return STATUS::OK;
    }

    STATUS Prepare() {
        _polygon.setOrder();
        _polygon.setEdges();
        _polygon.print_cerr();
        _polygon.setEvents();

        _set_prepared(true);
    }

    STATUS Calculate() {
        if (!_get_prepared()) {
            return STATUS::NOT_PREPARED;
        }


    }

    STATUS Output(std::ostream &os) {

    }

    STATUS Clear() {
        _polygon.clear();
    }
};

void solve(std::istream &is, std::ostream &os) {
    uint32_t tests;
    is >> tests;

    TestCase<Geometry::Point<int>> test;
    for (int i = 0; i < tests; ++i) {
        test.Input(is);
        test.Query(is);

        test.Prepare();
        test.Calculate();

        test.Output(std::cout);

        test.Clear();
    }
}

int main() {
    solve(std::cin, std::cout);
    return 0;
}