// function for solving kmeans kmeans::solve() line 165
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <limits>
using namespace std;

void clrscr()
{ std::cout << "\x1B[2J\x1B[H"; }

struct Point
{
    string name;
    double x;
    double y;

    string info()
    {
        stringstream ss;
        ss << name << ' ' << x << ' ' << y;
        return ss.str();
    }
};

struct Centroid
{
    Point p;
    vector<double> distance;
};

using Cluster = vector<Point*>;
using Clusters = vector<Cluster>;
using Points = vector<Point>;
using Centroids = vector<Centroid>;

namespace Color
{
    const char* NORMAL  = "\x1B[0m";
    const char* BOLD = "\x1B[1m";
    const char* BOLD_FORE_RED  = "\x1B[1;31m";
}

void print(Centroids& centroids, Clusters& cluster)
{
    stringstream ss;
    for (int i=0;i<centroids.size();i++)
    {
        cout << Color::BOLD_FORE_RED << "Cluster " << i+1 << Color::NORMAL << '\n';
        ss << "(" << centroids.at(i).p.x << ", " << centroids.at(i).p.y << ")";
        string s = ( centroids.at(i).p.name.empty() )? ss.str():centroids.at(i).p.name;
        cout << s << '\n';
        ss.str("");
        cout << "\t{ ";
        for (int p=0;p<cluster.at(i).size();p++)
        {
            if (cluster.at(i).at(p) == nullptr) continue;
            cout << cluster.at(i).at(p)->name << ' ';
        }
        cout << "}\n";
    }
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
    using Column = vector< Row >;
private:
    Column table;
    int width=10;
    string center(Cell& a)
    {
        if (a.value.size() >= width) return a.print();
        int pad = width - a.value.size();
        int leftpad = pad / 2;
        return string(leftpad,' ') + a.print() + string(pad-leftpad,' ');
    }
public:
    Column::size_type rows_size() { return table.size(); }
    Row::size_type column_size() { return (!table.empty())? table[0].size():0; }
    void push_row(Row& a)
    {
        if (a.size() != column_size() && column_size() != 0) a.resize(column_size());
        table.push_back( std::move(a) );
    }
    void push_col(Row& a)
    {
        Column::iterator table_it = table.begin();
        if (a.size() != rows_size() && rows_size() != 0)
            a.resize(rows_size());
        Row::iterator a_it = a.begin();

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
            for (int i=0;i<column_size();i++)
                line += string(width,'-') + '+';
        };
        for (auto& curr_row: table)
        {
            sep(line);
            line += "\n|";
            for (int i=0;i<column_size();i++)
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
{ return abs(a.x - b.x) + abs(a.y - b.y); }

double round_trunc(double v)
{ return ( (double) ( (int) round(v * 10.0) ) ) / 10.0; }

void new_centroid_from(Centroid& ce, Cluster& cl)
{
    if (cl.empty()) return;
    double mSumX = 0;
    double mSumY = 0;
    int ctr=0;
    for (auto& it: cl)
    {
        if (it == nullptr) continue;
        mSumX += it->x;
        mSumY += it->y;
        ctr++;
    }
    double newX = 0.0;
    double newY = 0.0;
    newX = round_trunc( mSumX / ctr );
    newY = round_trunc( mSumY / ctr );

    if (!ce.p.name.empty())
        ce.p.name = "";
    ce.p.x = newX;
    ce.p.y = newY;
}

bool solve(
    Points& points,
    Centroids& centroids,
    Clusters& cl,
    Clusters& prevCl,
    double threshold
)
{
    // get distance to each centroids
    for (auto& ce: centroids)
    {
        ce.distance.clear();
        for (auto& p: points)
        {
            double dist = manhattan_distance(ce.p, p);
            ce.distance.emplace_back(round_trunc( dist ));
        }
    }

    // move previous cluster to prevCl variable
    if (!cl.empty())
        prevCl = std::move(cl);
    cl.resize(centroids.size());
    for (auto& it: cl)
        it.resize(points.size());

    // clustering
    for (int i=0;i<points.size();i++)
    {
        double pminDist = 10000.0;
        int ceMin = -1;
        int pMin = i;
        for(int j=0;j<centroids.size();j++)
        {
            double cDist = centroids.at(j).distance.at(i);
            if (pminDist > cDist)
            {
                pminDist = cDist;
                ceMin = j;
                pMin = i;
            }
        }
        if (ceMin != -1)
            cl.at(ceMin).at(pMin) = &points.at(pMin);
    }

    // get new centroid
    for (int i=0;i<centroids.size();i++)
         new_centroid_from(centroids.at(i),cl.at(i));

    // compare prev and current cluster
    int ctr = 0;
    if (!prevCl.empty())
    {
        for (int i=0;i<centroids.size();i++)
            if (prevCl.at(i) == cl.at(i))
                ctr++;
    }
    return ctr == centroids.size();
}
}

int main(int argc, char* argv[])
{
    // d <= 10 points
    // k < 10
    int d = 0, k = 0;
    cout << "Enter number of points: ";
    cin >> d;
    Points points;
    Centroids centroids;
    for (int i=0;i<d;i++)
    {
        double x=0, y=0;
        cout << "Enter x and y (separated by space): ";
        cin >> x >> y;
        points.push_back({
            .name = "P" + to_string(i+1),
            .x = x,
            .y = y
        });
    }
    cout << "Enter number of clusters (k): ";
    cin >> k;
    for (int i=0;i<k;i++)
    {
        if (i < points.size())
        {
            Point& p = points.at(i);
            centroids.push_back({
                .p = Point{
                    .name = p.name,
                    .x = p.x,
                    .y = p.y
                }
            });
        }
    }
    double thresDist = 0.0;
    cout << "Enter threshold distance (d): ";
    cin >> thresDist;

    Clusters cluster;
    Clusters prevCl;
    Table table;
    Table::Row row{
        Cell{.color = Color::BOLD, .value = "Points"},
        Cell{.color = Color::BOLD, .value = "x"},
        Cell{.color = Color::BOLD, .value = "y"},
    };
    table.push_row(row);
    stringstream ss;
    for (auto& p: points)
    {
        auto s = [&ss](double val)
        {
            ss << val;
            string r = ss.str();
            ss.str("");
            return r;
        };
        row = {
            Cell{.value = p.name},
            Cell{.value = s(p.x)},
            Cell{.value = s(p.y)}
        };
        table.push_row(row);
    }
    int step = 1;

    clrscr();
    cout << Color::BOLD_FORE_RED << "Step " << step << Color::NORMAL << '\n';
    cout << "K (" << k << ")\n";
    for (int i=0;i<centroids.size();i++)
        cout << "\tt" << i+1 << '(' << centroids.at(i).p.name << ")\n";

    cout << Color::BOLD_FORE_RED << "Press Enter to Continue..." << Color::NORMAL;

    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.peek();

    ss.str("");
    Table::Column col(centroids.size());
    int indOfCentroids[centroids.size()];
    for (int p=0;p<points.size();p++)
        for (auto& it: centroids)
            if (it.p.name == points.at(p).name) indOfCentroids[p] = p;
    while(1)
    {
        step++;
        for (int ice=0;ice<centroids.size();ice++)
        {
            auto ce = centroids.at(ice);
            if (ce.p.name.empty())
            {
                ss << '(' << ce.p.x << ", " << ce.p.y << ')';
                col.at(ice).push_back(Cell{
                    .color = Color::BOLD_FORE_RED,
                    .value = ss.str()
                });
                ss.str("");
            } else
                col.at(ice).emplace_back(Cell{
                    .color = Color::BOLD_FORE_RED,
                    .value = ce.p.name
                });
        }

        bool solved = kmeans::solve(
            points,
            centroids,
            cluster,
            prevCl,
            thresDist
        );

        for (int ice=0;ice<centroids.size();ice++)
        {
            auto ced = centroids.at(ice).distance;
            assert(ced.size() == points.size());
            for (int ip=0;ip<points.size();ip++)
            {
                bool isPCentroid = false;
                if (step == 2) {
                    for (int i=0;i<centroids.size();i++)
                        if (ip == indOfCentroids[i])
                            isPCentroid = true;
                }

                if (isPCentroid)
                    col.at(ice).push_back(Cell{ .value = "-", });
                else {
                    ss << ced.at(ip);
                    col.at(ice).push_back(Cell{
                        .value = ss.str()
                    });
                    ss.str("");
                }
            }
        }
        for (auto& t: col) {
            table.push_col(t);
            t.clear();
        }
        clrscr();
        table.print();
        assert(cluster.size() ==centroids.size() );
        cout << Color::BOLD_FORE_RED << "Step " << step << Color::NORMAL << '\n';
        print(centroids,cluster);
        cout << Color::BOLD_FORE_RED << "Press Enter to Continue..." << Color::NORMAL;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin.peek();
        if (solved) break;
        if (step == 5) break;
    }
    // bool okThreshold = sqrt(cDist) > threshold && sqrt(cDist) < threshold;

    prevCl = std::move(cluster);
    cluster.resize(centroids.size());
    for (auto& i: cluster)
        i.resize(points.size());

    // filter out outliers
    for (int i=0;i<points.size();i++)
    {
        double pminDist = 10000.0;
        int ceMin = -1;
        int pMin = i;
        for(int j=0;j<centroids.size();j++)
        {
            double cDist = centroids.at(j).distance.at(i);
            bool okThreshold = round(sqrt(cDist)) < thresDist;
            if (pminDist > cDist && okThreshold)
            {
                pminDist = cDist;
                ceMin = j;
                pMin = i;
            }
        }
        if (ceMin != -1)
            cluster.at(ceMin).at(pMin) = &points.at(pMin);
    }
    clrscr();
    step++;
    table.print();
    cout << Color::BOLD_FORE_RED << "Step " << step << Color::NORMAL << '\n';
    print(centroids,cluster);
    cout << Color::BOLD_FORE_RED << "Press Enter to Continue..." << Color::NORMAL;
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.peek();

    return 0;
}
