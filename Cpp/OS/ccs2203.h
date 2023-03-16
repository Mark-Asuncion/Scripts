#ifndef _CCS2203_
#define _CCS2203_

#include <iostream>
#include <limits>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>

// header for CCS 2203 class
// Namespace:
    //Color::
    // const strings
// Namespace:
    // Format
    // string Format::toString(float,int)
    // float Format::truncate(float,int)
    // float Format::fround(float,int)
// class Table (for table printer)
// Namespace:
    // Memory::
    // Job
    // Partition
    // Manager

#define NAME "Asuncion, Mark Anthony"
std::string act_name = "";
std::string unit = "M";

void label(){
    std::cout << act_name << std::endl;
    std::cout << NAME << std::endl << std::endl;
}
void clrscr(){
    std::cout << "\x1B[2J\x1B[H";
}

void cont(){
    std::cout << std::endl;
    std::cout << "Press Enter to continue";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.peek();
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clrscr();
}
void conts(){
    std::cout << std::endl;
    std::cout << "Press Enter to continue";
    std::cin.peek();
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clrscr();
}

template<typename type>
type input(std::string prompt, std::string &message){
    type in;
    while(true){
        label();
        std::cout << message;
        std::cout << prompt;
        if (std::cin.peek() == 10){
            std::cout << "Wrong Input. Please Input a number." << std::endl;
            cont();
            continue;
        }
        std::cin >> in;
        if(std::cin.fail()){
            std::cout << "Wrong Input. Please Input a number." << std::endl;
            cont();
            continue;
        }
        break;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clrscr();
    return in;
}
template<typename type>
type input(std::string prompt, std::string &message, bool isNegative){
    type in;
    while(true){
        label();
        std::cout << message;
        std::cout << prompt;
        if (std::cin.peek() == 10){
            std::cout << "Wrong Input. Please Input a number." << std::endl;
            cont();
            continue;
        }
        std::cin >> in;
        if(std::cin.fail()){
            std::cout << "Wrong Input. Please Input a number." << std::endl;
            cont();
            continue;
        }
        else if(!isNegative && in < 0){
            std::cout << "Wrong Input. Please Input a postive number." << std::endl;
            cont();
            continue;
        }
        break;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clrscr();
    return in;
}
template<typename type>
type input(std::string prompt, std::string &message, int greaterthan, int lessthan){
    type in;
    while(true){
        label();
        std::cout << message;
        std::cout << prompt;
        if (std::cin.peek() == 10){
            std::cout << "Wrong Input. Please Input a number." << std::endl;
            cont();
            continue;
        }
        std::cin >> in;
        if(std::cin.fail()){
            std::cout << "Wrong Input. Please Input a number." << std::endl;
            cont();
            continue;
        }
        else if(in < greaterthan || in > lessthan){
            std::cout << "Wrong Input. Please Input a number that is within "<< greaterthan << " and " << lessthan << "." << std::endl;
            cont();
            continue;
        }
        break;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clrscr();
    return in;
}

namespace Color{
    // color foreground background
    // Black	30	40
    // Red	31	41
    // Green	32	42
    // Yellow	33	43
    // Blue	34	44
    // Magenta	35	45
    // Cyan	36	46
    // White	37	47
    // Default	39	49
    // Reset	0	0
    const std::string NORMAL  = "\x1B[0m";
    const std::string BOLD = "\x1B[1m";
    const std::string Bold_Fore_Red  = "\x1B[1;31m";
    const std::string Bold_Fore_Green = "\x1B[1;32m";
    const std::string Bold_Fore_Yellow  = "\x1B[1;33m";
    const std::string Fore_White_Back_Green  = "\x1B[1;37;42m";
    const std::string Fore_White_Back_Red  = "\x1B[1;37;41m";
    const std::string Fore_Cyan_Back_Green  = "\x1B[1;36;42m";
    const std::string Fore_Cyan_Back_Red  = "\x1B[1;36;41m";
    const std::string Strike_Yellow =  "\x1B[9;33m";
    const std::string Reset_Strike =  "\x1B[29m";
    const std::string Fore_Black =  "\x1B[0;30m";
    const std::string Back_Black =  "\x1B[0;40m";
    const std::string Fore_Red = "\x1B[0;31m";
    const std::string Back_Red =  "\x1B[0;41m";
    const std::string Fore_Green = "\x1B[0;32m";
    const std::string Back_Green = "\x1B[0;42m";
    const std::string Fore_Yellow = "\x1B[0;33m";
    const std::string Back_Yellow =  "\x1B[0;43m";
    const std::string Fore_White =  "\x1B[0;37m";
    const std::string Back_White =  "\x1B[0;47m";
    const std::string Fore_Default =  "\x1B[0;39m";
    const std::string Back_Default =  "\x1B[0;49m";
}
namespace Format{
    std::stringstream ss;
    // if length is > than floating values it will return it as it is
    float truncate(float val, int length){
        int whole_number = floor(val);
        float rmnd = val - whole_number;
        int decimals = rmnd * pow(10,length);
        float fdecimals = decimals / pow(10,length);
        return whole_number + fdecimals;
    }
    std::string toString(float const &val){
        ss << val;
        std::string f = ss.str();
        ss.str(std::string());
        return f;
    }
    // val = value to round
    // length = how many decimal places to keep
    float fround(float const &val,int length){
        return ceil(val * pow(10,length)) / pow(10,length);
    }
}

class Table{
    using vct =  std::vector<std::string>;
    using vcct = std::vector<std::vector<std::string>>;
    
    private:
    int width = 10;
    vct *header;
    vcct *rows = new vcct();

    vct color_header;
    vcct color_rows;
    std::string str_trunc(std::string &x){
        if(x.size() <= width) return x;
        std::string f = "..";
        for(int i = x.size()-width+3;i<x.size();i++){
            f += x[i];
        }
        return f;
    }
    void tresize(){
        if(rows->empty()) return;
        if(header->size() != rows->begin()->size()){
            for(vcct::iterator i = rows->begin();i!=rows->end();i++){
                i->resize(header->size());
            }
            for(vcct::iterator i = color_rows.begin();i!=color_rows.end();i++){
                i->resize(header->size());
            }
        }
    }
    public:
    //constructor===================
    Table(){
        this->header = new vct();
    }
    Table(vct *header){
        this->header = header;
        color_header.assign(header->size(),"");
    }
    Table(vct *header,std::string color){
        this->header = header;
        color_header.assign(header->size(),color);
    }
    ~Table(){
        delete header,rows;
    }
    void setWidth(int val){
        if(val < 10) return;
        this->width = val;
    }
    const int& getWidth() {return this->width; }
    //===============================
    const vct& Header(){ return *header;}
    const vcct& Rows() {return *rows; }
    void push_header(std::string val){
        header->push_back(val);
        color_header.push_back("");
        tresize();
    }
    void push_header(std::string color, std::string val){
        color_header.push_back(color);
        header->push_back(val);
        tresize();
    }
    void edit_header(int pos, std::string newVal){
        if(pos < 0 || pos >= header->size()){
            throw(std::out_of_range("edit_header. Invalid pos"));
        }
        header->at(pos) = newVal;
    }
    void edit_header(int pos, std::string newColor,std::string newVal){
        if(pos < 0 || pos >= header->size()){
            throw(std::out_of_range("edit_header. Invalid pos"));
        }
        header->at(pos) = newVal;
        color_header.at(pos) = newColor;
    }
    void push_row(vct nrow){
        if(nrow.size() != header->size()) nrow.resize(header->size());
        color_rows.push_back(vct(nrow.size(),""));
        rows->push_back(nrow);
    }
    void push_row(vct color, vct nrow){
        if(nrow.size() != header->size()) nrow.resize(header->size());
        if(color.size() != nrow.size()){
            color.assign(nrow.size(),*(color.begin()));
        }
        color_rows.push_back(color);
        rows->push_back(nrow);
    }
    void edit_row(int pos1,int pos2, std::string newVal){
        if(pos1 < 0 || pos1 >= rows->size()){
            throw(std::out_of_range("edit_row. Invalid pos1"));
        }
        if(pos2 < 0 || pos2 >= (*(rows->begin() + pos1)).size()){
            throw(std::out_of_range("edit_row. Invalid pos2"));
        }
        rows->at(pos1).at(pos2) = newVal;
    }
    void edit_row(int pos1,int pos2, std::string color,std::string newVal){
        if(pos1 < 0 || pos1 >= rows->size()){
            throw(std::out_of_range("edit_row. Invalid pos1"));
        }
        if(pos2 < 0 || pos2 >= (*(rows->begin() + pos1)).size()){
            throw(std::out_of_range("edit_row. Invalid pos2"));
        }
        rows->at(pos1).at(pos2) = newVal;
        color_rows.at(pos1).at(pos2) = color;
    }
    void edit_hcolor(int pos, std::string color){
        if(pos < 0 || pos >= header->size()){
            throw(std::out_of_range("edit_hcolor. Invalid pos"));
        }
        
        color_header.at(pos) = color;
    }
    void edit_rcolor(int pos1,int pos2, std::string color){
        if(pos1 < 0 || pos1 >= rows->size()){
            throw(std::out_of_range("edit_rcolor. Invalid pos1"));
        }
        if(pos2 < 0 || pos2 >= (*(rows->begin()+pos1)).size()){
            throw(std::out_of_range("edit_rcolor. Invalid pos2"));
        }
        color_rows.at(pos1).at(pos2) = color;
    }
    void clear_hcolor() { 
        if(color_header.empty()) throw(std::out_of_range("clear_hcolor. Header is empty"));
        color_header.assign(header->size(),""); 
    }
    void clear_rcolor() { 
        if(color_rows.empty()) throw(std::out_of_range("clear_rcolor. Row is empty"));
        color_rows.assign(rows->size(),vct(header->size(),"")); 
    }
    void del_header(int pos){
        if(header->empty()) throw(std::out_of_range("del_header. Header is empty"));
        if(pos < 0 || pos >= header->size()) throw(std::out_of_range("del_header. Invalid pos"));
        color_header.erase(color_header.begin()+pos);
        header->erase(header->begin()+pos);
        tresize();
    }
    void del_row(int pos){
        if(rows->empty()) throw(std::out_of_range("del_row. Row is empty"));
        if(pos < 0 || pos >= rows->size()) throw(std::out_of_range("del_row. Invalid pos"));
        color_rows.erase(color_rows.begin()+pos);
        rows->erase(rows->begin()+pos);
    }
    //
    //std:: std::setfill std::setw std::left, std::right
    //printer
    void print(){
        //header=============================
        std::string x = std::to_string(rows->size());
        std::cout << '+' <<std::setfill('-') << std::setw((width+1) * header->size() + x.size()) << "" << '+'<<std::endl;
        std::cout << '|' << std::setfill(' ') << std::left << std::setw(x.size()) << 0 << "|";
        vct::iterator color = color_header.begin();
        for(vct::iterator i = header->begin();i!=header->end();i++){
            std::string retToNormal = (*color == "")? "":Color::NORMAL;
            std::cout << *color;
            std::cout << std::setfill(' ') << std::right << std::setw(width)  << str_trunc(*i) << retToNormal << '|';
            color++;
        }
        std::cout << std::endl;
        //
        std::cout << '|' <<std::setfill('-') << std::left << std::setw(x.size()) << "" << "+";
        for(int i =0;i<header->size();i++){
            std::cout << std::setfill('-') << std::setw(width) << ""<< "+";
        }
        std::cout << std::endl;
        //body===============================
        int n_row = 0;
        vcct::iterator row_color = color_rows.begin();
        for(vcct::iterator i = rows->begin();i!=rows->end();i++){
            n_row++;
            
            std::cout <<'|' <<std::setfill(' ') << std::left << std::setw(x.size()) << n_row << "|";
            color = row_color->begin();
            for(vct::iterator j = i->begin();j!=i->end();j++){
                std::string retToNormal = (*color == "")? "":Color::NORMAL;
                std::cout << *color;
                std::cout << std::setfill(' ') << std::right << std::setw(width) << str_trunc(*j) << retToNormal << '|';
                color++;
            }
            std::cout << std::endl;
            //
            if(i == rows->end()-1) continue;
            std::cout << '|' <<std::setfill('-') << std::left << std::setw(x.size()) << "" << "+";
            for(int i =0;i<header->size();i++){
                std::cout << std::setfill('-') << std::setw(width) << ""<< "+";
            }
            std::cout << std::endl;
            row_color++;
            //
        }
        std::cout << '+' <<std::setfill('-') << std::setw((width+1) * header->size() + x.size()) << ""<< '+'<<std::endl;
    }
};

namespace Memory{
#define NOTHING 0
#define WAIT 1
#define ALLOCATED 2
#define CANT_ALLOCATE 3

class Job{
    private:
    float size;
    std::string name;
    int id;
    int tat = 0;
    public:
    int status = 0;
    Job(int id,float size,int tat){
        this->name = "J" + std::to_string(id);
        this->size = size;
        this->id = id;
        this->tat = tat;
    }
    const float& getSize() { return this->size; }
    const std::string& getName() { return this->name; }
    const int& getID() { return this->id; }
    const int& getTat() { return this->tat; }
    const int& decrementTat() { 
        if(tat <= 0) return tat;
        tat--;
        return tat;
    }
    std::string getInfo(){ return this->name + " (" + Format::toString(this->size) + unit + ")"; }
    friend bool operator<(Job const &a, Job const &b){ return a.size < b.size; }
};

class Partition{
    private:
    std::string name;
    int id;
    float size;
    Job *job = nullptr;
    public:
    Partition(int id, float size){
        this->name = "P" + std::to_string(id);
        this->size = size;
        this->id = id;
    }
    const std::string& getName() {return this->name; }
    const float& getSize() { return this->size; }
    const int& getID() { return this->id; }
    bool isHandling() { return job; }
    Job& pJob() { return *job; }
    void setJob(Job* job){ this->job = job; }
    void clear(){ job = nullptr; }
    friend bool operator<(Partition const &a,Partition const &b){ return *(a.job) < *(b.job); }
};

class Manager{
    private:
    float size;
    float os;
    std::vector<Partition> partitions;
    std::vector<Partition*> alloc_order;
    std::vector<Partition*> sorted;
    std::vector<Job*> exec_jobs;
    std::vector<Job*> fail_jobs;
    public:
    Manager(float size, float os){
        this->size = size;
        this->os = os;
    }
    std::vector<Partition>& getPartitions() { return partitions; }
    const std::vector<Job*>& Exec_jobs() { return this->exec_jobs; }
    const std::vector<Job*>& Fail_jobs() { return this->fail_jobs; }
    const float& getSize(){ return this->size; }
    const float& getOS(){ return this->os; }
    float getPartitionsSize(){
        if(partitions.empty()) return 0.0;
        float size = 0.0;
        for(std::vector<Partition>::iterator i = partitions.begin();i!=partitions.end();i++){
            size += i->getSize();
        }
        return size;
    }
    //Allocation
    void allocate(std::vector<Job> &jobs);
    //deallocation
    void deallocate();
    void summary(std::vector<Job> &jobs){
        for(std::vector<Job>::iterator i = jobs.begin();i!=jobs.end();i++){
            if (i->status == ALLOCATED) exec_jobs.push_back(&(*i));
            if (i->status == CANT_ALLOCATE) fail_jobs.push_back(&(*i));
        }
    }
};
}

#endif