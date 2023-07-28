#![allow(unused)]
mod ccs2203;
use ccs2203::{ color, Format, Table, };

fn test_table() {
    let mut x = Table::new(vec![String::from("Hello")],color::BOLD_FORE_GREEN.to_string());
    {
        let body: Vec<Vec<String>> = vec![ vec![String::from("Hello"),String::from("test")] ];
        x.push_body(body,None);
    }
    // println!("┌─┬─┐");
    // println!("│ │ │");
    // println!("├─┼─┤");
    // println!("│ │ │");
    // println!("└─┴─┘");
}
fn main() {
    let x: i32 = 5;
    let msg = "Enter a number:";
    let x = ccs2203::input(x,msg).unwrap();
}
