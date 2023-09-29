// Compile with -std=c++17 flag
#include <ios>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <sstream>
#include <cassert>
#include <tuple>
#include <utility>
#include <vector>
#include <limits.h>
#include <algorithm>
using namespace std;


void clrscr()
{ std::cout << "\x1B[2J\x1B[H"; }

struct Point
{
    string name;
    double x;
    double y;
};

struct Centroid
{
    Point* c;
    // vector<Point*> points;
    vector<double> distance;
};

bool operator==(const Centroid& a,const Point& b)
{ return a.c->name == b.name && a.c->x == b.x && a.c->y == b.y; }

bool operator==(const Point& a,const Point& b)
{ return a.x == b.x && a.y == b.y && a.name == b.name; }

// syntax sugar
using Cluster = vector<Point*>;
using Points = vector<Point>;

namespace Color
{
    const char* NORMAL  = "\x1B[0m";
    const char* BOLD = "\x1B[1m";
    const char* BOLD_FORE_RED  = "\x1B[1;31m";
    const char* BOLD_FORE_GREEN = "\x1B[1;32m";
    const char* BOLD_FORE_YELLOW  = "\x1B[1;33m";
}

struct Cell
{
    string color;
    string value;
    string print() { return color + value; }
};

class Table
    {
    public:
        using Row = vector<Cell>;
        using Column = vector< vector<Cell> >;
    private:
        Column table;
        int width=15;
        string center(Cell& a)
        {
            if (a.value.size() >= width) return a.print();
            int pad = width - a.value.size();
            int leftpad = pad / 2;
            return string(leftpad,' ') + a.print() + string(pad-leftpad,' ');
        }
    public:
        Column::size_type column_size() { return table.size(); }
        Row::size_type row_size() { return (!table.empty())? table[0].size():0; }
        void push_row(Row&& a)
        {
            if (a.size() < row_size() || a.size() > row_size() && row_size() != 0) a.resize(row_size());
            table.push_back( Row(a) );
        }
        void push_col(Row&& a)
        {
            Column::iterator table_it = table.begin();
            if (a.size() < column_size() || a.size() > column_size() && column_size() != 0) a.resize(column_size());
            Row::iterator a_it = a.begin();

            assert(a.size() == table.size());

            while (table_it != table.end() && a_it != a.end())
            {
                table_it->push_back(*a_it);
                table_it++;
                a_it++;
            }
        }
        void print()
        {
            string line;
            auto sep = [=](string& line)
                {
                    line += '+';
                    for (int i=0;i<row_size();i++)
                    {
                        line += string(width,'-') + '+';
                    }
                };
            for (auto& curr_row: table)
            {
                sep(line);
                line += "\n|";
                for (int i=0;i<row_size();i++)
                {
                    line += center( curr_row.at(i) ) + Color::NORMAL + '|';
                }
                line += "\n";
            }
            sep(line);
            cout << line << endl;
        }
    };

namespace kmeans
{
double manhattan_distance(Point& a,Point& b)
{
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    return ( (x < 0)? -x:x ) + ( (y < 0)? -y:y );
}

bool is_equal_to_prev(Cluster& a,Cluster& b,Cluster& prev_a, Cluster& prev_b)
{ return (a==prev_a) && (b==prev_b); }

double round_trunc(double v)
{ return (double) (int) round(v * 10.0) / 10.0; }

void new_centroid_from(Centroid& ce,Cluster& cl)
{
    double sum_x = 0, sum_y = 0, divisor = cl.size();
    for (auto& it: cl)
    {
        sum_x += it->x;
        sum_y += it->y;
    }
    if (ce.c->name.empty()) { delete ce.c; }
    double ave_a = (cl.empty())? 0:sum_x/divisor;
    double ave_b = (cl.empty())? 0:sum_y/divisor;
    ce.c = new Point{
        .name="",
        .x = round_trunc( ave_a ),
        .y = round_trunc( ave_b )
    };
}

bool solve(
    Points& points,
    Cluster& cl_a,
    Cluster& cl_b,
    Cluster& prev_cl_a,
    Cluster& prev_cl_b,
    Centroid& ce_a,
    Centroid& ce_b
)
{
    if (!cl_a.empty()) prev_cl_a = std::move(cl_a);
    if (!cl_b.empty()) prev_cl_b = std::move(cl_b);
    if (!ce_a.distance.empty() && !ce_b.distance.empty())
    {
        ce_a.distance.clear();
        ce_b.distance.clear();
    }

    Cluster consider_points;
    using namespace kmeans;
    for (auto& it : points)
    {
        if (ce_a == it || ce_b == it) continue;
        ce_a.distance.push_back( manhattan_distance(*ce_a.c, it) );
        ce_b.distance.push_back( manhattan_distance(*ce_b.c, it) );
        consider_points.push_back(&it);
    }

    assert( ce_a.distance.size() == ce_b.distance.size() );
    assert( ce_a.distance.size() == consider_points.size() &&
           ce_b.distance.size() == consider_points.size());

    if (points[0] == *ce_a.c) cl_a.push_back(&points[0]);
    if (points[1] == *ce_b.c) cl_b.push_back(&points[1]);

    for (int i=0;i<ce_a.distance.size();i++) {
        if (ce_a.distance[i] > ce_b.distance[i]) cl_b.push_back(&(*consider_points[i]));
        else cl_a.push_back(&(*consider_points[i]));
    }
    bool res = is_equal_to_prev(cl_a, cl_b, prev_cl_a, prev_cl_b);
    new_centroid_from(ce_a,cl_a);
    new_centroid_from(ce_b,cl_b);
    return res;
}
}

