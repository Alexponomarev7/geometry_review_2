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
        enum Type {
            QUERY,
            CLOSE,
            OPEN,
        };
    private:
        ssize_t _id;
        Type type;
        U p;
    public:
        Event(ssize_t id, Type type, U p) : _id(id), type(type), p(p) {}

        const U& getPoint() const {
            return p;
        }

        ssize_t getId() const {
            return _id;
        }

        Type getType() const {
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
    std::vector<Geometry::State> _ans;

    std::vector<Event<T>> _events;
    Geometry::AdvancedPolygon<T> _polygon;
    std::map<int, std::vector<T>> _x_points;

    int _redirection(typename Event<T>::Type e) {
        switch (e) {
            case Event<T>::Type::OPEN:
                return -1;
            case Event<T>::Type::CLOSE:
                return 1;
            case Event<T>::Type::QUERY:
                return 0;
        }
        return 0;
    }

    std::vector<Event<T>> _prepare(int x) {
        std::vector<Event<T>> ev;
        int j = 0;
        for (Geometry::Edge<T> i : _polygon.getVerticalEdges()[x]) {
            ev.push_back(Event<T>(j, Event<T>::Type::OPEN, i.minY()));
            ev.push_back(Event<T>(j, Event<T>::Type::CLOSE, i.maxY()));
            j++;
        }

        for (T i : _x_points[x]) {
            ev.push_back(Event<T>(i.getId(), Event<T>::Type::QUERY, i));
        }

        return ev;
    }

    void _peform(const std::vector<Event<T>> &ev) {
        int balance = 0;
        for (const Event<T>& i : ev) {
            if (i.getType() == Event<T>::Type::OPEN)
                balance++;
            if (i.getType() == Event<T>::Type::CLOSE)
                balance--;
            if (i.getType() == Event<T>::Type::QUERY && balance > 0)
                _ans[i.getId()] = Geometry::State::BORDER;
        }
    }

    void _answer_for_verticals() {
        auto cmp = [=](Event<T> a, Event<T> b) {
            if (a.getPoint().getY() == b.getPoint().getY()) {
                return _redirection(a.getType()) < _redirection(b.getType());
            }

            return a.getPoint().getY() < b.getPoint().getY();
        };

        for (auto x : _x_points) {
            std::vector<Event<T>> ev = std::move(_prepare(x.first));
            sort(ev.begin(), ev.end(), cmp);
            _peform(ev);
        }
    }

    void _answer_for_others() {
        auto cmp = [](const Geometry::Edge<T> &a, const Geometry::Edge<T> &b) {
            double x = std::max(a.minX().getX(), b.minX().getX());
            double ay = a.y(x), by = b.y(x);
            double x1 = std::min(a.maxX().getX(), b.maxX().getX());
            double ay1 = a.y(x1), by1 = b.y(x1);
            return (ay < by || (ay == by && ay1 < by1));
        };
        std::multiset<Geometry::Edge<T>, decltype(cmp)> open(cmp);

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
                    Geometry::Edge<T> query(e.getPoint(), e.getPoint());

                    auto it = open.lower_bound(query);
                    if (it != open.end() && it->y(e.getPoint().getX()) == (double)e.getPoint().getY())
                        _ans[e.getId()] = Geometry::State::BORDER;
                    if (it != open.begin()) {
                        --it;
                        if (it->getPosition() == Geometry::Position::UP)
                            _ans[e.getId()] = std::max(_ans[e.getId()], Geometry::State::INSIDE);
                    }
                    break;
            }
        }
    }
public:
    MultiBelongingAlgorithm() = default;

    MultiBelongingAlgorithm(const std::vector<value>& _points, const std::vector<value>& _queries) :
                            _polygon(Geometry::AdvancedPolygon<T>(_points)) {
        reserve_query(_queries.size());
        for (auto q : _queries)
            push_query(q);
    }

    void setOrder() {
        if (_polygon.OrientArea() > 0) {
            _polygon.revertOrder();
        }
    }

    void run() {
        _answer_for_verticals();
        _answer_for_others();
    }

    void setEvents() {
        size_type id = 0;
        for (auto e : _polygon.getEdges()) {
            if (e.getPosition() != Geometry::Position::VERTICAL) {
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
            _ans[p.getId()] = Geometry::State::BORDER;

        _query.push_back(std::forward<T>(p));
        _x_points[p.getX()].push_back(std::forward<T>(p));
    }

    void clear() {
        _query.clear();
        _ans.clear();
        _events.clear();
        _x_points.clear();
    }

    std::vector<Geometry::State> ans() const {
        return _ans;
    }
};

template <class T>
class Test {
private:
    using value = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using size_type = size_t;

    size_t _size;
    MultiBelongingAlgorithm<value> *_algorithm;

    std::string _serialize(Geometry::State state) {
        switch (state) {
            case Geometry::State::INSIDE:
                return "INSIDE";
            case Geometry::State::OUTSIDE:
                return "OUTSIDE";
            case Geometry::State::BORDER:
                return "BORDER";
        }

    }

    std::vector<Geometry::State> _ans;
    std::vector<value> _points, _queries;
public:
    Test() = default;

    void Input(std::istream &is) {
        is >> _size;

        _points.resize(_size);
        for (size_t i = 0; i < _size; ++i) {
            is >> _points[i];
            _points[i].setId(i);
        }
    }

    void Query(std::istream &is) {
        is >> _size;

        _queries.resize(_size);
        for (size_t i = 0; i < _size; ++i) {
            is >> _queries[i];
            _queries[i].setId(i);
        }
    }

    void Prepare() {
        _algorithm = new MultiBelongingAlgorithm<value>(_points, _queries);

        _algorithm->setOrder();
        _algorithm->setEdges();
        _algorithm->setEvents();
        _algorithm->sortEvents();
    }

    void Calculate() {
        _algorithm->run();
        _ans = _algorithm->ans();
    }

    void Output(std::ostream &os) {
        for (auto state : _ans) {
            os << _serialize(state) << '\n';
        }
    }

    void Clear() {
        delete _algorithm;
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
    std::vector<Test<T>> _tests;
public:
    TestCase() = default;

    TestCase(size_t _size) {
        _tests.resize(_size);
    }

    Test<T>& operator[](int index) {
        return _tests[index];
    }
};

void solve(std::istream &is, std::ostream &os) {
    uint32_t tests;
    is >> tests;

    TestCase<Geometry::Point<double>> test(tests);
    for (int i = 0; i < tests; ++i) {
        test[i].Input(is);
        test[i].Query(is);
    }

    for (int i = 0; i < tests; ++i) {
        test[i].Prepare();
        test[i].Calculate();
        test[i].Clear();
    }

    for (int i = 0; i < tests; ++i) {
        test[i].Output(std::cout);
    }

}

int main() {
    solve(std::cin, std::cout);
    return 0;
}