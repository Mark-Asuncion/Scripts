#![allow(unused)]

use std::io::Write;
pub fn input<T:std::ops::Add + std::str::FromStr + std::fmt::Debug>(n: T,message: &str) -> Option<T>
where <T as std::str::FromStr>::Err: std::fmt::Debug {
    print!("{message} ");
    std::io::stdout().flush().unwrap();

    let mut str_input = String::new();
    let err_code = match std::io::stdin().read_line(&mut str_input) {
        Ok(n) => n,
        Err(e) => {
            println!("Error {e}");
            0
        },
    };
    if err_code == 0 { return input(n,message); }
    let str_input: Vec<&str> = str_input.trim().split(|n| {
        if n == ' ' || n == '\n' {
            return true;
        }
        false
    })
        .collect();
    if str_input[0] == "" { println!("Invalid Input, input is whitespace"); return input(n,message) }
    let res: T = str_input[0].parse().unwrap();
    Some(res)
}
use std::vec::Vec;
pub mod color {
    pub const NORMAL: &str = "\x1B[0m";
    pub const BOLD: &str = "\x1B[1m";
    pub const BOLD_FORE_RED: &str = "\x1B[1;31m";
    pub const BOLD_FORE_GREEN: &str = "\x1B[1;32m";
    pub const BOLD_FORE_YELLOW: &str = "\x1B[1;33m";
    pub const FORE_WHITE_BACK_GREEN: &str = "\x1B[1;37;42m";
    pub const FORE_WHITE_BACK_RED: &str = "\x1B[1;37;41m";
    pub const FORE_CYAN_BACK_GREEN: &str = "\x1B[1;36;42m";
    pub const FORE_CYAN_BACK_RED: &str = "\x1B[1;36;41m";
    pub const FORE_RED: &str = "\x1B[0;31m";
    pub const FORE_GREEN: &str = "\x1B[0;32m";
    pub const FORE_YELLOW: &str = "\x1B[0;33m";
}

pub struct Format {
    ss: String,
}
impl Format {
    pub fn truncate(val: f64, length: i32) -> f64 {
        let whole_number = val.floor() as i32 as f64;
        let remainder = val - whole_number;
        const TEN: f64 = 10.0;
        let decimals = (remainder * TEN.powi(length)) as i32 as f64;
        let decimals = decimals / TEN.powi(length);
        whole_number + decimals
    }
    pub fn fround(val: f64,length: i32) -> f64 {
        const TEN: f64 = 10.0;
        let x = val * TEN.powi(length) / TEN.powi(length);
        x.ceil()
    }
}

pub struct Table {
    header: Vec<String>,
    header_color: Vec<String>,
    body: Vec<Vec<String>>,
    body_color: Vec<Vec<String>>,
}
impl Table {
    pub fn new(header: Vec<String>,mheader_color: String) -> Table {
        Table {
            header_color: {
                let mut x: Vec<String> = Vec::new();
                x.resize(header.len(),mheader_color);
                x
            },
            header,
            body: Vec::new(),
            body_color: Vec::new(),
        }
    }
    pub fn get_col(&self) -> usize { self.header.len() }
    pub fn get_row(&self) -> usize { self.body.len() }
    fn resize(&mut self) {
        let col: usize = self.get_col();
        for i in 0..self.get_row() {
            if self.body[i].len() <= col { continue; }
            self.body[i].resize(col,String::from(""));
            self.body_color[i].resize(col,String::from(""));
        }
    }
    pub fn push_header(&mut self, mut val: Vec<String>, color: Option< Vec<String> >) {
        self.header.append(&mut val);
        match color {
            Some(mut i) => {
                self.header_color.append(&mut i);
            },
            None => {
                self.header_color.resize(self.header.len(),String::from(""));
            },
        };
        dbg!(&self.header);
        dbg!(&self.header_color);
    }
    pub fn push_body(&mut self, mut val: Vec<Vec<String>>, color: Option< Vec<Vec<String>> >) {
        self.body.append(&mut val);
        match color {
            Some(mut i) => {
                self.body_color.append(&mut i);
            },
            None => {
                let mut x: Vec<String> = Vec::new();
                x.resize(self.body[0].len(),String::from(""));
                self.body_color.resize(self.header.len(),x);
            },
        };
        self.resize();
        dbg!(&self.body);
        dbg!(&self.body_color);
    }
    pub fn edit_header(&mut self, column: usize, val: String, color: Option<String>) {
        eprintln!("before edit");
        dbg!(&self.header);
        dbg!(&self.header_color);
        if column >= self.header.len() { return; }
        self.header[column] = val;
        match color {
            Some(i) => { self.header_color[column] = i; },
            _ => (),
        }
        eprintln!("after edit");
        dbg!(&self.header);
        dbg!(&self.header_color);
    }
    pub fn edit_body(&mut self,row: usize, column: usize, val: String, color: Option<String>) {
        eprintln!("before edit");
        dbg!(&self.body);
        dbg!(&self.body_color);
        if row >= self.get_row() || column >= self.get_col() { return; }
        self.body[row][column] = val;
        match color {
            Some(i) => { self.body_color[row][column] = i; },
            _ => (),
        }
        eprintln!("after edit");
        dbg!(&self.body);
        dbg!(&self.body_color);
    }
    pub fn print(&self) {
        // ┌─┬─┐
        // │ │ │
        // ├─┼─┤
        // │ │ │
        // └─┴─┘
        println!("print()");
    }
}
