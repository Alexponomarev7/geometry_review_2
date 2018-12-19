#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#include "geometry.h"

template <class T>
class MultiBelongingAlgorithm {
private:
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

        const U& getPoint() const {
            return p;
        }

        ssize_t getId() const {
            return _id;
        }

        TYPE getType() const {
            return type;
        }

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

    std::vector<value> _query;
    std::vector<Geometry::STATE> _ans;

    std::vector<Event<T>> _events;
    Geometry::Polygon<T> _polygon;
    std::map<int, std::vector<T>> _x_points;

    int _redirection(typename Event<T>::TYPE e) {
        switch (e) {
            case Event<T>::TYPE::OPEN:
                return -1;
            case Event<T>::TYPE::CLOSE:
                return 1;
            case Event<T>::TYPE::QUERY:
                return 0;
        }
        return 0;
    }

    void _answer_for_verticals() {
        auto cmp = [=](Event<T> a, Event<T> b) {
            if (a.getPoint().getY() == b.getPoint().getY()) {
                return _redirection(a.getType()) < _redirection(b.getType());
            }

            return a.getPoint().getY() < b.getPoint().getY();
        };

        for (auto x : _x_points) {
            std::vector<Event<T>> ev;
            int j = 0;
            for (Geometry::Segment<T> i : _polygon.getVerticalEdges()[x.first]) {
                ev.push_back(Event<T>(j, Event<T>::TYPE::OPEN, i.minY()));
                ev.push_back(Event<T>(j, Event<T>::TYPE::CLOSE, i.maxY()));
                j++;
            }

            j = 0;
            for (T i : _x_points[x.first]) {
                ev.push_back(Event<T>(i.getId(), Event<T>::TYPE::QUERY, i));
                j++;
            }
            sort(ev.begin(), ev.end(), cmp);
            int balance = 0;
            for (Event<T>& i : ev) {
                if (i.getType() == Event<T>::TYPE::OPEN)
                    balance++;
                if (i.getType() == Event<T>::TYPE::CLOSE)
                    balance--;
                if (i.getType() == Event<T>::TYPE::QUERY && balance > 0)
                    _ans[i.getId()] = Geometry::STATE::BORDER;
            }
        }
    }
public:
    MultiBelongingAlgorithm() = default;

    MultiBelongingAlgorithm(const std::vector<value>& _points) : _polygon(Geometry::Polygon<T>(_points)) {}

    void setOrder() {
        if (_polygon.square() > 0) {
            _polygon.revertOrder();
        }
    }

    void run() {
        _answer_for_verticals();

        auto cmp = [](const Geometry::Segment<T> &a, const Geometry::Segment<T> &b) {
            double x = std::max(a.minX().getX(), b.minX().getX());
            double ay = a.y(x), by = b.y(x);
            double x1 = std::min(a.maxX().getX(), b.maxX().getX());
            double ay1 = a.y(x1), by1 = b.y(x1);
            return (ay < by || (ay == by && ay1 < by1));
        };
        std::multiset<Geometry::Segment<T>, decltype(cmp)> open(cmp);

        for (auto e : _events) {
            switch (e.getType()) {
                case Event<T>::OPEN:
                    open.insert(_polygon.getEdges()[e.getId()]);
                    break;
                case Event<T>::CLOSE:
                    open.erase(open.find(_polygon.getEdges()[e.getId()]));
                    break;
                case Event<T>::QUERY:
                    if (open.empty())
                        continue;
                    Geometry::Segment<T> query(e.getPoint(), e.getPoint());

                    auto it = open.lower_bound(query);
                    if (it != open.end() && it->y(e.getPoint().getX()) == (double)e.getPoint().getY())
                        _ans[e.getId()] = Geometry::STATE::BORDER;
                    if (it != open.begin()) {
                        --it;
                        if (it->getPosition() == Geometry::POSITION::UP)
                            _ans[e.getId()] = std::max(_ans[e.getId()], Geometry::STATE::INSIDE);
                    }
                    break;
            }
        }
    }

    void setEvents() {
        size_type id = 0;
        for (auto e : _polygon.getEdges()) {
            if (e.getPosition() != Geometry::POSITION::VERTICAL) {
                _events.push_back(Event<T>(e.getId(), Event<T>::OPEN, e.minX()));
                _events.push_back(Event<T>(e.getId(), Event<T>::CLOSE, e.maxX()));
            }
            id++;
        }

        for (auto e : _query) {
            _events.push_back(Event<T>(e.getId(), Event<T>::QUERY, e));
        }
    }

    void sortEvents() {
        sort(_events.begin(), _events.end());
    }

    void setEdges() {
        _polygon.setEdges();
    }

    void reserve_query(size_type size) {
        _query.reserve(size);
        _ans.resize(size);
    }

    template <class U>
    void push_query(U&& p) {
        if (_polygon.getVerticies().count(std::forward<T>(p)))
            _ans[p.getId()] = Geometry::STATE::BORDER;

        _query.push_back(std::forward<T>(p));
        _x_points[p.getX()].push_back(std::forward<T>(p));
    }

    void clear() {
        _query.clear();
        _ans.clear();
        _events.clear();
        _x_points.clear();
    }

    std::vector<Geometry::STATE> ans() const {
        return _ans;
    }
};

template <class T>
class TestCase {
private:
    using value = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using size_type = size_t;

    size_t _size;
    MultiBelongingAlgorithm<value> _algorithm;
public:
    void Input(std::istream &is) {
        is >> _size;

        std::vector<value> points(_size);
        for (size_t i = 0; i < _size; ++i) {
            is >> points[i];
            points[i].setId(i);
        }
        _algorithm = MultiBelongingAlgorithm<value>(points);
    }

    void Query(std::istream &is) {
        is >> _size;

        _algorithm.reserve_query(_size);
        value point;
        for (size_t i = 0; i < _size; ++i) {
            is >> point;
            point.setId(i);
            _algorithm.push_query(point);
        }
    }

    void Prepare() {
        _algorithm.setOrder();
        _algorithm.setEdges();
        _algorithm.setEvents();
        _algorithm.sortEvents();
    }

    void Calculate() {
        _algorithm.run();
    }

    void Output(std::ostream &os) {
        for (auto state : _algorithm.ans()) {
            switch (state) {
                case Geometry::STATE::INSIDE:
                    os << "INSIDE\n";
                    break;
                case Geometry::STATE::OUTSIDE:
                    os << "OUTSIDE\n";
                    break;
                case Geometry::STATE::BORDER:
                    os << "BORDER\n";
                    break;
            }
        }
    }

    void Clear() {
        _algorithm.clear();
    }
};

void solve(std::istream &is, std::ostream &os) {
    uint32_t tests;
    is >> tests;

    TestCase<Geometry::Point<double>> test;
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