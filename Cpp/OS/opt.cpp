#include <iostream>
#include "ccs2203.h"
using namespace std;
using namespace Memory;
using namespace Color;


void Memory::FrameManager::allocator(std::deque<Page*>& pages, Table& table){
    table.push_col({pages.front()->name});
    bool inFrame = false;
    // Page* curr = (*(pages.begin()+pages_ptr));
    Page* curr_page = pages.front();
    for(frame_it i = frames.begin();i!=frames.end();i++){
            if(!i->isHandling()){
                inFrame = true;
                i->setPage(curr_page);
                lru.push_back(curr_page);
                pages.pop_front();
                // pages_ptr++;
                page_fault++;
                table.edit_col(table.colSize()-1,table.Header().size()-1,Color::Fore_White_Back_Red,"*");
                break;
            }
            if (i->getPage().name == curr_page->name){
                inFrame = true;
                pages.pop_front();
                lru.push_back(curr_page);
                page_hit++;
                // pages_ptr++;
                break;
            }
        }
    if (!inFrame){
        page_fault++;
        // doesn't take into account of pages frequency
        int farthest_idx = 0;
        for (Pages_it i = pages.begin() + 1;i!=pages.end();i++){
            for (frame_it j = frames.begin();j!=frames.end();j++){
                if ((*i)->name == j->getPage().name && !j->appeared_opt){
                    j->appeared_opt = true;
                    farthest_idx = j->getID()-1;
                    // opt.push_back(&(*j));
                    break;
                }
            }
        }
        int first_lru_idx = -1;
        int count_lru = 0;
        for(frame_it i = frames.begin();i!=frames.end();i++){
            if(!i->appeared_opt){
                count_lru++;
                if (first_lru_idx == -1){
                    first_lru_idx = i->getID()-1;
                }
            }
        }

        // for(frame_it i = frames.begin();i!=frames.end();i++){
        //     if(!i->appeared_opt) {
        //         count_lru++;
        //         if(first_lru_idx == -1){
        //             first_lru_idx = i->getID()-1;
        //         }
        //         continue;
        //     }
        //     if (frames[farthest_idx].opt_dist <= i->opt_dist){
        //         farthest_idx = i->getID()-1;
        //     }
        // }

        if (count_lru == 1){
            farthest_idx = first_lru_idx;
        }
        else if (count_lru > 1){
            // Do lru
            for (Pages_rit i = lru.rbegin()+1;i!=lru.rend();i++){
                for (frame_it j = frames.begin();j!=frames.end();j++){
                    if (j->appeared_opt) continue;
                    if ((*i)->name == j->getPage().name && !j->appeared_lru){
                        j->appeared_lru = true;
                        farthest_idx = j->getID()-1;
                        break;
                    }
                }
            }
        }
        frames[farthest_idx].setPage(curr_page);
        table.edit_ccolor(farthest_idx+1,table.Header().size()-2,Fore_Yellow);
        table.edit_ccolor(farthest_idx+1,table.Header().size()-1,Fore_Red);
        pages.pop_front();
        // pages_ptr++;
    }
    for (frame_it i = frames.begin();i!=frames.end();i++){
        i->resetAppeared();
    }
    for(int i=1;i<table.colSize()-1;i++){
        if (!frames.at(i-1).isHandling()) continue;
        table.edit_col(i,table.Header().size()-1,frames.at(i-1).getPage().name);
    }
}


int main(){
    act_name = "ML-M4: ACT5 - OPT";
    // in << frame count
    // in << page count
    int f_count, p_count;
    string ms = "";
    // f_count
    f_count = input<int>("Enter No. of Frames: ", ms, false);
    FrameManager frame_table;
    Table table(new vector<string>{"Frames"},BOLD);
    for(int i = 0;i<f_count;i++){
        frame_table.getFrames().push_back(Frame(i+1));
        table.push_row({frame_table.getFrames().at(i).getName()});
    }
    label();
    table.print();
    table.push_row({BOLD},{"Page Fault"});
    // p_count
    conts();
    p_count = input<int>("Enter No. of Pages: ", ms, false);
    vector<Page> pages;
    Table page_table(new vector<string>{"Pages"},BOLD);
    for(int i = 0;i<p_count;i++){
        pages.push_back(Page(char(65+i)));
        page_table.push_row({pages.at(i).name});
    }
    label();
    page_table.print();

    conts();
    clrscr();
    int request_count;
    deque<Page*> requested_pages;
    request_count = input<int>("Enter Request Count: ", ms, false);
    ms = "Enter No. of Request Page: " + to_string(request_count) + '\n';
    int i = 0;
    while(i < request_count){
        clrscr();
        char requested_page;
        label();
        cout << ms;
        cout << "Enter Requested Page No." << i+1 << ": ";
        cin >> requested_page;
        requested_page = toupper(requested_page);
        if(int(requested_page) > 65+(p_count-1) || int(requested_page) < 65) {
            cout << "Wrong Input. Please input existing Page." << '\n';
            cont();
            continue;
        }
        i++;
        int idx_page = int(requested_page) - 65;
        ms += "Enter Requested Page No." + to_string(i) + ": " +requested_page + '\n';
        requested_pages.push_back(&(pages.at(idx_page)));
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    while(!requested_pages.empty()){
        clrscr();
        frame_table.allocator(requested_pages,table);
        table.print();
        conts();
    }
    table.print();
    cout << "\nConclusion: \n";
    cout << "Page Fault: " << frame_table.pageFault() << '\n';
    cout << "Page Hit: " << frame_table.pageHit() << '\n';
    return 0;
}
