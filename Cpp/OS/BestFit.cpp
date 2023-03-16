#include <iostream>
#include "ccs2203.h"
using namespace std;
using namespace Format;

void Memory::Manager::allocate(std::vector<Job> &jobs){
    if(sorted.empty()){
        auto sort = [](Memory::Partition *a,Memory::Partition *b){
            if(b->getSize() == a->getSize()){
                int ia = a->getID();
                int ib = b->getID();
                return ia < ib;
            }
            return a->getSize() < b->getSize();
        };
        for(std::vector<Memory::Partition>::iterator i = partitions.begin();i!=partitions.end();i++){
            sorted.push_back(&(*i));
        }
        std::sort(sorted.begin(),sorted.end(),sort);
    }
    std::vector<Memory::Job>::iterator job_it;
    std::vector<Memory::Partition*>::iterator part_it;
    int full_ctr = 0;
    for(std::vector<Memory::Partition>::iterator i=partitions.begin();i!=partitions.end();i++){
        if (i->isHandling()) full_ctr++;
    }
    for(job_it=jobs.begin();job_it!=jobs.end();job_it++){
        int ctr=0;
        if(full_ctr >= partitions.size()) break;
        if(job_it->status == CANT_ALLOCATE || job_it->status == ALLOCATED) continue;
        for(part_it=sorted.begin();part_it!=sorted.end();part_it++){
            if(job_it->getSize() > (*part_it)->getSize()){
                ctr++;
                continue;
            }
            if((*part_it)->isHandling()){
                job_it->status = WAIT;
                continue;
            }
            job_it->status = ALLOCATED;
            (*part_it)->setJob(&(*job_it));
            alloc_order.push_back(*part_it);
            full_ctr++;
            break;
        }
        if(ctr == sorted.size()){
            job_it->status = CANT_ALLOCATE;
        }
    }
}
void Memory::Manager::deallocate(){
    if(alloc_order.empty()) return;
    std::vector<Memory::Partition*>::iterator dealloc = alloc_order.begin();
    (*dealloc)->clear();
    dealloc++;
    if(dealloc != alloc_order.end()){
        (*dealloc)->clear();
        alloc_order.erase(alloc_order.begin(),alloc_order.begin()+2);
    }
    else {
        alloc_order.erase(alloc_order.begin());
    }
}

void update_table(Table &table, vector<Memory::Partition> &partitions){
    vector<Memory::Partition>::iterator part_it = partitions.begin();
    int width = 10;
    for(int i=1;i<table.Rows().size();i++){
        if(part_it == partitions.end()) break;
        string jobInfo = "";
        string color = "";
        if(part_it->isHandling()){
            jobInfo = part_it->pJob().getInfo();
        }
        if(table.Rows().at(i).at(1) == ""){
            color = Color::Fore_Green;
        }
        else {
            color = Color::NORMAL;
        }
        if(jobInfo.size() > width) width = jobInfo.size() + 2;
        table.edit_row(i,1,color,jobInfo);
        part_it++;
    }
    if(width != 10){
        table.setWidth(width);
    }
}

void dealloc_update_table(Table &table, vector<Memory::Partition> &partitions){
    vector<Memory::Partition>::iterator part_it = partitions.begin();
    for(int i=1;i<table.Rows().size();i++){
        if(part_it == partitions.end()) break;
        if(part_it->isHandling()) {
            part_it++;
            continue;
        }
        if(table.Rows().at(i).at(1) == "") {
            part_it++;
            continue;
        }
        table.edit_rcolor(i,1,Color::Fore_Yellow);
        part_it++;
    }
}

bool isFinished(vector<Memory::Partition> &partitions,vector<Memory::Job> &jobs){
    bool ret = true;
    for(vector<Memory::Job>::iterator i=jobs.begin();i!=jobs.end();i++){
        if(i->status == 0 || i->status == WAIT) {
            ret = false;
            break;
        }
    }
    for(vector<Memory::Partition>::iterator i=partitions.begin();i!=partitions.end();i++){
        if(i->isHandling()) {
            ret = false;
            break;
        }
    }
    return ret;
}

