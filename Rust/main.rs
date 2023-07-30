#![allow(unused)]
mod ccs2203;
use ccs2203::{ color, Format, Cell, Table, };

fn test_table() {
    let fore_green: &str = color::BOLD_FORE_GREEN;
    let cell: Vec<Cell> = vec![Cell::new("Hello",fore_green),Cell::new("Test",fore_green), Cell::new("asjdhaskjdhaksjhdk",fore_green)];
    let mut table = Table::new(cell);
    let cell: Vec<Vec<Cell>> = vec![vec![Cell::new("Col1",fore_green),Cell::new("Col2",fore_green), Cell::new("Col3",fore_green)],vec![Cell::new("Hello",fore_green),Cell::new("Test",fore_green)],vec![Cell::new("Hello",fore_green)]];
    table.push(cell);
    // table.dbg();
    table.print();
}
fn test_input() {
    let mut x: i32 = 5;
    dbg!(&x);
    let msg = "Enter a number:";
    ccs2203::input(msg,&mut x);
    dbg!(&x);
}
fn main() {
    test_input();
    // test_table();
}
