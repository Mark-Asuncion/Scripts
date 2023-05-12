#include <iostream>
#include <string>
#include "ccs2203.h"
using namespace std;
using namespace Memory;
using namespace Color;
using Format::ss;

class TimelinePrinter : public Table {
    public:
    void print(){
        auto center = [](string val,int w) -> string{
            w -= val.size();
            w/=2;
            return string(w,' ') + val;
        };
        auto center_num = [](string val) -> string {
            return string(val.size()/2,' ') + val;
        };
        int w = getWidth()*rowSize()+rowSize()-1;
        int ww = getWidth();
        std::cout <<'+'<< std::setfill('-')<<std::setw(w)<< '-' << "+\n";
        std::cout<<'|';
        for(int i=0;i<rowSize();i++){
            std::cout<<std::setfill(' ')<<std::setw(ww)<<std::left<< center(Header()[i],getWidth())<<'|';
        }
        std::cout<<"\n+";
        for(int i=0;i<rowSize();i++){
            std::cout << std::setfill('-')<<std::setw(ww)<< '-' << '+';
        }
        std::cout <<"\n0";
        for(int i=0;i<1;i++){
            for(int j=0;j<rowSize();j++){
                std::cout <<std::setfill(' ')<<std::setw(ww+1)<<std::right<< center_num(Rows()[i][j]);
            }
            std::cout << '\n';
        }
        std::cout<<'+';
        for(int i=0;i<rowSize();i++){
            std::cout << std::setfill('-')<<std::setw(ww)<< '-' << '+';
        }
    }
};
void p_process(vector<Process> &process,vector<int>& to_update,TimelinePrinter& process_table, int &curr_time, int &next){
    string p_name="",p_ct="";
    int curr_process = -1;
    int last_time = curr_time;
    auto psort = [&process](int a,int b) -> bool{
        bool c = process[a].remaining_time < process[b].remaining_time;
        if(c) { return c; }
        else if(process[a].remaining_time == process[b].remaining_time){
            return process[a].arrival_time < process[b].arrival_time;
        }
        return false;
    };
    auto ppsort = [&process](int a,int b) -> bool{
        return process[a].arrival_time < process[b].arrival_time;
    };
    if(next != -1){
        curr_process = next;
        p_name = to_string(process[curr_process].id);
        curr_time += process[curr_process].remaining_time;
        p_ct = to_string(curr_time);
        next = -1;
    }
    else {
        int min = -1;
        for(int i=0;i<process.size();i++){
            if(process[i].isDone) continue;

            if(min == -1 || process[min].arrival_time > process[i].arrival_time){ min = i; }

            if(process[i].arrival_time > curr_time) continue;
            if(find(to_update.begin(),to_update.end(),i) != to_update.end()) continue;
            to_update.push_back(i);
        }
        if(to_update.empty() && min != -1){
            p_name = "idle";
            curr_time = process[min].arrival_time;
            p_ct = to_string(curr_time);
        }
        else {
            sort(to_update.begin(),to_update.end(),psort);
            p_name = to_string(process[to_update.at(0)].id);
            curr_time += process[to_update.at(0)].remaining_time;
            p_ct = to_string(curr_time);
            curr_process = to_update.at(0);
        }
    }
    /* for(auto& i : to_update) cout << i << ' '; */
    /* cout << '\n'; */
    // interrupt
    if(curr_process != -1){
        vector<int> copy;
        for(int i=0;i<process.size();i++){
            if(process[i].isDone || i == curr_process) continue;
            if(process[i].arrival_time >= curr_time || process[i].arrival_time < process[curr_process].arrival_time) continue;
            copy.push_back(i);
        }
        sort(copy.begin(),copy.end(),ppsort);
        /* cout << "sorted copy: "; */
        /* for(auto i : copy) cout << i << ' '; */
        /* cout << '\n'; */
        /* for(auto i : process) cout <<"Process: "<< i.remaining_time << ' '; */
        /* cout << '\n'; */
        int curr_interrupt = 0;
        if(!copy.empty()){
            while(curr_interrupt < copy.size()){
                int interrupt = copy[curr_interrupt];
                int vt_remain_time = curr_time - process[interrupt].arrival_time;
                if(process[interrupt].arrival_time > last_time && vt_remain_time > process[interrupt].remaining_time){
                    int calc = process[curr_process].remaining_time - vt_remain_time;
                    curr_time = last_time + calc;
                    process[curr_process].remaining_time = vt_remain_time;
                    p_ct = to_string(curr_time);
                    break;
                }
                curr_interrupt++;
            }
            if(curr_interrupt < copy.size()){
                next = copy[curr_interrupt];
                /* cout << "interrupted: " << next << ' ' << process[next].arrival_time << "\n"; */
                /* cout << "curr_process-remain: " << process[curr_process].remaining_time << '\n'; */
                /* cout << "Time: " << curr_time << '\n'; */
            }
        }
        if(curr_interrupt >= copy.size() || copy.empty()) {
            process[curr_process].isDone = true;
            process[curr_process].remaining_time = 0;
            process[curr_process].completion_time = curr_time;
            // to_update.erase( find(to_update.begin(),to_update.end(),curr_process) );
            for(int i=0;i<to_update.size();i++){
                if(to_update.at(i) == curr_process) {
                    to_update.erase(to_update.begin()+i);
                    break;
                }
            }
        }
    }
    process_table.push_col({p_name,p_ct});
}