int main(){
    act_name = "ML-M3: ACT2 Fixed Partition - Best Fit";
    float memsize, ossize;
    string ms = "";
    //Mem and OS=====
    memsize = input<float>("Enter memory size(M): ",ms,false);
    ss << "Enter memory size(M): " << truncate(memsize,2) <<endl;
    ms = ss.str();
    do{
        ossize = input<float>("Enter OS size(M): ",ms,false);
        if(ossize > memsize){
            label();
            cout << "Error. OS size is greater than Memory Size." << endl;
            conts();
        }
    }while(ossize > memsize);
    Memory::Manager memory(memsize,ossize);
    ss.str(string());
    ms = "";
    Table table_m(new vector<string>{"Partition","Size"},Color::BOLD);
    table_m.push_row(vector<string>{"OS",toString(memory.getOS())+unit});
    table_m.push_row(vector<string>{"Remaining Size",toString(memory.getSize()-memory.getOS())+unit});
    table_m.setWidth(15);
    label();
    table_m.print();
    table_m.setWidth(10);
    table_m.del_row(1);
    conts();
    //partition=======
    int npartition;
    npartition = input<int>("Enter number of partitions: ",ms,false);
    while(1){
        for(int i=0;i<npartition;i++){
            float partition_size = input<float>("Enter Partition "+to_string(i+1)+" size: ",ms,false);
            ss << "Enter Partition "<<to_string(i+1)<<" size: " << truncate(partition_size,2) << endl;
            ms += ss.str();
            memory.getPartitions().push_back(Memory::Partition(i+1,truncate(partition_size,2)));
            ss.str(string());
        }
        if(memory.getPartitionsSize() > (memory.getSize() - memory.getOS())){
            ms = "";
            memory.getPartitions().clear();
            label();
            cout << "Error. Partitions size is greater than the remaining size." << endl;
            conts();
            continue;
        }
        if(memory.getPartitionsSize() + memory.getOS() < memory.getSize()){
            float size = truncate(memory.getSize() - memory.getOS() - memory.getPartitionsSize(),2);
            memory.getPartitions().push_back(Memory::Partition(memory.getPartitions().size() + 1,size));
    }
        break;
    }
    
    ms = "";
    //================
    
    for(vector<Memory::Partition>::iterator i=memory.getPartitions().begin();i!=memory.getPartitions().end();i++){
        table_m.push_row(vector<string>{i->getName(),toString(i->getSize())+unit});
    }
    label();
    table_m.print();
    table_m.edit_header(1,"Job");
    table_m.edit_row(0,0,"OS ("+toString(memory.getOS())+"M)");
    table_m.edit_row(0,1,"OS");
    for(int i=1;i<table_m.Rows().size();i++){
        string x = table_m.Rows().at(i).at(0) +" (" +table_m.Rows().at(i).at(1) + ")";
        table_m.edit_row(i,0,x);
        table_m.edit_row(i,1,"");
    }
    conts();
    //================
    int njobs;
    vector<Memory::Job> jobs;
    njobs = input<int>("Enter number of jobs: ",ms,false);
    for(int i=0;i<njobs;i++){
        float size = input<float>("Enter Job "+ to_string(i+1) + " size: ",ms,false);
        ss << "Enter Job "<< to_string(i+1) << " size: "<< truncate(size,2) << endl;
        ms += ss.str();
        jobs.push_back(Memory::Job(i+1,truncate(size,2)));
        ss.str(string());
    }
    //================
    Table job_table(new vector<string>{"Name","Size","Status"},Color::BOLD);
    for(vector<Memory::Job>::iterator i=jobs.begin();i!=jobs.end();i++){
        job_table.push_row(vector<string>{i->getName(),toString(i->getSize())+unit});
    }
    label();
    job_table.print();
    conts();
    //================
    auto center = [](int width, string val){
        if(val.size() > width) return val;
        string f = "";
        int offset = (width - val.size())/2;
        for(int i=0;i<offset;i++){
            f += " ";
        }
        f+= val;
        for(int i=0;i<(width - (val.size() + offset));i++){
            f+= " ";
        }
        return f;
    };
    int sets = 0;
    while(!isFinished(memory.getPartitions(),jobs)){
        sets++;
        string s = to_string(memory.getPartitions().size());
        memory.allocate(jobs);
        if(isFinished(memory.getPartitions(),jobs)) {
            sets--;
            break;
        }
        label();
        update_table(table_m,memory.getPartitions());
        int w = table_m.Header().size() * (table_m.getWidth()+1) + (s.size());
        cout << '+' << setfill('-') << setw(w) << "" << '+' << endl;
        cout << '|' << setfill(' ') <<setw(w) << center(w,"Allocation. Set "+to_string(sets)) << '|' << endl;
        table_m.print();
        conts();
        //
        label();
        memory.deallocate();
        cout << '+' << setfill('-') << setw(w) << "" << '+' << endl;
        cout << '|' << setfill(' ') <<setw(w) << left <<center(w,"Deallocation. Set "+to_string(sets)) << '|' << endl;
        dealloc_update_table(table_m,memory.getPartitions());
        table_m.print();
        update_table(table_m,memory.getPartitions());
        conts();
    }
    memory.summary(jobs);
    label();
    int width = 10;
    vector<Memory::Job>::iterator job_it = jobs.begin();
    for(int i = 0;i<job_table.Rows().size();i++){
        string color = "", val = "";
        if(job_it->status == ALLOCATED) {
            color = Color::Fore_White_Back_Green;
            val = "Allocated";
        }
        else if(job_it->status == CANT_ALLOCATE){
            color = Color::Fore_White_Back_Red;
            val = "Can't Allocate";
            width = 15;
        }
        job_table.edit_row(i,2,color,val);
        job_it++;
    }
    job_table.setWidth(width);
    job_table.print();
    cout << "Conclusion: " << endl;
    if(sets > 0){
        cout << "\tThere are " << sets << " set/s of Allocation.\n";
    }else{
        cout << "\tThere are no Allocation made.\n";
    }
    if(!memory.Exec_jobs().empty()){
        cout << "\tThere are " << memory.Exec_jobs().size() << " Jobs Executed.\n";
    }
    if(!memory.Fail_jobs().empty()) { 
        cout << "\tThere are " << memory.Fail_jobs().size() << " Jobs failed to allocate. Because the Job size is larger than the partitions.\n";
    }
    return 0;
}