fstream* file_open(string& filename)
{
    return new fstream(filename, fstream::in);
}

tuple<double,double> parse_line(string& line)
{
    stringstream ss(line);
    double x,y;
    ss >> x >> y;
    ss.ignore(numeric_limits<streamsize>::max());
    return tuple(x,y);
}

void file_input(string& filename)
{
    fstream* input_file = file_open(filename);
    if (input_file->fail()) {
        cout << Color::BOLD_FORE_RED << "Error:: File does not exist" << Color::NORMAL << endl;
        return;
    }
    string line;
    int id = 1;
    Points points;

    while (getline(*input_file,line,'\n'))
    {
        tuple<double,double> xy = parse_line(line);
        points.push_back(Point{
            .name = "P" + to_string(id),
            .x = get<0>(xy),
            .y = get<1>(xy)
        });
        id++;
    }
    input_file->close();

    Cluster cl_a;
    Cluster cl_b;
    Cluster prev_cl_a;
    Cluster prev_cl_b;
    Centroid ce_a { .c = &points[0] };
    Centroid ce_b { .c = &points[1], };
    Table table;
    table.push_row({
        Cell{ .color = Color::BOLD, .value = "Name" },
        Cell{ .color = Color::BOLD, .value = "x" },
        Cell{ .color = Color::BOLD, .value = "y" }
    });
    for (auto& it:points)
    {
        stringstream ss;
        ss << it.x << ' ' << it.y;
        string x,y;
        ss >> x >> y;
        table.push_row(
            {
                Cell{ .color = Color::BOLD_FORE_GREEN, .value = it.name, },
                Cell{ .color = "", .value = x },
                Cell{ .color = "", .value = y }
            }
        );
    }

    stringstream ss;
    bool is_found = false;
    while ( !is_found ) {
        clrscr();
        Table::Row&& vec_a = Table::Row();
        Table::Row&& vec_b = Table::Row();

        ss.str("");
        ss << "t1 = (" << ce_a.c->x << ", " << ce_a.c->y << ')';
        auto header = [](stringstream& ss,Centroid& a)
            { return (a.c->name.empty())? ss.str():a.c->name; };

        vec_a.push_back(Cell {
            .color = Color::BOLD,
            .value = header(ss,ce_a),
        });
        ss.str("");
        ss << "t2 = (" << ce_b.c->x << ", " << ce_b.c->y << ')';
        vec_b.push_back(Cell {
            .color = Color::BOLD,
            .value = header(ss,ce_b),
        });

        if (points[0] == *ce_a.c && points[1] == *ce_b.c)
        {
            vec_a.push_back(Cell {
                .color = Color::BOLD_FORE_YELLOW,
                .value = "-"
            });
            vec_a.push_back(Cell {
                .color = Color::BOLD_FORE_YELLOW,
                .value = "-"
            });
            vec_b.push_back(Cell {
                .color = Color::BOLD_FORE_YELLOW,
                .value = "-"
            });
            vec_b.push_back(Cell {
                .color = Color::BOLD_FORE_YELLOW,
                .value = "-"
            });
        }

        is_found = kmeans::solve(points,cl_a,cl_b,prev_cl_a,prev_cl_b,ce_a,ce_b);

        for (int i = 0; i<ce_a.distance.size();i++)
        {
            ss.str("");
            ss << ce_a.distance[i];
            vec_a.push_back(Cell {
                .color = "",
                .value = ss.str(),
            });
            ss.str("");
            ss << ce_b.distance[i];
            vec_b.push_back(Cell {
                .color = "",
                .value = ss.str(),
            });
        }

        table.push_col(std::forward<Table::Row>(vec_a));
        table.push_col(std::forward<Table::Row>(vec_b));
        table.print();
    }
    // table.print();
    cout << "Cluster A: \n{ ";
    for(auto& it: cl_a) { cout << it->name << ' '; }
    cout << "}\n";
    cout << "Cluster B: \n{ ";
    for(auto& it: cl_b) { cout << it->name << ' '; }
    cout << "}\n";
    cout << "Centroid A: (" << ce_a.c->x << ", " << ce_a.c->y << ")\n";
    cout << "Centroid B: (" << ce_b.c->x << ", " << ce_b.c->y << ")\n";
    cout << flush;
}

int main(int argc, char* argv[])
{
    if(argc > 1)
    {
        string file = string(filesystem::current_path()) + '/' + argv[argc-1];
        file_input(file);
    }
    else
    {
        assert(0 && "Ask input; not implemented yet");
    }
    return 0;
}
