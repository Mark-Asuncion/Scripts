#![allow(unused)]
mod ccs2203;
use ccs2203::{ color, Format, Cell, Table, mem };

fn test_table() {
    let fore_green: &str = color::BOLD_FORE_GREEN;
    let cell: Vec<Cell> = vec![Cell::new("Hello",fore_green),Cell::new("Test",fore_green), Cell::new("asjdhaskjdhaksjhdk",fore_green)];
    let mut table = Table::new(cell);
    let cell: Vec<Vec<Cell>> = vec![vec![Cell::new("Col1",""),Cell::new("Col2",""), Cell::new("Col3","")],vec![Cell::new("Hello",""),Cell::new("Test","")],vec![Cell::new("Hello","")]];
    table.push(cell);
    table.width = 20;
    // let cell : Vec<Cell> = vec![ Cell::new("Col1",""),Cell::new("Col2",""), Cell::new("Col3","") ];
    // table.push_col(cell);
    // table.dbg();
    table.print();
}
fn test_table_col() {
    let fore_green: &str = color::BOLD_FORE_GREEN;
    let cell: Vec<Cell> = vec![Cell::new("Hello",fore_green),Cell::new("Test",fore_green), Cell::new("asjdhaskjdhaksjhdk",fore_green)];
    let mut table = Table::new(cell);
    let cell: Vec<Vec<Cell>> = vec![vec![Cell::new("Col1",""),Cell::new("Col2",""), Cell::new("Col3","")],vec![Cell::new("Hello",""),Cell::new("Test","")],vec![Cell::new("Hello","")]];
    table.push(cell);
    let cell : Vec<Cell> = vec![ Cell::new("new_Col1",""),Cell::new("new_Col2",""), Cell::new("new_Col3","") ];
    table.push_col(cell);
    table.edit(3,3,Some("newTest"),None);
    table.edit(0,0,Some("newTest2"),Some(color::NORMAL));
    table.edit(0,5,Some("newTest2"),None);
    table.print();
}
fn test_partition() {
    let mut job = mem::Job::new(64.0,"J",0,100,mem::State::Nothing);
}
fn test_input() {
    let mut x: i32 = 5;
    dbg!(&x);
    let msg = "Enter a number:";
    ccs2203::input(msg,&mut x);
    dbg!(&x);
}
fn main() {
    // test_input();
    test_table();
    test_table_col();
}
