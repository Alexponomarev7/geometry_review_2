#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#include "geometry.h"

template <class T>
class MultiBelongingAlgorithm {
private:
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

    std::vector<value> _points, _query;
    std::vector<Geometry::STATE> _ans;

    std::vector<Geometry::Segment<T>> _edges;
    std::vector<POSITION> _is_inside;

    std::vector<Event<T>> _events;
    std::multiset<T> _verticies;
    std::vector<T> points_x;
    std::map<int, std::vector<T>> x_points;
    std::map<int, std::vector<Geometry::Segment<T>>> _vert_edges;
public:
    MultiBelongingAlgorithm() {}

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
            _edges.push_back(Geometry::Segment<T>(_points[i], _points[(i + 1) % _points.size()], i));
            if (_points[i].getX() == _points[(i + 1) % _points.size()].getX()) {
                _vert_edges[_points[i].getX()].push_back(_edges.back());
            }

            if (_edges.back().first().getX() < _edges.back().second().getX()) {
                _is_inside.push_back(POSITION::DOWN);
            } else if (_edges.back().first().getX() == _edges.back().second().getX()){
                _is_inside.push_back(POSITION::VERTICAL);
            } else {
                _is_inside.push_back(POSITION::UP);
            }
        }
    }

    int redirection(typename Event<T>::TYPE e) {
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

    void answer_for_vert() {
        auto cmp = [=](Event<T> a, Event<T> b) {
            if (a.getPoint().getY() == b.getPoint().getY()) {
                return redirection(a.getType()) < redirection(b.getType());
            }

            return a.getPoint().getY() < b.getPoint().getY();
        };

        for (auto x : x_points) {
            std::vector<Event<T>> ev;
            int j = 0;
            for (Geometry::Segment<T> i : _vert_edges[x.first]) {
                ev.push_back(Event<T>(j, Event<T>::TYPE::OPEN, i.minY()));
                ev.push_back(Event<T>(j, Event<T>::TYPE::CLOSE, i.maxY()));
                j++;
            }

            j = 0;
            for (T i : x_points[x.first]) {
                ev.push_back(Event<T>(i.getId(), Event<T>::TYPE::QUERY, i));
                j++;
            }
            sort(ev.begin(), ev.end(), cmp);
            int bal = 0;
            for (Event<T>& i : ev) {
                if (i.getType() == Event<T>::TYPE::OPEN)
                    bal++;
                if (i.getType() == Event<T>::TYPE::CLOSE)
                    bal--;
                if (i.getType() == Event<T>::TYPE::QUERY && bal > 0)
                    _ans[i.getId()] = Geometry::STATE::BORDER;
            }
        }
    }

    void run() {
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
                    open.insert(_edges[e.getId()]);
                    break;
                case Event<T>::CLOSE:
                    open.erase(open.find(_edges[e.getId()]));
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
                        if (_is_inside[it->getId()] == POSITION::UP) _ans[e.getId()] = std::max(_ans[e.getId()], Geometry::STATE::INSIDE);
                    }
                    break;
            }
        }
    }

    void setEvents() {
        size_type id = 0;
        for (auto e : _edges) {
            if (_is_inside[id] != POSITION::VERTICAL) {
                _events.push_back(Event<T>(e.getId(), Event<T>::OPEN, e.minX()));
                _events.push_back(Event<T>(e.getId(), Event<T>::CLOSE, e.maxX()));
            }
            id++;
        }

        for (auto e : _query) {
            _events.push_back(Event<T>(e.getId(), Event<T>::QUERY, e));
        }

        sort(_events.begin(), _events.end());
    }

    void reserve_query(size_type size) {
        _query.reserve(size);
        _ans.resize(size);
    }

    template <class U>
    void push_back(U&& p) {
        _verticies.insert(std::forward<T>(p));
        _points.push_back(std::forward<T>(p));
    }

    template <class U>
    void push_query(U&& p) {
        if (_verticies.count(std::forward<T>(p)))
            _ans[p.getId()] = Geometry::STATE::BORDER;

        _query.push_back(std::forward<T>(p));
        x_points[p.getX()].push_back(std::forward<T>(p));
    }

    void clear() {
        _points.clear();
        _query.clear();
        _ans.clear();
        _edges.clear();
        _is_inside.clear();
        _events.clear();
        _verticies.clear();
        points_x.clear();
        x_points.clear();
        _vert_edges.clear();
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

        _algorithm.reserve(_size);
        value point;
        for (size_t i = 0; i < _size; ++i) {
            is >> point;
            point.setId(i);
            _algorithm.push_back(point);
        }
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
    }

    void Calculate() {
        _algorithm.answer_for_vert();
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