int main(){
    act_name = "FL-M5: ACT3 - SRT";
    // input n-process
    string ms = "";
    int n_process = input<int>("Enter number of process: ",ms,false);
    vector<Process> process;
    Table table({"Processes","Arrival","Burst","CT","TAT","WT"},BOLD);
    // enter arrival time
    ss << "Arrival Time\n";
    ms += ss.str();
    ss.str(string());
    for(int i=0;i<n_process;i++){
        int arrival_time = input<int>("Enter arrival time of P" + to_string(i+1) + ": ",ms,false);
        ss << "Enter arrival time of P" << to_string(i+1) << ": " << arrival_time << '\n';
        ms += ss.str();
        process.push_back({.id = i+1,.arrival_time = arrival_time, .isDone = false});
        ss.str(string());
    }
    // conts();
    // enter burst time
    ms = "";
    ss << "Burst Time\n";
    ms += ss.str();
    ss.str(string());
    for(int i=0;i<n_process;i++){
        int burst_time = input<int>("Enter burst time of " + to_string(i+1) + ": ",ms,false);
        ss << "Enter burst time of " << to_string(i+1) << ": " << burst_time << '\n';
        ms += ss.str();
        process[i].burst_time = burst_time;
        process[i].remaining_time = burst_time;
        ss.str(string());
    }
    for (vector<Process>::iterator i=process.begin();i!=process.end();i++){
        table.push_row( {
                        to_string(i->id),
                        to_string(i->arrival_time), 
                        to_string(i->burst_time) 
        } );
    }
    label();
    // process_table.print();
    TimelinePrinter process_table;
    // process_table.reserve(20);
    // process_table.setWidth(5);
    vector<int> to_update;
    to_update.reserve(n_process+1);
    process.reserve(n_process+1);
    int curr_time = 0;
    auto isDone = [&process]() -> bool {
        for(vector<Process>::iterator i=process.begin();i!=process.end();i++){ if(!i->isDone) return false; }
        return true;
    };
    auto str_center = [](string str, int width) -> string{
        if (width < 0) return str;
        width -= str.size();
        width /= 2;
        return string(width,' ') + str;
    };
    int next = -1;
    while(1){
        if(isDone()) break;
        clrscr();
        label();
        p_process(process,to_update,process_table,curr_time,next);
        int w = (process_table.getWidth() * process_table.rowSize()) + process_table.rowSize() - 1;
        cout << '+' << setfill('-') << setw(w) << '-' << "+\n";
        cout << '|' << setfill(' ') << setw(w) << std::left << str_center("Timeline",w) << "|\n";
        process_table.print();
        conts();
    }
    label();
    // calc tat, wt here
    float ave_tat = 0.0, ave_wt = 0.0;
    int colidx = table.rowSize();
    for(int i=0;i<process.size();i++){
        int tat = process[i].completion_time - process[i].arrival_time;
        int wt = tat - process[i].burst_time;
    
        table.edit_col(i+1,colidx-3,to_string(process[i].completion_time));
        table.edit_col(i+1,colidx-2,to_string(tat));
        table.edit_col(i+1,colidx-1,to_string(wt));
        
        ave_tat += tat;
        ave_wt += wt;
    }
    ave_tat /= process.size();
    ave_wt /= process.size();
    table.print();
    cout << '\n';
    process_table.print();
    cout << '\n';
    ave_tat = Format::truncate(ave_tat,2);
    ave_wt = Format::truncate(ave_wt,2);
    cout << "Average Turn Around Time: " << Format::toString(ave_tat) << '\n';
    cout << "Average Waiting Time: " << Format::toString(ave_wt) << '\n';

    return 0;
}
